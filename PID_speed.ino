
#include "TimerOne.h"
#define encod_a 2
#define encod_b 3
#define k_p 1.0
#define k_i 1.0

#define set_value 5

int ticks=0;
int MotorDir=8;
int MotorBrk=10;
int MotorPwm=9;

float rpm=0,error,pid=0,integral;

void setup() {
  pinMode(MotorDir,OUTPUT);
  pinMode(MotorBrk,OUTPUT);
  pinMode(MotorPwm,OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  attachInterrupt(digitalPinToInterrupt(encod_a),inc_ticks,CHANGE);
  attachInterrupt(digitalPinToInterrupt(encod_b),inc_ticks,CHANGE);
  Timer1.attachInterrupt(evaluate,2000000);
  digitalWrite(9,HIGH);
  Serial.begin(9600);
}

void loop() {
  error = abs(set_value)-abs(rpm);
  integral +=error;
  pid= k_p*error +k_i*integral;
  pid = set_value+error;
   
  constrain(pid,0,10);
  map(pid,0,10,0,255);
  
  //analogWrite(m_1,pid);
  //analogWrite(m_2,0);
  Forward(pid);
  
}

void inc_ticks(){
  ticks++;
}

void evaluate(){
  rpm = ticks*60.0*0.8/360;
  ticks = 0;
  Serial.println("Speed:");
  Serial.print(rpm);
  
}

void Forward(int OCR_Value)
{
  if(OCR_Value>0)
  {
  digitalWrite(MotorDir,LOW);
  digitalWrite(MotorBrk,LOW);
  analogWrite(MotorPwm,OCR_Value);
  }

  if(OCR_Value<0)
 {
  digitalWrite(MotorDir,HIGH);
  digitalWrite(MotorBrk,LOW);
  analogWrite(MotorPwm,abs(OCR_Value));
 }

}

void Brake()
{
    digitalWrite(MotorBrk,HIGH);
    analogWrite(MotorPwm,0);
}
