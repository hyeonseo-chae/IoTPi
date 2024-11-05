#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C 주소 0x27, 16x2 LCD 설정

const int buzzerPin = 10; // 버저 출력 핀
const int segments[] = {2, 3, 4, 5, 6, 7, 8}; // 7세그먼트 핀 배열 (a, b, c, d, e, f, g)
int receivedNumber = -1;

void setup() {
  Serial.begin(9600); // USB를 통한 직렬 통신 초기화
  pinMode(buzzerPin, OUTPUT);
  
  // 7세그먼트 핀 출력 설정
  for (int i = 0; i < 7; i++) {
    pinMode(segments[i], OUTPUT);
  }
  
  lcd.begin();
  lcd.backlight();
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    
    if (command.startsWith("NUMBER:")) {
      receivedNumber = command.substring(7).toInt();
      displayNumber(receivedNumber);
    } else if (command.startsWith("TEXT:")) {
      String text = command.substring(5);
      lcd.clear();
      lcd.print(text);
    } else if (command == "BUZZER") {
      playMelody();
    }
  }
}

void displayNumber(int number) {
  static const bool digits[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
  
  for (int i = 0; i < 7; i++) {
    digitalWrite(segments[i], digits[number][i]);
  }
}

void playMelody() {
  // 간단한 멜로디 재생
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}
