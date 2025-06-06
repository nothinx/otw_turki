#include <cstdlib>
// Fungsi untuk mengontrol aktuator dengan PWM
void setActuator(uint8_t pin, uint8_t value) {
    analogWrite(pin, value);  // Mengontrol aktuator dengan PWM
}



// Fungsi untuk menyesuaikan PWM dan memastikan nilainya tidak lebih dari 240
void adjustPWM(uint8_t pin, int &pwmValue, char input) {
    if (input == '+') {
        pwmValue += 20;
        if (pwmValue > 255) pwmValue = 255;  // Membatasi PWM maksimal 240
    } else if (input == '-') {
        pwmValue -= 20;
        if (pwmValue < 0) pwmValue = 0;  // Membatasi PWM minimal 0
    }
    analogWrite(pin, pwmValue);
    Serial.print("PWM ");
    Serial.print(pin);
    Serial.print(" : ");
    Serial.println(pwmValue);
}

void setup_aktuator(){
     // Setup pin modes untuk aktuator
    pinMode(FAN_PERL, OUTPUT);
    pinMode(FAN_RAD, OUTPUT);
    pinMode(GROWLIGHT, OUTPUT);
    pinMode(RPWM_PELT, OUTPUT);
    pinMode(UV, OUTPUT);
    pinMode(HUMIDIFIER, OUTPUT);
    pinMode(WATERPUMP, OUTPUT);
    pinMode(led, OUTPUT);
    digitalWrite(UV, HIGH);
    digitalWrite(HUMIDIFIER, HIGH);
    digitalWrite(WATERPUMP,  HIGH);
    digitalWrite(led, HIGH);

}

void kontrol_aktuator(){
 // Pengaturan PWM berdasarkan input dari Jetson Orin Nano (contoh: menggunakan serial input)
  if (Serial.available() > 0) {
    char input = Serial.read();

    // Adjust PWM for Peltier fan
    if (input == 'p')  {
        adjustPWM(FAN_PERL, pwmPFan, '-');
    } else if (input == 'P') {
        adjustPWM(FAN_PERL, pwmPFan, '+');
    }

    // Adjust PWM for Radiator fan
    if (input == 'r') {
        adjustPWM(FAN_RAD, pwmRFan, '-');
    } else if (input == 'R') {
        adjustPWM(FAN_RAD, pwmRFan, '+');
    }

    if (input == 'd') {
        adjustPWM(RPWM_PELT, pwmPelt, '-');
    } else if (input == 'D') {
        adjustPWM(RPWM_PELT, pwmPelt, '+');
    }

    // Adjust PWM for Growlight
    if (input == 'g') {
        adjustPWM(GROWLIGHT, pwmGLed, '-');
    } else if (input == 'G') {
        adjustPWM(GROWLIGHT, pwmGLed, '+');
    }

    // Kontrol Water Pump
    if (input == 'W') {
        digitalWrite(WATERPUMP, LOW);  // Nyalakan Water Pump
        delay(500);
        digitalWrite(WATERPUMP, HIGH);
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

}