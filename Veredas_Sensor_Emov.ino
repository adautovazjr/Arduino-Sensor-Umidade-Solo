#include <LiquidCrystal_I2C.h>

const int SENSOR_PIN = A0;

const int RED_PIN = 11;
const int GREEN_PIN = 9;
const int BLUE_PIN = 10;
const int BLINK_LED_PIN = 6;

const int SENSOR_MIN_VALUE = 300;  // Solo molhado (faixa típica para sensores capacitivos)
const int SENSOR_MAX_VALUE = 900;  // Solo seco ou ar (ajuste conforme seu sensor real)

const int BLINK_DELAY_SLOW = 1000;
const int BLINK_DELAY_FAST = 200;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorValue = 0;
int moisturePercent = 0;
int previousMoisturePercent = -1;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BLINK_LED_PIN, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.print("TECNOLIFE SENSOR");
  lcd.setCursor(0, 1);
  lcd.print("INICIANDO...");
  delay(3000);
}

void loop() {
  sensorValue = analogRead(SENSOR_PIN);

  // Constrain para evitar valores fora da faixa
  sensorValue = constrain(sensorValue, SENSOR_MIN_VALUE, SENSOR_MAX_VALUE);

  // Mapeamento inverso: quanto menor o valor, mais úmido o solo
  moisturePercent = map(sensorValue, SENSOR_MAX_VALUE, SENSOR_MIN_VALUE, 0, 100);

  // Limitar para ficar entre 0 e 100%
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Humidity: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  updateLcd();

  // Controle simples de LED de alerta
  if (moisturePercent < 20) {
    digitalWrite(BLINK_LED_PIN, HIGH);
    delay(BLINK_DELAY_FAST);
    digitalWrite(BLINK_LED_PIN, LOW);
    delay(BLINK_DELAY_FAST);
  } else {
    digitalWrite(BLINK_LED_PIN, LOW);
  }

  delay(1000);
}

void updateLcd() {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (moisturePercent <= 10) {
    lcd.print("CRITICO SECO!");
  } else if (moisturePercent <= 25) {
    lcd.print("Muito seco!");
  } else if (moisturePercent <= 45) {
    lcd.print("Precisa agua!");
  } else if (moisturePercent <= 65) {
    lcd.print("Umidade boa");
  } else if (moisturePercent <= 100) {
    lcd.print("Solo ideal");
  }

  lcd.setCursor(0, 1);
  lcd.print(moisturePercent);
  lcd.print("% UMIDADE");
}
