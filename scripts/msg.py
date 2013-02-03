import smbus
import time

# Use this for timeout exceptions
class TimeoutException(Exception):
    pass

# Dump the buffer
def dump_buffer(io_buffer):
    for idx in range(len(io_buffer)):
        print io_buffer[idx]


# Prepare message header
def prepare_header():

    # Allocate a buffer
    iobuffer = bytearray()

    # Add header fields
    iobuffer.append(b'A')
    iobuffer.append(b'P')
    iobuffer.append(b'C')
    iobuffer.append(b'P')

    # Version
    iobuffer.append(b'1')

    # And a stub length for now
    iobuffer.append(0)
    iobuffer.append(0)

    # Back to caller with this
    return (iobuffer)

# Append a TLV byte entry
def add_tlv_byte(iobuffer, command, value):

    # Append the command character
    iobuffer.append(command)

    # Lnegth is 1
    iobuffer.append(1)

    # And then the value
    iobuffer.append(value)

    # Back to caller with the length
    return (3)


def put_byte(bus, slave_nr, attempts, value):

    # Try a number of times
    print 'entering put_byte()'
    c = 0
    got_it = 0
    for attempt in range(attempts):
        try:
            print 'trying to write to slave'
            c = bus.write_byte(slave_nr, value)
            print 'worked'
            got_it = 1
            break
        except:
            print 'failed, sleeping a bit and trying again'
            time.sleep(0.05) # Delay 50ms
            pass
    if (got_it):
        return (c)

    # Not able to get data in time
    raise TimeoutException('Failed to write before timeout expired')

# Send the buffer to the other side
def send_buffer(iobuffer, length, bus, slave_nr, attempts):

    # Set the length
    iobuffer[5] = (length & 0xff)
    iobuffer[6] = ((length >> 8) & 0xff)

    # Enter loop to send all the data to the i2c peer
    print 'starting to send to slave'
    for idx in range(length):
        print 'sending', iobuffer[idx]
        put_byte(bus, slave_nr, attempts, iobuffer[idx])
        time.sleep(0.100) # Delay 50ms

    # Done
    print 'send to slave complete'

                
# Send hello message
def send_hello(bus, slave_nr, attempts):

    # Get the header setup
    iobuffer = prepare_header()

    # Set the message type to hello
    len = 7                                   # Length of header
    len += add_tlv_byte(iobuffer, b't', b'H') # Hello message
    len += add_tlv_byte(iobuffer, b'm', b'H') # Report back with a Hello from your side
    len += add_tlv_byte(iobuffer, b'e', 0)    # End of packet

    # Send it
    send_buffer(iobuffer, len, bus, slave_nr, attempts)
    

# Get one byte. 
def get_byte(bus, slave_nr, attempts):

    # Try a number of times
    c = 0
    got_it = 0
    for attempt in range(attempts):
        try:
            c = bus.read_byte(slave_nr)
            got_it = 1
            break
        except:
            time.sleep(0.05) # Delay 50ms
            pass
    if (got_it):
        return (c)

    # Not able to get data in time
    raise TimeoutException('Failed to read before timeout expired')

# Put one byte. 
def put_byte(bus, slave_nr, attempts, value):

    # Try a number of times
    c = 0
    got_it = 0
    for attempt in range(attempts):
        try:
            c = bus.write_byte(slave_nr, value)
            got_it = 1
            break
        except:
            time.sleep(0.05) # Delay 50ms
            pass
    if (got_it):
        return (c)

    # Not able to get data in time
    raise TimeoutException('Failed to write before timeout expired')

# Read one packet from the wire
def read_packet(bus, slave_nr, initial_tries, subsequent_tries, sensor_dictionary):

    # Working array
    iobuffer = bytearray()

    # Read zeros
    tries = initial_tries
    while True:
        c = get_byte(bus, slave_nr, tries)
        if (c != 0):
            iobuffer.append(c)
            break
        tries = subsequent_tries

    # Read the (rest of the) header
    iobuffer.append(get_byte(bus, slave_nr, subsequent_tries))
    iobuffer.append(get_byte(bus, slave_nr, subsequent_tries))
    iobuffer.append(get_byte(bus, slave_nr, subsequent_tries))
    if ((iobuffer[0] != ord(b'A')) or (iobuffer[1] != ord(b'P')) or (iobuffer[2] != ord(b'C')) or (iobuffer[3] != ord(b'P'))):
        dump_buffer(iobuffer)
        print 'failed header check'
        return None

    # Then the version
    iobuffer.append(get_byte(bus, slave_nr, subsequent_tries))
    if (iobuffer[4] != ord(b'1')):
        print 'failed version check'
        dump_buffer(iobuffer)
        return None

    # Then the length
    iobuffer.append(get_byte(bus, slave_nr, subsequent_tries))
    iobuffer.append(get_byte(bus, slave_nr, subsequent_tries))

    # Figure out how many bytes to read
    total_length = iobuffer[5] | (iobuffer[6]<<8)
    remaining_bytes = total_length - 7
    #print remaining_bytes

    # Read the rest
    for c in range(remaining_bytes):
        iobuffer.append(get_byte(bus, slave_nr, subsequent_tries))

    # Iterate through the payload
    next_byte = 7
    msg_type = 'Unknown'
    while (next_byte < total_length):

        # Catch the end of a response
        if (iobuffer[next_byte] == ord(b'e')):
            next_byte += 3
            if (msg_type == 'H'):
                print sensor_dictionary
            continue

        # Cach message type
        if (iobuffer[next_byte] == ord(b't')):
            msg_type = chr(iobuffer[next_byte + 2])
            next_byte += 3
            continue

        length = iobuffer[next_byte+1]
        #print 'sublen', length
        start = next_byte+2
        data = str(iobuffer[start:start+length])
        #print iobuffer[next_byte], data

        # Process a value from 'Hello'
        if (msg_type == 'H'):
            if (iobuffer[next_byte] == ord(b'i')):
                strobe_interval = int(data)
            else:
                sensor_dictionary[chr(iobuffer[next_byte])] = data
            next_byte += (2 + length)
            continue

        # Process a value from 'Sensor'
        if (msg_type == 'S'):
            print sensor_dictionary[chr(iobuffer[next_byte])] + ": " + data
            next_byte += (2 + length)
            continue
            
        # Not able to match the type
        next_byte += (2 + length)
    
    # Back to caller
    print "----"
    return (iobuffer)


# This is our main method
def main():

    # Get a context to the bus. NOTE: Must be bus #1 for 512MB PI
    bus = smbus.SMBus(1)

    # Send hello
    send_hello(bus, 4, 10)

    # Enter read loop
    sensor_dictionary = {}
    while True:

        # Wait 2 seconds
        time.sleep(2)
        io_buffer = read_packet(bus, 4, 10, 30, sensor_dictionary)

# Check if we're being run straight from the shell
if __name__ == '__main__':
    main()
