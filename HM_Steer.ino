#define encoder0PinA 2

#define encoder0PinB 3

volatile double Angle_Measured = 0;

int MotorDir=8;
int MotorBrk=10;
int MotorPwm=9;

volatile int OCR_Value=0; 

//int ticks=0;
//float rpm;
int i=1;

void setup() {
  // put your setup code here, to run once:
  pinMode(MotorDir,OUTPUT);
  pinMode(MotorBrk,OUTPUT);
  pinMode(MotorPwm,OUTPUT);
  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT); 
// encoder pin on interrupt 0 (pin 2)

  attachInterrupt(0, doEncoderA, CHANGE);
// encoder pin on interrupt 1 (pin 3)

  attachInterrupt(1, doEncoderB, CHANGE);  
  Serial.begin (115200);

for(int i=0;i<255;i++)
{
  Forward(i);
  Serial.println (Angle_Measured);
  delay(20); 
}
/*
for(int i=255;i>-255;i--)
{
  Forward(i);
  Serial.println (Angle_Measured);
  delay(20); 
}
for(int i=-255;i<=0;i++)
{
  Forward(i);
  Serial.println (Angle_Measured); 
  delay(20);
}


*/
while(i)
{
   Forward(-200);

  Serial.println(Angle_Measured);
  delay(20);
    if(Angle_Measured>abs(360))
  {
    i=0;
    Brake();
      
  }
 
}
}


void loop() {
  // put your main code here, to run repeatedly:
  
  //delay(10);
 
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

void doEncoderA(){

  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {  
      Angle_Measured = Angle_Measured + 0.8;         // CW
    } 
    else {
      Angle_Measured = Angle_Measured - 0.8;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinB) == HIGH) {   
      Angle_Measured = Angle_Measured + 0.8;          // CW
    } 
    else {
      Angle_Measured = Angle_Measured - 0.8;          // CCW
    }
  }
  // Serial.println (Angle_Measured);         
  // use for debugging - remember to comment out
}

void doEncoderB(){

  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {  
      Angle_Measured = Angle_Measured + 0.8;         // CW
    } 
    else {
      Angle_Measured = Angle_Measured - 0.8;         // CCW
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinA) == LOW) {   
      Angle_Measured = Angle_Measured + 0.8;          // CW
    } 
    else {
      Angle_Measured = Angle_Measured - 0.8;          // CCW
    }
  }
}

