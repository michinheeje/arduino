#include <SPI.h>
#include <WiFi.h>
#include <SD.h>

char ssid[] = "iptime";
char pass[] = "12345678";
int keyIndex = 0;

int redPin = 5;
int yellowPin = 6;
int greenPin = 7;
int doorlockPin = 3;

File myFile;
String password;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(doorlockPin, OUTPUT);

  Serial.print(F("SD card check..."));

  if (!SD.begin(4)) {
    Serial.println(F("failed!"));
    return;
  }
  Serial.println(F("done."));

  // 비밀번호 초기화
  myFile = SD.open("pw.txt", O_WRITE | O_CREAT | O_TRUNC);
  
  if(myFile) {
    Serial.print(F("Default password setting... "));
    myFile.println("0000");
    myFile.close();
    Serial.println(F("0000"));
  } else {
    Serial.println(F("Error"));
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
  Serial.println(F("WiFi shield not present")); 
   while(true);        // don't continue
   } 

   // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print(F("Attempting to connect to Network named: "));
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
     status = WiFi.begin(ssid, pass);
     // wait 10 seconds for connection:
     delay(10000);
   } 
  server.begin();
  printWifiStatus();
}



void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println(F("new client"));
    String currentLine = "";
    while (client.connected()) {
     if (client.available()) {
     char c = client.read();
     Serial.write(c);
     if (c == '\n') {

      if (currentLine.length() == 0) {  
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();

        // the content of the HTTP response follows the header:
        client.print("<meta charset='utf-8'>");
        client.print("<form id='door_pw' method='post'>");
        client.print("<input type='hidden' name='m' value='1'>");
        client.print("암호 : <input type='text' name='pw'> ");
        client.print("<input type='hidden' name='f' value='1'>");
        client.print("<input type='submit' value='전송'>");
        client.print("</form>");

        client.print("<form id='door_chpw' method='post'>");
        client.print("<input type='hidden' name='m' value='2'>");
        client.print("기존 암호 : <input type='text' name='pw'><br>");
        client.print("변경 암호 : <input type='text' name='pw2'><br>");
        client.print("<input type='hidden' name='f' value='1'>");
        client.print("<input type='submit' value='전송'>");
        client.print("</form>");
        client.println();
        
        break;         
       } 
       else {
         currentLine = "";
       }
     }     
     else if (c != '\r') {
      currentLine += c;
    }
    
    if (currentLine.endsWith("f=1")) {
        myFile = SD.open("pw.txt");
        
        if(myFile) {
          password = "";
          while (myFile.available()) {
            char txt = myFile.read();
            password += txt;
          }
          myFile.close();
        } else {
          Serial.println(F("\n<-- error opening password setting! -->\n"));
        }
        
        password.trim();
        String mode = currentLine.substring(2, 3);
        
        if(mode == "1") {
            String check = currentLine.substring(7, (currentLine.length() - 4));
            
            if(password == check) {
              digitalWrite(greenPin, HIGH);
              digitalWrite(doorlockPin, HIGH);
              delay(3000);
              digitalWrite(doorlockPin, LOW);
              digitalWrite(greenPin, LOW);
            } else {
              digitalWrite(redPin, HIGH);
              delay(1000);
              digitalWrite(redPin, LOW);
              delay(1000);
              digitalWrite(redPin, HIGH);
              delay(1000);
              digitalWrite(redPin, LOW);
            }
        }

        if(mode == "2") {
            String check = currentLine.substring(7, (7 + password.length()));
            String chpw = currentLine.substring((12 + password.length()), (currentLine.length() - 4));
            
            if(password == check) {
              if(chpw.length() > 0) {
                myFile = SD.open("pw.txt", O_WRITE | O_CREAT | O_TRUNC);
                
                if(myFile) {
                  Serial.print(F("Change password setting..."));
                  myFile.println(chpw);
                  myFile.close();
                  Serial.println(chpw);
                } else {
                  Serial.println(F("\n<-- error opening password setting! -->\n"));
                }
                digitalWrite(greenPin, HIGH);
                delay(3000);
                digitalWrite(greenPin, LOW);
              } else {
                digitalWrite(yellowPin, HIGH);
                delay(1000);
                digitalWrite(yellowPin, LOW);
                delay(1000);
                digitalWrite(yellowPin, HIGH);
                delay(1000);
                digitalWrite(yellowPin, LOW);
              }
            } else {
              digitalWrite(redPin, HIGH);
              delay(1000);
              digitalWrite(redPin, LOW);
              delay(1000);
              digitalWrite(redPin, HIGH);
              delay(1000);
              digitalWrite(redPin, LOW);
            }
        }
    }
   }
 }
// close the connection:
client.stop();
Serial.println(F("client disonnected"));
  }
}

void printWifiStatus() {  
  Serial.print(F("SSID: "));
  Serial.println(F(WiFi.SSID()));

 // print your WiFi shield's IP address:
 IPAddress ip = WiFi.localIP();
 Serial.print(F("IP Address: "));
 Serial.println(F(ip));

 // print the received signal strength:
 long rssi = WiFi.RSSI();
 Serial.print(F("signal strength (RSSI):"));
 Serial.print(F(rssi));
 Serial.println(F(" dBm"));
// print where to go in a browser:
Serial.print(F("To see this page in action, open a browser to http://"));
Serial.println(F(ip));
 }
