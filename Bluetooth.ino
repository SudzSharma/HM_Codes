

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
volatile int b_angle=0;
volatile int b_dac=0;
//String str1="";
//String str2="";
int switch_flag=0;//1-for forward,0-backward
void setup() {
  // initialize serial:
 Serial3.begin(9600);
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
   Serial.print(inputString);
    // clear the string:
    getdata();
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent3() {
  while (Serial3.available()) {
    // get the new byte:
    char inChar = (char)Serial3.read();
    
        delay(5);

    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    
    
  }
        stringComplete = true;

}



void getdata()
{   String str1 = "", str2 = "", str = "";
    int i, s_len = 0;
    int cmd_flag = 0;
    s_len = inputString.length();

    for(i=0; i <= s_len; i++)
    {
       str += inputString[i];
       if(inputString[i] == ' ')
       {
          cmd_flag = 1;
       }
    }

    if(cmd_flag == 0)
    {
      if(str == "stop")
      {
        b_angle=0;
        Serial.println("   GetSetStop  ");
        b_dac=0;
        //Disable Drive Motor.
      }
      else if(str == "start")
      {
        b_angle=0;
        Serial.println("   GetSettart  ");
        b_dac=80;
        //Enable Drive Motor.
      }
    }
    else if(cmd_flag == 1)
    {
        for(i=0; inputString[i] != ' '; i++)
        {
          str1 += inputString[i];
        }
        i++;
        
        for(; i <= s_len; i++)
        {
           str2 += inputString[i]; 
        }
        
        if(str1=="left")
        {
           b_angle = str2.toInt();
           b_angle=-b_angle;
         //Serial.print("   Angle ");Serial.print(b_angle);
        }
        else if(str1=="right")
        { 
            b_angle=str2.toInt();
          //Serial.print("   Angle ");Serial.print(b_angle);
        }  
        else if(str2 == "forward")
        {
           b_angle=0;
           switch_flag=1;
           if(str1=="move")
              b_dac=85;       
           if(str1=="slow")
              b_dac=80;
           if(str1=="fast")
              b_dac=90;
        }
        else if(str2=="reverse")
        {
            b_angle=0;
            switch_flag=0;
            if(str1=="move")
              b_dac=80;
        }
        
    }  
      
    Serial.print("   Angle ");Serial.print(b_angle);
    Serial.print("   DAC = ");Serial.print(b_dac);
    Serial.print("   Str1 =  ");Serial.print(str1);
    Serial.print("   Str2 =  ");Serial.println(str2);
}
