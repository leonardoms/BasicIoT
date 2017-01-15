#include <VirtualWire.h>

int ledAlert = 13;

void setup() {
  pinMode(ledAlert, OUTPUT);
  Serial.begin(9600);

  
  vw_set_rx_pin(7);
  vw_setup(5000); 
  
  vw_rx_start();
}

void loop() {

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  vw_wait_rx(); // wait for some message...

  if (vw_get_message(buf, &buflen)) {

    Serial.println((char*)&(buf[0]));
    activity(ledAlert);
  }

}

void  activity(int ledAlert) {
  for( int i = 0; i < 5; i++ ) {
    digitalWrite(ledAlert, HIGH);   
    delay(50);                  
    digitalWrite(ledAlert, LOW);
    delay(50);
  }
}
