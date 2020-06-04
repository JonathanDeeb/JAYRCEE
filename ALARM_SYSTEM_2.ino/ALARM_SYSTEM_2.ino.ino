#define BLYNK_PRINT Serial                        //Alarm System by JAYRCEE
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <HCSR04.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4r8SaDm2rLWYNTTRQs4yGVR39t5kzZf-";       //Put your own Authentication Token

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Jonathan";                               //Put your WiFi SSID(name)
char pass[] = "nointernet";                             //Put your WiFi Password                            

UltraSonicDistanceSensor distanceSensor(D13, D12);       // Initialize sensor that uses digital 
                                                    //pins D13 For Trig and D12 for Echo .
int sirenrelay=D2;                                  // Digital Pin that has the siren relay connected to it         
int pushbutton=D7;                                 //Digital Pin connected to Push button
int buzzer=D8;                                     //Digital Pin connected to Buzzer
int led=D9;                                        //Digital Pin connected to led
WidgetLED led1(V1);                                //Virtual Pin connected to the LED Widget                            
int buttonState;
int lastButtonState;
const uint32_t debounceTime = 5;  // 5 mSec, enough for most switches
const bool switchOn  = false;     // using INPUT_PULLUP
const bool switchOff = true;
bool lastState   = switchOff;
bool newState    = switchOff;
bool toggleState = false;

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(buzzer,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(sirenrelay,OUTPUT);
  pinMode(pushbutton,INPUT_PULLUP);
}
void loop()
{
   Serial.println(distanceSensor.measureDistanceCm());
   Blynk.run();
   
   newState = digitalRead( pushbutton );

  if( lastState != newState ) // state changed
  {
    delay( debounceTime );
    lastState = newState;
   
    // push on, push off
    if( newState == switchOn && toggleState == false )
    {
      toggleState = true;
      Serial.println( F ( "Switched ON" ) );
      delay(5000);                                      //Time Before the Alarm is Armed
      Serial.println( F ( "Alarm System Armed" ) );   
    }
    else if( newState == switchOn && toggleState == true )
    {
      toggleState = false;
      Serial.println( F ( "Switched OFF" ) );
      digitalWrite(sirenrelay,LOW);
      digitalWrite(led,HIGH);
      led1.on();
    }
  }
  if (toggleState == true)
  { 
    intrusion();
    digitalWrite(led,LOW);
  }
}

void intrusion(){
  
  Serial.println("Intrusion System is Turned ON");
  Serial.println(distanceSensor.measureDistanceCm());
  if(distanceSensor.measureDistanceCm()<=50 &&distanceSensor.measureDistanceCm()!=-1)
  {
    Blynk.notify("There is an intrusion");
    digitalWrite(sirenrelay,HIGH);
    for(int i=0;i<=5;i++)
    {
    digitalWrite(buzzer,HIGH);
    digitalWrite(led,HIGH);
    led1.on();
    delay(300);
    digitalWrite(buzzer,LOW);
    digitalWrite(led,LOW);
    delay(300);
    led1.off();
    }
  }
  digitalWrite(sirenrelay,LOW);
  
}
