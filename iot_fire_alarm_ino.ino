#define BLYNK_TEMPLATE_ID  "TMPL3W_DkHXUO"
#define BLYNK_TEMPLATE_NAME  "fire alarm security system"
#define BLYNK_AUTH_TOKEN "LpaN4rS3tigMYQtfdrajDJmPG0S5pAfr"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "realme 12+ 5G";
char pass[] = "12345678";

#define FLAME_SENSOR 32
#define GAS_SENSOR   33
#define PIR_SENSOR   25
#define BUZZER       26

bool flameSent = false;
bool gasSent = false;
bool motionSent = false;

void setup() {
  Serial.begin(115200);

  pinMode(FLAME_SENSOR, INPUT);
  pinMode(GAS_SENSOR, INPUT);
  pinMode(PIR_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  int flame = digitalRead(FLAME_SENSOR);
  int gas = digitalRead(GAS_SENSOR);
  int motion = digitalRead(PIR_SENSOR);

  String flameStatus = (flame == LOW) ? " Fire Detected!" : " Safe";
  String gasStatus = (gas == LOW) ? "💨 Gas Leak!" : "Safe";
  String motionStatus = (motion == HIGH) ? "🚶 Motion Detected!" : " No Motion";

  Blynk.virtualWrite(V0, flameStatus);
  Blynk.virtualWrite(V2, gasStatus);
  Blynk.virtualWrite(V1, motionStatus);

  // Trigger Events for Notification - only once per event
  if (flame == LOW && !flameSent) {
    Blynk.logEvent("fire_alert", " Flame Detected!");
    flameSent = true;
  } else if (flame == HIGH) {
    flameSent = false;
  }

  if (gas == LOW && !gasSent) {
    Blynk.logEvent("gas_alert", "💨 Gas Detected!");
    gasSent = true;
  } else if (gas == HIGH) {
    gasSent = false;
  }

  if (motion == HIGH && !motionSent) {
    Blynk.logEvent("motion_alert", " Motion Detected!");
    motionSent = true;
  } else if (motion == LOW) {
    motionSent = false;
  }

  // Buzzer triggers only on fire or gas
  bool danger = (flame == LOW || gas == LOW);

  if (danger) {
    digitalWrite(BUZZER, HIGH);
    Blynk.virtualWrite(V3, " Alarm Active!");
  } else {
    digitalWrite(BUZZER, LOW);
    Blynk.virtualWrite(V3, "
     Normal");
  }

  delay(500);
}
