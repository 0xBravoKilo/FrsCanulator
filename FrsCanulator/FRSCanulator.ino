// FRSCanulator by Bryan K.

#include "mcp2515.h"
#include <SPI.h>

//Rx Vars
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
unsigned long rpm = 0;
unsigned long cts = 0;
unsigned long oilPr = 0;
unsigned long iat = 0;
unsigned long tps = 0;

//Tx Vars
byte sendSts = 0;
float ctsHigh = 190;				//flash check engine light if high
float oilPrLow = 20;				//flash check engine light if low
char val_c = 0;
int val_i = 0;
unsigned long elapsedA = 0;
unsigned long elapsedD = 0;
unsigned long elapsedE = 0;
unsigned long elapsedG = 0;

//ECM Data
byte dataA[8] = {0x00, 0x00, 0x68, 0x46, 0x00, 0x00, 0x09, 0x21};	//140 10ms extra
byte dataB[8] = {0x8B, 0x26, 0xFA, 0x27, 0xC9, 0x86, 0xA7, 0x00};	//141 10ms
byte dataC[8] = {0x84, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	//142 10ms extra
byte dataD[8] = {0xC0, 0x00, 0x4C, 0x17, 0x94, 0xA4, 0xA0, 0x00};	//144 20ms extra
byte dataE[8] = {0xD7, 0x00, 0x6F, 0x50, 0x32, 0x00, 0x01, 0x00};	//360 50ms
byte dataF[8] = {0x00, 0x29, 0x00, 0xD3, 0x00, 0xD0, 0x73, 0x72};	//361 50ms
//SRS Data
byte dataG[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	//372 100ms

#define CAN0_INT 2						// Set INT to pin 2
MCP_CAN CAN0(10);						// Set CS to pin 10
#define CAN1_INT 3						// Set INT to pin 3
MCP_CAN CAN1(9);						// Set CS to pin 9

unsigned long toVal(unsigned char b[8], char ofs)
{
	unsigned long val = b[0+ofs];		//push bytes to long, ofs determines first or last 4 bytes of buffer.
	val = (val << 8) + b[1+ofs];
	val = (val << 8) + b[2+ofs];
	val = (val << 8) + b[3+ofs];
	return val/256;						//remove Holley multiplier
}

void setup()
{
	//Serial.begin(115200);
  
	if(CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK)
	{
		//Serial.println("Can0 Init OK");
	}
	else
	{
		//Serial.println("Can0 Init Error");
	}
	if(CAN1.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
	{
		//Serial.println("Can1 Init OK");
	}
	else
	{
		//Serial.println("Can1 Init Error");
	}
	
	CAN0.setMode(MCP_NORMAL);			// Set operation mode to normal so the MCP2515 sends acks to received data. MCP_NORMAL MCP_LISTENONLY
	pinMode(CAN0_INT, INPUT);			// Configuring pin for /INT input
	CAN1.setMode(MCP_NORMAL);			// Set operation mode to normal so the MCP2515 sends acks to received data. MCP_NORMAL MCP_LISTENONLY
	pinMode(CAN1_INT, INPUT);			// Configuring pin for /INT input
}

void loop()
{
	if(!digitalRead(CAN0_INT))						// If CAN0_INT pin is low, read receive buffer
	{
		CAN0.readMsgBuf(&rxId, &len, rxBuf);		// Read data: len = data length, buf = data byte(s)
		rxId = rxId & 0x7FFFF000;					// Filter holley serial #

		if(rxId == 0x1E001000)//0x1E001000
		{
			rpm = toVal(rxBuf, 4);
		}
		if(rxId == 0x1E021000)//0x1E021000
		{
			cts = toVal(rxBuf, 4);
		}
		if(rxId == 0x1E025000)//0x1E025000
		{
			oilPr = toVal(rxBuf, 0);
		}
		if(rxId == 0x1E01D000)//0x1E01D000
		{
			iat = toVal(rxBuf, 0);
			tps = toVal(rxBuf, 4);
		}
	}
	
	val_i = round(rpm + 32768);
	dataB[4] = lowByte(val_i);			//tach (0x8000 - 0xA300) = (0-8960rpm)
	dataB[5] = highByte(val_i);			//datb[4,5] = index 5 0x80, index 4 0x00

	val_c = cts - 32;					//byte scales well with -32 (88-128Cold 128-148Norm 148-163Hot)
	dataE[3] = val_c;					//date[3] = 0x80; //index 3 temp gauge

	if(oilPr < oilPrLow)
	{
		dataF[1] = 0x40;				//check engine light flash fast
	}
	else if(cts > ctsHigh)
	{
		dataF[1] = 0x80;				//check engine light flash slow
	}
	else
	{
		dataF[1] = 0x29;				//check engine light off
	}
	if(dataA[1] > 0x0F)
	{
		dataA[1] = 0x00;
	}
	if(dataF[4] > 0x0F)
	{
		dataF[4] = 0x00;
	}

	unsigned long currentTm = millis();

	if(currentTm >= elapsedA+10)
	{
		sendSts = CAN1.sendMsgBuf(0x140, 0, 8, dataA);	//misc
		sendSts = CAN1.sendMsgBuf(0x141, 0, 8, dataB);	//tachometer
		sendSts = CAN1.sendMsgBuf(0x142, 0, 8, dataC);	//misc
		elapsedA = currentTm;
		dataA[1] = dataA[1]+1;
	}
	if(currentTm >= elapsedD+20)
	{
		sendSts = CAN1.sendMsgBuf(0x144, 0, 8, dataD);	//misc
		elapsedD = currentTm;
	}
	if(currentTm >= elapsedE+50)
	{
		sendSts = CAN1.sendMsgBuf(0x360, 0, 8, dataE);	//temp gauge
		sendSts = CAN1.sendMsgBuf(0x361, 0, 8, dataF);	//check engine light
		elapsedE = currentTm;
		dataF[4] = dataF[4]+1;
	}
	if(currentTm >= elapsedG+100)
	{
		sendSts = CAN1.sendMsgBuf(0x372, 0, 8, dataG);	//airbag light
		dataG[1] = dataG[1]+16;
		elapsedG = currentTm;
	}
}
