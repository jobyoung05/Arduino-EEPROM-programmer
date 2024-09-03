/*
Arduino Uno based EEPROM programmer
Copyright (C) 2024 Job Young

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

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

// Value that will fill leftover addresses
const int DEFAULT_VALUE = 0xEA;

/* Change this array to whatever you want to program the EEPROM with.
   If you need to upload more than 10 bytes, simply make the array longer.
   The program will compensate and fill any remaining addresses with DEFAULT_VALUE */
const byte DATA[10] = {0xF0, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF, 0xF0, 0xF1, 0xF8, 0xF9};

// Number of bytes displayed per line when reading the EEPROM
const int READ_LINE_WIDTH = 32;




void pushAddress(int address){
  /* int is always a 16-bit value, shiftOut takes a byte
     so we use shifts to get the low and high byte individually */
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

  //TODO: use toggle bit to detect cycle completion
  
  delay(8);

}


byte read(int addr){

  // Ensure EEPROM is in read state
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
  char addressCode[4];
  char hexByte[2];

  // Print the contents of the EEPROM to the serial monitor
  for (long i = 0; i <= MAX_ADDR; i += READ_LINE_WIDTH){
    sprintf(addressCode, "0x%04X   ", i);
    Serial.print(addressCode);

    for (int j = 0; j < READ_LINE_WIDTH; j++){
      sprintf(hexByte, "%02X  ", read(i+j));
      Serial.print(hexByte);
    }
    
    Serial.print("\n");
  }
}

void setup() {

  /* Ensure the EEPROM is in read mode before pinMode assignment
     so that we don't accidentally write bits */
  digitalWrite(OE, LOW);
  digitalWrite(WE, HIGH);

  // Outputs
  pinMode(SERIALOUT, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(WE, OUTPUT);

  // Inputs
  pinMode(WRITE_BUTTON, INPUT);
  pinMode(READ_BUTTON, INPUT);

  Serial.begin(115200);


  

}

void loop() {
  
  if (digitalRead(READ_BUTTON)){
    Serial.println("READING EEPROM...");
    printAll();
  }

  if (digitalRead(WRITE_BUTTON)){
    Serial.println("WRITING EEPROM...");

    // Program user defined data
    for (long i = 0; i < sizeof(DATA); i++)
    {
      write(i, DATA[i]);
    }
    
    // Program any leftover space with user specified DEFAULT_VALUE
    for (long i = sizeof(DATA) - 1; i <= MAX_ADDR; i++){
      write(i, DEFAULT_VALUE);
    }
    Serial.println("WRITE COMPLETE");

  }


}
