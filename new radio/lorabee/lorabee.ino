/* 430MHz LoRa Xbee Replacement Firmware
 * Nick Ames 2017
 * Takes data in/out on UART0 and sends/receives
 * it on the radio. Integrity of received data is
 * guaranteed, but reliable transmission is not.
 * The callsign is transmitted periodically for
 * FCC compliance.
 * Adapted from Adafruit example.
 * 
 * The over-the-air packet format is as follows:
 * <header><data...>
 * Header is 0x01 for a callsign packet and 0x02
 * for a data packet. Callsign packets are ignored by
 * the other side. */
#include <SPI.h>
#include <RH_RF95.h>
#include <string.h>

#define CALLSIGN_STR "\001Callsign: KG7PFX"
#define CALLSIGN_HEADER 1
#define CALLSIGN_PER 400
#define DATA_HEADER 2
#define MAX_LEN 80
uint8_t SendBuf[MAX_LEN];
uint8_t SendLen=1;
uint8_t RecvBuf[MAX_LEN];
uint8_t RecvLen;
uint32_t LastByteMilis;

#define INTERFACE_BAUD 115200
#define SERIAL_TIMEOUT 10 /* Number of milliseconds to wait for a new character before sending a packet. */

#define LED 13
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 3
#define RF95_FREQ 434.0
#define RF95_MODE rf95.Bw500Cr45Sf128
#define RF95_PREAMBLE 2

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void fail_loop(){
	while(1){
		
	}
	
}


void setup(){
	pinMode(LED, OUTPUT);     
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);
	while (!Serial){
		/* Wait */
	}
	Serial.begin(INTERFACE_BAUD);
	delay(100);

	/* Reset radio module */
	digitalWrite(RFM95_RST, LOW);
	delay(10);
	digitalWrite(RFM95_RST, HIGH);
	delay(10);

	while (!rf95.init()){
		fail_loop();
	}

	if (!rf95.setFrequency(RF95_FREQ)) {
		fail_loop();
	}
	
	RH_RF95::ModemConfig config;
	config.reg_1d = 0x92;
	config.reg_1e = 
	
	if(!rf95.setModemConfigLength(RF95_MODE)){ /* fast + short range */
		fail_loop();
	}
	rf95.setPreamble(RF95_PREABLE);
	rf95.setTxPower(23, false);
	
	LastByteMilis = millis();
}

/* Wait until the radio is ready, then transmit a packet.
 * If CALLSIGN_PER packets have been sent, transmit a callsign packet
 * after sending the data packet.
 * 
 * Packet data should include the header. */
void wait_and_transmit(uint8_t len, uint8_t *packet_data){
	static uint32_t non_callsign;
	rf95.send(packet_data, len);
	non_callsign++;
	if(non_callsign >= CALLSIGN_PER){
		rf95.send((uint8_t *) CALLSIGN_STR, strlen(CALLSIGN_STR));
		non_callsign = 0;
	}
}


void loop(){
	SendBuf[0] = DATA_HEADER;
	if(rf95.available()){
		RecvLen = MAX_LEN;
		if(rf95.recv(RecvBuf, &RecvLen)){
			if(RecvBuf[0] == DATA_HEADER){
				Serial.write(RecvBuf+1, RecvLen-1);
			}
		} else {
			fail_loop();
		}
	}

	uint16_t avail_count, read_count;
	if((avail_count = Serial.available())){
		LastByteMilis = millis();
		uint8_t count;
		read_count = Serial.readBytes(SendBuf + SendLen, min(MAX_LEN - SendLen, avail_count));
		if(read_count > 0){
			SendLen += read_count;
		}
	}
	if((SendLen > 1 && ((millis() - LastByteMilis) > SERIAL_TIMEOUT)) || SendLen >= MAX_LEN){
		wait_and_transmit(SendLen, SendBuf);
		SendLen = 1;
	}
}
