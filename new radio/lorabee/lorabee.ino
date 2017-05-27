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

#define CALLSIGN_STR "\x01Callsign: KG7PFX"
#define CALLSIGN_HEADER 1
#define CALLSIGN_PER 30
#define DATA_HEADER 2
#define MAX_LEN 250
uint8_t PacketBuf[MAX_LEN];
uint8_t PacketLen;

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 3
#define RF95_FREQ 434.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() 
{
  pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  while (!Serial);
  Serial.begin(9600);
  delay(100);
 
  Serial.println("Arduino LoRa RX Test!");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
 
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
}

/* Wait until the radio is ready, then transmit a packet.
 * If CALLSIGN_PER packets have been sent, transmit a callsign packet
 * after sending the data packet.
 * 
 * Packet data should include the header. */
void wait_and_transmit(uint8_t len, uint8_t *packet_data){
	
	
}

void loop(){
	
}

void loopseudocode(){
	if byte available
		add to buffer
		reset byte time counter
	if buffer full (MAXLEN - 1)
		wait until ready and transmit
	if time between bytes elapsed
		wait until ready and transmit
	if radio data available
		read
		check header
		if data
			send over uart
}
	
}
