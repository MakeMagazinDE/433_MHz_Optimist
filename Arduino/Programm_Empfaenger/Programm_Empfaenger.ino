//**** Funk ********************************************************************************
//**** https://github.com/cano64/ManchesterRF **********************************************
#include <Manchester.h>
#define RX_PIN 4

//**** SerialOut ***************************************************************************
//**** https://github.com/ArminJo/ATtinySerialOut ******************************************
#include <Arduino.h>            
#include "ATtinySerialOut.hpp"
#define  TX_PIN 2                                  // beim AtTiny85 sind möglich PB0 bis PB4

//**** Zusweisung der IOs ******************************************************************
#define Ruder 1 
#define Schot 0
int i=0;

//**** Array für empfangene Werte **********************************************************
int PWM[3];
int idx = -1;

void setup() { //###########################################################################
//**** Funk ********************************************************************************
man.setupReceive(RX_PIN, MAN_1200);
man.beginReceive();

//**** SerialOut ***************************************************************************
//das klappt mit der Übertragungsrate 115200
initTXPin();
writeString("OSCCAL=");
writeUnsignedByteHexWithPrefix(OSCCAL);

//**** Zusweisung der IOs ******************************************************************
pinMode(Ruder, OUTPUT);
pinMode(Schot, OUTPUT);

//**** Array für empfangene Werte **********************************************************
PWM[0]= 0;//Sicherheitsabfrage
PWM[1]= 0;//Ruder
PWM[2]= 0;//Schot
}

void loop() {//#############################################################################
  if (man.receiveComplete()) {
  uint8_t value = man.getMessage();
  man.beginReceive();
  
    if (value == 71) {
      if(idx == 3)   {
        Serial.print(PWM[0]); Serial.print  ("; "); // Sicherheitsabfrage    
        Serial.print(PWM[1]); Serial.print  ("; "); // Ruder     
        Serial.print(PWM[2]); Serial.print  ("; "); // Schot 
        int Anschlag = analogRead (A3);             // Anschlag für die Schot
                                                    // Zustand a) 500 ... 520 Limit BUG  Schot: 180...189 ist erlaubt     !!!!!!!!!!!
                                                    // Zustand b)           0 Limit HECK Schot: 193...200 ist erlaubt     !!!!!!!!!!!  
                                                    // Zustand c)        1023 Normalbetrieb                               !!!!!!!!!!!           
        Serial.println(Anschlag);

        int Ruhe = 190;

        //**** SCHOT: Limit BUG ist erreicht, maximal gefiert, es geht nur dichter ****************
             if ((Anschlag <  500)                       &&  (PWM[2] >= 180) && (PWM[2] < 189)){
        analogWrite (Schot, PWM[2]);
        }
        else if ((Anschlag <  500)                       &&  (PWM[2] >=                   190)){
        analogWrite (Schot, Ruhe);
        }

        //**** SCHOT: Limit HECK ist erreicht, maximal dicht,  es geht nur fieren *****************
        else if ((Anschlag >  500) && (Anschlag  <  520) && (PWM[2] >= 191) && (PWM[2] < 200)){
        analogWrite (Schot, PWM[2]);
        }              
        else if ((Anschlag >  500) && (Anschlag  <  520) && (PWM[2] < 191))                   {
        analogWrite (Schot, Ruhe);
        } 
       
        //**** SCHOT: Normalbetrieb ***************************************************************
        else                                                                                   {
        analogWrite (Schot, PWM[2]);
        } 

        //**** RUDER ******************************************************************************
        if(
          (value == 71) &&                              // Sicherheitsabfrage: Sind das meine Werte?
          (PWM[1]    >    0) && (PWM[1]    <  130)      // Schot hat einen sinnvollen Wert
        ){
        //int scale= map(PWM[1], 0, 130,  0, 250); //Ruder
        //analogWrite (Ruder, scale);
        analogWrite (Ruder, PWM[1]);
        }
 
    }                           // if(idx == 3)
    idx = 0;
    }                           // if (value == 71) {
    
    if(idx>=0 && idx<=2) {
    PWM[idx] = value;
    idx = idx+1;
    }
  }                             // if (man.receiveComplete()) {
}                               // main loop
