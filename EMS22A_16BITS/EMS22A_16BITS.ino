#include <MIDI.h>

const int PIN_CS = 5;
const int PIN_CLOCK = 6;
const int PIN_DATA = 7;

int magnitude = 0;

int lastAnalogValue = 128;

void setup() {
  MIDI.begin(4);
  Serial.begin(115200);

  //SETUP BOURNS EMS22A
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, INPUT);

  digitalWrite(PIN_CLOCK, HIGH);
  digitalWrite(PIN_CS, LOW);
}


//byte stream[16];
void loop() {

  //READ BOURNS
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);

   for (int i=0; i<16; i++) {
   digitalWrite(PIN_CLOCK, LOW);
   digitalWrite(PIN_CLOCK, HIGH);
   Serial.print(digitalRead(PIN_DATA));
   }

  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_CLOCK, HIGH);

  Serial.println(""); 

}
