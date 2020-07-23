#include <LiquidCrystal.h>
#define REDLITE 24
#define GREENLITE 22

int brightness = 255;
int hr_pin = 12;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
int counter = 10;
boolean didIcall = false;
char lcd_line1[16];
char lcd_line2[16];
char lowhr[16];
char highhr[16];
int button = 13;
int speakerpin = 10;
int length = 1;
char notes[] = "cccccccccccccccc";
int beats[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int tempo = 100;
int lowHR;
int highHR;
int state = 2;

void playTone (int tone, int duration)
{
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerpin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerpin, LOW);
    delayMicroseconds(tone);
  }
}
void playNote(char note, int duration) {
  char names[] = { 'c', 'c', 'c', 'c', 'c', 'c', 'c', 'c' };
  int tones[] = { 1915, 1915, 1915, 1915, 1915, 1915, 1915, 1915 };
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
void setup() {
  pinMode(hr_pin, INPUT);
  pinMode(REDLITE, OUTPUT);
  pinMode(GREENLITE, OUTPUT);
  lcd.begin(16, 2);
  pinMode(speakerpin, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
}


void loop() {
  
  while (lowHR == 0) {
   while(Serial.available() > 0) { // Put in LOWHR first
     lowHR = Serial.parseInt();
     Serial.end();
   }
  } 

  while (highHR == 0) {
    while(Serial.available() > 0) {
     lcd.clear();
     highHR = Serial.parseInt();
     Serial.end();
   }
 }

  int hr_value = pulseIn(hr_pin, HIGH);
  sprintf(lcd_line1, "Heart Rate :%3d", hr_value);
  lcd.setCursor(0, 0);
  lcd.print(lcd_line1);
  if (digitalRead(button) == LOW) {
   if (hr_value < lowHR || hr_value > highHR) {
    analogWrite(REDLITE, 0);
    analogWrite(GREENLITE, 255);

    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
        delay(beats[i] * tempo); // rest
        delay(250);
      }

      else {
        playNote(notes[i], beats[i] * tempo);
        delay(tempo / 2);
        delay(250);
      }
    }

    if (counter > 0) {
      counter = counter - 1;
      sprintf(lcd_line2, "Calling in : %2d secs", counter);
      lcd.setCursor(0, 1);
      lcd.print(lcd_line2);
      delay(100);
    }
    else {
      if (didIcall == false);
      Serial.begin(9600);
      Serial.write(1);
      Serial.end();
      didIcall = true;
    }
  }
  Serial.begin(9600);
  Serial.print(hr_value);
  Serial.end();
  delay(1000);
  }
  else {
  exit(0);
  }
 } 
