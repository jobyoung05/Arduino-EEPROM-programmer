#define EEPROM_LSB 2
#define EEPROM_MSB 9
#define SERIAL 10
#define SRCLK 11
#define RCLK 12
#define OE 13
#define WE A0
#define WRITE_BUTTON A1
#define READ_BUTTON A2

#define MAX_ADDR 0x7FFF

byte data[10] = {0xF0, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0xF0, 0xF1, 0xF8, 0xF9};

bool flag = true;


void pushAddress(int address){
  /* int is always a 16-bit value, shiftOut takes a byte
     so we use shifts to get the low and high byte individually*/
  shiftOut(SERIAL, SRCLK, MSBFIRST, byte(address >> 8));
  shiftOut(SERIAL, SRCLK, MSBFIRST, byte(address));


  /* 74HC595 shift registers also have a storage register
     so we pulse RCLK to move the data from the shift register
     into the storage register, and thus onto the output pins */
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}


void write(int addr, byte data){
  pushAddress(addr);
  digitalWrite(OE, HIGH);

  for (int i = EEPROM_LSB; i <= EEPROM_MSB; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, data & 1);
    data = data >> 1;
  }

  digitalWrite(WE, LOW);
  delayMicroseconds(1);
  digitalWrite(WE, HIGH);

  // making use of AT28C256 toggle bit to detect cycle completion
  
  delay(10);

}


byte read(int addr){

  // ensure EEPROM is in read state
  digitalWrite(OE, LOW);
  digitalWrite(WE, HIGH);

  pushAddress(addr);

  byte result;

  for (int i = EEPROM_MSB; i >= EEPROM_LSB; i--){
    pinMode(i, INPUT);
    result = result << 1;
    result += digitalRead(i);
  }

  return result;

}


void printAll(){
  int lineSize = 32;
  char addressCode[6];
  char hexByte[2];

  for (int i = 0; i <= MAX_ADDR; i += lineSize){
    sprintf(addressCode, "0x%06X   ", i);
    Serial.print(addressCode);
    for (int j = 0; j < lineSize; j++){
      sprintf(hexByte, "%02X  ", read(i+j));
      Serial.print(hexByte);
    }
    Serial.print("\n");
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

  Serial.begin(115200);


  

}

void loop() {
  
  if (digitalRead(READ_BUTTON)){
    Serial.println("READING EEPROM...");
    printAll();
  }

  if (digitalRead(WRITE_BUTTON) & flag){
    Serial.println("WRITING EEPROM...");
    for (int i = 0; i <= MAX_ADDR; i++){
      write(i, 0x00);
      if ((i & 0x0F) == 0x0F){
        Serial.println(i, HEX);
      }
    }
    Serial.println("WRITE COMPLETE");
    flag = false;

  }


}
