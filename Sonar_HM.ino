#include <avr/io.h>
//#define F_CPU 16000000UL
#include <avr/interrupt.h>


#define Trig 24
#define Echo 21

volatile int ovf=0;
volatile float distance=0;
volatile int flag=0;
volatile float temp;
volatile int a=0;



ISR(TIMER3_OVF_vect)
{
  ovf++;
  
}


ISR(INT0_vect)
{

  temp=(ovf*65536.0)+TCNT3;
  distance=(float)((temp*16300.0)/16000000.0);

//delay(10);
  digitalWrite(Trig,LOW);
 delayMicroseconds(2);
  digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
 
  ovf=0;
TCNT3=0;
 }


void setup() {
  // put your setup code here, to run once:
pinMode(Trig,OUTPUT);
pinMode(Echo,INPUT);

  Serial.begin(115200);
  TCCR3A=0;
  TCCR3B=(1<<CS30);//Prescaler=0.
  TIMSK3=(1<<TOIE3);//OVF Interrupt Enable.
  
  EIMSK=(1<<INT0);
  EICRA=(1<<ISC01);//Falling Edge Interrupt Set.
  sei();
  
  digitalWrite(Trig,LOW);
 delayMicroseconds(2);
   digitalWrite(Trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig,LOW);
  ovf=0;
  TCNT3=0;

}

void loop() {
  // put your main code here, to run repeatedly:
 
 Serial.print("DISTANCE"); 
 Serial.print("     ");
Serial.print(distance);
Serial.print("     ");Serial.print("OVF");
Serial.print("     ");
Serial.print(ovf);
Serial.print("     ");
Serial.println(temp);

}


