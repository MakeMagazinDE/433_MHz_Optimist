//**** Funk *****************************************************************************
//**** https://github.com/cano64/ManchesterRF *******************************************
#include <Manchester.h>
#define TX_PIN 0  //pin where your transmitter is connected

uint16_t transmit_data = 0 ;
int PWM[3];

void setup() {
//man.setupTransmit(TX_PIN, MAN_300);
man.setupTransmit(TX_PIN, MAN_1200);
}

void loop() {
PWM[0]=71;                                       //Sicherheitsabfrage
PWM[1] = analogRead(A3)/2;                       //Ruder, Faktor: Bereichsanpassung auf Anwendung

//**** über Lesen der Werte habe ich meine Ruhestellung für die Schot bei 190 gefunden. 
//**** 190+-10 genügt zum Dichtholen und Fieren.
PWM[2]= map(analogRead (A2), 0, 1023,180, 200); //Schot

//**** genau 190 zu treffen um Ruhe zu haben ist schwer. Darum die Aufweitung des
//**** Ruhebereichs um +- 3. Je nach Anwendung wird das aber ggf. nicht gebraucht.
if((PWM[2]>187) && (PWM[2]<193)){
PWM[2]=190;
}

for (int i=0; i <= 2; i++) {  
transmit_data= PWM[i];
man.transmit(transmit_data);
delay (5);  // gib dem Empfänger Zeit zum Verarbeiten der empfangenen Werte, bevor der nächste Wert raus geht.
}
delay (10);
}
