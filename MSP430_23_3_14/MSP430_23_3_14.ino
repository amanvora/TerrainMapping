#include <SoftwareSerial.h>
SoftwareSerial ZigBee_Serial(P1_4, P1_3); // RX, TX
int GPRMC_data_flag=0,i=0,j=0,k=0;
int sx1,sx,sy,sz,normal_flag;
char Serial_data,control,stored_char,GPRMC_data[53]={0};
char ZigBee_receive[17]={0};
char charecteristic_detected,inconsistency_ahead;
void setup()
{
  pinMode(2,OUTPUT);    // L293D motor control 
  pinMode(8,OUTPUT);    // P1.0 & P2.0 for one motor
  pinMode(9,OUTPUT);    // P2.1 & P2.2 for the other motor
  pinMode(10,OUTPUT);

  pinMode(7,INPUT);     // Accelerometer pins
  pinMode(14,INPUT);
  //pinMode(15,INPUT);

  pinMode(11,OUTPUT);   // Decoder select pins
  pinMode(12,OUTPUT);   // P2.3, P2.4 & P2.5
  pinMode(13,OUTPUT);   // 13 is MSB and 11 is LSB  
  Serial.begin(9600);
  ZigBee_Serial.begin(9600);
}

void motor_control()
{
  if(ZigBee_Serial.available())
  {
    control = ZigBee_Serial.read();
  }
  switch(control)
    {
    case 'S':
      digitalWrite(2,LOW);    // Stop vehicle
      digitalWrite(8,LOW);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      break;
    case 'L':
      digitalWrite(2,LOW);    // Take left
      digitalWrite(8,LOW);
      digitalWrite(9,HIGH);
      digitalWrite(10,LOW);
      break;
    case 'R':
      digitalWrite(2,LOW);    // Take right
      digitalWrite(8,HIGH);
      digitalWrite(9,LOW);
      digitalWrite(10,LOW);
      break;
    case 'F':
      digitalWrite(2,LOW);    // Go front
      digitalWrite(8,HIGH);
      digitalWrite(9,HIGH);
      digitalWrite(10,LOW);
      break;
    }
}

void ZigBee_transmit()
{
    ZigBee_Serial.print(GPRMC_data);
    ZigBee_Serial.print(",");
    ZigBee_Serial.print(charecteristic_detected);
    ZigBee_Serial.print("\r");
}

void warning_leds()
{
  if(ZigBee_Serial.available())
  {
    inconsistency_ahead = ZigBee_Serial.read();
  }
    switch(inconsistency_ahead)
    {
    case 'h':
      digitalWrite(13,LOW);  // Select pins : 000b
      digitalWrite(12,LOW);
      digitalWrite(11,LOW);
      break;
    case 'p':
      digitalWrite(13,LOW);  // Select pins : 001b
      digitalWrite(12,LOW);
      digitalWrite(11,HIGH);
      break;
    case 'e':
      digitalWrite(13,LOW);  // Select pins : 010b
      digitalWrite(12,HIGH);
      digitalWrite(11,LOW);
      break;
    case 'r':
      digitalWrite(13,LOW);  // Select pins : 011b
      digitalWrite(12,HIGH);
      digitalWrite(11,HIGH);
      break;
    case 'l':
      digitalWrite(13,HIGH);  // Select pins : 100b
      digitalWrite(12,LOW);
      digitalWrite(11,LOW);
      break;
    default:
      digitalWrite(13,HIGH);  // Select pins : 101b
      digitalWrite(12,LOW);
      digitalWrite(11,HIGH);
    }
}   


void inconsistency_detection()
{
  normal_flag = 1; 
  sx = analogRead(A5);   //P1_5//blue
  sy = analogRead(A6);   //P1_6//yellow
  //sz = analogRead(A7);   //P1_7//
  //+5 red GND Black
  // Serial.println(sx);
  // Serial.println(sy);
  // Serial.println(sz);     // test points

  if(sx>380|| sx<355)
  {
    delay(1800);
    sx1 = analogRead(A5);
  if((sx - sx1)>20)
      {
        Serial.print("h");
        charecteristic_detected = 'h';
        normal_flag = 0;
      } 
   if((sx1 - sx)>20)
      {
        Serial.print("p");
        charecteristic_detected = 'p';
        normal_flag = 0;
      }
if(((sx-sx1)<20 && (sx-sx1)>0) || (((sx1-sx)<20)&&((sx1-sx)>0)))
    {  Serial.print("e");
      charecteristic_detected = 'e';
      normal_flag = 0;
    } 
  }
  
  if(sy>380)
  {
    Serial.print("l");
    charecteristic_detected = 'l';
    normal_flag = 0;
  }
  if(sy<350)
  {
    Serial.print("r");
    charecteristic_detected = 'r';
    normal_flag = 0;
  }
  if(normal_flag == 1)
  {
    Serial.print("n");
    charecteristic_detected = 'n';
  }
  //charecteristic_detected: R,L,E,N
  //delay(500);
}

void loop()
{
  //digitalWrite(2,HIGH);
  if(Serial.available())
  {
    Serial_data=Serial.read();
    if(Serial_data=='R')
      GPRMC_data_flag = 1;
    if(GPRMC_data_flag == 1)
    {
      GPRMC_data[i++] = Serial_data;
    }  
    if(i>40)
    {
      Serial.print(GPRMC_data);
      inconsistency_detection();
      Serial.print(",");
      Serial.print(charecteristic_detected);
      ZigBee_transmit();
      Serial.println();
      GPRMC_data_flag = 0;
      i=0;
    }
      motor_control();
      warning_leds(); 
    }
}

