// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String outputStateLED1 = "off";
String outputStateLED2 = "off";
String DrejerA0Value = "";
String DrejerA1Value = "";
unsigned long startMillis;
unsigned long currentMillis;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // while (Serial2.available()) {
  WiFiClient client = server.available();  // Listen for incoming clients

  if (client) {  // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    String currentLine = "";                                                   // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {  // if there's bytes to read from the client,
        char c = client.read();  // read a byte, then

        header += c;
        if (c == '\n') {  // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
              
              currentMillis = millis();
              if(currentMillis - startMillis >= 500){  
              Serial2.print("B");
              delay(200);
              String valueDrejer2 = Serial2.readString();
              Serial.print(valueDrejer2);
              DrejerA1Value = valueDrejer2;
              startMillis = currentMillis;
}
            // turns the GPIOs on and off
            if (header.indexOf("GET /LED1/on") >= 0) {
              Serial2.print("LED1ON");
              outputStateLED1 = "on";

            } else if (header.indexOf("GET /LED1/off") >= 0) {
              Serial2.print("LED1OFF");
              outputStateLED1 = "off";

            } else if (header.indexOf("GET /LED2/on") >= 0) {
              Serial2.print("LED2ON");
              outputStateLED2 = "on";

            } else if (header.indexOf("GET /LED2/off") >= 0) {
              Serial2.print("LED2OFF");
              outputStateLED2 = "off";
            } else if (header.indexOf("GET /DREJER1VALUE") >= 0) {
              Serial2.print("A");
              delay(200);
              String valueDrejer1 = Serial2.readString();
              Serial.print(valueDrejer1);
              DrejerA0Value = valueDrejer1;

            // } else if (header.indexOf("GET /DREJER2VALUE") >= 0) {
            //   Serial2.print("B");
            //   delay(200);
            //   String valueDrejer2 = Serial2.readString();
            //   Serial.print(valueDrejer2);
            //   DrejerA1Value = valueDrejer2;
             }


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");

            // Display current state, and ON/OFF buttons for LED 1
            client.println("<p>LED 1 - State " + outputStateLED1 + "</p>");
            // If the outputStateLED1 is off, it displays the ON button
            if (outputStateLED1 == "off") {
              client.println("<p><a href=\"/LED1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for LED 2
            client.println("<p>LED 2 - State " + outputStateLED2 + "</p>");
            // If the outputStateLED2 is off, it displays the ON button
            if (outputStateLED2 == "off") {
              client.println("<p><a href=\"/LED2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //DREJER READ A0
            client.println("<h1>Read drejer A0</h1>");
            client.println("<p>valueDrejer1 = " + DrejerA0Value + "</p>");
            client.println("<p><a href=\"/DREJER1VALUE\"><button class=\"button button2\">Get new value from A0</button></a></p>");

            //DREJER READ A1
            client.println("<h1>Read drejer A1</h1>");
            client.println("<h2>Forsoeg paa automatisk opdatering</h2>");     
            client.println("<p>valueDrejer2 = " + DrejerA1Value + "</p>");



            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else {  // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        // }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
}