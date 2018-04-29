String data;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {

  while (Serial.available()) {
    data = Serial.readString();
  }
  
  if (data == "9:40 10:40 11:40 9:40 10:40 11:40") digitalWrite(13, HIGH);
}

