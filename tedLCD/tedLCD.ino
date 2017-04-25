#include <stdio.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte numBytes = 32;
byte receivedBytes[numBytes];
byte numReceived = 0;
int cnt = 0;

boolean newData = false;

void setup() {
    Serial.begin(1200);
    Serial.println("<Arduino is ready>");
   // Serial.begin(1200);
    lcd.begin();
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
                cnt = ndx;
                if (ndx >= numBytes) {
                    ndx = numBytes - 1;
                    
              
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
    if ( receivedBytes[0]==0x4E )  {  // change this to match your MTU it - it will display in serial print
     if (cnt == 10) {
    byte j = ~receivedBytes[6];
    byte k = ~receivedBytes[7];
    byte m = ~receivedBytes[3];
    byte r = ~receivedBytes[4];


double voltage = 0;
double power = 0;

voltage = ( k<< 8) | j;
voltage= (123.6 + (voltage - 27620) / 85 * 0.4);
  // Serial.print("voltage ");
   //    Serial.println(voltage);  

 
 power = (r<<8) + m;
 power = 1.19 + 0.84 * ((power - 288) / 204);
     float pow = power;   
      // Serial.print("power out KW ");
     //  printf("power out %.4f",power);     
      Serial.println(pow); 
    lcd.clear();      
    lcd.setCursor(0,0);
    lcd.print("volts ");
    lcd.print(voltage);
    lcd.setCursor(1,1);
    lcd.print("kw ");
    lcd.print(power);
       
       }
    }
    cnt = 0;   
 
}
