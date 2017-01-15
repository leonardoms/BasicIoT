#include <OneWire.h>
#include <DallasTemperature.h>
#include <VirtualWire.h>


// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9 // Lower resolution
int numberOfDevices; // Number of temperature devices found
DeviceAddress tempDeviceAddress;
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int ledAlert = 13;

void setup() {
  pinMode(ledAlert, OUTPUT);

  
  sensors.begin();
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();

  if( numberOfDevices == 0 )
    panic(2); // blink two times

  if( sensors.getAddress(tempDeviceAddress, 0) == false)
    panic(3); // ghost device found, check cables and power!

  sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);

  // initialize RF transmiter
  vw_set_tx_pin(4);
  vw_setup(5000);
}

void loop() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(tempDeviceAddress);

  // make text
  int tempC_Int = (int) tempC;
  int tempC_Dec = (int) ( tempC - tempC_Int ) * 10; // one decimal 
  char msg[24];
  sprintf(msg, "%i.%i", tempC_Int,tempC_Dec);
  vw_send((uint8_t *)msg, strlen(msg)); // send data
  vw_wait_tx();

  delayMinutes(10);
}

void panic(int times) {
  while(true) {
    for( int i = 0; i < times; i++ ) {
      digitalWrite(ledAlert, HIGH);   
      delay(100);                  
      digitalWrite(ledAlert, LOW);
      delay(100);
    }
    delay(1900);
  }
}

void delayMinutes(int minutes) {

  int limit = minutes * 60;

  for(int i = 0; i < limit; i++) 
  {
    delay(1000);
  }
}

