 #include <Wire.h> 

#define address 0x1E //0011110b, I2C 7bit address of HMC5883


const int G_SENS=131;          //a division factor that directly gives omega calculated
const float dt=0.0001;//derivative time interval

float y_old_theta=0,y_theta=0;       
float y_sum_theta=0;float y_diff;

float z_old_theta=0,z_theta=0;       
float z_sum_theta=0;float z_diff;


const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
void setup(){
  Wire.begin();
  Serial.begin(9600);
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
    
  Wire.beginTransmission(address);
  Wire.write(0x02); //select mode 
  Wire.write(0x00); //CM mode
  Wire.endTransmission();

  Wire.beginTransmission(address);
  Wire.write(0x01);  
  Wire.write(0b00100000); //1.3 gauss
  Wire.endTransmission();
}
 
 uint8_t buffer[6];

  void loop(){
  
  int x,y,z; 
  
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Wire.endTransmission();

 int GYRZ=(float)GyZ/G_SENS;          //GYRY is the omega about y axis          
  //G_SENS is factor that directly gives the omega from raw data of gyroscope in IMU          
  
  


  data_read();
 
  x = (buffer[0] << 8) | buffer[1];
  z = (buffer[2] << 8) | buffer[3];
  y = (buffer[4] << 8) | buffer[5];
 
  x =x *0.82;
  y =y *0.82;
  z =z *0.82;
  
  float heading = atan2(y,x);

  // Correct for when signs are reversed.
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;

  heading = heading*(180/PI);
  

  
  
  z_theta=0.85*(z_old_theta+GYRZ*dt)+0.15*(heading);//complimentary filter                                                         
  z_old_theta=z_theta;
    Serial.println(z_theta);
  
  delay(25);
}

void data_read()
{
   Wire.beginTransmission(address);
 Wire.write(0x03);
 Wire.endTransmission();
 
 Wire.beginTransmission(address);
 Wire.requestFrom(address, 6);

 if(Wire.available() == 6)
 {
     for(uint8_t i = 0; i < 6; i++)
     {
         buffer[i] = Wire.read();
     }
 }
 Wire.endTransmission();
}

void self_test()
{
  Wire.beginTransmission(address
}

