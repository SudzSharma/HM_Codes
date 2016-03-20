char  Keychain[14]="0900910720BF0";

void setup() {
 
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  Serial1.begin(9600);

}
volatile int cnt=0;
char RFID_data[14];
//int RFID_digit
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(3,HIGH);
  

while(Serial1.available())
{
  //Serial.print(Serial1.read());
  RFID_data[cnt]=(char)Serial1.read();
  cnt++;
}
if(cnt==12)
{
  RFID_data[12]='0';
  RFID_data[13]='\0';
Serial.println(RFID_data);
cnt=0;

if(!strcmp(Keychain,RFID_data))
{
  Serial.println("Keychain Found");
}

}


}
