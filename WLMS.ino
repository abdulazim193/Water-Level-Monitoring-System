#include <LiquidCrystal_I2C.h>

// LCD setup (address 0x27, 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin definitions
const int sensorPin = A0;
const int relayPin = 7;
const int switchPin = 8;

// Variables
int lastSwitchState = HIGH;
bool manualMode = false;
unsigned long lastToggleTime = 0;
unsigned long lastDisplayTime = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(relayPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  digitalWrite(relayPin, LOW);  // Pump initially OFF
  
  lcd.print("Water Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read water level
  int raw = analogRead(sensorPin);
  int percent = map(raw, 0, 1023, 0, 100);
  percent = constrain(percent, 0, 100);
  
  // Read switch (manual override)
  int currentSwitchState = digitalRead(switchPin);
  
  // Debounced switch detection
  if (currentSwitchState == LOW && lastSwitchState == HIGH) {
    delay(50);  // debounce
    if (digitalRead(switchPin) == LOW) {
      manualMode = !manualMode;
      digitalWrite(relayPin, !digitalRead(relayPin));
      lastToggleTime = millis();
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(manualMode ? "MANUAL MODE" : "AUTO MODE");
      delay(1000);
      lcd.clear();
    }
  }
  lastSwitchState = currentSwitchState;
  
  // Automatic control (only if NOT in manual mode)
  if (!manualMode) {
    if (percent >= 100) {
      digitalWrite(relayPin, LOW);  // Full -> STOP pump
    } 
    else if (percent <= 20) {
      digitalWrite(relayPin, LOW);  // Low level -> keep pump OFF (dry run protection)
    }
    else {
      // Between 21-99%: pump can be ON (auto-fill logic)
      // For now, it stays as is or add auto-fill:
      if (percent < 30) digitalWrite(relayPin, HIGH);  // Auto-fill when low
    }
  }
  
  // Update LCD display (every 500ms to reduce flicker)
  if (millis() - lastDisplayTime > 500) {
    lastDisplayTime = millis();
    lcd.setCursor(0, 0);
    lcd.print("Water:");
    lcd.print(percent);
    lcd.print("%  ");  // extra spaces clear old chars
    
    lcd.setCursor(0, 1);
    if (manualMode) {
      lcd.print("MANUAL       ");
    } 
    else if (percent >= 100) {
      lcd.print("FULL - STOP   ");
    } 
    else if (percent <= 20) {
      lcd.print("LOW - FILL    ");
    } 
    else if (percent >= 90) {
      lcd.print("HIGH WARNING  ");
    } 
    else {
      lcd.print("OK            ");
    }
  }
  
  // Optional: Auto exit manual mode after 10 seconds of no level change
  delay(100);
}
