/*
 * BLIND ASSISTANCE SYSTEM – ESP32 ACCESS POINT (PRECISION)
 * Sensors: FRONT, RIGHT, BACK
 * Output: HTTP text messages (voice-ready)
 * Phone connects directly to ESP32 Wi-Fi
 *
 * Improvements:
 * - Median filtering
 * - Cross-talk prevention
 * - Stable distance calculation
 */

#include <WiFi.h>
#include <WebServer.h>

// ================= SENSOR PINS =================
// FRONT
#define TRIG_FRONT  27
#define ECHO_FRONT  26

// RIGHT
#define TRIG_RIGHT  33
#define ECHO_RIGHT  32

// BACK
#define TRIG_BACK   12
#define ECHO_BACK   14

// ================= DISTANCE THRESHOLDS (cm) =================
#define DANGER_CM   50
#define WARNING_CM  100
#define INFO_CM     200

// ================= SENSOR CONFIG =================
#define SAMPLES        5
#define MIN_ECHO_US    150
#define MAX_ECHO_US    30000
#define SENSOR_DELAY   60   // ms between sensors (CRITICAL)

WebServer server(80);

// ================= MEDIAN FILTER =================
int median(int *arr, int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (arr[j] < arr[i]) {
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
      }
    }
  }
  return arr[n / 2];
}

// ================= DISTANCE FUNCTION =================
int getDistanceFiltered(int trig, int echo) {
  int readings[SAMPLES];
  int count = 0;

  for (int i = 0; i < SAMPLES; i++) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    long duration = pulseIn(echo, HIGH, MAX_ECHO_US);

    if (duration > MIN_ECHO_US && duration < MAX_ECHO_US) {
      readings[count++] = duration;
    }

    delay(10);
  }

  if (count < 3) return 999;

  long echo_us = median(readings, count);

  // Accurate conversion
  int distance_cm = echo_us / 58.2;
  return constrain(distance_cm, 2, 400);
}

// ================= HTTP HANDLER =================
void handleAlert() {
  int front = getDistanceFiltered(TRIG_FRONT, ECHO_FRONT);
  delay(SENSOR_DELAY);

  int right = getDistanceFiltered(TRIG_RIGHT, ECHO_RIGHT);
  delay(SENSOR_DELAY);

  int back  = getDistanceFiltered(TRIG_BACK, ECHO_BACK);
  delay(SENSOR_DELAY);

  // Debug
  Serial.print("F:");
  Serial.print(front);
  Serial.print("  R:");
  Serial.print(right);
  Serial.print("  B:");
  Serial.println(back);

  int minDist = front;
  String dir = "front";

  if (right < minDist) {
    minDist = right;
    dir = "right";
  }
  if (back < minDist) {
    minDist = back;
    dir = "back";
  }

  if (minDist > INFO_CM) {
    server.send(200, "text/plain", "Path clear");
    return;
  }

  String level;
  if (minDist < DANGER_CM) level = "very close";
  else if (minDist < WARNING_CM) level = "close";
  else level = "approaching";

  String message =
    "Obstacle " + dir + ", " +
    level + ", " +
    String(minDist) + " centimeters";

  Serial.println(message);
  server.send(200, "text/plain", message);
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);
  pinMode(TRIG_BACK, OUTPUT);
  pinMode(ECHO_BACK, INPUT);

  digitalWrite(TRIG_FRONT, LOW);
  digitalWrite(TRIG_RIGHT, LOW);
  digitalWrite(TRIG_BACK, LOW);

  // ===== ESP32 ACCESS POINT =====
  const char* ssid = "BlindAssist";
  const char* password = "12345678";

  WiFi.softAP(ssid, password);

  Serial.println("=================================");
  Serial.println(" ESP32 BLIND ASSIST ACCESS POINT");
  Serial.println("=================================");
  Serial.print("Wi-Fi name: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("URL: http://192.168.4.1/alert");

  // ===== HTTP SERVER =====
  server.on("/alert", handleAlert);
  server.begin();
  Serial.println("HTTP server started");
}

// ================= LOOP =================
void loop() {
  server.handleClient();
}
