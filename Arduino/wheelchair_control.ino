char data; 
 
 
#define TRIG_PIN A0 
#define ECHO_PIN A1 
 
long duration; 
int distance; 
 
void setup() { 
Serial.begin(9600);  // Bluetooth / Voice commands 
 
 
pinMode(7, OUTPUT); // Motor 1 IN1 
pinMode(6, OUTPUT); // Motor 1 IN2 
pinMode(5, OUTPUT); // Motor 2 IN1 
pinMode(4, OUTPUT); // Motor 2 IN2 
 
pinMode(TRIG_PIN, OUTPUT); 
pinMode(ECHO_PIN, INPUT); 
} 
 
 
void loop() { 
measureDistance(); 
// -------- SAFETY STOP -------- 
if (distance <= 50) { 
Stop(); 
} 
// -------- READ COMMAND --------

if (Serial.available() > 0) { 
data = Serial.read(); 
 
if (data == 'F') { 
moveForward(); 
} 
else if (data == 'B') { 
 
 
 
moveBackward(); 
} 
else if (data == 'L') { 
turnLeft(); 
} 
else if (data == 'R') { 
turnRight(); 
} 
else if (data == 'S') { 
Stop(); 
} 
} 
 
delay(30); 
} 
 
 
// 
// FUNCTIONS 
// 
 
 
void measureDistance() { 
digitalWrite(TRIG_PIN, LOW); 
delayMicroseconds(2); 
 
digitalWrite(TRIG_PIN, HIGH);

delayMicroseconds(10); 
digitalWrite(TRIG_PIN, LOW); 
 
duration = pulseIn(ECHO_PIN, HIGH); 
distance = duration * 0.034 / 2; 
 
 
// Debugging only (Serial Monitor) 
Serial.print("Distance: "); 
Serial.println(distance); 
} 
 
 
void moveForward() { 
if (distance > 50) { 
digitalWrite(7, LOW); 
digitalWrite(6, HIGH); 
digitalWrite(5, LOW); 
digitalWrite(4, HIGH); 
} 
} 
 
 
void moveBackward() { 
digitalWrite(7, HIGH); 
digitalWrite(6, LOW); 
digitalWrite(5, HIGH); 
digitalWrite(4, LOW); 
delay(1500); 
Stop(); 
} 
 
 
void turnLeft() { 
digitalWrite(7, LOW); 
digitalWrite(6, HIGH); 
digitalWrite(5, HIGH);

digitalWrite(4, LOW); 
delay(1200); 
Stop(); 
} 
 
 
void turnRight() { 
digitalWrite(7, HIGH); 
digitalWrite(6, LOW); 
digitalWrite(5, LOW); 
digitalWrite(4, HIGH); 
delay(1200); 
Stop(); 
} 
 
 
void Stop() { 
digitalWrite(7, LOW); 
digitalWrite(6, LOW); 
digitalWrite(5, LOW); 
digitalWrite(4, LOW); 
}
