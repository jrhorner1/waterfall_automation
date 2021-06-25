#include <FastLED.h>

#define DATA_PIN    9
#define NUM_LEDS   24
#define BRIGHTNESS 15 // (max = 255)
CRGB leds[NUM_LEDS];

#define DELAYVAL 64

#define SENSOR_1 7
#define SENSOR_2 8
#define RELAY_PIN 2

void setup() { 
  Serial.begin(9600);
  pinMode(SENSOR_1,INPUT);
  pinMode(SENSOR_2,INPUT);
  pinMode(RELAY_PIN,OUTPUT);
  
  FastLED.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  digitalWrite(RELAY_PIN, LOW); 
  int waterfall = digitalRead(SENSOR_1);
  int reservoir = digitalRead(SENSOR_2);
  uint8_t green = 0;
  uint8_t orange = 48;
  uint8_t red = 96;
  uint8_t blue = 172;
  
  if (waterfall == HIGH && reservoir == HIGH) {
    Serial.println("Water levels look good.");
    fill_solid(&(leds[0]), NUM_LEDS, CHSV(green, 255, 64));
    FastLED.show(); 
  } 
  else if (waterfall == LOW && reservoir == HIGH) {
    Serial.println("Refilling waterfall from reservoir.");
    uint32_t counter = 0;
    do { 
      counter++;
      digitalWrite(RELAY_PIN, HIGH);
      circle(blue);
      if (counter == 5) { break; }
    } while(sensorLOW(SENSOR_1));
    digitalWrite(RELAY_PIN, LOW); 
  } 
  else if (waterfall == HIGH && reservoir == LOW) {
    Serial.println("Resevoir low, please refill.");
    pendullum(red);
  } 
  else if (waterfall == LOW && reservoir == LOW) {
    Serial.println("Waterfall and resevoir are low, please refill.");
    blink(red);
  } 
  else {
    Serial.println("Unknown State");
    blink(orange);
    digitalWrite(RELAY_PIN, LOW); 
  }
}

bool sensorLOW(int sensor) {
  switch (digitalRead(sensor)){
    case LOW: return true; break;
    case HIGH: return false; break;
    default: return true;
  }
}

void blink(uint8_t hue) {
  fill_solid(&(leds[0]), NUM_LEDS, CHSV(hue, 255, 255));
  FastLED.show(); 
  delay(500);
  fill_solid(&(leds[0]), NUM_LEDS, CRGB::Black);
  FastLED.show(); 
  delay(500);
}

void circle(uint8_t hue) {
  uint8_t scale = 196;
  for ( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue, 255, 255);
      FastLED.show(); 
      fadeall(scale);
      delay(DELAYVAL);
    }
}

void pendullum(uint8_t hue) {
  uint8_t scale = 100;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
    FastLED.show(); 
    fadeall(scale);
    delay(DELAYVAL);
  }
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    leds[i] = CHSV(hue, 255, 255);
    FastLED.show();
    fadeall(scale);
    delay(DELAYVAL);
  }
}

void fadeall(uint8_t scale) { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(scale); } }
