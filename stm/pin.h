#define TCA_ADDR 0x70          // Alamat I2C TCA9548A
#define CCS811_ADDR 0x5A       // Alamat I2C CCS811

// ===============PIN AKTUATOR====================
#define FAN_PERL     PB13 //PB13    // Fan Peltier (PWM)
#define FAN_RAD      PB14    // Fan Radiator (PWM)
#define RPWM_PELT    PB15    // RPWM Peltier (PWM)
#define LPWM_PELT    PA8     // LPWM Peltier (PWM)
#define GROWLIGHT    PB10    // Grow Light
#define UV           PB4     // UV Light
#define HUMIDIFIER   PA9     // Humidifier
#define WATERPUMP    PA10     // Water Pump
#define led          PC13    // LED indicator

// Alamat sensor dan objek
CCS811 mySensor(CCS811_ADDR);   // Objek sensor CCS811
Adafruit_BME280 bme;            // Objek sensor BME280
BH1750 lightMeter;              // Objek sensor BH1750


int pwmPFan = 250;  // Initial PWM value for Peltier fan
int pwmRFan = 250;  // Initial PWM value for Radiator fan
int pwmGLed = 0;  // Initial PWM value for Growlight
int pwmPelt = 250;

// Fungsi untuk memilih channel pada TCA9548A
void TCA9548A(uint8_t bus) {
    Wire.beginTransmission(TCA_ADDR);
    Wire.write(1 << bus);  // Pilih channel
    Wire.endTransmission();
}