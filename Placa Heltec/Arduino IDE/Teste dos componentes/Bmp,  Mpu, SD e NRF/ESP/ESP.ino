#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SD true
#define ENABLE_NRF true



#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "nRF24L01.h"

#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

#define CS_NRFPIN 12
#define CE_PIN 2



SPIClass vspi(VSPI);
SPIClass hspi(HSPI);




// instantiate an object contadorfor the nRF24L01 transceiver
RF24 radio(CE_PIN, CS_NRFPIN);

// Let these addresses be used for the pair
uint8_t address[][6] = {"1Node", "2Node"};
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
// 0 uses address[0] to transmit, 1 uses address[1] to transmit
bool radioNumber = 1;

// Used to control whether this node is sending or receiving
// true = TX role, false = RX role
bool role = true;

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
float payload = 0.0;

/* CONFIGURAÇÕES PARA O USO DO CARTÃO SD */
#include <SD.h>

#define CS_SDPIN 5

File logfile;
int contador = 0;


/* CONFIGURAÇÕES PARA O USO DO BMP e MPU */
#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68

Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;


/* FUNÇÕES BMP280 */
void setupBMP() {
  if(!bmp.begin(BMP_ADRESS)) {
    Serial.println("BMP not working ...");
    while(1);
  }
  Serial.println("BMP conectado!");

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}

void testBMP() {
  float number = bmp.readAltitude(1013);
  Serial.println("---------------------------------");
  Serial.println(String("BMP:") + String(number));
  Serial.println("---------------------------------");
}

/* FUNÇÕES MPU6050 */
void setupMPU() {
  if (!mpu.begin(MPU_ADRESS)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(100);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void testMPU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  Serial.println("---------------------------------");
  Serial.println("MPU:");
  Serial.println(a.acceleration.v[0]);
  Serial.println(a.acceleration.v[1]);
  Serial.println(a.acceleration.v[2]);
  Serial.println(a.acceleration.z);
  Serial.println(g.gyro.pitch);
  Serial.println(g.gyro.roll);
  Serial.println("---------------------------------");
}

/* FUNÇÕES CARTÃO MICROSD */
void writeSd(String text){
  logfile = SD.open("/teste.txt", FILE_APPEND);
  if(logfile){
    logfile.println(text);
    Serial.println("Gravando...");
    logfile.close();
  } else {
    Serial.println("Não foi possível gravar...");
  }
}

void setupSD() {
if(!SD.begin(CS_SDPIN)){
    Serial.println("SD not working ...");
    while(1);
  }
  Serial.println("MicroSD Conectado!");
}

/* FUNÇÕES NRF */
void setupNRF() {
  // initialize the transceiver on the SPI bus
  delay(100);
  while(!radio.begin()) {
    
    Serial.println(F("Radio hardware is not responding!!"));
    Serial.println(F("NRF24L01 NÃO CONFIGURADO!\n"));
    delay(100);
  }

  // print example's introductory prompt
  Serial.println(F("RF24/examples/GettingStarted"));

  // To set the radioNumber via the Serial monitor on startup
  Serial.println(F("Which radio is this? Enter '0' or '1'. Defaults to '0'"));
  while (!Serial.available()) {
    // wait for user input
  }
  char input = Serial.parseInt();
  radioNumber = input == 1;
  Serial.print(F("radioNumber = "));
  Serial.println((int)radioNumber);

  // role variable is hardcoded to RX behavior, inform the user of this
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  delay(100);
  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  // RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  // float datatype occupies 4 bytes
  radio.setPayloadSize(sizeof(payload));

  // set the TX address of the RX node into the TX pipe
  // always uses pipe 0
  radio.openWritingPipe(address[radioNumber]);     

  // set the RX address of the TX node into a RX pipe
  // using pipe 1
  radio.openReadingPipe(1, address[!radioNumber]); 

  // additional setup specific to the node's role
  if (role) {
    // put radio in TX mode
    radio.stopListening();  
  } else {
    // put radio in RX mode
    radio.startListening(); 
  }
}

void updateNRF() {
  if (role) {
    // This device is a TX node
    unsigned long start_timer = micros();                    // start the timer
    bool report = radio.write(&payload, sizeof(float));      // transmit & save the report
    unsigned long end_timer = micros();                      // end the timer

    if (report) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println(payload);                               // print payload sent
      payload += 0.01;                                       // increment float payload
    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }

    // to make this example readable in the serial monitor
    delay(100);

  } else {
    // This device is a RX node

    delay(100);
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&payload, bytes);            // fetch payload from FIFO
      Serial.print(F("Received "));
      Serial.print(bytes);                    // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);                     // print the pipe number
      Serial.print(F(": "));
      Serial.println(payload);                // print the payload's value
    }
  }

  if (Serial.available()) {
    // change the role via the serial monitor

    char c = toupper(Serial.read());
    if (c == 'T' && !role) {
      // Become the TX node

      role = true;
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      radio.stopListening();

    } else if (c == 'R' && role) {
      // Become the RX node

      role = false;
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      radio.startListening();
    }
  }
}





void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  
  /*
  vspi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_NRFPIN);
  hspi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_SDPIN);
  */
  
  pinMode(CS_NRFPIN, OUTPUT);
  pinMode(CS_SDPIN, OUTPUT);
  
  if(ENABLE_NRF) {
    digitalWrite(CS_NRFPIN, LOW);
    delay(100);
    setupNRF();
    digitalWrite(CS_NRFPIN, HIGH);
  }

  if(ENABLE_SD) {
    digitalWrite(CS_SDPIN, LOW);
    delay(100);
    setupSD();
    digitalWrite(CS_SDPIN, HIGH);
  }

  if(ENABLE_BMP) {
    setupBMP();
  }

  if(ENABLE_MPU) {
    setupMPU();
  }
}

void loop() {
  
  if(ENABLE_NRF) {
    digitalWrite(CS_NRFPIN, LOW);
    delay(100);
    updateNRF();
    digitalWrite(CS_NRFPIN, HIGH);
  }

  if(ENABLE_SD) {
    digitalWrite(CS_SDPIN, LOW);
    delay(100);
    writeSd("Alanzinho" + String(contador));
    digitalWrite(CS_SDPIN, HIGH);
  }

  if(ENABLE_BMP) {
    testBMP();
  }

  if(ENABLE_MPU) {
    testMPU();
  }

  
  contador++;
  delay(100);
}
