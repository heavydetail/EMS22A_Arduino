const int PIN_CS = 5;
const int PIN_CLOCK = 6;
const int PIN_DATA = 7;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_DATA, INPUT);

  digitalWrite(PIN_CLOCK, HIGH);
  digitalWrite(PIN_CS, LOW);
}


//byte stream[16];
void loop() {

  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  int pos = 0;
  for (int i=0; i<10; i++) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);
   
    byte b = digitalRead(PIN_DATA) == HIGH ? 1 : 0;
    pos += b * pow(2, 10-(i+1));
  }
  for (int i=0; i<6; i++) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);
  }
  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_CLOCK, HIGH);
  Serial.println(pos);
}
