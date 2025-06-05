#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SparkFunCCS811.h"     // Library untuk sensor CCS811
#include <Adafruit_BME280.h>    // Library untuk BME280
#include <BH1750.h>             // Library untuk BH1750
#include "pin.h"
#include "sensor.h"
#include "aktuator.h"

void setup() {
    Serial.begin(9600);
    Wire.setSDA(PB9);  // Set SDA pin STM32
    Wire.setSCL(PB8);  // Set SCL pin STM32
    Wire.setClock(400000);  // Set I2C clock speed
    Wire.begin();
    setup_oled();
    setup_aktuator();
    setup_sensor();
  
}

void loop() {
    baca_sensor();
    kontrol_aktuator();
    delay(1000);  // Delay sebelum membaca lagi
}
