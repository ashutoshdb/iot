const int sw = 22;
const int buzz= 21;

int val;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(sw,INPUT);
  pinMode(buzz,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(sw);
Serial.println(val);
if(val==1)
{
  digitalWrite(buzz,1);
  Serial.println("button pressed");
  delay(5000);
}
else
{
 digitalWrite(buzz,0);
  delay(500);  
}
}
