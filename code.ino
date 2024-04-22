#define EEPROM_LSB 2
#define EEPROM_MSB 9
#define SERIAL 10
#define SRCLK 11
#define RCLK 12
#define OE 13
#define WE A0

void setup() {
  pinMode(SERIAL, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
