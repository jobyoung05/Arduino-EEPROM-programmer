const int EEPROM_LSB = 2;
const int EEPROM_MSB = 9;
const int SERIALOUT = 10;
const int SRCLK = 11;
const int RCLK = 12;
const int OE = 13;
const int WE = A0;
const int WRITE_BUTTON = A1;
const int READ_BUTTON = A2;

const int MAX_ADDR = 0x7FFF;

byte data[10] = {0xF0, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0xF0, 0xF1, 0xF8, 0xF9};

bool flag = true;


void pushAddress(int address){
  /* int is always a 16-bit value, shiftOut takes a byte
     so we use shifts to get the low and high byte individually*/
  shiftOut(SERIALOUT, SRCLK, MSBFIRST, byte(address >> 8));
  shiftOut(SERIALOUT, SRCLK, MSBFIRST, byte(address));


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
  
  delay(8);

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

  for (long i = 0; i <= MAX_ADDR; i += lineSize){
    sprintf(addressCode, "0x%04X   ", i);
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
  pinMode(SERIALOUT, OUTPUT);
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
    for (long i = 0; i <= MAX_ADDR; i++){
      write(i, 0x00);
      if (i%1024 == 0){
        Serial.print("#");
      }
    }
    Serial.println("WRITE COMPLETE");
    flag = false;

  }


}
