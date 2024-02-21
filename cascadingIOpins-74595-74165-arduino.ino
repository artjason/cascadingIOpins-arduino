
byte segmentPatterns[]= {0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00}; //active LOW 7 segment patterns
void setup() {
Serial.begin(9600);
//595
 DDRC |= B00000111; //pins A0= data in, A1= latch, A2=shift/clock (output)
//165
   DDRB &= ~(1 << 0); //serial output set as input
   DDRB |= (1 << 1); //load/latch
   DDRB |= (1 << 2); //clk
}

void loop() {
  int index = 0;
  byte inputdata = input();
  Serial.println(inputdata, BIN);
     for (int i = 7; i >= 0; i--) {
    if (((inputdata >> i) & 0x01) == 0) {
      index = i + 1;  // Adjust to 1-based index
      break;
    }
   }
  output(segmentPatterns[index]);
}

byte input() {
  byte result = 0;
  PORTB &= ~(1 << 1); // clear load pin
  PORTB |= (1 << 1); // set load pin
  
  for (int i = 7; i >=0 ; i--) {
    byte bit = (PINB & (1 << 0)); //read bits from Serial Data from MSB to LSB
    result |= (bit << i);
    PORTB |= (1 << 2); //set clk
    PORTB &= ~(1 << 2);//clear clk
  }
  return result;
}

void output(byte data) {
  PORTC &= B11111101; // Clear latch
  for (int i = 7; i >= 0; i--){
    PORTC = (PORTC & B11111010) | ((data & 0x80) >> 7); //clear serial clock and ds pin low, then read the MSB first
    PORTC |= (1 << 2); //set serial clock
    data  = data << 1; //shift data to read the next bit from MSB to LSB
  }
  PORTC |= B00000010; // Set latch
}