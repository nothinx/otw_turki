#include <Wire.h>

// Libraries untuk semua sensor dan display
#include <BH1750.h>                 // Library untuk BH1750
#include <Adafruit_BME280.h>        // Library untuk BME280
#include "SparkFunCCS811.h"         // Library khusus untuk SparkFun CCS811
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definisi OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// === IMPLEMENTASI DARI FILE pin.h ANDA ===
#define CCS811_ADDR 0x5A       // Alamat I2C CCS811 sesuai file Anda

// Buat Objek untuk setiap perangkat
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
BH1750 lightMeter;
Adafruit_BME280 bme;
CCS811 ccs(CCS811_ADDR); // <<<<<< Inisialisasi CCS811 dengan alamat kustom

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Inisialisasi I2C

  // Inisialisasi OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Gagal menginisialisasi SSD1306"));
    for(;;);
  }
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Menginisialisasi sensor...");
  display.display();

  // Inisialisasi BH1750
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("Gagal menginisialisasi BH1750"));
  }

  // Inisialisasi BME280
  if (!bme.begin(0x76)) { // Cek alamat 0x76 atau 0x77
    Serial.println(F("Gagal menemukan sensor BME280"));
  }

  // Inisialisasi SparkFun CCS811
  if (ccs.begin() == false) {
    Serial.print("Gagal menginisialisasi CCS811. Cek alamat I2C dan koneksi.");
    while(1);
  }
  
  Serial.println("Semua sensor siap!");
  delay(1000);
}

void loop() {
  // ----- Baca Data dari Semua Sensor -----

  // 1. Baca Sensor Cahaya BH1750
  float lux = lightMeter.readLightLevel();

  // 2. Baca Sensor Lingkungan BME280
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();

  // 3. Baca Sensor Kualitas Udara CCS811
  uint16_t eco2 = 0;
  uint16_t tvoc = 0;
  
  // Berikan data suhu dan kelembapan ke CCS811 untuk kompensasi
  ccs.setEnvironmentalData(hum, temp);

  // Cek jika ada data baru dari CCS811
  if (ccs.dataAvailable()) {
    ccs.readAlgorithmResults();
    eco2 = ccs.getCO2();
    tvoc = ccs.getTVOC();
  }

  // ----- Tampilkan Data -----
  
  // 1. Tampilkan ke Serial Monitor (untuk debugging)
  Serial.println("------------------------------");
  Serial.print("Suhu: "); Serial.print(temp, 1); Serial.println(" C");
  Serial.print("Kelembapan: "); Serial.print(hum, 1); Serial.println(" %");
  Serial.print("Cahaya: "); Serial.print(lux, 0); Serial.println(" lx");
  Serial.print("eCO2: "); Serial.print(eco2); Serial.println(" ppm");
  Serial.print("TVOC: "); Serial.print(tvoc); Serial.println(" ppb");

  // 2. Tampilkan ke Layar OLED
  display.clearDisplay();
  display.setCursor(0,0);

  display.print("Suhu: "); display.print(temp, 1); display.println(" C");
  display.print("Lembab: "); display.print(hum, 1); display.println(" %");
  display.print("Cahaya: "); display.print(lux, 0); display.println(" lx");
  display.println("--------------------");
  display.print("eCO2: "); display.print(eco2); display.println(" ppm");
  display.print("TVOC: "); display.print(tvoc); display.println(" ppb");
  
  display.display();
}