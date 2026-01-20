#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

// Motor pins
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENA D5
#define ENB D6

int speedVal = 800;   // Default speed (0–1023)

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  WiFi.softAP("Robot_Car", "12345678");

  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.on("/speed", handleSpeed);

  server.begin();

  stopMotor();
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
}

void loop() {
  server.handleClient();
}

// ================= WEB PAGE =================
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
  font-family: Arial, Helvetica, sans-serif;
  background: #f2f4f7;
  text-align: center;
  margin: 0;
  padding: 0;
}

.container {
  max-width: 360px;
  margin: auto;
  padding: 20px;
}

h2 {
  color: #333;
  margin-bottom: 20px;
}

.btn {
  width: 100px;
  height: 55px;
  font-size: 16px;
  margin: 6px;
  border: none;
  border-radius: 8px;
  background: #4a6fa5;
  color: white;
}

.btn:active {
  background: #395a85;
}

.stop {
  background: #d9534f;
}

.stop:active {
  background: #b52b27;
}

.speed-box {
  margin-top: 20px;
  background: #ffffff;
  padding: 15px;
  border-radius: 10px;
}

input[type=range] {
  width: 100%;
}
</style>
</head>

<body>
<div class="container">
  <h2>Wi-Fi Robot Controller</h2>

  <!-- Forward -->
  <button class="btn"
    onmousedown="cmd('F')" onmouseup="cmd('S')"
    ontouchstart="cmd('F')" ontouchend="cmd('S')">
    Forward
  </button><br>

  <!-- Left | Stop | Right -->
  <button class="btn"
    onmousedown="cmd('L')" onmouseup="cmd('S')"
    ontouchstart="cmd('L')" ontouchend="cmd('S')">
    Left
  </button>

  <button class="btn stop" onclick="cmd('S')">
    Stop
  </button>

  <button class="btn"
    onmousedown="cmd('R')" onmouseup="cmd('S')"
    ontouchstart="cmd('R')" ontouchend="cmd('S')">
    Right
  </button><br>

  <!-- Backward -->
  <button class="btn"
    onmousedown="cmd('B')" onmouseup="cmd('S')"
    ontouchstart="cmd('B')" ontouchend="cmd('S')">
    Backward
  </button>

  <div class="speed-box">
    <p>Speed Control</p>
    <input type="range" min="0" max="1023" value="800"
           onchange="setSpeed(this.value)">
  </div>
</div>

<script>
function cmd(c){
  fetch('/move?dir=' + c);
}
function setSpeed(v){
  fetch('/speed?val=' + v);
}
</script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}


// ================= CONTROL HANDLERS =================
void handleMove() {
  String dir = server.arg("dir");

  if (dir == "F") forward();
  else if (dir == "B") backward();
  else if (dir == "L") left();
  else if (dir == "R") right();
  else stopMotor();

  server.send(200, "text/plain", "OK");
}

void handleSpeed() {
  speedVal = server.arg("val").toInt();
  analogWrite(ENA, speedVal);
  analogWrite(ENB, speedVal);
  server.send(200, "text/plain", "Speed Set");
}

// ================= MOTOR FUNCTIONS =================
void backward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, HIGH);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void left() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void stopMotor() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}