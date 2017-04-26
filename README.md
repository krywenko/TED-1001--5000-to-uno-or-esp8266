# TED-1001--5000-to-uno-or-esp8266

here is a method to convert a a ted 5000 MTU into a network monitor via either esp8266 or Arduino or display directly on LCD.
it only requirs to wires  a ground wire and a  serial pin wire then connect it to the rX of your arduino/esp to be network connected.

there are other files related to TED 1001 and the ted display module  to capture  and store info directly to computer  it also contains various Pl and sh to data  store to RRD and graphing to webpages .   also included is one of my original GTI limiter based on  data grabed from  TED MTU module

Basically how it works is that we capture the TX data being sent  from the  5000 mtu CPU to its powerline encoder and pass it onto the to the arduino or esp8266  to decode  and transmit or display the info on LCD

http://www.theenergydetective.com/1001mtu.html

to get this sketch to work with your TED MTU  you need to edit this line and add in your MTU - it is inverted binary of your MTU to invert your MTU just  follow this example:


example : MTU 229 = hex E5  //decimal to hex converter
hex to binary  
e5 = 1110 0101
invert the value for E5 
1110 0101 = 00011010
binary to hex
00011010 = 1A //insert this into the sketch

 edit this section of the sketch with your MTU
 
void showGroupsOfBytes() { 

   if ( receivedBytes[0]==0x1A )  { //inverted binary of your MTU ie for 0x1A   hex = 0xE5 mTU decimal = 22
   
   // if (numReceived < 9) {
    
    byte j = 0;
    
    byte k = 0;
    
    byte m = 0;
    
    byte r = 0;
    
    byte h = 0x1A;  //edit here too
    
    byte n = (0);
