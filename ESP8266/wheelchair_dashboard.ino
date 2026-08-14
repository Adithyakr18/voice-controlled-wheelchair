#include <ESP8266WiFi.h> 
#include <SoftwareSerial.h> 
#include <TinyGPS++.h> 
/* ================= WIFI ================= */ 
const char* ssid = "YOUR_WIFI_SSID"; 
const char* password = "YOUR_WIFI_PASSWORD"; 
/* ========== BUTTON & BUZZER ============= */ 
#define BTN D7 
#define BUZZ D8 
 
/* ================= GPS ================== */ 
#define GPS_RX D5 // GPS TX -> D5 
#define GPS_TX D6 // GPS RX -> D6 
SoftwareSerial gpsSerial(GPS_RX, GPS_TX); 
TinyGPSPlus gps; 
 
/* ================= SERVER =============== */ 
WiFiServer server(80); 
 
/* ============== VARIABLES =============== */ 
bool emergency = false; 
double latitude = 0.0; 
double longitude = 0.0; 
 
/* ===== DEFAULT LOCATION (GPS FAIL) ====== */ 
const double DEFAULT_LAT = 0.0; 
const double DEFAULT_LON = 0.0; 
 
/* =========== HTML IN FLASH ============== */ 
const char MAIN_PAGE[] PROGMEM = R"rawliteral( 
<!DOCTYPE html> 
<!DOCTYPE html> 
<html lang="en"> 
<head> 
<meta charset="UTF-8"> 
<meta name="viewport" content="width=device-width, initial-scale=1.0"> 
<title>WheelChair Monitoring System</title> 
<style> 
* { 
margin: 0; 
padding: 0; 
box-sizing: border-box; 
} 
body { 
font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;

background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); 
min-height: 100vh; 
color: #333; 
display: flex; 
justify-content: center; 
align-items: center; 
padding: 20px; 
} 
 
.container { 
max-width: 1200px; 
width: 100%; 
margin: 0 auto; 
} 
 
header { 
background: white; 
padding: 20px 30px; 
border-radius: 15px; 
box-shadow: 0 10px 40px rgba(0,0,0,0.15); 
margin-bottom: 25px; 
display: flex; 
justify-content: space-between; 
align-items: center; 
text-align: center; 
} 
 
.logo { 
display: flex; 
align-items: center; 
gap: 15px; 
justify-content: center; 
flex: 1; 
} 
.logo-icon { 
width: 45px; 
height: 45px; 
background: linear-gradient(135deg, #667eea, #764ba2); 
border-radius: 10px; 
display: flex; 
align-items: center; 
justify-content: center; 
font-size: 22px; 
} 
 
h1 { 
font-size: 26px; 
color: #2d3748; 
font-weight: 600;

} 
 
.status-badge { 
padding: 8px 18px; 
border-radius: 25px; 
font-weight: 600; 
font-size: 13px; 
text-transform: uppercase; 
letter-spacing: 1px; 
margin: 8px auto; 
display: inline-block; 
} 
 
.main-grid { 
display: grid; 
grid-template-columns: repeat(2, 1fr); 
grid-template-rows: 250px 300px; 
gap: 20px; 
margin-bottom: 25px; 
} 
 
.card { 
background: white; 
padding: 25px; 
border-radius: 15px; 
box-shadow: 0 10px 40px rgba(0,0,0,0.15); 
text-align: center; 
transition: transform 0.3s ease, box-shadow 0.3s ease; 
overflow: hidden; 
} 
 
.card:hover { 
transform: translateY(-5px); 
box-shadow: 0 15px 50px rgba(0,0,0,0.2); 
} 
 
.alert-card { 
grid-column: 1; 
grid-row: 1; 
padding: 30px; 
display: flex; 
flex-direction: column; 
justify-content: center; 
align-items: center; 
} 
 
.location-card { 
grid-column: 2; 
grid-row: 1; 
padding: 25px;

display: flex; 
flex-direction: column; 
 
} 
 
.history-card { 
grid-column: 1 / -1; 
grid-row: 2; 
padding: 25px; 
display: flex; 
flex-direction: column; 
} 
 
.alert-icon { 
font-size: 60px; 
margin-bottom: 15px; 
animation: pulse 2s infinite; 
} 
 
@keyframes pulse { 
0%, 100% { transform: scale(1); } 
50% { transform: scale(1.1); } 
} 
 
.alert-text { 
font-size: 28px; 
font-weight: 700; 
margin-bottom: 10px; 
line-height: 1.2; 
} 
 
.alert-timestamp { 
font-size: 13px; 
color: #718096; 
margin-top: 8px; 
} 
 
.card-title { 
font-size: 20px; 
font-weight: 600; 
color: #2d3748; 
margin-bottom: 20px; 
display: flex; 
align-items: center; 
justify-content: center; 
gap: 8px; 
} 
 
.card-title::before { 
content: '¡•
˙';

font-size: 22px; 
} 
 
.location-info { 
display: flex; 
 
flex-direction: column; 
gap: 12px; 
flex: 1; 
justify-content: center; 
} 
 
.info-row { 
display: flex; 
justify-content: space-between; 
align-items: center; 
padding: 12px 15px; 
background: #f7fafc; 
border-radius: 8px; 
border-left: 4px solid #667eea; 
} 
 
.info-label { 
font-size: 14px; 
color: #718096; 
font-weight: 500; 
text-transform: uppercase; 
letter-spacing: 0.5px; 
} 
 
.info-value { 
font-size: 16px; 
font-weight: 600; 
color: #2d3748; 
font-family: 'Courier New', monospace; 
} 
 
.map-button { 
margin-top: 20px; 
width: 180px; 
padding: 12px; 
background: linear-gradient(135deg, #667eea, #764ba2); 
color: white; 
border: none; 
border-radius: 8px; 
font-size: 15px; 
font-weight: 600; 
cursor: pointer; 
transition: all 0.3s ease; 
display: block;

margin-left: auto; 
margin-right: auto; 
} 
 
.map-button:hover { 
transform: translateY(-2px); 
box-shadow: 0 5px 20px rgba(102, 126, 234, 0.4); 
} 
.history-list { 
flex: 1; 
overflow-y: auto; 
margin-top: 10px; 
} 
 
.history-item { 
padding: 12px 15px; 
background: #f7fafc; 
border-radius: 8px; 
margin-bottom: 8px; 
display: flex; 
justify-content: space-between; 
align-items: center; 
border-left: 4px solid #cbd5e0; 
} 
 
.history-item.emergency { 
border-left-color: #fc8181; 
background: #fff5f5; 
} 
 
.connection-status { 
display: flex; 
align-items: center; 
gap: 8px; 
font-size: 13px; 
color: #718096; 
justify-content: center; 
} 
 
.status-dot { 
width: 8px; 
height: 8px; 
border-radius: 
50%; 
animation: blink 2s infinite; 
} 
 
@keyframes blink { 
0%, 100% { opacity: 1; } 
50% { opacity: 0.3; } 
}

.status-safe { 
background: #48bb78; 
color: #48bb78; 
} 
 
.status-emergency { 
background: #f56565; 
color: #f56565; 
 
} 
 
/* Map Overlay Styles */ 
.map-overlay { 
position: fixed; 
top: 0; 
left: 0; 
width: 100%; 
height: 100%; 
background: rgba(0, 0, 0, 0.7); 
display: flex; 
align-items: center; 
justify-content: center; 
z-index: 1000; 
opacity: 0; 
visibility: hidden; 
transition: all 0.3s ease; 
} 
 
.map-overlay.show { 
opacity: 1; 
visibility: visible; 
} 
 
.map-container { 
width: 90%; 
max-width: 800px; 
height: 70vh; 
background: white; 
border-radius: 20px; 
overflow: hidden; 
box-shadow: 0 25px 50px rgba(0,0,0,0.3); 
transform: scale(0.95); 
transition: transform 0.3s ease; 
} 
 
.map-overlay.show .map-container { 
transform: scale(1); 
} 
.map-header { 
background: linear-gradient(135deg, #667eea, #764ba2);

color: white; 
padding: 20px; 
display: flex; 
justify-content: space-between; 
align-items: center; 
} 
 
.map-title { 
font-size: 18px; 
font-weight: 600; 
 
} 
 
.map-close { 
background: rgba(255,255,255,0.2); 
border: none; 
color: white; 
width: 35px; 
height: 35px; 
border-radius: 50%; 
cursor: pointer; 
font-size: 20px; 
display: flex; 
align-items: center; 
justify-content: center; 
transition: background 0.3s ease; 
} 
 
.map-close:hover { 
background: rgba(255,255,255,0.3); 
} 
 
.map-frame { 
width: 100%; 
height: calc(100% - 60px); 
border: none; 
} 
 
@media (max-width: 1024px) { 
.main-grid { 
grid-template-columns: 1fr; 
grid-template-rows: auto auto auto; 
} 
 
.alert-card { 
grid-column: 1; 
grid-row: 1; 
} 
 
.location-card {

grid-column: 1; 
grid-row: 2; 
} 
 
.history-card { 
grid-column: 1; 
grid-row: 3; 
} 
} 
 
@media (max-width: 768px) { 
body { 
padding: 10px; 
 
} 
 
.container { 
width: 100%; 
} 
 
header { 
flex-direction: column; 
gap: 15px; 
padding: 15px 20px; 
} 
 
.logo { 
flex-direction: column; 
text-align: center; 
} 
 
h1 { 
font-size: 20px; 
} 
 
.card { 
padding: 20px; 
} 
 
.alert-text { 
font-size: 22px; 
} 
.alert-icon { 
font-size: 50px; 
} 
 
.info-row { 
flex-direction: column; 
gap: 8px;

text-align: center; 
} 
 
.info-label, .info-value { 
width: 100%; 
} 
 
.history-item { 
flex-direction: column; 
gap: 5px; 
text-align: center; 
} 
 
.map-container { 
width: 95%; 
height: 60vh; 
 
} 
} 
 
@media (max-width: 480px) { 
.map-container { 
width: 100%; 
height: 70vh; 
border-radius: 0; 
} 
 
.map-overlay { 
padding: 10px; 
} 
 
.alert-text { 
font-size: 20px; 
} 
 
.card-title { 
font-size: 18px; 
} 
} 
</style> 
</head> 
<body> 
<div class="container"> 
<header> 
<div class="logo"> 
<div class="logo-icon">⬛˙
 C</div> 
<h1>WheelChair Monitoring System</h1> 
</div> 
<div class="connection-status">

<span class="status-dot" id="statusDot"></span> 
<span id="connectionText">Connecting...</span> 
</div> 
</header> 
 
<div class="main-grid"> 
<div class="card alert-card"> 
<div class="alert-icon" id="alertIcon">✓</div> 
<div class="alert-text" id="alertText">ALL SYSTEMS NORMAL</div> 
<div class="status-badge" id="statusBadge">SAFE</div> 
<div class="alert-timestamp" id="lastUpdate">Last update: --</div> 
</div> 
 
<div class="card location-card"> 
<div class="card-title">Current Location</div> 
<div class="location-info"> 
<div class="info-row"> 
<span class="info-label">Latitude</span 
 
 
<span class="info-value" id="lat">--</span> 
</div> 
<div class="info-row"> 
<span class="info-label">Longitude</span> 
<span class="info-value" id="lon">--</span> 
</div> 
<div class="info-row"> 
<span class="info-label">Current Time</span> 
<span class="info-value" id="currentTime">--</span> 
</div> 
</div> 
<button class="map-button" id="mapBtn">View on Map</button> 
</div> 
 
<div class="card history-card"> 
<div class="card-title">Event History</div> 
<div class="history-list" id="historyList"> 
<div style="text-align: center; padding: 30px; color: #a0aec0;"> 
No events recorded yet 
</div> 
</div> 
</div> 
</div> 
</div> 
 
<!-- Map Overlay --> 
<div class="map-overlay" id="mapOverlay"> 
<div class="map-container"> 
<div class="map-header"> 
<div class="map-title">¡•
˙ Emergency Location</div>

<button class="map-close" id="mapClose">×</button> 
</div> 
<iframe class="map-frame" id="mapFrame"></iframe> 
</div> 
</div> 
 
<script> 
let alertCount = 0; 
let startTime = Date.now(); 
let history = []; 
let isEmergency = false; 
 
function updateCurrentTime() { 
const now = new Date(); 
const timeString = now.toLocaleTimeString('en-US', { 
hour: '2-digit', 
minute: '2-digit', 
second: '2-digit', 
hour12: true 
}); 
 
document.getElementById('currentTime').textContent = timeString; 
} 
 
function addToHistory(type, message) { 
const timestamp = new Date().toLocaleString(); 
history.unshift({ type, message, timestamp }); 
if (history.length > 6) history.pop(); 
 
const historyList = document.getElementById('historyList'); 
historyList.innerHTML = history.map(item => ` 
<div class="history-item ${item.type === 'EMERGENCY' ? 'emergency' : ''}"> 
<div> 
<strong>${item.type}</strong>: ${item.message} 
</div> 
<div style="font-size: 11px; color: #a0aec0;">${item.timestamp}</div> 
</div> 
`).join(''); 
} 
 
function openMap(lat, lon) { 
const mapOverlay = document.getElementById('mapOverlay'); 
const mapFrame = document.getElementById('mapFrame'); 
 
mapFrame.src = `https://www.google.com/maps?q=${lat},${lon}&output=embed&z=15`; 
mapOverlay.classList.add('show'); 
document.body.style.overflow = 'hidden'; 
} 
 
function closeMap() {

const mapOverlay = document.getElementById('mapOverlay'); 
mapOverlay.classList.remove('show'); 
document.body.style.overflow = 'auto'; 
 
const mapFrame = document.getElementById('mapFrame'); 
mapFrame.src = ''; 
} 
 
function updateStatus(data) { 
const alertText = document.getElementById('alertText'); 
const alertIcon = document.getElementById('alertIcon'); 
const statusBadge = document.getElementById('statusBadge'); 
const alertCard = document.querySelector('.alert-card'); 
const currentEmergency = data.alert !== 'ALL RIGHT'; 
if (currentEmergency !== isEmergency) { 
isEmergency = currentEmergency; 
if (isEmergency) { 
alertCount++; 
addToHistory('EMERGENCY', `Alert triggered at ${data.lat}, ${data.lon}`); 
 
openMap(data.lat, data.lon); 
} else { 
addToHistory('INFO', 'System returned to normal'); 
} 
} 
 
if (isEmergency) { 
alertText.textContent = data.alert || 'EMERGENCY ALERT'; 
alertIcon.textContent = '.ı '; 
statusBadge.textContent = 'EMERGENCY'; 
statusBadge.style.background = '#fc8181'; 
statusBadge.style.color = 'white'; 
alertCard.style.background = 'linear-gradient(135deg, #fff5f5, #fed7d7)'; 
} else { 
alertText.textContent = 'ALL SYSTEMS NORMAL'; 
alertIcon.textContent = '✓'; 
statusBadge.textContent = 'SAFE'; 
statusBadge.style.background = '#c6f6d5'; 
statusBadge.style.color = '#22543d'; 
alertCard.style.background = 'white'; 
} 
 
document.getElementById('lat').textContent = data.lat?.toFixed(6) || '--'; 
document.getElementById('lon').textContent = data.lon?.toFixed(6) || '--'; 
document.getElementById('lastUpdate').textContent = `Last update: ${new 
Date().toLocaleTimeString()}`; 
const statusDot = document.getElementById('statusDot'); 
2

const connectionText = document.getElementById('connectionText'); 
statusDot.className = 'status-dot status-safe'; 
 
connectionText.textContent = 'Connected'; 
} 
 
function fetchStatus() { 
fetch('/status') 
.then(r => r.json()) 
.then(data => { 
updateStatus(data); 
}) 
.catch(err => { 
console.error('Connection error:', err); 
document.getElementById('connectionText').textContent = 'Connection Lost'; 
document.getElementById('statusDot').className = 'status-dot'; 
}); 
} 
 
document.getElementById('mapBtn').addEventListener('click', () => { 
const lat = document.getElementById('lat').textContent; 
const lon = document.getElementById('lon').textContent; 
if (lat !== '--' && lon !== '--') { 
openMap(parseFloat(lat), parseFloat(lon)); 
 
 
} 
}); 
 
document.getElementById('mapClose').addEventListener('click', closeMap); 
 
document.getElementById('mapOverlay').addEventListener('click', (e) => { 
if (e.target === document.getElementById('mapOverlay')) { 
closeMap(); 
} 
}); 
 
document.addEventListener('keydown', (e) => { 
if (e.key === 'Escape') { 
closeMap(); 
} 
}); 
setInterval(updateCurrentTime, 1000); 
setInterval(fetchStatus, 1000); 
 
updateCurrentTime(); 
fetchStatus(); 
addToHistory('INFO', 'System initialized'); 
</script>

</body> 
</html> 
 
)rawliteral"; 
 
/* ======================================= */ 
 
void setup() { 
Serial.begin(115200); 
gpsSerial.begin(9600); 
 
pinMode(BTN, INPUT_PULLUP); 
pinMode(BUZZ, OUTPUT); 
digitalWrite(BUZZ, LOW); 
 
WiFi.begin(ssid, password); 
while (WiFi.status() != WL_CONNECTED) { 
delay(500); 
} 
 
server.begin(); 
Serial.println("ESP8266 IP: "); 
Serial.println(WiFi.localIP()); 
} 
 
void loop() { 
 
 
/* BUTTON & BUZZER */ 
emergency = (digitalRead(BTN) == LOW); 
digitalWrite(BUZZ, emergency ? HIGH : LOW); 
 
/* GPS READ */ 
while (gpsSerial.available()) { 
gps.encode(gpsSerial.read()); 
} 
bool gpsValid = gps.location.isValid(); 
if (gpsValid) { 
latitude = gps.location.lat(); 
longitude = gps.location.lng(); 
} else { 
latitude = DEFAULT_LAT; 
longitude = DEFAULT_LON; 
} 
/* WEB SERVER */ 
WiFiClient client = server.available(); 
if (!client) return;

while (!client.available()) delay(1); 
String request = client.readStringUntil('\r'); 
client.flush(); 
/* STATUS API */ 
if (request.indexOf("/status") != -1) { 
client.println("HTTP/1.1 200 OK"); 
client.println("Content-Type: application/json"); 
client.println("Connection: close"); 
client.println(); 
 
client.print("{\"alert\":\""); 
client.print(emergency ? "EMERGENCY ALERT" : "ALL RIGHT"); 
client.print("\",\"lat\":"); 
client.print(latitude, 6); 
client.print(",\"lon\":"); 
client.print(longitude, 6); 
client.println("}"); 
 
return; 
} 
 
/* MAIN PAGE FROM FLASH */ 
client.println("HTTP/1.1 200 OK"); 
client.println("Content-Type: text/html"); 
client.println("Connection: close"); 
client.println(); 
 
client.print(FPSTR(MAIN_PAGE)); 
}
