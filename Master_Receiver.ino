#include <LiquidCrystal.h>      //library for lcd display
#include <RF24Network.h>        //networking library
#include <RF24Network_config.h>
#include <SPI.h>                //SPI configuration library
#include <RF24.h>               //library for connecting nrf24lo1
#include <RF24_config.h>               

int Time=50;
unsigned long data1, data2;

LiquidCrystal lcd( 8,9,4,5,6,7 );  //interfacing pins

RF24 radio(2,3); //ce and csn pins for nrf24lo1
RF24Network network(radio); // Include the radio in the network
const uint16_t this_node = 00; // Address of our node in Octal format
unsigned long data[6]; // number of sensors

void setup() {
  Serial.begin(9600);
  //staring services
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  //starting lcd display by defining the type of lcd display
  lcd.begin(16, 2);
  //setting cursor at start and printing message for initialization
  lcd.setCursor(0,0);
  lcd.print("Connecting...");
  delay(2000);
}

void loop()
{
  network.update();
  while ( network.available() )
  {
    RF24NetworkHeader header;
    network.read(header, &data, sizeof(data)); // Read the incoming data

    if (header.from_node == 01) { // If data comes from Node 01
      data1 = data[0]; 
    }
     
    if (header.from_node == 02) { // If data comes from Node 02
      data2 = data[0]; 
    }
    //printing the data received from slave nodes to the Serial print of Arduino IDE
    Serial.println("data1: ");
    Serial.println(data1);
    Serial.println("data2: ");
    Serial.println(data2);
    lcd.setCursor(0,0);
    lcd.print("Left  : ");
    //defining if function to print message based on the distance calculated from node 01
    if (data1 >0 && data1<200){
      lcd.print("Warning");
    }else{
      lcd.print("Safe    ");
    }

    //defining if function to print message based on the distance calculated from node 02
    lcd.setCursor(0,1);
    lcd.print("Right : ");
    if (data2 >0 && data2<200){
      lcd.print("Warning");
    }else{
      lcd.print("Safe    ");
    }
  }
}
