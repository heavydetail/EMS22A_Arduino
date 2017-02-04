#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

const int PIN_CS = 5;
const int PIN_CLOCK = 6;
const int PIN_DATA = 7;

int magnitude = 0;

int pos;
int oldPos = 0;

int upperThrs = 1000;
int lowerThrs = 24;

int lastAnalogValue = 128;

byte stream[16] = {0};//replace testStream with stream for actual data
//byte testStream[] = {1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}; //test data stream
//byte testStream[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}; //test data stream

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

void loop() {
  //READ BOURNS
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);

  stream[16] = {0};
  for (int i = 0; i < 16; i++) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);

    stream[i] = digitalRead(PIN_DATA);
  }

  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_CLOCK, HIGH);

  oldPos = pos;
  
  //extract 10 bit position from data stream use testStream
  pos = 0; //clear previous data
  for (int i = 0; i < 10; i++) {
    pos = pos << 1;
    //pos += testStream[i]; //replace testStream with stream
    pos += stream[i];
  }

  //TRANSITION TESTING!?
  if(oldPos > upperThrs && pos < lowerThrs) magnitude++;
  if(oldPos < lowerThrs && pos > upperThrs) magnitude--;

  //print position 0-1023
  //Serial.print(pos, BIN);
  //Serial.print('\t');
  //Serial.print(magnitude);
  //Serial.print('\t');
  //Serial.println(pos);

  //MAP TO MIDI 127
  int analogValue = map(pos, 0, 1023, 0, 127); //1023

  // potentiometer could be too sensitive and
  // give different (+-1) values.
  // send CC only when the difference between last value
  // is more than 1 
  
  if ((analogValue-lastAnalogValue) > 1 || (analogValue-lastAnalogValue) < -1) {
    // value changed?
    if (analogValue != lastAnalogValue) {
      // send serial value (ControlNumber 1, ControlValue = analogValue, Channel 1)
      // more info: http://arduinomidilib.sourceforge.net/a00001.html
      
      MIDI.sendControlChange(1, analogValue, 1); //Uncomment for MIDI!!!!!!!
      
      lastAnalogValue = analogValue;
    }
  }

  //extract 6 bits of status
  //for (int i = 10; i < 16; i++)
    //Serial.println(testStream[i]);//replace testStream with stream
    //Serial.println(stream[i]);
  //delay(500);
}
