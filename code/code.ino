#define EEPROM_LSB 2
#define EEPROM_MSB 9
#define SERIAL 10
#define SRCLK 11
#define RCLK 12
#define OE 13
#define WE A0
#define WRITE_BUTTON A1
#define READ_BUTTON A2

#define MAX_ADDR 0x8FFF


void pushAddress(int address){
  /* int is always a 16-bit value, shiftOut takes a byte
     so we use shifts to get the low and high byte individually*/
  shiftOut(SERIAL, SRCLK, MSBFIRST, byte(address >> 8);
  shiftOut(SERIAL, SRCLK, MSBFIRST, byte(address));


  /* 74HC595 shift registers also have a storage register
     so we pulse RCLK to move the data from the shift register
     into the storage register, and thus onto the output pins */
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}


byte read(int addr){

  // ensure EEPROM is in read state
  digitalWrite(OE, LOW);
  digitalWrite(WE, HIGH);

  pushAddress(addr);

  byte result;

  for (int i == EEPROM_MSB; i >= EEPROM_LSB; i--){
    pinMode(i, INPUT);
    result <<;
    result += digitalRead(i);
  }

}


void printAll(){
  for (int i = 0; i <= MAX_ADDR; i++){
    
  }
}

void setup() {

  /* ensure the EEPROM is in read mode before pinMode assignment
     so that we don't accidentally write bits */
  digitalWrite(OE, LOW);
  digitalWrite(WE, HIGH);

  // outputs
  pinMode(SERIAL, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);

  // inputs
  pinMode(WRITE_BUTTON, INPUT);
  pinMode(READ_BUTTON, INPUT);

  Serial.begin(9600);


  

}

void loop() {
  
  if (digitalRead(READ_BUTTON)){
    Serial.println("READING EEPROM...")


  }

}
