/*
LCD
ADDRESS 0X27 
16X2
VCC-5V
GND-GND
SCA-A4
SCL-A5


KEYPAD
R1-DP9
R2-DP8
R3-DP7
R4-DP6
C1-DP5
C2-DP4
C3-DP3
C4-DP2
*/



#include <LiquidCrystal_I2C.h>
#include<Wire.h>
#include <Keypad.h> //Header file for Keypad from https://github.com/Chris--A/Keypad

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns


char keys[ROWS][COLS] = {


  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
                        };


byte rowPins[ROWS] = { 9,8,7,6};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 5,4,3,2 }; 
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); 
//  Create the Keypad



LiquidCrystal_I2C lcd(0X27,16,2);

const int greenLed = 13;
const int redLed = 12;
const int buzzer = 11;
 
const int passwordLength = 4;
char correctPassword[passwordLength] = {'1', '2', '3', '4'};
char enteredPassword[passwordLength];

int incorrectAttempts = 0;


void setup() 
{

lcd.init();
lcd.backlight();
lcd.print("SECURITY BASED SYSTEM"); 
lcd.setCursor(0, 1);   // set the cursor to column 0, line 1
delay(2000); 
lcd.clear(); 
pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(buzzer, LOW);
  lcd.print("Enter Password:");
}


void loop() 
{

char key = keypad.getKey();

  if (key) {
    if (key == '#') {
      if (checkPassword()) {
        // Correct password
        lcd.clear();
        lcd.print("Access granted!");
        digitalWrite(greenLed, HIGH);
        digitalWrite(redLed, LOW);
        digitalWrite(buzzer, HIGH);
        tone(11,453,2000);
        delay(2000);
        digitalWrite(greenLed, LOW);
        digitalWrite(buzzer, LOW);
       

        lcd.clear();
        lcd.print("Enter Password:");
      } else {
        // Incorrect password
        lcd.clear();
        lcd.print("Incorrect password");
        digitalWrite(greenLed, LOW);
        digitalWrite(redLed, HIGH);
        digitalWrite(buzzer, HIGH);
        tone(11,453,2000);
        delay(2000);
        delay(1000);
        digitalWrite(redLed, LOW);
        digitalWrite(buzzer, LOW);
        
        lcd.clear();
        lcd.print("Enter Password:");
        incorrectAttempts++;

        if (incorrectAttempts >= 3) {
          // Three incorrect attempts, activate continuous blinking
          continuousBlinking();
        }
      }

      // Reset entered password
      memset(enteredPassword, 0, passwordLength);
    } else {
      // Add digit to entered password
      addDigitToPassword(key);
    }
  }
}

void addDigitToPassword(char digit) {
  static int index = 0;
  if (index < passwordLength) {
    enteredPassword[index++] = digit;
    lcd.setCursor(index - 1, 1);
    lcd.print('*');
  }
}

bool checkPassword() {
  for (int i = 0; i < passwordLength; i++) {
    if (enteredPassword[i] != correctPassword[i]) {
      return false;
    }
  }
  return true;
}

void continuousBlinking() {
  while (true) {
    digitalWrite(redLed, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(redLed, LOW);
    digitalWrite(buzzer, LOW);
    delay(500);
  }
}

