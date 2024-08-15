// this is code that is run on the arduino to control the motors
// its not run on the web app at all, but im writing it here for reference
// we need to write endpoints so that the arduino can send get requests to the web app
// to get the current command and then execute it
// im thinking arduino loop will be something like
// check if the command has changed
// if it has, execute the command

/*
the r4 has a wifi module called esp32 and ithink we can write a server on it kinda like this

endpoints:
To open the water motor: http://<ESP32_IP>/water/open
To close the water motor: http://<ESP32_IP>/water/close
To open the food motor: http://<ESP32_IP>/food/open
To close the food motor: http://<ESP32_IP>/food/close
(the webapp and arduino must be on the same network, thats important)


*/

#include <WiFi.h>
#include <ArduinoHttpClient.h>

// Replace with your network credentials
const char* ssid = "irvin";
const char* password = "ilovedimsum";

// Express server URL
const char* serverHost = "172.20.10.6"; // Your local IP address
const int serverPort = 3000;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverHost, serverPort);

void checkConnectivity() {
  WiFiClient wifi;
  HttpClient client = HttpClient(wifi, "httpbin.org", 80);

  Serial.println("Making test GET request to httpbin.org...");
  client.get("/get");

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Test Status code: ");
  Serial.println(statusCode);
  Serial.println("Test Response:");
  Serial.println(response);

  client.stop();
}

void setup() {
restart:
  Serial.begin(19200); // Ensure this baud rate matches the Serial Monitor's baud rate

  WiFi.begin(ssid, password); // Start connecting to Wi-Fi
  Serial.print("Connecting to Wi-Fi");

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    attempts++;

    // If it takes too long to connect, reset the Wi-Fi module
    if (attempts > 20) {
      Serial.println("Failed to connect to Wi-Fi. Restarting...");
      goto restart;
    }
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Check connectivity
  checkConnectivity();

  // Set the timeout period for the HTTP client (e.g., 15 seconds)
  client.connectionKeepAlive();
  client.setTimeout(15000);

  // Construct the URL with query parameter
  String fullUrl = String("http://") + serverHost + ":" + serverPort + "/api/arduino?data=Hello%20from%20Arduino";
  Serial.print("Constructed URL: ");
  Serial.println(fullUrl);

  // Make the HTTP GET request to Express server
  Serial.println("Making GET request to Express server...");
  client.get(fullUrl);

  // Read the response
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.println("Response:");
  Serial.println(response);

  if (statusCode == -3) {
    Serial.println("Error: Connection failed. The server is unreachable.");
  } else if (statusCode == 408) {
    Serial.println("Error: Request Timeout. The server didn't respond in time. Restarting.");
    goto restart;
  }
  client.stop();
}

void loop() {
  // Put your main code here, to run repeatedly:
}