// #include <Talkie.h>
// #include <TalkieUtils.h>
// #include <Vocab_US_Large.h>
// #include <Vocab_Special.h>

#include <Arduino.h>
#include <U8x8lib.h>


// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

// void setup(void) {
//   //u8x8.setBusClock(100000);  // If you breakout other modules, please enable this line
 
// }

#include <stdlib.h>

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11


U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

int button_n_pin = 6;
int button_n_state = 0;
int led_pin = 4;
int button_y_pin = 7;
int button_y_state = 0;
int buzzer = 12;
int letter_step = 2;

boolean clockwise = true;
boolean counter_clockwise = false;
int Steps = 0;
boolean Direction = true;  // gre
unsigned long last_time;
unsigned long currentMillis;
int steps_left = 4095;
long time;
int current_letter = 0;
int new_letter = 0;
int ascii_offset = 101;
// int dist = 500;
int steps_to_move = 0;
int full_rotation = 4096;
int min_char = 2;
int num_chars = 12;

double diameter = 20;
double circumference = diameter * 3.14159;
double char_width = 8;
// int new_letter_dist = (circumference / char_width) / 100 * full_rotation;
int new_letter_dist = 286;

void setup() {
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(button_n_pin, INPUT);
  pinMode(button_y_pin, INPUT);
  srand(analogRead(1));
  u8x8.begin();
  u8x8.setFlipMode(1);
  // delay(1000);
}

int old_dir = 0; // left
float slack_adj = .15;

// START ON LEFT, "A", 
void loop() {

  u8x8.setFont(u8x8_font_inr33_3x6_r);
  u8x8.setCursor(0, 0);
  new_letter = (rand() % num_chars  + min_char) * letter_step;
  u8x8.print((char) ((new_letter / letter_step) + ascii_offset));
  
  Serial.println(current_letter);
  Serial.println(new_letter);
  Serial.println("\n");
  steps_to_move = new_letter_dist * (current_letter - new_letter);

  if (steps_to_move >= 0) {
      // DECREASING ALPHABET
      if (old_dir == 1) {
        steps_to_move += full_rotation * slack_adj;
        old_dir = 0;
      }
    move(steps_to_move, clockwise);
    
    } else {
    if (old_dir == 0) {
        steps_to_move += full_rotation * slack_adj;
        old_dir = 1;
    }
    // INCREASING ALPHABET
    move(-steps_to_move, counter_clockwise);
  }

  delay(1000);

  // Read the letter out loud
  for (int i = 0; i < new_letter + 1; i++) {
    beep();
  }

  button_n_state = digitalRead(button_n_pin);
  button_y_state = digitalRead(button_y_pin);

  while (button_n_state == 0 && button_y_state == 0) {
    digitalWrite(led_pin, HIGH);
    button_n_state = digitalRead(button_n_pin);
    button_y_state = digitalRead(button_y_pin);
    delay(100);
  }

  digitalWrite(led_pin, LOW);
  if (button_y_state == 1) {
    tone(buzzer, 1000);
    delay(250);
    noTone(buzzer);
    blink(led_pin);
  } 
  else {
    tone(buzzer, 1000);
    delay(250);
    noTone(buzzer);
    delay(250);
    tone(buzzer, 1000);
    delay(250);
    noTone(buzzer);
    blink(led_pin);
    delay(100);
    blink(led_pin);
  }
  current_letter = new_letter;
}

void beep () {
  tone(buzzer, 1000);
  delay(250);
  noTone(buzzer);
  delay(250);
}

void blink(int light) {
  digitalWrite(light, HIGH);
  delay(100);
  digitalWrite(light, LOW);
  delay(100);
  digitalWrite(light, HIGH);
  delay(100);
  digitalWrite(light, LOW);
  delay(100);
  digitalWrite(light, HIGH);
  delay(100);
  digitalWrite(light, LOW);
  delay(100);
  digitalWrite(light, HIGH);
  delay(100);
  digitalWrite(light, LOW);
}

void move(int steps_left, boolean dir) {
  Direction = dir;
  while (steps_left > 0) {
    currentMillis = micros();
    if (currentMillis - last_time >= 1000) {
      stepper(1);
      time = time + micros() - last_time;
      last_time = micros();
      steps_left--;
    }
  }
  // Serial.println(time);
  // Serial.println("Wait...!");
  delay(2000);
}

void stepper(int xw) {
  for (int x = 0; x < xw; x++) {
    switch (Steps) {
      case 0:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;
      case 1:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, HIGH);
        break;
      case 2:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;
      case 3:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        break;
      case 4:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
      case 5:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
      case 6:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
      case 7:
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        break;
      default:
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        break;
    }
    SetDirection();
  }
}
void SetDirection() {
  if (Direction == 1) { Steps++; }
  if (Direction == 0) { Steps--; }
  if (Steps > 7) { Steps = 0; }
  if (Steps < 0) { Steps = 7; }
}