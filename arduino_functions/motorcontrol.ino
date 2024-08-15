#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>  // Make sure to install the ArduinoJson library, its needed for parsing
#include <Servo.h>

Servo waterMotor;  // Servo object for controlling the water motor
Servo foodMotor;   // Servo object for controlling the food motor


// Network credentials
const char* ssid = "irvin"; // hotspot name, will be wifi name in future
const char* password = "ilovedimsum"; // pw

// Firebase database URL
const char* firebaseHost = "theta-tau-pet-feeder-default-rtdb.firebaseio.com"; // firebase DB url
const int httpsPort = 443; // HTTPS port 

WiFiSSLClient sslClient; // client used for connecting through HTTPS, firebase does not support HTTP
HttpClient client = HttpClient(sslClient, firebaseHost, httpsPort);

void setup() {
  Serial.begin(19200);
  waterMotor.attach(9);  // Attaches the water motor servo to pin 9
  foodMotor.attach(10);  // Attaches the food motor servo to pin 10
  WiFi.begin(ssid, password); // begins wifi connections

  while (WiFi.status() != WL_CONNECTED) { // wifi connection check
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // Check FeedOn status
  client.get("/FeedOn.json");
  String feedStatus = client.responseBody().c_str();
  Serial.println("Received FeedOn status: " + feedStatus);  // Debug output print

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, feedStatus); // parsing the json output
  String value = doc["value"].as<String>();
  value.trim();
  
  if (value.equalsIgnoreCase("true")) { // feeding on and turn off motor after 
    Serial.println("Feeding...");
    operateMotor(foodMotor, true);
    delay(2000);
    Serial.println("Feeding complete");
    operateMotor(foodMotor, false);
  } else {
    Serial.println("Food status is set to false");
    Serial.println("Checking again in 1 second");
  }

  // Check WaterOn status // same logic of the feeding loop
  client.get("/WaterOn.json");
  String waterStatus = client.responseBody().c_str();
  Serial.println("Received WaterOn status: " + waterStatus);  // Debug output

  DynamicJsonDocument doc2(1024);
  deserializeJson(doc2, waterStatus);
  String value2 = doc2["value"].as<String>();
  value2.trim();

  if (value2.equalsIgnoreCase("true")) { //JSOn
    Serial.println("Watering...");
    operateMotor(waterMotor, true);
    delay(2000);
    operateMotor(waterMotor, false);
    Serial.println("Watering complete");
  } else {
    Serial.println("Water status is set to false");
    Serial.println("Checking again in 1 second");
  }

  delay(500); // Wait for 10 seconds before next check
}
// MOTOR OPERATION LOGIC
void operateMotor(Servo &motor, bool activate) {
  if (activate) {
    motor.write(180);  // Activate the motor
  } else {
    motor.write(90);  // Deactivate the motor
  }
}

