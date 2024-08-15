#include <WiFiS3.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>  // Make sure to install the ArduinoJson library, its needed for parsing
#include <Servo.h>

void checkWaterOn();
void checkFeedOn();
void sendWater();

//Water pins and Values
#define waterPowPin 7
#define waterSigPin A0
#define waterMin 332
#define waterMax 425
int Wvalue = 0; // variable to store the sensor value
int Wlevel = 0; // variable to store the water level

// Servo objects for controlling the motors
Servo waterMotor;  // Servo object for controlling the water motor
Servo foodMotor;   // Servo object for controlling the food motor


// Network credentials
const char* ssid = "UCLA_RES_IOT"; // hotspot name, will be wifi name in future
const char* password = "PleaseWork123*&"; // pw

// Firebase database URL
const char* firebaseHost = "theta-tau-pet-feeder-default-rtdb.firebaseio.com"; // firebase DB url
const int httpsPort = 443; // HTTPS port 

WiFiSSLClient sslClient; // client used for connecting through HTTPS, firebase does not support HTTP
HttpClient client = HttpClient(sslClient, firebaseHost, httpsPort);

void setup()
{
  Serial.begin(115200); // Serial start

  // Water sensor configurations
  pinMode(waterPowPin, OUTPUT); 
  digitalWrite(waterPowPin, LOW);

  // Motor configurations
  waterMotor.attach(9);  // Attaches the water motor servo to pin 9
  foodMotor.attach(10);  // Attaches the food motor servo to pin 10

  Serial.println("Attempting wifi connection");

  // Connect to wifi
  connectWifi();

  Serial.println("Wifi connection complete");

}

void loop()
{
  // Check Statuses:
  checkFeedOn();
  checkWaterOn();
  // Check Water Level and send new water level
  readWater();
  int statusCode = client.responseStatusCode();
  Serial.print("HTTP Status Code: ");
  Serial.println(statusCode);

}

// MOTOR OPERATION LOGIC
void operateMotor(Servo &motor, bool activate)
{
  if (activate) {
    motor.write(180);  // Activate the motor
  } else {
    motor.write(90);  // Deactivate the motor
  }
}


// // Read water level, for some reason it fucks up the ceiling when i return the val
 void readWater()
 {
  digitalWrite(waterPowPin, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  Wvalue = analogRead(waterSigPin); // read the analog value from sensor
  digitalWrite(waterPowPin, LOW);   // turn the sensor OFF

  Wlevel = map(Wvalue, waterMin, waterMax, 0, 4); // 4 levels

  if(Wlevel < 0)
  {
    Wlevel = 0;
  }
  Serial.print("Water level: ");
  Serial.println(Wlevel);
  sendWater();
  return;
  }


void sendWater()
{
  // Prepare the JSON payload
  DynamicJsonDocument doc(1024);
  doc["WaterLevel"] = Wlevel;

  String jsonPayload;
  serializeJson(doc, jsonPayload);  // Convert the JSON document to String

  // Send the PUT request to Firebase with the jsonPayload
  client.put("/WaterLevel.json", "application/json", jsonPayload);
  
  // Get the response to see if the data was sent successfully
  String response = client.responseBody();
  Serial.print("Response from server: ");
  Serial.println(response);  // Print the server's response
}

void connectWifi()
{
  WiFi.begin(ssid, password); // begin wifi connection
  
  while (WiFi.status() != WL_CONNECTED) // wifi connection check
  { 
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void checkFeedOn()
{
  client.get("/FeedOn.json");
  String feedStatus = client.responseBody().c_str();
  //Serial.println("Received FeedOn status: " + feedStatus);  // Print for debugging purposes

  // Parse the JSON output 
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, feedStatus); 
  String value = doc["value"].as<String>();
  value.trim();
  
  if (value.equalsIgnoreCase("true"))
  { // feeding on and turn off motor after 
    Serial.println("Feeding...");
    operateMotor(foodMotor, true);
    delay(2000);
    Serial.println("Feeding complete");
    operateMotor(foodMotor, false);
  } 
  else 
  {
    Serial.println("Food status is set to false");
  }
}

void checkWaterOn()
{
  // Check WaterOn status // same logic of the feeding loop
  client.get("/WaterOn.json");
  String waterStatus = client.responseBody().c_str();
  // Serial.println("Received WaterOn status: " + waterStatus);  // Debug output

  // Parse the JSON output
  DynamicJsonDocument doc2(1024);
  deserializeJson(doc2, waterStatus);
  String value2 = doc2["value"].as<String>();
  value2.trim();

  if (value2.equalsIgnoreCase("true"))
  { 
    Serial.println("Watering...");
    operateMotor(waterMotor, true);
    delay(2000);
    operateMotor(waterMotor, false);
    Serial.println("Watering complete");
  } 
  else 
  {
    Serial.println("Water status is set to false");
  }
}
