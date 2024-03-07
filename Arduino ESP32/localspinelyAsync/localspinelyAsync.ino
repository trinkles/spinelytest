// Please update SSID, Password, and XAMPP Server
// Refer to wifi_connection.h shared locally, contact any member

#include <WiFi.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Redmi";
const char* password = "37dbc92gem8rxz7";
AsyncWebServer server(80); 
const char* xamppServer = "192.168.63.213";
const int xamppServerPort = 8000; // for xampp

struct Calibration {
  float value;
};
Calibration calibrationData[] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };

struct Monitoring {
  float value;
};
Monitoring monitoringData[] = { {0}, {0}, {0}, {0}, {0} };

String name;
String username;
int userId;
int sessionId;
bool loggedIn = false;
bool sessionStarted = false;

String postureStatus = "";
const int sensorPin[] = {34, 39, 35, 36, 32};
float sensVal[5];
float forMin[5];
float forMax[5];

float mapValue(float value, float inMin, float inMax, float outMin, float outMax) {
  return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

/* Calibration and mapping */
const int SENSOR_MIN_VALUE = 0;
const int SENSOR_MAX_VALUE = 4095;
const float ANGLE_MIN = 0;
const float ANGLE_MAX = 90;

void calibration() {
  for (int sensor = 0; sensor < 5; sensor++) {
    sensVal[sensor] = analogRead(sensorPin[sensor]);
    forMin[sensor] = sensVal[sensor];
    forMax[sensor] = sensVal[sensor];
  }

  delay(100); // Adjust the delay time as needed

  for (int sensor = 0; sensor < 5; sensor++) {
    sensVal[sensor] = analogRead(sensorPin[sensor]);
    if (sensVal[sensor] < forMin[sensor]) {
      forMin[sensor] = sensVal[sensor];
    }
    if (sensVal[sensor] > forMax[sensor]) {
      forMax[sensor] = sensVal[sensor];
    }
  }

  for (int sensor = 0; sensor < 5; sensor++) {
    forMin[sensor] = (mapValue(forMin[sensor], SENSOR_MIN_VALUE, SENSOR_MAX_VALUE, ANGLE_MIN, ANGLE_MAX))-53;
    forMax[sensor] = (mapValue(forMax[sensor], SENSOR_MIN_VALUE, SENSOR_MAX_VALUE, ANGLE_MIN, ANGLE_MAX))-47;
  }

  calibrationData[0].value = forMin[0];
  calibrationData[1].value = forMax[0];
  calibrationData[3].value = forMin[1];
  calibrationData[4].value = forMax[1];
  calibrationData[6].value = forMin[2];
  calibrationData[7].value = forMax[2];
  calibrationData[9].value = forMin[3];
  calibrationData[10].value = forMax[3];
  calibrationData[12].value = forMin[4];
  calibrationData[13].value = forMax[4];

  WiFiClient client;
  
  if (client.connect(xamppServer, xamppServerPort)) {
    String postData = "userId=" + String(userId); // Include userId in postData

    for (int i = 0; i < 15; i++) {
      postData += "&sensor" + String(i + 1) + "=" + String(calibrationData[i].value);
    }

    client.print("POST ");
    client.print("/save_calibration.php");
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(xamppServer);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
    client.stop();
  }
}


// Monitoring and mapping function
void monitoring() {
  unsigned long startTime = millis();
  while (millis() - startTime < 3000) { // 3 seconds in total
    for (int sensor = 0; sensor < 5; sensor++) {
      sensVal[sensor] = analogRead(sensorPin[sensor]);
      delay(600); // Delay between sensor readings, adjusted for 5 sensors
    }
  }
  for (int sensor = 0; sensor < 5; sensor++) {
    // float sum = 0;
    // for (int value = 0; value < 10; value++) {
    //   sum += sensVal[sensor];
    // }
    // monitoringData[sensor].value = sum / 10;
    // monitoringData[sensor].value = sensVal[sensor]; 
    monitoringData[sensor].value = (mapValue(sensVal[sensor], 0, 4095, 0, 90))-50;
  }
  int sensorProper = 0; // Counter for properly positioned sensors
  for (int sensor = 0; sensor < 5; sensor++) {
    if (monitoringData[sensor].value >= forMin[sensor] && monitoringData[sensor].value <= forMax[sensor]) {
      sensorProper++; // Sensor angle within the acceptable range or matches the average value
    }
  }
  if (sensorProper == 5) {
    postureStatus = "proper";
  } else {
    postureStatus = "improper";
  }

  WiFiClient client;
  if (client.connect(xamppServer, xamppServerPort)) {
    String postData = "userId=" + String(userId) + "&sessionId=" + String(sessionId) + "&status=" + postureStatus;
    for (int sensor = 0; sensor < 5; sensor++) {
      postData += "&sensor" + String(sensor + 1) + "=" + String(monitoringData[sensor].value);
    }

    client.print("POST ");
    client.print("/save_monitoring.php");
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(xamppServer);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
    client.stop();
  }
}

bool monitoringRunning = false;

// Function to generate HTML page content
String getPage() {
    String page = "<html lang=en-EN><head>";
    page += "<title>Spinely test page</title>";
    page += "<style> body { background-color: #ffffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>";

    page += "</head><body><h1>Spinely</h1>";

    // Display user information if logged in
    if (loggedIn) {
        page += "<p>Hello, " + username + "</p>";
        page += "<p>UID: " + String(userId) + "</p>";
        page += "<p>Session Number: " + String(sessionId) + "</p>";
        // Logout button
        page += "<form action='/logout' method='post'><button type='submit'>Logout</button></form>";

        // New Session button
        if (!sessionStarted) {
            page += "<form action='/newSession' method='post'><button type='submit'>New Session</button></form>";
        }

        // End Session button
        if (sessionStarted) {
            page += "<form action='/endSession' method='post'><button type='submit'>End Session</button></form>";
            page += "<meta http-equiv='refresh' content='3'/>"; // Auto-refresh meta tag

            if (!monitoringRunning) {
                page += "<form action='/start' method='post'><button type='submit' ";
                if (!sessionStarted) page += "disabled";
                page += ">Start Monitoring</button></form>";
            } else {
                page += "<form action='/stop' method='post'><button type='submit' ";
                if (!sessionStarted) page += "disabled";
                page += ">Stop Monitoring</button></form>";

            }
        }
        
        // Calibrate button
        page += "<form action='/calibrate'><button type='submit'>Calibrate</button></form>";
    } else {
        // Signup form
        page += "<h3>Signup</h3>";
        page += "<form action='/signup' method='post'>";
        page += "Name: <input type='text' name='name'><br>";
        page += "Username: <input type='text' name='username'><br>";
        page += "Email: <input type='email' name='email'><br>";
        page += "<input type='submit' value='Signup'>";
        page += "</form>";

        // Login form
        page += "<h3>Login</h3>";
        page += "<form action='/login' method='post'>";
        page += "Username: <input type='text' name='username'><br>";
        page += "<input type='submit' value='Login'>";
        page += "</form>";
    }

    // Calibration data display
    page += "<h3>Calibrated posture</h3>";
    page += "<ul><li>Cervical (Upper back): ";
    page += "min: " + String(calibrationData[0].value) + " max: " + String(calibrationData[1].value);
    page += "</li><li>Thoracic (Middle back): ";
    page += "min: " + String(calibrationData[3].value) + " max: " + String(calibrationData[4].value);
    page += "</li><li>Lumbar (Lower back): ";
    page += "min: " + String(calibrationData[6].value) + " max: " + String(calibrationData[7].value);
    // page += "</li><li>Lumbar Lateral (Left): ";
    // page += "min: " + String(calibrationData[9].value) + " max: " + String(calibrationData[10].value);
    page += "</li><li>Lumbar Lateral (Left/Right): ";
    page += "min: " + String(calibrationData[12].value) + " max: " + String(calibrationData[13].value);
    page += "</ul>";

    // Monitored posture data display
    page += "<h3>Monitored posture</h3>";
    page += "<ul><li>Posture Status: " + postureStatus + "</li>";
    page += "<li>Cervical: " + String(monitoringData[0].value) + "</li>";
    page += "<li>Thoracic: " + String(monitoringData[1].value) + "</li>";
    page += "<li>Lumbar: " + String(monitoringData[2].value) + "</li>";
    // page += "<li>Lumbar (left): " + String(monitoringData[3].value) + "</li>";
    page += "<li>Lumbar (left/right): " + String(monitoringData[4].value) + "</li>";
    page += "</ul>";


    return page;
}

// Function to handle root endpoint requests
void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html", getPage());
}

void handleSignup(AsyncWebServerRequest *request) {
  // Retrieve user information from the signup form
  name = request->arg("name");
  String username = request->arg("username");
  String email = request->arg("email");

  WiFiClient client;
  
  if (client.connect(xamppServer, xamppServerPort)) {
    // Check if the username already exists
    String postData = "username=" + username;
    client.print("POST ");
    client.print("/check_username.php"); // Change this to the appropriate PHP file to check username
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(xamppServer);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
    delay(100);
    String response = client.readStringUntil('\r');
    client.stop();
    
    if (response.indexOf("Username already exists") != -1) {
      request->send(200, "text/plain", "Username already exists");
      return;
    }

    // Check if the email already exists
    if (client.connect(xamppServer, xamppServerPort)) {
      postData = "email=" + email;
      client.print("POST ");
      client.print("/check_email.php"); // Change this to the appropriate PHP file to check email
      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.println(xamppServer);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println();
      client.print(postData);
      delay(100);
      response = client.readStringUntil('\r');
      client.stop();
      
      if (response.indexOf("Email already exists") != -1) {
        request->send(200, "text/plain", "Email already exists");
        return;
      }
    } else {
      Serial.println("Failed to connect to server");
      request->send(500, "text/plain", "Failed to connect to server");
      return;
    }

    // If username and email are unique, proceed with signup
    if (client.connect(xamppServer, xamppServerPort)) {
      String postData = "name=" + name + "&username=" + username + "&email=" + email;
      client.print("POST ");
      client.print("/signup.php"); // Change this to the appropriate PHP file handling signup
      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.println(xamppServer);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postData.length());
      client.println();
      client.print(postData);
      client.stop();
      
      request->send(200, "text/plain", "Signup successful");
    } else {
      Serial.println("Failed to connect to server");
      request->send(500, "text/plain", "Failed to connect to server");
    }
  } else {
    Serial.println("Failed to connect to server");
    request->send(500, "text/plain", "Failed to connect to server");
  }
}

void handleLogin(AsyncWebServerRequest *request) {
  String loginUsername = request->arg("username");

  WiFiClient client;
  
  if (client.connect(xamppServer, xamppServerPort)) {
    String postData = "loginUsername=" + loginUsername;

    client.print("POST ");
    client.print("/login.php"); // Change this to the appropriate PHP file handling login
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(xamppServer);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);

    // Wait for response from the server
    delay(100); // Adjust delay as needed
    
    // Read response headers
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }
    
    // Read the actual response content
    String response = client.readStringUntil('\r');
    client.stop();
    
    // Trim leading/trailing whitespace from response
    response.trim();

    if (response.length() > 0) {
      // Update username
      username = response;

      // Now, retrieve userId from a different PHP file
      if (client.connect(xamppServer, xamppServerPort)) {
        String bpostData = "loginUsername=" + loginUsername;

        // Make a request to the "get_userId.php" file
        client.print("POST ");
        client.print("/get_userId.php"); // Change this to the appropriate PHP file
        client.println(" HTTP/1.1");
        client.print("Host: ");
        client.println(xamppServer);
        client.println("Connection: close");
        client.println("Content-Type: application/x-www-form-urlencoded");
        client.print("Content-Length: ");
        client.println(postData.length());
        client.println();
        client.print(bpostData);

        // Wait for response from the server
        delay(100); // Adjust delay as needed
        
        // Read and print response headers
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            Serial.println(line);
            if (line == "\r") {
                break;
            }
        }

        // Read the actual response content (userId)
        String userIdResponse;
        boolean headerEnd = false;
        String line;
        while (client.available()) {
            char c = client.read();
            if (headerEnd) {
                userIdResponse += c;
            } else if (c == '\n') {
                // Check if the next line is empty, indicating the end of headers
                if (line == "\r") {
                    headerEnd = true;
                }
                line = "";
            } else {
                line += c;
            }
        }
        client.stop();
        
        // Trim leading/trailing whitespace from response
        userIdResponse.trim();

        // Update userId if response is not empty
        if (userIdResponse.length() > 0) {
            userId = userIdResponse.toInt();
        }

        // Set loggedIn flag to true upon successful login
        loggedIn = true;

        // Print logged-in message
        Serial.println("User logged in: " + username + ", userId: " + String(userId));
    } else {
        // Handle case when user is not found
        Serial.println("User not found");
    }
  } else {
    Serial.println("Failed to connect to server");
  }

  request->redirect("/");
}
}

void handleLogout(AsyncWebServerRequest *request) {
  // Perform logout actions here
  // For example, reset user variables or set loggedIn to false
  loggedIn = false;
  userId = 0;
  name = "";
  username = "";
  sessionId = 0;
  
  // Redirect to the homepage or any other desired page
  request->redirect("/");
}

void handleNewSession(AsyncWebServerRequest *request) {
  sessionStarted=true;
  WiFiClient client;
  
  if (client.connect(xamppServer, xamppServerPort)) {
    String postData = "userId=" + String(userId);

    client.print("POST ");
    client.print("/create_session.php");
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(xamppServer);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);

    // Wait for response headers from the server
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        break;
      }
    }

    // Read the response content from the server
    String responseContent = "";
    while (client.available()) {
      char c = client.read();
      responseContent += c;
    }

    // Parse the response content to extract the session ID
    sessionId = responseContent.toInt(); // Assuming the response is the session ID

    // Redirect or handle as needed
    request->redirect("/");
  } else {
    Serial.println("Failed to connect to server");
  }
}

void handleEndSession(AsyncWebServerRequest *request) {
  sessionStarted = false;
  WiFiClient client;
  
  // Assuming you have access to the userId and sessionId variables
  if (client.connect(xamppServer, xamppServerPort)) {
    String postData = "userId=" + String(userId) + "&sessionId=" + String(sessionId);

    client.print("POST ");
    client.print("/end_session.php"); // Change this to the appropriate PHP file for ending a session
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(xamppServer);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
    client.stop();
  } else {
    Serial.println("Failed to connect to server");
  }

  // Clear session-related variables or perform any other necessary cleanup
  // sessionId = 0;

  request->redirect("/");
}

// Function to handle calibration request
void handleCalibrate(AsyncWebServerRequest *request) {
  // Perform calibration
  calibration();
  // Redirect to the root page after calibration
  request->redirect("/");
}

// Function to handle start monitoring request
void handleStartMonitoring(AsyncWebServerRequest *request) {
  monitoringRunning = true;
  // Redirect to the root page after starting monitoring
  request->redirect("/");
}

// Continuous monitoring loop
void continuousMonitoring(void* parameter) {
  (void)parameter; // Unused parameter
  while (true) {
    if (monitoringRunning) {
      monitoring();
    }
    delay(100); // Delay to avoid busy waiting
  }
}

// Function to handle stop monitoring request
void handleStopMonitoring(AsyncWebServerRequest *request) {
  monitoringRunning = false;
  // Redirect to the root page after stopping monitoring
  request->redirect("/");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/calibrate", HTTP_GET, handleCalibrate);
  server.on("/start", HTTP_POST, handleStartMonitoring);
  server.on("/stop", HTTP_POST, handleStopMonitoring);
  server.on("/signup", HTTP_POST, handleSignup); // Handle signup POST requests
  server.on("/login", HTTP_POST, handleLogin);   // Handle login POST requests
  server.on("/newSession", HTTP_POST, handleNewSession);
  server.on("/endSession", HTTP_POST, handleEndSession);
  server.on("/logout", HTTP_POST, handleLogout);

  server.begin();
  Serial.println("HTTP server started");

  // Start continuous monitoring loop in a separate thread
  xTaskCreatePinnedToCore(continuousMonitoring, "Continuous Monitoring", 4096, NULL, 1, NULL, 0);
}

void loop() {
}
