void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("Blink test for all platforms");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("on");
  delay(250);
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("off");
  delay(250);
}
