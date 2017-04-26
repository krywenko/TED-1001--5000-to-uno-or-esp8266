// adapted from Serial Input Basics
// char data type replaced by byte
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

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
    if ( receivedBytes[0]==0x1A )  { //4e
    // if (numReceived < 9) {
    byte j = 0;
    byte k = 0;
    byte m = 0;
    byte r = 0;
    byte h = 0x1A;
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

     
  }

