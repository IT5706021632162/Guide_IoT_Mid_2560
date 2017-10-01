#include <LiquidCrystal_I2C.h>

#include "DHT.h"
#include <ESP8266WiFi.h>
//thingspeak.com

#define DHTPIN D4     // what pin we're connected to GPIO2 (D4)
#define DHTTYPE DHT22   // DHT 22

#define DEBUG

#define DEBUG_PRINTER Serial

#ifdef DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif

const char *ssid = "Bigcamp_FTTx";              
const char *password = "bc123456";

DHT *dht;

void connectWifi();
void reconnectWifiIfLinkDown();
void initDht(DHT **dht, uint8_t pin, uint8_t dht_type);
void readDht(DHT *dht, float *temp, float *humid);
void uploadThingsSpeak(float t, float h);               //ส่งข้อมูลอะไรไปให้ thingspeak.com ให้ดูตรงนี้

void setup() {
    Serial.begin(115200);
    delay(10);
    connectWifi();
    initDht(&dht, DHTPIN, DHTTYPE);
}

void loop() {
    static float t_dht;
    static float h_dht;

    readDht(dht, &t_dht, &h_dht);
    
    uploadThingsSpeak(t_dht, h_dht);

    // Wait a few seconds between measurements.
    delay(10 * 1000);
    reconnectWifiIfLinkDown();
}

void reconnectWifiIfLinkDown() {
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINTLN("WIFI DISCONNECTED");
        connectWifi();
    }
}

void connectWifi() {
    DEBUG_PRINTLN();
    DEBUG_PRINTLN();
    DEBUG_PRINT("Connecting to ");
    DEBUG_PRINTLN(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        DEBUG_PRINT(".");
    }

    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("WiFi connected");
    DEBUG_PRINTLN("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
}

void initDht(DHT **dht, uint8_t pin, uint8_t dht_type) {
     *dht = new DHT(pin, dht_type, 30);
    (*dht)->begin();
    DEBUG_PRINTLN(F("DHTxx test!"))  ;
}

void uploadThingsSpeak(float t, float h) {
    static const char* host = "api.thingspeak.com"; /////////////////
   // static const char* apiKey = "DMKEBXBZDUI3M5JV"; ///////////////////////
    static const char* apiKey = "8U963VSRZFSHHLR8";

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        DEBUG_PRINTLN("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/update/";
    //  url += streamId;
    url += "?key=";
    url += apiKey;
    url += "&field1=";
    url += t;
    url += "&field2=";
    url += h;

    DEBUG_PRINT("Requesting URL: ");
    DEBUG_PRINTLN(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
}


void readDht(DHT *dht, float *temp, float *humid) {

    if (dht == NULL) {
        DEBUG_PRINTLN(F("[dht11] is not initialised. please call initDht() first."));
        return;
    }

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht->readHumidity();

    // Read temperature as Celsius
    float t = dht->readTemperature();
    // Read temperature as Fahrenheit
    float f = dht->readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
        DEBUG_PRINTLN("Failed to read from DHT sensor!");
        return;
    }

    // Compute heat index
    // Must send in temp in Fahrenheit!
    float hi = dht->computeHeatIndex(f, h);
    Serial.println("----------------------------------");
    DEBUG_PRINT("Humidity: ");
    DEBUG_PRINT(h);
    DEBUG_PRINT(" %\t");
    Serial.println("");
    DEBUG_PRINT("Temperature: ");
    DEBUG_PRINT(t);
    DEBUG_PRINT(" *C  ");
    DEBUG_PRINT(f);
    DEBUG_PRINT(" *F\t");
    Serial.println("");
    DEBUG_PRINT("Heat index: ");
    DEBUG_PRINT(hi);
    DEBUG_PRINTLN(" *F");

    *temp = t;
    *humid = h;
}
