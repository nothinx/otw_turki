// Fungsi untuk menampilkan nilai sensor pada OLED
float suhu1, suhu2,suhuAvg, hum1, hum2, humAvg, lux1, lux2, luxAvg;


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
    suhu1 = bme.readTemperature();
    Serial.print(" C, Humidity 1 : ");
    Serial.print(bme.readHumidity());
    hum1 = bme.readHumidity();
    Serial.print(" %, Lux 1: ");
    Serial.println(lightMeter.readLightLevel());
    lux1 = lightMeter.readLightLevel();
    

    TCA9548A(3);
    Serial.print("Temp 2: ");
    Serial.print(bme.readTemperature());
    suhu2 = bme.readTemperature();
    Serial.print(" C, Humidity 2 : ");
    Serial.print(bme.readHumidity());
    hum2 = bme.readHumidity();
    Serial.print(" %, Lux 2: ");
    Serial.println(lightMeter.readLightLevel());
    lux2 = lightMeter.readLightLevel();
    Serial.println("------------------------");

    
    suhuAvg = (suhu1 + suhu2)/2;
    humAvg = (hum1+ hum2)/2;
    luxAvg = (lux1 + lux2)/2;
    Serial.print("Suhu (rata-rata) = ");
    Serial.println(suhuAvg);
    Serial.print("Humifidy (rata-rata) = ");
    Serial.println(humAvg);
    Serial.print("lux (rata-rata) =");
    Serial.println(luxAvg);
}

