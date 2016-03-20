#define encoder0PinA 2
#define encoder0PinB 3

//PID Values

float Kp=4;
float Ki=0;
float Kd=0.4;

int Thresh=10;//The  Min OCR_Value For The Motor



float target = 360;//the Set Point.


float error =0 , last_error = 0;

volatile double t =0 , lt = 0 ,dt =0 ;//Time

float integral=0;

float diff=0;

float pid =0;

volatile double Angle;
volatile int pwm;

int i =1;


int MotorDir=8;
int MotorBrk=10;
int MotorPwm=9;





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
  
  last_error = error;
  error = target - Angle;
  
  integral = integral + error * dt;
  
  diff = (error - last_error)/dt;

   
  pid = Kp*error + Ki*integral + Kd*diff;
  
  
  pwm = (int)(constrain(pid,-220,220));
  
  pwm = pwm * (-1);

  
  if (abs(pwm) < Thresh)
     {
       pwm = 0;
     } 
  
}


void setup() {
  
  pinMode(MotorDir,OUTPUT);
  pinMode(MotorBrk,OUTPUT);
  pinMode(MotorPwm,OUTPUT);

  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT); 

  
// encoder pin on interrupt 0 (pin 2)
// encoder pin on interrupt 1 (pin 3)

  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE); 
 
  Serial.begin (115200);
  
}

void loop()
  
  { 
    t = millis()/1000.0;
    
    PID();
    
    Forward (pwm);
    Serial.print (" pos  ");
    Serial.print (Angle);
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
     
    
  }



void doEncoderA()
{
 // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH)
  { 
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW)
    {  
      Angle = Angle + 0.2;         // CW
    } 
    else
    {
      Angle = Angle - 0.2;         // CCW
    }
  }
  
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinB) == HIGH) 
    {   
      Angle = Angle + 0.2;          // CW
    } 
    else
    {
      Angle = Angle - 0.2;          // CCW
    }
  }
            
  // use for debugging - remember to comment out
}


void doEncoderB()
{

  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) 
  {   
   // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH)
    {  
      Angle = Angle + 0.2;         // CW
    } 
    else
    {
      Angle = Angle - 0.2;         // CCW
    }
  }
  // Look for a high-to-low on channel B
  else
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinA) == LOW)
    {   
      Angle = Angle + 0.2;          // CW
    } 
    else 
    {
      Angle = Angle - 0.2;          // CCW
    }
  }
}
