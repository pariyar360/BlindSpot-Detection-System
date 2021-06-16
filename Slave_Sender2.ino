#include <RF24Network.h>    //networking library
#include <RF24.h>           //library for connecting nrf24lo1
#include <RF24_config.h>
#include <SPI.h>            //SPI configuration library
#include "NewPing.h"        //library for calculating distance using ultrasonic sensor

//initializing ultrasonic sensor pins
#define TRIGGER_PIN  3  
#define ECHO_PIN     2
#define MAX_DISTANCE 400
int data2;

RF24 radio(7,8); // nRF24L01 (CE,CSN)
RF24Network network(radio); // Include the radio in the network
const uint64_t this_node = 02; // address of this node
const uint16_t node00 = 00;
unsigned long data[6]; // number of sensors

//initializing function to calculate distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup(void)
{
  Serial.begin(9600);
  //starting services
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);

  //initializing output pin for ultrasonic sensor
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop(void)
{
  network.update();
  // calculating distance and storing to variable distance
  unsigned long distance = sonar.ping_cm();
  data[0] = distance;

  //defining header to send data through the network
  RF24NetworkHeader header8(node00);
  bool ok = network.write(header8, &data, sizeof(data)); // Send the data
}
