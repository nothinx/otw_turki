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

void setup_sensor(){
      // Inisialisasi sensor
    TCA9548A(6);  // CCS811
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

    TCA9548A(3);  // BME280 & BH1750
    if (!bme.begin(0x76)) {
        Serial.println("BME280 gagal mulai.");
    }
    if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println("BH1750 gagal mulai.");
    }
}

void setup_oled(){
     display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Alamat I2C OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("hey!"));
    display.display();
}

void baca_sensor(){
    // Pembacaan sensor
    TCA9548A(6);
    if (mySensor.dataAvailable()) {
        Serial.println("----------------------");
        mySensor.readAlgorithmResults();
        Serial.print("CO2: ");
        Serial.print(mySensor.getCO2());
        Serial.print(" ppm, TVOC: ");
        Serial.println(mySensor.getTVOC());
    }

    TCA9548A(1);
    Serial.print("Temp 1: ");
    Serial.print(bme.readTemperature());
    Serial.print(" C, Humidity 1 : ");
    Serial.print(bme.readHumidity());
    Serial.print(" %, Lux 1: ");
    Serial.println(lightMeter.readLightLevel());
    TCA9548A(3);
    Serial.print("Temp 2: ");
    Serial.print(bme.readTemperature());
    Serial.print(" C, Humidity 2: ");
    Serial.print(bme.readHumidity());
    Serial.print(" %, Lux 2: ");
    Serial.println(lightMeter.readLightLevel());
    Serial.println("------------------------");

    // Menampilkan data sensor pada OLED
    displaySensorData();
}