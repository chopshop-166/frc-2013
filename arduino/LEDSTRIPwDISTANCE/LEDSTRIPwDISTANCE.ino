
 #include <NewPing.h>
#define trigPin 10
#define echoPin 9
#define REDPIN 3
#define GREENPIN 6
#define BLUEPIN 11
int  distance;
NewPing sonar(trigPin, echoPin);
void setup() {
  Serial.begin(9600);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  analogWrite(REDPIN,254);//First logo colours! 
  delay(2500);
  analogWrite(REDPIN,255);
  analogWrite(GREENPIN, 255);
  analogWrite(BLUEPIN, 255);
  delay(2500);
  analogWrite(REDPIN,4);
  analogWrite(GREENPIN, 60);
  analogWrite(BLUEPIN, 145);
  delay(2500);

}
 
 
void loop() {
   int r, g, b;
   digitalWrite(trigPin, HIGH);
   delay(100);
   digitalWrite(trigPin, LOW);
   distance = sonar.ping_in();
   Serial.print(distance);
   Serial.println(" arbitrary units");
   if(distance==0)
   distance=37;//when the object is too far away, the sensor displays a 0
  //every distance unit is 1 inch
if(distance < 12){
   analogWrite(REDPIN, 205);
   analogWrite(GREENPIN, 0);
   analogWrite(BLUEPIN, 0);}

if(distance < 24 && distance >= 12){ 

   analogWrite(GREENPIN, 200); 
   analogWrite(REDPIN, 255);
   analogWrite(BLUEPIN, 0);}
    
if(distance < 36 && distance >= 24){

    analogWrite(BLUEPIN, 255);
    analogWrite(REDPIN, 0);
    analogWrite(GREENPIN, 0);}
    
if(distance >= 48){  
    analogWrite(BLUEPIN, 0);
    analogWrite(REDPIN, 0);
    analogWrite(GREENPIN, 255);}
    
  
delay(100);
}
