#include <stdio.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <EmonLib.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte numBytes = 32;
const int slaveSelectPin = 10;
EnergyMonitor emon1;

byte receivedBytes[numBytes];
byte numReceived = 0;
int cnt = 0;
float kw = 0; 
float volts = 124; 
float stp = 255;

float inverter = 1;
float kw1 =0;
float le=127;         // level saved condition
int rl1=127; // rlevel saved condtion

int level=127;    // main pot  control for PWM
int rlevel=127;    // if you need reverse pot  direction of PWM

boolean newData = false;

void setup() {
    Serial.begin(1200);
    Serial.println("<Arduino is ready>");
   // Serial.begin(1200);
    lcd.begin();
     pinMode(slaveSelectPin, OUTPUT);
      SPI.begin();
      
  //emon1.voltage(2, 234.26, 1.7);  
      // Voltage:input pin, calibration,phase_shift
  emon1.current(1, 111.1);       
       // Current: input pin, calibration.

      
}

void loop() {
    recvBytesWithStartEndMarkers();
    showNewData();
 //   output();
  //  potset();
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

      volts=voltage;

 power = (r<<8) + m;
 power = 1.19 + 0.84 * ((power - 288) / 204);
     kw = power;   

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
 double Irms = emon1.calcIrms(1480);  // Calculate Irms only
 /* emon1.calcVI(20,2000);         
       // Calculate all. No.of half wavelengths (crossings), time-out
  emon1.serialprint();          
      // Print out all variables (realpower, apparent power,      
      // Vrms,Irms, power factor)
  
  //float realPower       = emon1.realPower;      
         //extract Real Power into variable
 // float apparentPower   = emon1.apparentPower;    
         //extract Apparent Power into variable
  //float powerFActor     = emon1.powerFactor;     
         //extract Power Factor into Variable
  float supplyVoltage   = emon1.Vrms;             
         //extract Vrms into Variable
  float Irms            = emon1.Irms;            
   */      //extract Irms into Variable

  inverter = ((Irms*volts)/1000);  // calculates GTI output 
  
Serial.print(" level value  ");
Serial.println(level);
Serial.print("kw value  ");
Serial.println(kw);

stp = (inverter / kw);
stp = abs(stp);
Serial.print(" stp value  ");
Serial.println(stp);

le=((255-level)/255)*stp;
//l1=abs(l1);
Serial.print(" l1 value  ");
Serial.println(le);

if( kw < 0){
  
  Serial.println(" kw  less than zero ");

     if (le > 1){
       level=255;  
       rlevel=0;
        Serial.println("  <   level low condition 1  ");
        } //else{
      if (le < 1){
        Serial.println("  < level low  condition 2 ");
        level= (255-level)*stp;
        rlevel=(255-level);
     }


}else{
  
  Serial.println("kw  greater than zero ");
       if(le < 1){
        rlevel= (255-level)*stp;
         level=(255-rlevel);
         
         
          Serial.println("   > level high condition 1 ");
     }
     if(le > 1){
         rlevel = 255;  
         level = 0;
         Serial.println("   > level high condition 2 ");
         }//else{
     

}

if(kw < 0){
 
    for (int scan = 0; scan < 1 ; scan++) {
    digitalPotWrite(1, level);
        Serial.print("Hchannel ");
   Serial.println(level);
 
     // delay(10);
    digitalPotWrite(2, level);
  //      Serial.println("Hchanne2");
   // Serial.println(level);
  //  Serial.println(2);
      //delay(10);
    digitalPotWrite(3, level);
   //     Serial.println("Hchanne3");
   // Serial.println(level);
   // Serial.println(3);
      //delay(10);
    digitalPotWrite(4, rlevel);
    //    Serial.println("Hchanne4");
   // Serial.println(rlevel);
    //Serial.println(4);
      //delay(10);
    digitalPotWrite(5, rlevel);
  //      Serial.println("Hchanne5");
  //  Serial.println(rlevel);
  //  Serial.println(5);
      //delay(10);
    digitalPotWrite(6, rlevel);
    Serial.print("Hchannel6 ");
    Serial.println(rlevel);
 
   // Serial.println(Irms);
    //level=level+1;
   
   
      //delay(10);
   
  }
 }
 if(kw>0)
{
  //if(level>=0)
   for (int scan = 0; scan < 1; scan++) {
  {
    digitalPotWrite(1, level);
        Serial.print("lchannel ");
    Serial.println(level);
 
     // delay(10);
    digitalPotWrite(2, level);
    //    Serial.println("lchanne2");
   // Serial.println(level);
   // Serial.println(2);
      //delay(10);
    digitalPotWrite(3, level);
  //      Serial.println("lchanne3");
   // Serial.println(level);
   // Serial.println(3);
      //delay(10);
    digitalPotWrite(4, rlevel);
   //     Serial.println("lchanne4");
 //   Serial.println(rlevel);
  //  Serial.println(4);
     // delay(10);
    digitalPotWrite(5, rlevel);
   //     Serial.println("lchanne5");
   // Serial.println(rlevel);
  //  Serial.println(5);
     // delay(10);
    digitalPotWrite(6, rlevel);
    Serial.print("lchannel6  ");
    Serial.println(rlevel);
     
    //Serial.println(Irms);
   // level=level-1;
    
  }

  }
 

 }

     Serial.print("level ");
    Serial.println(level);
     Serial.print("rlevel ");
     Serial.println(rlevel);  
      Serial.println(" "); 
}


void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
 digitalWrite(slaveSelectPin, HIGH);
          


}
