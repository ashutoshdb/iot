const int LED=22;
const int BUZZER=23;


void setup() {
  // put your setup code here, to run once:
pinMode(LED, OUTPUT);
// pinMode(BUZZER, OUTPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(LED,HIGH);
// digitalWrite(BUZZER,HIGH);
Serial.print("LED ON \n");
delay(2000);

digitalWrite(LED,LOW);
// digitalWrite(BUZZER,LOW);
Serial.print("LED OFF \n");
delay(2000);
}
