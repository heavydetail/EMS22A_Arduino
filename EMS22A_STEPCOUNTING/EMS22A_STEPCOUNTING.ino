const int PIN_CS = 5;
const int PIN_CLOCK = 6;
const int PIN_DATA = 7;

int magnitude = 0;
int stepCounter = 0;
int oldStep = 0;

int pos;
byte stream[16] = {0};//replace testStream with stream for actual data
//byte testStream[] = {1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}; //test data stream
//byte testStream[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}; //test data stream

void setup() {
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

  //extract 10 bit position from data stream use testStream
  pos = 0; //clear previous data
  for (int i = 0; i < 10; i++) {
    pos = pos << 1;
    //pos += testStream[i]; //replace testStream with stream
    pos += stream[i];
  }

  //COUNT STEPS UP OR DOWN
  //if(pos-oldStep > 1 || pos-oldStep < -1) { //exclude sensor jumps?
    if(pos != oldStep) {
      if(pos > oldStep) {
        stepCounter++;
        oldStep = pos;
      }
      
      if(pos < oldStep) {
        stepCounter--;
        oldStep = pos;
      }
    }
  //}
  magnitude = stepCounter / 1024;
  
  //print position 0-1023
  Serial.print(pos, BIN);
  Serial.print('\t');
  Serial.print(stepCounter);
  Serial.print('\t');
  Serial.print(magnitude);
  Serial.print('\t');
  Serial.println(pos);

  //extract 6 bits of status
  //for (int i = 10; i < 16; i++)
    //Serial.println(testStream[i]);//replace testStream with stream
    //Serial.println(stream[i]);
  //delay(500);
}
