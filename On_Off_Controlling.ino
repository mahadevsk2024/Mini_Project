#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#define DS18B20 13

OneWire ourWire(DS18B20);
DallasTemperature sensor(&ourWire);
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
byte degree_symbol[8] = 
              {
                0b00111,
                0b00101,
                0b00111,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000
              };
const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'.','0','#'}
};
byte rowPins[ROWS] = {A5, A4, A3, A2}; 
byte colPins[COLS] = {4,5,6}; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup()
{
  pinMode(2, OUTPUT);
   Serial.begin(9600); 
   sensor.begin();
   lcd.begin(16, 2);
   lcd.createChar(1, degree_symbol);
   lcd.setCursor(0,0);
   lcd.print("Digital Temp.");
   lcd.setCursor(0,1);
   lcd.print("  controller  ");
   delay(4000);
   lcd.clear();
}

void loop()
{
  float temp;
  int key = getsetpoint();
  while(1)
  {
   sensor.requestTemperatures();
   temp=sensor.getTempCByIndex(0);
   Serial.print(temp);
   Serial.println("°C");
   lcd.setCursor(7,0);
   lcd.print(temp);
   lcd.write(1);
    lcd.print("C");
   if(temp >= key)
   {
        lcd.setCursor(0,1);
        lcd.print("SP:");
        lcd.setCursor(3,1);
        lcd.print(key);
        lcd.setCursor(6,1);
        lcd.print("Heater OFF");
        digitalWrite(2, LOW);
   }
   if(temp < key)
   {
      lcd.setCursor(0,1);
      lcd.print("SP:");
      lcd.setCursor(3,1);
      lcd.print(key);
      lcd.setCursor(7,1);
      lcd.print("Heater ON");
      digitalWrite(2, HIGH);
   }
    
   delay(1000);
  }
}

int getsetpoint()
{
  int value=0;
  int isnum;
  long keyvalue;
  lcd.setCursor(1,0);
  Serial.print("set point:");
  lcd.print("Set point("); 
  lcd.write(1);
  lcd.print("C)");
  lcd.setCursor(6,1);
  do
  {
    keyvalue=keypad.getKey();
    isnum = (keyvalue >= '0' && keyvalue<= '9');
    if (isnum)
    {
      Serial.print(keyvalue - '0');
      lcd.print(keyvalue - '0');
      value=value*10+keyvalue - '0';
    }
  }while(isnum || !keyvalue);
    delay(1000);
    lcd.clear();
    lcd.print("Processing....");
    delay(1000);
    delay(800);
    lcd.clear();
    lcd.print("Temp = ");
  return value;
}