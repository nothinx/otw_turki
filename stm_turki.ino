#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SparkFunCCS811.h"     // Library untuk sensor CCS811
#include <Adafruit_BME280.h>    // Library untuk BME280
#include <BH1750.h>             // Library untuk BH1750

#define TCA_ADDR 0x70          // Alamat I2C TCA9548A
#define CCS811_ADDR 0x5A       // Alamat I2C CCS811

// Pin Aktuator
#define FAN_PERL     PB13    // Fan Peltier (PWM)
#define FAN_RAD      PB14    // Fan Radiator (PWM)
#define RPWM_PELT    PB15    // RPWM Peltier (PWM)
#define LPWM_PELT    PA8     // LPWM Peltier (PWM)
#define GROWLIGHT    PB10    // Grow Light
#define UV           PB7     // UV Light
#define HUMIDIFIER   PB6     // Humidifier
#define WATERPUMP    PB5     // Water Pump
#define led          PC13    // LED indicator

// Alamat sensor dan objek
CCS811 mySensor(CCS811_ADDR);   // Objek sensor CCS811
Adafruit_BME280 bme;            // Objek sensor BME280
BH1750 lightMeter;              // Objek sensor BH1750
Adafruit_SSD1306 display(-1);    // OLED display

int pwmPFan = 0;  // Initial PWM value for Peltier fan
int pwmRFan = 0;  // Initial PWM value for Radiator fan
int pwmGLed = 0;  // Initial PWM value for Growlight

// Fungsi untuk memilih channel pada TCA9548A
void TCA9548A(uint8_t bus) {
    Wire.beginTransmission(TCA_ADDR);
    Wire.write(1 << bus);  // Pilih channel
    Wire.endTransmission();
}

// Fungsi untuk mengontrol aktuator dengan PWM
void setActuator(uint8_t pin, uint8_t value) {
    analogWrite(pin, value);  // Mengontrol aktuator dengan PWM
}

// Fungsi untuk menampilkan nilai sensor pada OLED
void displaySensorData() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("CO2: ");
    display.println(mySensor.getCO2());
    display.print("TVOC: ");
    display.println(mySensor.getTVOC());
    display.print("Temp: ");
    display.println(bme.readTemperature());
    display.print("Humidity: ");
    display.println(bme.readHumidity());
    display.print("Lux: ");
    display.println(lightMeter.readLightLevel());
    display.display();
}

// Fungsi untuk menyesuaikan PWM dan memastikan nilainya tidak lebih dari 240
void adjustPWM(uint8_t pin, int &pwmValue, char input) {
    if (input == '+') {
        pwmValue += 20;
        if (pwmValue > 240) pwmValue = 240;  // Membatasi PWM maksimal 240
    } else if (input == '-') {
        pwmValue -= 20;
        if (pwmValue < 0) pwmValue = 0;  // Membatasi PWM minimal 0
    }
    analogWrite(pin, pwmValue);
}

void setup() {
    Serial.begin(115200);
    Wire.setSDA(PB9);  // Set SDA pin STM32
    Wire.setSCL(PB8);  // Set SCL pin STM32
    Wire.setClock(400000);  // Set I2C clock speed
    Wire.begin();

    // Setup pin modes untuk aktuator
    pinMode(FAN_PERL, OUTPUT);
    pinMode(FAN_RAD, OUTPUT);
    pinMode(GROWLIGHT, OUTPUT);
    pinMode(UV, OUTPUT);
    pinMode(HUMIDIFIER, OUTPUT);
    pinMode(WATERPUMP, OUTPUT);
    pinMode(led, OUTPUT);

    // Inisialisasi OLED
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Alamat I2C OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Albi Asw"));
    display.display();

    // Inisialisasi sensor
    TCA9548A(3);  // CCS811
    if (!mySensor.begin()) {
        Serial.println("CCS811 gagal mulai.");
        while (1);
    }

    TCA9548A(1);  // BME280 & BH1750
    if (!bme.begin(0x76)) {
        Serial.println("BME280 gagal mulai.");
    }
    if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("BH1750 gagal mulai.");
    }
}

void loop() {
    // Pembacaan sensor
    TCA9548A(3);
    if (mySensor.dataAvailable()) {
        mySensor.readAlgorithmResults();
        Serial.print("CO2: ");
        Serial.print(mySensor.getCO2());
        Serial.print(" ppm, TVOC: ");
        Serial.println(mySensor.getTVOC());
    }

    TCA9548A(1);
    Serial.print("Temp: ");
    Serial.print(bme.readTemperature());
    Serial.print(" C, Humidity: ");
    Serial.print(bme.readHumidity());
    Serial.print(" %, Lux: ");
    Serial.println(lightMeter.readLightLevel());

    // Menampilkan data sensor pada OLED
    displaySensorData();

    // Pengaturan PWM berdasarkan input dari Jetson Orin Nano (contoh: menggunakan serial input)
    if (Serial.available() > 0) {
        char input = Serial.read();

        // Adjust PWM for Peltier fan
        if (input == 'p' || input == 'P') {
            adjustPWM(FAN_PERL, pwmPFan, input);
        }

        // Adjust PWM for Radiator fan
        if (input == 'r' || input == 'R') {
            adjustPWM(FAN_RAD, pwmRFan, input);
        }

        // Adjust PWM for Growlight
        if (input == 'g' || input == 'G') {
            adjustPWM(GROWLIGHT, pwmGLed, input);
        }
        
        // Kontrol Water Pump
        if (input == 'W') {
            digitalWrite(WATERPUMP, LOW);  // Nyalakan Water Pump
        } else if (input == 'w') {
            digitalWrite(WATERPUMP, HIGH);  // Matikan Water Pump
        }

        // Kontrol UV LED
        if (input == 'U') {
            digitalWrite(UV, LOW);  // Nyalakan UV LED
        } else if (input == 'u') {
            digitalWrite(UV, HIGH);  // Matikan UV LED
        }

        // Kontrol LED Grow Light
        if (input == 'L') {
            digitalWrite(led, LOW);  // Nyalakan LED Grow Light
        } else if (input == 'l') {
            digitalWrite(led, HIGH);  // Matikan LED Grow Light
        }

        // Kontrol Humidifier
        if (input == 'H') {
            digitalWrite(HUMIDIFIER, LOW);  // Nyalakan Humidifier
        } else if (input == 'h') {
            digitalWrite(HUMIDIFIER, HIGH);  // Matikan Humidifier
        }
    }

    delay(1000);  // Delay sebelum membaca lagi
}
