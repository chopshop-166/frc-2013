#include <Wire.h>
#include <stdio.h>
#include <dht11.h>
#include <NewPing.h>

typedef enum {INIT, PREPARING, RESPONDING, IDLE, RECEIVING} state_t;
typedef enum {HELLO, ERRORS, SENSORS} send_type_t;

#define INITIAL_NULLS (10)
#define HEADER_SIGNATURE (0)
#define HEADER_VERSION (4)
#define HEADER_LENGTH (5)
#define HEADER_PAYLOAD_START (7)
#define MIN_RESERVE (3)

// TLV types
#define TLV_K_END 'e'
#define TLV_K_MSG_TYPE 't'

// Hello
#define MSG_T_HELLO 'H'
#define TLV_K_STROBE_INTERVAL 'i'
#define TLV_K_IOBUFFER_SIZE 'b'

// Error
#define MSG_T_ERROR 'E'
#define TLV_K_ERROR_FLAGS 'f'
#define TLV_K_ERROR_COUNT 'g'

// Sensor
#define MSG_T_SENSOR 'S'
#define TRIGPIN 12
#define ECHOPIN 11
#define MAX_DISTANCE 1023
// Misc
#define TLV_K_MODE 'm'

#if 0
unsigned int out_idx;
unsigned int buffer_len;
unsigned char flags;
unsigned int nulls_tx_remaining;
state_t current_state = INIT;
send_type_t send_what = HELLO;
char io_buffer[MAX_BUFFER];
float dht11_humidity;
float dht11_temperature_c;
float dht11_fahrenheit;
float dht11_kelvin;
float dht11_dewPoint;
float dht11_dewPointFast;
int rx_pending;
unsigned int loop_counter;
#endif

// Debug printing
#define DEBUG
#if defined(DEBUG)
#define DBG_PRINT(arg) Serial.print(arg)
#define DBG_PRINTLN(arg) Serial.println(arg)
#else
#define DBG_PRINT(arg)
#define DBG_PRINTLN(arg)
#endif

// Duration of each loop, in ms
#define CTX_K_LOOP_DURATION_MS (10)

// Sensor strobe interval in units of CTX_K_LOOP_DURATION_MS
#define CTX_K_SENSOR_STROBE_INTERVAL (50)

// Preferred strobe interval by master in units of CTX_K_LOOP_DURATION_MS
#define CTX_K_MASTER_STROBE_INTERVAL (10)

// Max size of our IO buffer
#define CTX_K_MAX_IOBUFFER (1024)

// Slave ID for us on the I2C bus
#define CTX_K_I2C_SLAVE_ID (4)

// Baud rate for our console
#define CTX_K_CONSOLE_BAUD_RATE (115200)

// Context flags
#define CTX_M_OK (1<<0)                                       // All is well
#define CTX_M_RX_PENDING (1<<1)                               // RX data is pending from the master
#define CTX_M_SEND_OVERFLOW (1<<2)                            // Output buffer overflow
#define CTX_M_TRUNCATED_RESPONSE (1<<3)                       // Response from the server was truncated
#define CTX_M_ABORTED_RECEIVE (1<<4)                          // Receive of data was aborted by transmit
#define CTX_M_RX_GARBAGE (1<<5)                               // Garbage data from master
#define CTX_M_RX_TOO_LONG (1<<6)                              // RX data overflow

// Global work context
struct
{
  unsigned int flags;                                         // Context flags
  unsigned int loop_counter;                                  // loop() function call counter
  unsigned int buffer_used;                                   // Bytes used in I/O buffer
  unsigned int nulls_tx_remaining;                            // Remaining NULLs to send
  unsigned int out_idx;                                       // Index to next byte in IO buffer to send
  send_type_t send_what;                                      // What our next message to the master is going to be
  state_t current_state;                                      // State of our communications interface
  char io_buffer[CTX_K_MAX_IOBUFFER];                         // Communication I/O buffer

} controller_ctx;

// Forward declarations of routines so we can re-shuffle the code a bit
void strobe_sensor_dht11(struct dht11_sensor *dht11);
void report_sensor_dht11(struct dht11_sensor *dht11);
void explain_sensor_dht11(struct dht11_sensor *dht11);

void strobe_sensor_hcsr4(struct hcsr4_sensor *hcsr4);
void report_sensor_hcsr4(struct hcsr4_sensor *hcsr4);
void explain_sensor_hcsr4(struct hcsr4_sensor *hcsr4);

void setup(void);
void loop(void);
void put_header(void);
void put_str(char cmd, unsigned char *str);
void put_int(char cmd, int val);
void put_float(char cmd, float val);
void put_byte(char cmd, unsigned char val);
void put_end(void);
void report_sensors(void);
void explain_sensors(void);
void prepare_buffer_for_send(void);
void i2c_master_reading(void);
int process_input_packet(void);
void i2c_master_writing(int howMany);
void i2c_master_drain(void);

// Data structure for DHT11 sensor
struct dht11_sensor
{

  float humidity;                                             // Relative humidity
  float celcius;                                              // Temperature in Celcius
  float fahrenheit;                                           // Temperature in Fahrenheit
  float kelvin;                                               // Temperature in Kelvin
  float dewPoint;                                             // Dewpoint in degrees
  float dewPointFast;                                         // Fast dew point

} dht11_sensor_readings;

// Data structure for DHT11 sensor
struct hcsr4_sensor
{

  int distance;
  int trigPin;
  int echoPin;

} hcsr4_sensor_readings;

// This function strobes the DHT11 sensor
dht11 DHT11 = dht11(D1, BUSA); //we are placing our device on D1 on BUSA
//NewPing sonar(TRIGPIN, ECHOPIN, MAX_DISTANCE);
NewPing sonar(TRIGPIN, ECHOPIN, MAX_DISTANCE);


void strobe_sensor_dht11(struct dht11_sensor *dht11)
{

  // Read the DHT11 sensor
  int chk = DHT11.read();

  // Check status to see if things are fine before proceeding
  switch (chk)
    {
    case 0: Serial.println("sensor OK "); break;
    case -1: Serial.println("sensor -1 "); return;
    case -2: Serial.println("sensor -2 "); return;
    default: Serial.println("sensor U "); return;
    }

  // Looks good. Capture the values in local variables for later reference
  dht11->humidity = (float)DHT11.humidity;
  dht11->celcius = (float)DHT11.temperature;
  dht11->fahrenheit = DHT11.fahrenheit();
  dht11->kelvin = DHT11.kelvin();
  dht11->dewPoint = DHT11.dewPoint();
  dht11->dewPointFast = DHT11.dewPointFast();

  // Done
  return;
}


void strobe_sensor_hcsr4(struct hcsr4_sensor *hcsr4)
{

  // Read the DHT11 sensor
  
  digitalWrite(hcsr4->trigPin, HIGH);
  delay(100);
  digitalWrite(hcsr4->trigPin, LOW);
  
  hcsr4->distance = sonar.ping_in();
   Serial.print(hcsr4->distance);
    Serial.println(" inches");
  // Done
  return;
}
// Explain the DHT11 sensor(s)
void explain_sensor_dht11(struct dht11_sensor *dht11)
{

  // Report out the various sensors we have and the key code for each
  put_str('H', (unsigned char *)"Humidity (%)");
  put_str('C', (unsigned char *)"Temperature (?C)");
  put_str('F', (unsigned char *)"Temperature (?F)");
  put_str('K', (unsigned char *)"Temperature (?K)");
  put_str('D', (unsigned char *)"Dew Point (?C)");
  put_str('Q', (unsigned char *)"Dew PointFast (?C)");

  // Done
  return;
}


void explain_sensor_hcsr4(struct hcsr4_sensor *hcsr4)
{

  // Report out the various sensors we have and the key code for each
  put_str('X', (unsigned char *)"Distance (Inches)");

  // Done
  return;
}
// Report out information about the DHT11 sensor
void report_sensor_dht11(struct dht11_sensor *dht11)
{
  
  // Report out values we captured during the main loop
  put_float('H', dht11->humidity);
  put_float('C', dht11->celcius);
  put_float('F', dht11->fahrenheit);
  put_float('K', dht11->kelvin);
  put_float('D', dht11->dewPoint);
  put_float('Q', dht11->dewPointFast);
  
  // Done
  return;
}

void report_sensor_hcsr4(struct hcsr4_sensor *hcsr4)
{
  
  // Report out values we captured during the main loop
  put_float('X', hcsr4->distance);
  
  // Done
  return;
}

// This method explains all the sensors we have attached by calling the specific explain function of each
void explain_sensors(void)
{

  // Explain the DHT11
  explain_sensor_dht11(&dht11_sensor_readings);
  
  explain_sensor_hcsr4(&hcsr4_sensor_readings);
}

// This function strobes all the sensors attached to the Arduino
void strobe_sensors(void)
{

  // Strobe the DHT11 sensor
  strobe_sensor_dht11(&dht11_sensor_readings);
  
   strobe_sensor_hcsr4(&hcsr4_sensor_readings);

  // Done
  return;
}

// This function reports out all of our sensors 
void report_sensors(void)
{

  // Report out the DHT11
  report_sensor_dht11(&dht11_sensor_readings);

  report_sensor_hcsr4(&hcsr4_sensor_readings);
  // Done
  return;
}

// ***************** COMMON CODE ***********************

// This is the Arduino configuration routine. It gets called during bootstrap
void setup(void)
{

  // Initialize our work context
  //bzero((void *)&controller_ctx, sizeof(controller_ctx));
  memset((void *)&controller_ctx, 0, sizeof(controller_ctx));
  
  controller_ctx.send_what = HELLO;
  controller_ctx.current_state = INIT;

  // Join the I2C bus and tickle it with a zero
  Wire.begin(CTX_K_I2C_SLAVE_ID);
  Wire.write(0);

  // Establish a callback handler for when the master is waiting to read a response from us
  Wire.onRequest(i2c_master_reading);

  // Establish a callback handler for when the master wants to write to us
  Wire.onReceive(i2c_master_writing);

  // Initialize our console (for debug)
  Serial.begin(CTX_K_CONSOLE_BAUD_RATE);
  Serial.println("configured");

  // Done
  return;
}

// This is the Arduino loop function that gets called repeatedly
void loop(void)
{

  // Bump our loop counter
  controller_ctx.loop_counter++;

  // Strobe our sensors if it is time
  if (!(controller_ctx.loop_counter % CTX_K_SENSOR_STROBE_INTERVAL))
    strobe_sensors();

  // Shall we grab any pending RX data?
  if (controller_ctx.flags & CTX_M_RX_PENDING) {

    // Yes. Drain it
    i2c_master_drain();

    // Indicate that we've taken care of it
    controller_ctx.flags &= ~CTX_M_RX_PENDING;
  }     

  // Delay for next loop call and then back to caller
  delay(CTX_K_LOOP_DURATION_MS);
  return;
}

// Stuff a protocol header into the io buffer
void put_header(void)
{
  
  // Header label itself
  controller_ctx.io_buffer[controller_ctx.buffer_used++] = 'A';
  controller_ctx.io_buffer[controller_ctx.buffer_used++] = 'P';
  controller_ctx.io_buffer[controller_ctx.buffer_used++] = 'C';
  controller_ctx.io_buffer[controller_ctx.buffer_used++] = 'P';
  
  // Protocol version number: 1
  controller_ctx.io_buffer[controller_ctx.buffer_used++] = '1';
  
  // Preset the length to zero for now
  controller_ctx.io_buffer[controller_ctx.buffer_used++] = 0;
  controller_ctx.io_buffer[controller_ctx.buffer_used++] = 0;

  // Done
  return;
}

// Append a string TLV to the io buffer
void put_str(char cmd, unsigned char *str)
{

  // Determine how long the input string is
  int i = 0;
  while (str[i])
    i++;

  // Do we have room to add this command and value to the io buffer?
  if (controller_ctx.buffer_used + i + 2 < (sizeof(controller_ctx.io_buffer) - MIN_RESERVE - 1)) {

    // Yes. Stuff command and length in first
    controller_ctx.io_buffer[controller_ctx.buffer_used++] = cmd;
    controller_ctx.io_buffer[controller_ctx.buffer_used++] = i;

    // Then append the string itself
    i = 0;
    unsigned char c;
    while ((c = str[i]))
      controller_ctx.io_buffer[controller_ctx.buffer_used++] = str[i++];
  } else {

    // No, we do not have room. Report an error
    Serial.println("Overflow on put_str()");
    controller_ctx.flags |= CTX_M_SEND_OVERFLOW;
  }

  // Done
  return;
}

// Append an ASCII integer TLV to the io buffer
void put_int(char cmd, int val)
{
  
  // First, convert the integer into text
  unsigned char aval[14];
  sprintf((char *)&aval, "%d", val);

  // Then append this as a string TLV
  put_str(cmd, aval);

  // Done
  return;
}

// Append an ASCII floating point TLV to the io buffer
void put_float(char cmd, float val)
{

  // Convert floating point to a string
  unsigned char aval[14];
  dtostrf(val, 2, 1, (char *)&aval);

  // Then append as a string TLV
  put_str(cmd, aval);

  // Done
  return;
}

// Append a single byte TLV command to the io buffer, such as an END
void put_byte(char cmd, unsigned char val)
{
  
  // Do we have room for this TLV?
  if (controller_ctx.buffer_used + 3 < (sizeof(controller_ctx.io_buffer) - MIN_RESERVE - 1)) {

    // Yes. Append the 3 bytes
    controller_ctx.io_buffer[controller_ctx.buffer_used++] = cmd;
    controller_ctx.io_buffer[controller_ctx.buffer_used++] = 1;
    controller_ctx.io_buffer[controller_ctx.buffer_used++] = val;
  } else {

    // No, record an error
    controller_ctx.flags |= CTX_M_SEND_OVERFLOW;
  }

  // Done
  return;
}

// Put an end marker to the io buffer and revise the length
void put_end(void)
{

  // Put the end TLV in there
  put_byte(TLV_K_END, (controller_ctx.flags ? controller_ctx.flags : CTX_M_OK));

  // Recompute the length of the buffer based on how much we actually wrote
  controller_ctx.io_buffer[HEADER_LENGTH] = (controller_ctx.buffer_used & 0xff);
  controller_ctx.io_buffer[HEADER_LENGTH+1] = (controller_ctx.buffer_used >> 8) & 0xff;

  // Done
  return;
}

// Prepare the io buffer for send
void prepare_buffer_for_send(void)
{

  // Reset the buffer and place a header in there
  controller_ctx.buffer_used = 0;
  put_header();

  // Dispatch based on what we're expected to send
  switch (controller_ctx.send_what) {

    // Sensors
    case SENSORS: {
      DBG_PRINTLN("Send sensors");
      put_byte(TLV_K_MSG_TYPE, MSG_T_SENSOR);
      report_sensors();
      break;
    }

    // Hello message
    case HELLO: {
      DBG_PRINTLN("Send hello..");
      put_byte(TLV_K_MSG_TYPE, MSG_T_HELLO);
      put_int(TLV_K_STROBE_INTERVAL, CTX_K_MASTER_STROBE_INTERVAL * CTX_K_LOOP_DURATION_MS);
      put_int(TLV_K_IOBUFFER_SIZE, CTX_K_MAX_IOBUFFER);
      explain_sensors();
      break;
    }

    // Error counters and flags
    case ERRORS: {
      DBG_PRINTLN("Send errors");
      put_byte(TLV_K_MSG_TYPE, MSG_T_ERROR);
      put_byte(TLV_K_ERROR_FLAGS, 0);
      put_int(TLV_K_ERROR_COUNT, 0);
      break;
    }
  }

  // We're doing. Put an end to the buffer
  put_end();

  // Unless we hear otherwise, the next message will be sensor message from us
  controller_ctx.send_what = SENSORS;

  // Done
  return;
}

// Callback for when master is requesting data
void i2c_master_reading(void)
{
  //  Serial.println("Master");
  switch (controller_ctx.current_state) {
    
    // We are in the process of receiving a message from the master. So this is unexpected.
    case RECEIVING: {
      DBG_PRINTLN("MX in RX mode");
      controller_ctx.flags |= CTX_M_ABORTED_RECEIVE;
      controller_ctx.current_state = INIT;
    }
    
    // Initializing 
    case INIT: {
    
      // Set a light to indicate we've seen the Master at all
      controller_ctx.current_state = IDLE;
    }
    
    // Idle state is just before we prepare
    case IDLE: {
      controller_ctx.current_state = PREPARING;
    }

    // Prepare the output buffer now
    case PREPARING: {
      DBG_PRINT("Knocking..");
      controller_ctx.out_idx = 0;
      controller_ctx.current_state = RESPONDING;
      prepare_buffer_for_send();
      DBG_PRINTLN("Prepared..");
      controller_ctx.nulls_tx_remaining = INITIAL_NULLS;
    }    

    // Send out the buffer
    case RESPONDING: {

      // Take care of null prepends first
      if (controller_ctx.nulls_tx_remaining) {
        Wire.write(0);
        controller_ctx.nulls_tx_remaining--;
        break;
      }

      // Output the next byte
      Wire.write(controller_ctx.io_buffer[controller_ctx.out_idx++]);

      // Are we done?
      if (controller_ctx.out_idx >= controller_ctx.buffer_used) {

	// Yes. Reset state and clear flags
        DBG_PRINTLN("Done");
        controller_ctx.current_state = IDLE;
        controller_ctx.flags = controller_ctx.flags & CTX_M_RX_PENDING;
      }
      break;
    }
  }
}

// For each byte that we receive from the master, we try to figure out if we can decipher it
int process_input_packet(void)
{

  int expected_length;                                          // Bytes expected
  int i;                                                        // Index within buffer we're checking

  // If we do not have enough bytes for a header, keep looking
  if (controller_ctx.buffer_used <= HEADER_PAYLOAD_START)
    return (0);

  // If we've detected input garbage, discard and pretend we have it all now
  if (controller_ctx.flags & CTX_M_RX_GARBAGE)
    return (1);

  // Is this a good header? If not, flag and indicate we're done
  if ((controller_ctx.io_buffer[HEADER_SIGNATURE+0] != 'A') ||
      (controller_ctx.io_buffer[HEADER_SIGNATURE+1] != 'P') ||
      (controller_ctx.io_buffer[HEADER_SIGNATURE+2] != 'C') ||
      (controller_ctx.io_buffer[HEADER_SIGNATURE+3] != 'P') ||
      (controller_ctx.io_buffer[HEADER_VERSION] != '1')) {
        controller_ctx.flags |= CTX_M_RX_GARBAGE;
        return (1);
      }

  // Do we have what we're expecting from the master? If not, keep picking up more bytes
  expected_length = (controller_ctx.io_buffer[HEADER_LENGTH] | (controller_ctx.io_buffer[HEADER_LENGTH+1] << 8));
  if (controller_ctx.buffer_used < expected_length)
    return (0);

  // Is this longer than what we expected? If so, treat as trash and say that we're done
  if (controller_ctx.buffer_used > expected_length) {
    controller_ctx.flags |= (CTX_M_RX_GARBAGE | CTX_M_RX_TOO_LONG);
    return (1);
  }
  
  // Start processing the TLV data in the payload
  i = HEADER_PAYLOAD_START;
  while (i < controller_ctx.buffer_used) {

    // Pick up length of next item. If it goes beyond the end of the packet, its garbage
    unsigned int item_len = controller_ctx.io_buffer[i + 1];
    if (i + 2 + item_len > controller_ctx.buffer_used) {
      controller_ctx.flags |= CTX_M_RX_GARBAGE;
      return (1);
    }
    
    // So far so good. Dispatch on TLV type
    switch (controller_ctx.io_buffer[i]) {

       // Master wants to set the type of packet we're going to send next
       case TLV_K_MODE: {

	 // Wrong length means this is junk
         if (item_len != 1) {
           controller_ctx.flags |= CTX_M_RX_GARBAGE;
           return (1);
         }

	 // Dispatch on the actual type
         switch (controller_ctx.io_buffer[i+2]) {
           case MSG_T_HELLO: {
             controller_ctx.send_what = HELLO;
             break;
           }
           case MSG_T_SENSOR: {
             controller_ctx.send_what = SENSORS;
             break;
           }
           case MSG_T_ERROR: {
             controller_ctx.send_what = ERRORS;
             break;
           }
           default: {
	     controller_ctx.flags |= CTX_M_RX_GARBAGE;
	     return (1);
             break;
           }
         }
         break;
       }

       // End marker
       case TLV_K_END: {

	 // Wrong length means this is junk
         if (item_len != 1) {
           controller_ctx.flags |= CTX_M_RX_GARBAGE;
           return (1);
         }
         break;
       }

       // For the rest, skip over
       default: {
         break;
       }
     }

    // Advance to the next item, using the length provided plus fixed size header
    i = i + 2 + item_len;
  }

  // Done.
  return (1);
}
  
// Interrupt handler that detects we have pending data from the master
void i2c_master_writing(int howMany)
{

  // Just set a flag. This can only be drained from the main loop
  controller_ctx.flags |= CTX_M_RX_PENDING;
  return;
}

// Drain data from the master
void i2c_master_drain(void)
{

  // Dispatch on our current state
  switch (controller_ctx.current_state) {

    // We are responding to a read from the master. That is now truncated.
    case RESPONDING: {
       controller_ctx.flags |= CTX_M_TRUNCATED_RESPONSE;
    }

    // Get ready to receive the first byte
    case INIT: {
       controller_ctx.buffer_used = 0;
       controller_ctx.current_state = RECEIVING;
    }

    // Pick up the next byte(s)
    case RECEIVING: {

      // Iterate as long as there is data to drain
      while (Wire.available()) {

	// Pick up the next character and save it, provided it fits in our io buffer
        unsigned char c = Wire.read();
        if (controller_ctx.buffer_used < sizeof(controller_ctx.io_buffer) - 1) {
          controller_ctx.io_buffer[controller_ctx.buffer_used++] = c;
        }
      }

      // Have we gotten enough to conclude we've got it all? If so, back to init state
      if (process_input_packet()) {
        controller_ctx.current_state = INIT;
      }
      break;
    }
  }
}
