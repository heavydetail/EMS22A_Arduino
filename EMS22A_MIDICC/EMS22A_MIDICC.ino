#include <MIDI.h>

const int PIN_CS = 5;
const int PIN_CLOCK = 6;
const int PIN_DATA = 7;

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
  
  int pos = 0;
  for (int i=0; i<10; i++) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);
   
    byte b = digitalRead(PIN_DATA) == HIGH ? 1 : 0;
    pos += b * pow(2, 10-(i+1));
  }

  for (int i=0; i<6; i++) { //READ STATUS BITS
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);
    
    Serial.print(digitalRead(PIN_DATA));
  }
  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_CLOCK, HIGH);
  
  Serial.print(" ");
  Serial.print(pos);
  Serial.println("");
  
  //MAP TO MIDI 127
  int analogValue = map(pos, 0, 1023, 0, 127);

  // potentiometer could be too sensitive and
  // give different (+-1) values.
  // send CC only when the difference between last value
  // is more than 1 
  
  if ((analogValue-lastAnalogValue) > 1 || (analogValue-lastAnalogValue) < -1) {
    // value changed?
    if (analogValue != lastAnalogValue) {
      // send serial value (ControlNumber 1, ControlValue = analogValue, Channel 1)
      // more info: http://arduinomidilib.sourceforge.net/a00001.html
      
      //MIDI.sendControlChange(1, analogValue, 1); //Uncomment for MIDI!!!!!!!
      
      lastAnalogValue = analogValue;
    }
  }
}
