
#include <avr/io.h>
#include <avr/interrupt.h>

int MotorDir=8;
int MotorBrk=10;
int MotorPwm=9;



int EncoderPinA=3;

int Reqovf=5;//Setting Time Interval As 20.9712ms


volatile int ovf=0;

volatile int Ticks=0;

volatile float RPM=0;

float Error=0;
float Last_Error=0;
float Integral=0;
long t=0,lt=0,dt=0;
float pid=0;


float kp=4;
float ki=0;
float kd=0;
float Speed=0;


float TimeInt(int Reqovf)
{
  
  float Timeint;
  
  Timeint=((Reqovf*65535*1024)/16000000.0);

  return Timeint;
  
}





ISR(TIMER1_OVF_vect)
{
  ovf++;
}


ISR(INT0_vect)
{
  Calculate_RPM(Ticks);
    Ticks++;//Counts The Ticks (The Angle).

}

void Calculate_RPM(int Ticks)
{
   if(ovf==Reqovf)
  {
    RPM=((Ticks*0.8*60)/(TimeInt(Reqovf)*360));//Calculates RPM  
    ovf=0;
    Ticks=0;
  }
 
 }



void setup()
{
  // put your setup code here, to run once:
  pinMode(EncoderPinA,INPUT);
  pinMode(MotorDir,OUTPUT);
  pinMode(MotorBrk,OUTPUT);
  pinMode(MotorPwm,OUTPUT);
Serial.begin(115200);

  TCCR1A=0;
  TCCR1B=(1<<CS12)|(1<<CS00);//Prescaler of 1024 to 16 bit timer.
  TIMSK1=(1<<TOIE1);//OVF Interrupt Enable.
  
  EICRB=(1<<ISC41)|(1<<ISC40);//INT0 Interrupt Enable
  EIMSK=(1<<INT4);
  
  }

void loop() 
{
  // put your main code here, to run repeatedly:
  Forward(pid);
  t=millis()/1000.0;
  Serial.print("Error     ");Serial.print(Error);
  Serial.print("     RPM     ");Serial.print(RPM);
  Serial.print("    pid     ") ;Serial.print(pid);
  Serial.print("   Ticks    ");Serial.println(Ticks);




dt=t-lt;
lt=t;

}


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



/*
void Speed_PID()
{
  
  Error=Speed-RPM;

  Integral=Integral+(Error*dt);
  
  pid=(kp*Error)+(ki*Integral)+(kd*(Error-Last_Error)/dt);

  pid=constrain(pid,-220,220);

}
*/

