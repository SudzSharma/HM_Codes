#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

float target = 30;
float error =0 , le = 0;
volatile double t =0 , lt = 0 ,dt =0 ;
float integral=0;
float diff=0;
float pid =0;
int i =1;

int  pot_in = A7;
int a;
float angle;



int MotorDir=51;
int MotorBrk=53;
int MotorPwm=8;


volatile int pwm;


void Forward(int OCR_Value)
{
  if(OCR_Value>0)
  {
  digitalWrite(MotorDir,LOW);
  digitalWrite(MotorBrk,LOW);
    }

  if(OCR_Value<0)
 {
  digitalWrite(MotorDir,HIGH);
  digitalWrite(MotorBrk,LOW);
   }
   
 analogWrite(MotorPwm,abs(OCR_Value));

}

void Brake()
{
    digitalWrite(MotorBrk,HIGH);
    analogWrite(MotorPwm,0);
}




void PID()
{
  le = error;
  error = target - angle;
  integral = integral + error * dt;
  diff = (error - le)/dt;
  pid = 5.5*error +0.0*integral + 0.0*diff;
  
  
  pwm = (int)(constrain(pid,-220,220));
 // pwm = map(pwm,-255,255,-180,180);
  pwm = pwm * (-1);
  if (abs(pwm)<15)
     {
       pwm = 0;
     } 
  
}




void setup() 
{
  
   pinMode(MotorDir,OUTPUT);
  pinMode(MotorBrk,OUTPUT);
  pinMode(MotorPwm,OUTPUT);
  
  
   Serial.begin (115200);
  
  
  // put your setup code here, to run once:

}

void loop() 
{
  
   t = millis()/1000.0;
    
  a = analogRead(pot_in);
  
    angle = map (a,0,1023,-135,135);
   
     PID();
    
    Forward (pwm);
   
    Serial.print(a);
    Serial.print ("      ");
    Serial.print(angle);
    Serial.print("      pid  ");
    Serial.print (pid);
    Serial.print("       pwm  ");
    Serial.print (pwm);
    Serial.print("       error ");
    Serial.print(error);
    Serial.print("       dt  ");
    Serial.print(dt);
    Serial.print("       integral ");
    Serial.print(integral);
    Serial.print("       diff ");
    Serial.print(diff);
    Serial.print("       error  ");
    Serial.println(error);
    
    
    
    
    
    lt = t;
    t = millis()/1000.0;
    dt = t -lt;
   
  // put your main code here, to run repeatedly:

}


