#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>


#define encoder0PinA 19

#define encoder0PinB 23



float target = 30;
float error =0 , le = 0;
volatile double t =0 , lt = 0 ,dt =0 ;
float integral=0;
float diff=0;
float pid =0;
volatile double encoder0Pos;
int i =1;
volatile int in;


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
  error = target - encoder0Pos;
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
  //sei();
  EIMSK = EIMSK | (1<<INT2);
  EICRA = EICRA | (1<<ISC20);//|(1<<ISC21);
  sei();
  
   pinMode(MotorDir,OUTPUT);
  pinMode(MotorBrk,OUTPUT);
  pinMode(MotorPwm,OUTPUT);

  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT); 
  
  
   Serial.begin (115200);
  
  
  // put your setup code here, to run once:

}

void loop() 
{
  
   t = millis()/1000.0;
    
    
    PID();
    
    Forward (pwm);
    Serial.print (" pos  ");
    Serial.print (encoder0Pos);
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

ISR (INT2_vect)
{
  in = digitalRead(encoder0PinB);
  
  if (digitalRead(encoder0PinA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (in== 0) {  
      encoder0Pos = encoder0Pos + 0.4;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 0.4;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (in== 1) {   
      encoder0Pos = encoder0Pos + 0.4;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 0.4;          // CCW
    }
  }
  
	
}


