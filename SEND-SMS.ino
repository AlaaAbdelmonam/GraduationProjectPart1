#include <DFRobot_SIM7000.h>

#define PIN_TX     7
#define PIN_RX     8
SoftwareSerial     mySerial(PIN_RX,PIN_TX);
DFRobot_SIM7000    sim7000;

void setup() {
  int signalStrength;
    Serial.begin(9600);
    while(!Serial);
    sim7000.begin(mySerial);
    Serial.println("Turn ON SIM7000......");
    if(sim7000.turnON()){                             //Turn ON SIM7000
        Serial.println("Turn ON !");
    }

    Serial.println("Set baud rate......");
    while(1){
        if(sim7000.setBaudRate(9600)){               //Set SIM7000 baud rate from 115200 to 19200 reduce the baud rate to avoid distortion
            Serial.println("Set baud rate:19200");
            break;
        }else{
            Serial.println("Faile to set baud rate");
            delay(1000);
        }
    }
     Serial.println("Check SIM card......");
    if(sim7000.checkSIMStatus()){                                //Check SIM card
        Serial.println("SIM card READY");
    }else{
        Serial.println("SIM card ERROR, Check if you have insert SIM card and restart SIM7000");
        while(1);
    }


    Serial.println("Set net mode......");
    while(1){
        if(sim7000.setNetMode(GPRS)){                              //Set net mod GPRS
            Serial.println("Set GPRS mode");
            break;
        }else{
            Serial.println("Fail to set mode");
            delay(1000);
        }
    }

    Serial.println("Get signal quality......");
    delay(500);
    signalStrength=sim7000.checkSignalQuality();                 //Check signal quality from (0-30)
    Serial.print("signalStrength =");
    Serial.println(signalStrength);
    delay(500);

    Serial.println("Attaching service......");
    while(1){
        if(sim7000.attacthService()){                            //Open the connection
            Serial.println("Attach service");
            break;
        }else{
            Serial.println("Fail to Attach service");
            delay(1000);
        }
    }

    sendSMS();
}


void loop() {
    mySerial.listen();
    while(mySerial.available()){
        Serial.write(mySerial.read());
    }
    mySerial.flush();
    while(Serial.available()){
        mySerial.write(Serial.read());
    }
}
void sendSMS() {
  // AT command to set SIM900 to SMS mode
  mySerial.print("AT+CMGF=1\r"); 
  delay(100);
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  mySerial.println("AT+CMGS=\"+201120842065\""); 
  delay(100);
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  mySerial.println("NBIOT"); 
  delay(100);
  // End AT command with a ^Z, ASCII code 26
  mySerial.println((char)26); 
  delay(100);
  mySerial.println();
  // Give module time to send SMS
  delay(5000); 
}
