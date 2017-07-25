
const int change_state=45;
const int b2=44;
const int b1=42;

void setup(){
	pinMode(change_state, INPUT);
	pinMode(b2, INPUT);
	pinMode(b1, INPUT);
	Serial.begin(9600);
}

void loop(){
	Serial.print("change_state: ");
	Serial.println(digitalRead(change_state));
	Serial.print("b1: ");
	Serial.println(digitalRead(b1));
	Serial.print("b2: ");
	Serial.println(digitalRead(b2));
  delay(100);

}
