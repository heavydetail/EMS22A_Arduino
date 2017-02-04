// include MIDI library
#include <MIDI.h>

// read digital pins number 2, 4, 7
int pushButton2 = 2;
int pushButton4 = 4;
int pushButton7 = 7;

// checks if the button is pressed
int buttonState2 = 0;
int buttonState4 = 0;
int buttonState7 = 0;

// play/stop notes in relation to buttons pressed 
int note2 = 0;
int note4 = 0;
int note7 = 0;

// read potentiometer value
int analogValue = 0;
// maximum MIDI value is 127, first/previous potentiometer
// move has to always be different than previous value
int lastAnalogValue = 128;

void setup() {
  MIDI.begin(4);
  // 115200 hairless MIDI
  Serial.begin(115200);
  pinMode(pushButton2, INPUT);
  pinMode(pushButton4, INPUT);
  pinMode(pushButton7, INPUT);
}

void loop() {
  // read state of buttons
  int buttonState2 = digitalRead(pushButton2);
  int buttonState4 = digitalRead(pushButton4);
  int buttonState7 = digitalRead(pushButton7);
  
  // Analog potentiometer
  
  // potentiometer gives values up to 1023
  // we need values not bigger than 127 
  int analogValue = analogRead(A0)/8;
  
  // potentiometer could be too sensitive and
  // give different (+-1) values.
  // send CC only when the difference between last value
  // is more than 1 
  if ((analogValue-lastAnalogValue) > 1 || (analogValue-lastAnalogValue) < -1) {
    // value changed?
    if (analogValue != lastAnalogValue) {
      // send serial value (ControlNumber 1, ControlValue = analogValue, Channel 1)
      // more info: http://arduinomidilib.sourceforge.net/a00001.html
      MIDI.sendControlChange(1, analogValue, 1);
      lastAnalogValue = analogValue;
    }
  }
  
  // Button 2
  
  // when button pressed:
  if (buttonState2 == HIGH) {
    // if note not playing
    if (note2 == 0) {
      // play note (note number, velocity, channel)
      // more info: http://arduinomidilib.sourceforge.net/a00001.html
      // MIDI notes chart http://www.phys.unsw.edu.au/jw/notes.html
      // 55 = G3, 127 = trigger note with max volume
      MIDI.sendNoteOn(55,127,1);
      // note is playing
      note2 = 1;
    }
  // when button released
  } else {
    // if note playing
    if (note2 == 1) {
      // if playing - stop
      MIDI.sendNoteOff(55,0,1);
    }
    // if button released note is off
    note2 = 0;
  }
  
  // Button 4
  if (buttonState4 == HIGH) {
    if (note4 == 0) {
      MIDI.sendNoteOn(63,127,1);
      note4 = 1;
    }
  } else {
    if (note4 == 1) {
      MIDI.sendNoteOff(63,0,1);
    }
    note4 = 0;
  }
  
  // Button 7
  if (buttonState7 == HIGH) {
    if (note7 == 0) {
      MIDI.sendNoteOn(60,127,1);
      note7 = 1;
    }
  } else {
    if (note7 == 1) {
      MIDI.sendNoteOff(60,0,1);
    }
    note7 = 0;
  }
  
  delay(1);
}
