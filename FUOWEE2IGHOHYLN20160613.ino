

/*
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS/SDA: Pin 10
* RST: Pin 9
*/
#include <Wire.h>
#include <SPI.h>
#include <RFID.h>
//#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN,RST_PIN);

uint8_t Str4Transfer[8];  //kph 1 bytes,Travel_dist 3 bytes,Step_Force 1 byte,rear wheel duration 2 bytes
uint8_t StrReceived[14];
int queue_num = 0;
char Requesting;
int led = 7;
int power = 8; 
int serNum[5];
int serNum_test[5];
int q_num=0;
int cards[][5] = {
  {69,90,156,99,224},
  {111,213,21,157,50},
  {35,201,121,188,47},
  {144,64,59,121,146}
};

bool access = false;

void setup(){

    Serial.begin(9600);
    SPI.begin();
    rfid.init();

    pinMode(led, OUTPUT);

    digitalWrite(led, LOW);

    Wire.begin(8);                // join i2c bus with address #8
    Wire.onRequest(requestEvent); // register event
    Wire.onReceive(receiveEvent);
   
}
void receiveEvent(int howMany)
{
  while (Wire.available())
  {
    // receive one byte from Master
    Requesting = Wire.read();
    Serial.println(Requesting);
  }
}

void requestEvent() {
  Wire.write(q_num);
  //Serial.print("Str4Transfer=");
  //Serial.println(Str4Transfer);
  
//  if (Requesting == 'P'){
//    Wire.write(&Step_Force[0],8); // respond with message of 6 bytes
//  } 
//  else if (Requesting == 'R'){
//     Wire.write(&Reed_Data[0],8);
//    
//  }

}


void loop(){
    
    if(rfid.isCard()){
    
        if(rfid.readCardSerial()){
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println("");

            for(int i= 0; i<5 ; i++){
              Str4Transfer[i]= (uint8_t)(rfid.serNum[i]);
              
            }
            
            for(int x = 0; x < sizeof(cards); x++){
              for(int i = 0; i < sizeof(rfid.serNum); i++ ){
                  if(rfid.serNum[i] != cards[x][i]) {
                      access = false;
                      break;
                  } else {
                      access = true;  

                  }
              }
              if(access) break;
            }
           
        }
        
       if(access){
          Serial.println("Welcome!"); 
          if(serNum_test[q_num] != rfid.serNum[q_num] ){
          q_num=q_num+1;
          Serial.print("queueing number:"); 
          Serial.print(" ");
          Serial.println(q_num);
          serNum_test[q_num] = rfid.serNum[q_num];             
          } else {
           q_num=q_num-1;
          Serial.print("queueing number:"); 
          Serial.print(" ");
          Serial.println(q_num);
          }
          digitalWrite(led, HIGH); 
          delay(1000);
          digitalWrite(led, LOW);
          digitalWrite(power, HIGH);
          delay(1000);
          digitalWrite(power, LOW);
      } else {
           Serial.println("Not allowed!"); 
           digitalWrite(led, HIGH);
           delay(500);
           digitalWrite(led, LOW); 
           delay(500);
           digitalWrite(led, HIGH);
           delay(500);
           digitalWrite(led, LOW);         
       }        
    }
    
    rfid.halt();
    
    }

