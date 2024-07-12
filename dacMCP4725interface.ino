#include <Wire.h>
#define MCP4725_ADDR 0x60  

void decimalToBinaryArray(int num, bool binArray[12]) {
  for (int i = 0; i < 12; i++) {
    binArray[11 - i] = (num >> i) & 1;
  }
}

void binaryRightShift(bool binArray[12], int shift, bool resultArray[12]) {
  for (int i = 0; i < 12 - shift; i++) {
    resultArray[i] = binArray[i + shift];
  }
  for (int i = 12 - shift; i < 12; i++) {
    resultArray[i] = 0;
  }
}

int extractLowByte(bool binArray[12]) {
  int lowByte = 0;
  for (int i = 4; i < 12; i++) {
    lowByte |= binArray[i] << (i - 4);
  }
  return lowByte;
}

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  float voltage = 5;
  setDACVoltage(voltage);
  delay(1000); 
}

void setDACVoltage(float voltage) {
  int value = (int)((voltage / 5.0) * 4095);
  
  bool binaryValue[12];
  decimalToBinaryArray(value, binaryValue);
  
  bool highByteArray[12];
  binaryRightShift(binaryValue, 8, highByteArray);
  
  byte highByte = 0;
  for (int i = 0; i < 8; i++) {
    highByte |= highByteArray[i] << (7 - i);
  }

  byte lowByte = extractLowByte(binaryValue);

  Wire.beginTransmission(MCP4725_ADDR);  
  Wire.write(0x40);                      
  Wire.write(highByte);                  
  Wire.write(lowByte);                   
  Wire.endTransmission();               
  Serial.print("Setting voltage to ");
  Serial.print(voltage);
  Serial.print("V (value: ");
  Serial.print(value);
  Serial.print(") HighByte: ");
  Serial.print(highByte, BIN);
  Serial.print(" LowByte: ");
  Serial.println(lowByte, BIN);
}
