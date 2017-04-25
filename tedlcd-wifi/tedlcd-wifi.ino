// adapted from Serial Input Basics
// char data type replaced by byte
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
float Vt;
float Pw;
LiquidCrystal_I2C lcd(0x27, 16, 2);

char data[80];
StaticJsonBuffer<200> jsonBuffer;

//Basic Wifi and MQTT-Server Information
const char* ssid = "wii30";
const char* password = "1234567890a";
char* mqtt_server = "192.168.168.150";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

const byte numBytes = 32;
byte receivedBytes[numBytes];
byte numReceived = 0;
int cnt = 0;

boolean newData = false;

void setup() {
    Serial.begin(1200);
    Serial.println("<Arduino is ready>");
    Serial.begin(1200);
lcd.init();
lcd.clear();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,0);
  lcd.print("connected");
  lcd.setCursor(1,1);
  lcd.print(WiFi.localIP());
  delay(5000);
  lcd.clear();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}





void loop() {


  
    recvBytesWithStartEndMarkers();
    showNewData();
}

void recvBytesWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0xAA;
    byte endMarker = 0xAA;
    byte rb;


    while (Serial.available() > 0 && newData == false) {
        rb = Serial.read();

        if (recvInProgress == true) {
          
            if (rb != endMarker) {
                receivedBytes[ndx] = rb;
                ndx++;
                if (ndx >= numBytes) {
                    ndx = numBytes - 1;
                    cnt++;
              
                }
            }
            else {
                receivedBytes[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                numReceived = ndx;  // save the number for use when printing
                ndx = 0;
                newData = true;
            }
        }

        else if (rb == startMarker) {
            recvInProgress = true;
       
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        for (byte n = 0; n < numReceived; n++) {
            Serial.print(receivedBytes[n], HEX);
            Serial.print(' ');
        }
        Serial.println();
        showGroupsOfBytes();
        newData = false;

    }
}

void showGroupsOfBytes() {
    if ( receivedBytes[0]==0x4E )  {
    // if (numReceived < 9) {
    byte j = 0;
    byte k = 0;
    byte m = 0;
    byte r = 0;
    byte h = 0x4E;
      byte n = (0);
      //n < numReceived; ) {
      h = receivedBytes[0];
      h = ~h;
     // Serial.print("MTU ");
      //  Serial.println(h, HEX);
       // Serial.print(' ');
       
         //    Serial.println ("Voltage");
                   j = receivedBytes[6] ;
                   k = receivedBytes[7];
            // Serial.print("hex  pre invert ");      
           // Serial.print(j, HEX);
            // Serial.print(" ");
           // Serial.println(k, HEX);
            j= ~j;
            k= ~k;
           // Serial.print("hex inverted "); 
          //  Serial.print(j, HEX);
            // Serial.print(" ");
           // Serial.println(k, HEX);
//Serial.print("Dec ");
 //Serial.print(j);
  // Serial.print(" ");
 //Serial.println(k); 
            

double voltage = 0;
double power = 0;
voltage = ( k<< 8) | j;
//Serial.print("voltage prequ ");
     // Serial.println(voltage);  
//byte volt = voltage;
         //Serial.println(volt);         
  voltage= (123.6 + (voltage - 27620) / 85 * 0.4);
   Serial.print("voltage ");
       Serial.println(voltage);  
      // Serial.println (" ");
 
// Serial.println ("Power");
                  m = receivedBytes[3] ;
                  r = receivedBytes[4];
           /*  Serial.print("hex  pre invert ");      
            Serial.print(m, HEX);
             Serial.print(" ");
            Serial.println(r, HEX);
            */
            m= ~m;
            r= ~r;
           /* Serial.print("hex inverted "); 
            Serial.print(m, HEX);
             Serial.print(" ");
            Serial.println(r, HEX);
 Serial.print("Dec ");
 Serial.print(m); 
  Serial.print(" ");
 Serial.println(r); 
*/            
 
 power = (r<<8) + m;
 
      // Serial.print("Power prequ  "); 
          //  Serial.println(power);  
            //volt = voltage;
            
           power = 1.19 + 0.84 * ((power - 288) / 204);
        Vt= voltage;
        Pw =power;
       Serial.print("power out KW ");     
       Serial.println(power); 
      //  }
 //   }
    Serial.println();
    
    lcd.setCursor(0,0);
    lcd.print("volts ");
   lcd.print(voltage);
    lcd.setCursor(1,1);
    lcd.print("kw ");
    lcd.print(power);
}
 if (!client.connected()) {
    reconnect();
  }

  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {   // 5 second sample
    lastMsg = now;
   // ++value;
    
  
  String value = "\"Voltage\": " + String(Vt) ;
String value1 = ",\"Power\": " + String(Pw) ;

   value = value + value1;
  String payload = "{ \"devices\": \"A\",\"payload\": {" + value + "}}";
   payload.toCharArray(data, (payload.length() + 1));
      
client.publish("TED",data);
  Serial.print("Publish message: ");
    Serial.println(data);    
    //delay(50);

  }
     
  }

