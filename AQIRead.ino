#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 25
#define DHTTYPE DHT22
#define MQ_PIN 33
#define PM_LED 32
#define PM_ADC 35

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

//PM SENSOR READ 
float readPMraw(int &rawADC, float &voltage) {
  digitalWrite(PM_LED, LOW);
  delayMicroseconds(280);

  rawADC = analogRead(PM_ADC);
  delayMicroseconds(40);

  digitalWrite(PM_LED, HIGH);
  delayMicroseconds(9680);

  voltage = (rawADC / 4095.0) * 3.3;

  float pm25 = 172.0 * voltage - 100.0;
  if (pm25 < 0) pm25 = 0;

  return pm25;
}

// AQI CALC 
float calcAQI(float pm) {
  float Clow, Chigh, Ilow, Ihigh;

  if (pm <= 12.0) { Clow=0; Chigh=12; Ilow=0; Ihigh=50; }
  else if (pm <= 35.4) { Clow=12.1; Chigh=35.4; Ilow=51; Ihigh=100;}
  else if (pm <= 55.4) { Clow=35.5; Chigh=55.4; Ilow=101; Ihigh=150;}
  else if (pm <= 150.4){ Clow=55.5; Chigh=150.4; Ilow=151; Ihigh=200;}
  else if (pm <= 250.4){ Clow=150.5; Chigh=250.4; Ilow=201; Ihigh=300;}
  else if (pm <= 350.4){ Clow=250.5; Chigh=350.4; Ilow=301; Ihigh=400;}
  else { Clow=350.5; Chigh=500.4; Ilow=401; Ihigh=500;}

  return ((Ihigh - Ilow)/(Chigh - Clow)) * (pm - Clow) + Ilow;
}

// HTML PAGE 
String page = R"====(
<!DOCTYPE html><html>
<head><meta name='viewport' content='width=device-width, initial-scale=1'>
<title>AQI Monitor</title>
<style>
  body{
    font-family: Arial;
    background: #f8f9fb;        /* lighter background */
    text-align:center;
    padding:20px;
    color:#222222;              /* darker text */
  }

  h2, h1{
    color:#003355;              /* darker navy */
    font-weight:700;
  }

  .card{
    background:#ffffff;         /* pure white */
    border-radius:18px;
    padding:20px;
    box-shadow:0 4px 12px rgba(0,0,0,0.15);
    width:80%;
    max-width:350px;
    margin:15px auto;
    border:1px solid #e2e6ea;   /* subtle border */
  }

  .big{
    padding:30px;
    font-size:22px;
    font-weight:bold;
    color:#ffffff;
  }

  .bigvalue{
    font-size:60px;
    font-weight:bold;
    margin:10px 0;
  }

  .value{
    font-size:32px;
    font-weight:bold;
    color:#003355;          /* darker stronger text */
  }

  .label{
    color:#444444;          /* darker label text */
    font-size:18px;
    font-weight:600;
  }
</style>

<script>
function update(){
 fetch("/data").then(r=>r.json()).then(d=>{
   document.getElementById("aqi").innerHTML=d.aqi.toFixed(0);
   document.getElementById("pm").innerHTML=d.pm.toFixed(1);
   document.getElementById("temp").innerHTML=d.temp.toFixed(1);
   document.getElementById("hum").innerHTML=d.hum.toFixed(1);
   document.getElementById("smoke").innerHTML=d.smoke;

   // color card
   let a=d.aqi, c=document.getElementById("aqicard");
   if(a<=50)c.style.background="#4caf50";
   else if(a<=100)c.style.background="#ffeb3b";
   else if(a<=200)c.style.background="#ff9800";
   else if(a<=300)c.style.background="#f44336";
   else c.style.background="#6a1b9a";
 });
}
setInterval(update,1000);
</script></head>

<body onload="update()">
<h2>ESP32 Air Quality Monitor</h2>

<div id='aqicard' class='card big'>
  <div class='label'>Air Quality Index</div>
  <div class='bigvalue' id='aqi'>--</div>
</div>

<div class='card'><div class='label'>PM2.5 (µg/m³)</div><div class='value' id='pm'>--</div></div>
<div class='card'><div class='label'>Temperature (°C)</div><div class='value' id='temp'>--</div></div>
<div class='card'><div class='label'>Humidity (%)</div><div class='value' id='hum'>--</div></div>
<div class='card'><div class='label'>MQ Smoke</div><div class='value' id='smoke'>--</div></div>

</body></html>
)====";

// JSON API 
void sendData(){
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  int mqRaw = analogRead(MQ_PIN);
  float mqPct = (mqRaw/4095.0)*100.0;
  String mqStat =
    mqPct < 10 ? "Clean" :
    mqPct < 30 ? "Mild Smoke" :
    mqPct < 60 ? "Polluted" :
    mqPct < 80 ? "Heavy" :
                 "Dangerous";

  int pmRaw;
  float pmVolt;
  float pm = readPMraw(pmRaw, pmVolt);

  // Delhi realistic correction 
  float estPM = pm;

  if(pm < 15){
    estPM = (0.45 * hum) + (mqPct * 0.8) + (pmVolt * 10);
    if(estPM < 10) estPM = 10;
    if(estPM > 200) estPM = 200;
  }

  float aqi = calcAQI(estPM);

  String json="{";
  json+="\"temp\":"+String(temp)+",";
  json+="\"hum\":"+String(hum)+",";
  json+="\"pm\":"+String(estPM)+",";
  json+="\"aqi\":"+String(aqi)+",";
  json+="\"smoke\":\""+mqStat+"\"";
  json+="}";

  server.send(200,"application/json",json);
}

// SETUP 
void setup(){
  Serial.begin(115200);
  pinMode(PM_LED, OUTPUT);
  dht.begin();

  WiFi.softAP("MyESP", "12345678");

  server.on("/", [](){ server.send(200,"text/html",page); });
  server.on("/data", sendData);
  server.begin();
}

void loop(){
  server.handleClient();
}
