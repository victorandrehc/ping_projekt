#include <LiquidCrystal.h>

/*** INITIALISIERUNG ***/
LiquidCrystal lcd(8, 9, 2, 3, 4, 5); 
const int disp      = 51;

void setup(){
    pinMode(disp, OUTPUT);
  digitalWrite(disp, HIGH);
 lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("HELLO WORLD");
}

void loop(){

}
