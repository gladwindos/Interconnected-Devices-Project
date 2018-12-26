#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial uno(6,7);

// Set Analog Sensors
const int tempSensor = A0;
const int lightSensor = A1;

// Initialize button pin and state
const int buttonPin = 9;
int buttonCounter = 0;
int buttonState = 0;
int prevButtonState = 0;

// Initialize LCD Screen
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);

  uno.begin(115200);
  
  lcd.begin(16, 2);
}

void setButtonState() {
  buttonState = digitalRead(buttonPin);

  if (buttonState != prevButtonState) {
    if (buttonState == HIGH) {
      buttonCounter++;
      Serial.print("Button Pressed! Total: ");
      Serial.println(buttonCounter);
    }
    delay(50); // avoid bouncing
  }

  prevButtonState = buttonState;
}

// Function to read the temperature sensor and return temperature in celcius
float getTemp() {
  float tempVal = 0.0;
  int i;

  for (i = 0; i < 250; i++) {
    tempVal += analogRead(tempSensor);
  }

  tempVal = tempVal / 250;  

  float voltage = (tempVal/1024.0) * 5.0;
  float tempCelcius = (voltage - 0.5) * 100;

  Serial.print("Temp: ");
  Serial.println(tempCelcius);

  return tempCelcius;
}

// Function to get the light level from the photoresistor and scale it
float getLightLevel() {
  float lightVal = 0.0;
  int i;

  for (i = 0; i < 250; i++) {
    lightVal += analogRead(lightSensor);
  } 

  lightVal = lightVal/250;

  float scaledVal = map(lightVal, 0, 1000, 0, 100);

  Serial.print("Light value: ");
  Serial.println(scaledVal);

  return scaledVal;
}

// Function that handles what is displayed on the LCD display
void setLCD(int temp, int light) {

  lcd.clear();
  if (buttonCounter % 2 == 0) {
    lcd.setCursor(0,0);
    lcd.print("Temperature") ;

    lcd.setCursor(0,1);
    lcd.print(temp);
    lcd.print((char)223);
    lcd.print("C");
  } else {
    lcd.setCursor(0,0);
    lcd.print("Light Level");

    lcd.setCursor(0,1);
    lcd.print(light);
  }
}

void loop() {

  setButtonState();
  
  int temperature = getTemp();

  int lightLevel = getLightLevel();
  
  setLCD(temperature, lightLevel);

  uno.print(temperature);
  uno.print(" ");
  uno.println(lightLevel);
  
  delay(50);
}
