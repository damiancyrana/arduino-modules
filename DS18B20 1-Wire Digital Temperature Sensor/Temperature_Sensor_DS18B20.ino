#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 2 // Data wire is plugged into digital pin 2 on the Arduino

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire device  
DallasTemperature sensors(&oneWire); // Pass oneWire reference to DallasTemperature library

byte degree_celcius[8] = {
  0b11110,
  0b10010,
  0b10010,
  0b11110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();
}

void loop() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  
  lcd.setCursor(0, 0);
  lcd.print("Temp:");

  lcd.setCursor(6, 0);
  lcd.print(sensors.getTempCByIndex(0));

  lcd.setCursor(12, 0);
  lcd.write(degree_celcius[8]);
  lcd.setCursor(13, 0);
  lcd.print("C");

  delay(500);
}
