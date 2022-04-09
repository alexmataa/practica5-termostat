/*********
  Nicolas Constantinou
  ESP32 SoftAP with analog and digital inputs displayed on a webpage with an Async Webserver
*********/

// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>



extern float humd;
extern float temp;
extern float termo;

// Variables to store temperature values
String temperatureC = "";
String humidityV = "";
String termoV = "";
String wifi_status = "";

//Call Sensors and store actual value after equation to string
String readhumidity () {     
    
                      
     return String(humd); 
}

void init_wifi();

String readTermo(){
  return String(termo);
}

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 300;

// Replace with your network credentials - CHANGED
const char* ssid = "MOVISTAR_04E0"; // Enter your SSID here
const char* password = "DHdGw9aNUhHXLiB8x6Lp"; //Enter your Password here

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDSTemperatureC() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  
  return String(temp);
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP32 Sensor Temp/Hum</h2>
  <p>
    <span class="ds-labels">Temperature</span><br> 
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="ds-labels">Humidity</span><br> 
    <span id="humidityValue">%HUMIDITYV%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    <span class="ds-labels">Termostato</span><br> 
    <span id="termoValue">%TERMOV%</span>
    <sup class="units">&deg;C</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
    var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidityValue").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidityValue", true);
  xhttp.send();
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("termoValue").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/termoValue", true);
  xhttp.send();
}, 500) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DS18B20 values - NEW - pressure values too
String processor(const String& var) {
  //Serial.println(var);
  if (var == "TEMPERATUREC") {
    return temperatureC;
  }
  else if (var == "HUMIDITYV") {
    return humidityV;
  }
  else if (var == "TERMOV") {
    return termoV;
  }
 
  return String();
}

void websensor(void* param);

void init_wifi(){
    xTaskCreate(
       websensor,
       "websebsor",
       10000,
       NULL,
       1,
       NULL
    );
}


void websensor(void* param) {
  // Serial port for debugging purposes
 // Serial.begin(115200);
  Serial.println();

    WiFi.begin(ssid, password);
    wifi_status="Connectant...";
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
    Serial.println(WiFi.localIP());
    wifi_status="IP: "+ WiFi.localIP().toString();

  //Serial.println(xPortGetCoreID());
  
  //Declare strings to be used by webserver
  temperatureC = readDSTemperatureC();
  
  humidityV = readhumidity();

  termoV = readTermo();


  // Wi-Fi Soft AP start
  //WiFi.softAP(ssid, password);

  // Print ESP Local IP Address
  //Serial.println(WiFi.localIP());
  //Serial.println();
  //Serial.print("IP address: ");
  //Serial.println(WiFi.softAPIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", temperatureC.c_str());
  });
  server.on("/humidityValue", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", humidityV.c_str());
  });
    server.on("/termoValue", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", termoV.c_str());
  });
  // Start server
  server.begin();


for (;;) {
  if ((millis() - lastTime) > timerDelay) {
    temperatureC = readDSTemperatureC();
    termoV=readTermo();
     humidityV = readhumidity(); ;
    lastTime = millis();
  }
  delay(500);

 }
 }