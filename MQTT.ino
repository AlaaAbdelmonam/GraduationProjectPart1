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
        if(sim7000.setBaudRate(19200)){               //Set SIM7000 baud rate from 115200 to 19200 reduce the baud rate to avoid distortion
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
        if(sim7000.setNetMode(NB)){                              //Set net mod GPRS
            Serial.println("Set LTE mode");
            break;
        }else{
            Serial.println("Fail to set mode");
            delay(1000);
        }
    }

    MQTT_SUB();
}


void updateSerial() {
  delay(500);
    mySerial.listen();
    while(mySerial.available()){
        Serial.write(mySerial.read());
    }
    mySerial.flush();
    while(Serial.available()){
        mySerial.write(Serial.read());
    }
}

void loop(){
  updateSerial();
  }
  
void MQTT_SUB() {
  
   mySerial.print("AT+CSQ\r"); //Check RF signal
   updateSerial();
  
   mySerial.print("AT+CGREG?\r"); //Check PS service
   updateSerial();
  
   mySerial.print("AT+COPS?\r"); //Query Network information, operator and network mode 
   updateSerial();
  
   mySerial.print("AT+CNACT?\r"); //Get local IP
   updateSerial();
 
   mySerial.print("AT+SMCONF=\"URL\",\"broker.hivemq.com\",\"1883\"\r"); //Set up server URL
   updateSerial();
 
   mySerial.print("AT+SMCONF=\"KEEPTIME\",60\r"); ///Set MQTT time to connect server
   updateSerial();

  mySerial.print("AT+SMCONN\r");
  delay(10000);
  updateSerial(); 
 
   mySerial.print("AT+SMSUB=\"intake40\",1\r"); //Subscription packet
   updateSerial();

   mySerial.print("AT+SMPUB=\"intake40\",\"5\",1,1\r"); //Send packet ,Get data on server
    delay(10000);
   updateSerial();

}
