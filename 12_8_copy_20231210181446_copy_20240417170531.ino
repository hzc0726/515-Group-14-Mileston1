#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_VEML7700.h>

#define LED_PIN 18
#define NUMPIXELS 60
#define PIR_PIN 27

Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_VEML7700 veml;

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  if (!veml.begin()) {
    Serial.println("Failed to initialize VEML7700 sensor!");
    while (1);
  }

  // Configure VEML7700 settings
  veml.setGain(VEML7700_GAIN_1_8);
  veml.setIntegrationTime(VEML7700_IT_800MS);
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN);
  float lux = veml.readLux();
  int brightness = calculateBrightness(lux);

  if (motionDetected) {
    Serial.println("Motion detected!");
    setColor(brightness);
  } else {
    strip.clear();
    strip.show();
    Serial.println("No motion, lights off.");
  }
  delay(100); // Adjust for faster sensor updates
}

int calculateBrightness(float lux) {
  // Convert lux to a brightness value (0-255)
  if (lux > 400) {
    return max(0, 255 - (int)(lux / 400 * 255));  // Decrease brightness as lux increases
  } else {
    return min(255, (int)(lux / 400 * 255));  // Increase brightness as lux decreases
  }
}

void setColor(int brightness) {
  strip.setBrightness(brightness);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));  // Set all pixels to blue
  }
  strip.show();
}