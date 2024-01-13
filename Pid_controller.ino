#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <PIDController.h>
#include <Keypad.h>
#define DS18B20 13

#define out 3

//#define __Kp 30 // Proportional constant
//#define __Ki 0.7 // Integral Constant
//#define __Kd 200 // Derivative Constant

OneWire ourWire(DS18B20);
DallasTemperature sensor(&ourWire);
LiquidCrystal lcd(12,11, 10, 9, 8, 7);
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
PIDController pid;
int __Kp,__Ki,__Kd;
void setup()
{
   Serial.begin(9600); 
   sensor.begin();
   pinMode(out, OUTPUT); // MOSFET output PIN
   lcd.begin(16, 2);
   pid.begin();         // initialize the PID instance
   lcd.createChar(1, degree_symbol);
   lcd.setCursor(0,0);
   lcd.print("Digital Temp.");
   lcd.setCursor(0,1);
   lcd.print("  controller  ");
   delay(4000);
   lcd.clear();
   pid.setpoint(50); 
   __Kp=getkp();  
   __Ki=getki(); 
   __Kd=getkd();  // The "goal" the PID controller tries to "reach"
   pid.tune(__Kp, __Ki,__Kd);    // Tune the PID, arguments: kP, kI, kD
   pid.limit(0, 255); 
   lcd.clear();
}

void loop()
{
  float temp;
  int key = getsetpoint();
  do
  {
   sensor.requestTemperatures();
   temp=sensor.getTempCByIndex(0);
   int output = pid.compute(temp);
   analogWrite(out, output);
   pid.setpoint(key);
   Serial.print(temp);
   Serial.println("°C");
   lcd.setCursor(7,0);
   lcd.print(temp);
   lcd.write(1);
   lcd.print("C");
   lcd.setCursor(0,1);
        lcd.print("SP:");
        lcd.setCursor(3,1);
        lcd.print(key);
        lcd.write(1);
        lcd.print("C");
      //  lcd.setCursor(9,1);
        //lcd.print("DC:");
      //  lcd.print(output);
   }while(1);
  lcd.clear();
  delay(5000);
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
  }while(keyvalue != '#');
    delay(1000);
    lcd.clear();
    lcd.print("Processing....");
    delay(1000);
    delay(800);
    lcd.clear();
    lcd.print("Temp = ");
  return value;
}

int getkp()
{
  int value=0;
  int isnum;
  long keyvalue;
  lcd.setCursor(1,0);
  Serial.print("kp:");
  lcd.print("Kp value:"); 
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
  }while(keyvalue != '#');
   delay(100);
   lcd.clear();
   return value;
}

int getki()
{
  int value=0;
  int isnum;
  long keyvalue;
  lcd.setCursor(1,0);
  Serial.print("ki:");
  lcd.print("Ki value:"); 
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
  }while(keyvalue != '#');
   delay(100);
   lcd.clear();
   return value;
}

int getkd()
{
  int value=0;
  int isnum;
  long keyvalue;
  lcd.setCursor(1,0);
  Serial.print("kd:");
  lcd.print("Kd value:"); 
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
  }while(keyvalue != '#');
   delay(100);
   lcd.clear();
   return value;
}

