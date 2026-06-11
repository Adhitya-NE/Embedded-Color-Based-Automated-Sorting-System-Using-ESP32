
#include <Adafruit_TCS34725.h>
#include <ESP32Servo.h>

// Pin HC-SR04
const int trigPin = 13;
const int echoPin = 12;

// Pin Motor Driver L298N
const int IN1 = 14;
const int IN2 = 27;
const int ENA = 33;

// Pin Servo
const int servoMerahPin = 25;
const int servoHijauPin = 26;

// Sensor Warna
Adafruit_TCS34725 tcs(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// Servo
Servo servoMerah;
Servo servoHijau;

// FSM States
enum State {
  IDLE,
  KONVEYOR_MAJU,
  DETEKSI_WARNA,
  SORTIR_MERAH,
  SORTIR_HIJAU,
  SORTIR_LAIN,
  SELESAI
};

State currentState = IDLE;
unsigned long stateStartTime = 0;
String warnaTerdeteksi = "";

void setup() {
  Serial.begin(115200);

  // Inisialisasi pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  matikanConveyor();

  // Servo
  servoMerah.attach(servoMerahPin);
  servoHijau.attach(servoHijauPin);
  servoMerah.write(0);      // posisi awal
  servoHijau.write(53);     // posisi awal

  // Sensor warna
  if (!tcs.begin()) {
    Serial.println("Sensor warna TIDAK terdeteksi!");
    while (1);
  }
  Serial.println("Sensor warna Terdeteksi.");
}

void loop() {
  switch (currentState) {
    case IDLE: {
      float jarak = bacaJarak();
      if (jarak < 10.0) {
        Serial.println("Barang terdeteksi!");
        currentState = KONVEYOR_MAJU;
        stateStartTime = millis();
        nyalakanConveyor();
      }
      break;
    }

    case KONVEYOR_MAJU:
      if (millis() - stateStartTime >= 1000) {
        matikanConveyor();
        delay(2000); // tunggu barang di sensor warna
        currentState = DETEKSI_WARNA;
      }
      break;

    case DETEKSI_WARNA:
      warnaTerdeteksi = deteksiWarna();
      Serial.println("Warna: " + warnaTerdeteksi);
      if (warnaTerdeteksi == "MERAH") {
        currentState = SORTIR_MERAH;
      } else if (warnaTerdeteksi == "HIJAU") {
        currentState = SORTIR_HIJAU;
      } else {
        currentState = SORTIR_LAIN;
      }
      stateStartTime = millis();
      break;

    case SORTIR_MERAH:
      servoMerah.write(53);
      nyalakanConveyor();
      if (millis() - stateStartTime >= 5000) {
        servoMerah.write(0);
        matikanConveyor();
        currentState = SELESAI;
      }
      break;

    case SORTIR_HIJAU:
      servoHijau.write(0);
      nyalakanConveyor();
      if (millis() - stateStartTime >= 5000) {
        servoHijau.write(53);
        matikanConveyor();
        currentState = SELESAI;
      }
      break;

    case SORTIR_LAIN:
      nyalakanConveyor();
      if (millis() - stateStartTime >= 5000) {
        matikanConveyor();
        currentState = SELESAI;
      }
      break;

    case SELESAI:
      delay(1000); // waktu jeda
      currentState = IDLE;
      break;
  }

  delay(50); // waktu stabil antar loop
}

// === Fungsi Pendukung ===

float bacaJarak() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return 999;  // timeout
  return duration * 0.034 / 2.0;
}

String deteksiWarna() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);

  Serial.print("RGB: ");
  Serial.print(r); Serial.print(", ");
  Serial.print(g); Serial.print(", ");
  Serial.println(b);

  if (r > g && r > b && r > 15) return "MERAH";
  else if (g > r && g > b && g > 15) return "HIJAU";
  else return "LAIN";
}

void nyalakanConveyor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(ENA, HIGH);
}

void matikanConveyor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(ENA, LOW);
}


