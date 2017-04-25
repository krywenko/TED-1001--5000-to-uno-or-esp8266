
int INFILE ();
int processPacket;
int buf =0;
void setup() {
  Serial.begin(1200);
        //HWSERIAL.begin(2400);
}

void loop() {
        
  {      
  if (Serial.available() > 0) {
 int INFILE = Serial.read();
    Serial.print("received: ");
    Serial.println(INFILE, HEX);
 
  }
 // }
/*int buf 0;

int started = 0;
//int sub processPacket();
int INFILE ();
while (read(INFILE, buf, 1)) {
   int d = ord(buf) ^ 0xff;
  if (d == 0x55 && started == 0) {
    started = 1;
    int a = 0;
  }
  if (started == 1) {
    int data[a] = d;
     int sum += data[a];
   // # printf("0x%02X ", $data[$a]);
    a++;
    if (a >= 11) {
      sum -= data[9];
      sum &= 0xff;
      if (sum == 0) { processPacket(data); }
      started=1;
      a = 0;
    */}
 //}
//}
  }


