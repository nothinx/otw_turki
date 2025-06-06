#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SparkFunCCS811.h"     // Library untuk sensor CCS811
#include <Adafruit_BME280.h>    // Library untuk BME280
#include <BH1750.h>             // Library untuk BH1750
#include "pin.h"
#include "sensor.h"
#include "aktuator.h"

// Pin untuk OLED reset (optional, bisa dihilangkan jika tidak digunakan)
#define OLED_RESET    -1

// Membuat objek untuk OLED display
Adafruit_SSD1306 display(OLED_RESET);

// Variabel untuk tracking waktu
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long interval = 2000;  // Interval untuk update tampilan sesi 1 dalam milidetik (2 detik)
unsigned long interval2 = 4000;  // Interval untuk update tampilan sesi 2 dalam milidetik (2 detik)

void displaySensorData() {
    unsigned long currentMillis = millis();  // Mendapatkan waktu sekarang

    // Menampilkan Sesi 1: CO2, TVOC, dan Temp
    if (currentMillis - previousMillis <= interval) {

        TCA9548A(0);
        display.clearDisplay();  // Menghapus tampilan sebelumnya
        display.setTextSize(0);
        
        // Menampilkan data CO2
        display.setCursor(0, 0);
        display.print("CO2: ");
        display.println(mySensor.getCO2());

        // Menampilkan data TVOC
        display.setCursor(0, 10);
        display.print("TVOC: ");
        display.println(mySensor.getTVOC());

        // Menampilkan data Temperatur
        display.setCursor(0, 20);
        display.print("Temp: ");
        display.println(suhuAvg);
        display.display();
    
    }
    // Menampilkan Sesi 2: Humidity dan Lux
    else if ( currentMillis - previousMillis >= interval && currentMillis -previousMillis <=interval2) {
          // Menyimpan waktu sekarang
        TCA9548A(0);
        display.clearDisplay();  // Menghapus tampilan sesi pertama
        display.setTextSize(0);

        // Menampilkan data Kelembapan (Humidity)
        display.setCursor(0, 0);  // Menampilkan di atas
        display.print("Humidity: ");
        display.println(humAvg);

        // Menampilkan data Lux
        display.setCursor(0, 10);  // Menampilkan di bawah Humidity
        display.print("Lux: ");
        display.println(luxAvg);

        display.display();  // Menampilkan semua data di OLED
        
    }
    else if (currentMillis - previousMillis  >= 4000){
          previousMillis = currentMillis;
          
        }
    Serial.print("waktu :");
    Serial.println(currentMillis);
    Serial.print("pengurang :");
    Serial.println(previousMillis);    
    Serial.print("interval: ");
    Serial.print(currentMillis - previousMillis);
    }


void setup() {
    Serial.begin(9600);

    // Inisialisasi I2C untuk STM32
    Wire.setSDA(PB9);  // Set SDA pin STM32
    Wire.setSCL(PB8);  // Set SCL pin STM32
    Wire.setClock(400000);  // Set I2C clock speed
    Wire.begin();
    
    // Inisialisasi display OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Pastikan alamat I2C adalah 0x3C (umumnya untuk OLED 128x64)
        Serial.println(F("OLED tidak ditemukan!"));
        while (1);  // Jika tidak ditemukan, berhenti
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Inisialisasi selesai"));
    display.display();

    setup_aktuator();  // Setup aktuator
    setup_sensor();    // Setup sensor
}

void loop() {
  
    baca_sensor();      // Membaca data dari sensor
    kontrol_aktuator(); // Mengontrol aktuator berdasarkan pembacaan sensor
    displaySensorData(); // Menampilkan data sensor pada OLED
    //Serial.println(millis());
    delay(100);
}
