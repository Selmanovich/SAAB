// demo: Wheel Adapter
#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];



MCP_CAN CAN0(9);                               // Set CS CAN ON PIN=9

//variables
long NXTbut = 0;
long PREVbut = 0;
long Depress = 0;
boolean forcebutton = 0; //Should we force button change
byte lastpressed = 0; // The last button pressed


//VAR PARAMS
long interv = 1000; 
long presstime = 1000;
short int DEBUGMODE = 1; //Debug mode level 0-off 1-basic debug 2-extended debug messages




void setup()
{
    Serial.begin(115200);
    delay(100);
    CAN0.begin(CAN_47K619BPS,2);                       // init can bus : baudrate = 33.3k 
    pinMode(2, INPUT);  // Setting pin 2 for /INT input
    Serial.println("SAAB 9-5 MY01 WHEEL ADAPTER\nLet's go!");
}

void loop()
{
    if(!digitalRead(2))                         // If pin 2 is low, read receive buffer
    {
        CAN0.readMsgBuf(&len, rxBuf);              // Read data: len = data length, buf = data byte(s)
        rxId = CAN0.getCanId();                    // Get message ID
        if (DEBUGMODE==2)                          // Check if extended Debug mode is on
        {
            Serial.print("ID: ");
            Serial.print(rxId, HEX);
            Serial.print("  Data: ");
            for(int i = 0; i<len; i++)                // Print each byte of the data
            {
                if(rxBuf[i] < 0x10)                     // If data byte is less than 0x10, add a leading zero
                {
                    Serial.print("0");
                }
            Serial.print(rxBuf[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
        
        if (rxId == 0x290) //Keys on wheel
        {
            switch (rxBuf[2]) 
            {
                case 0x04: //NXT
                    if (DEBUGMODE) {Serial.println("NXT Button pressed");}
                    break;
            
                case 0x10: //Next track
                    if (DEBUGMODE) {Serial.println("Next Track Button pressed");}
                    break;
                
                case 0x08: //Previous track
                    if (DEBUGMODE) {Serial.println("Previous Track Button pressed");}
                    break;

                case 0x20: //SRC
                    if (DEBUGMODE) {Serial.println("SRC Button pressed");}
                    break;
            
                case 0x40: //Volume up
                    if (DEBUGMODE) {Serial.println("Volume Up Button pressed");}
                    break;
                
                case 0x80: //Volume down
                    if (DEBUGMODE) {Serial.println("Volume Down Button pressed");}
                    break;
            }
        }
        
        if (rxId == 0x290) //SID keys
        {
            switch (rxBuf[3]) {
                case 0x08:
                    if (DEBUGMODE) {Serial.println("Night Panel Button pressed");}
                    break;
            }
        }
        
        if (rxId == 0x4A0) //Blinkers
        {
            switch (rxBuf[2])
            {
                case 0x40:
                    if (DEBUGMODE) {Serial.println("Left Blinker");}
                    break;
                
                case 0x20:
                    if (DEBUGMODE) {Serial.println("Right Blinker");}
                    break;
            }
        }
    }// End of If recieved new can message
}



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
