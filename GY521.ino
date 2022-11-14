// MPU-6050 Short Example Sketch
//www.elegoo.com
//2016.12.9

#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,GyX,GyY,GyZ,Tmp;
int blueLED = 53;
int yellowLED = 51;
int buzzer = 25;
const int SW_Pin = 2;
const int X_Pin = 0;
const int Y_Pin = 1;

char apple;
void setup(){
  pinMode(blueLED, OUTPUT); 
  pinMode(yellowLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(SW_Pin, INPUT);
  digitalWrite(SW_Pin, HIGH); 
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop(){
  //Gyro Correction
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers

  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=(Wire.read()<<8|Wire.read());  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=(Wire.read()<<8|Wire.read());  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=(Wire.read()<<8|Wire.read());  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

//print out accelerometer/gyrometer data
//Serial.print(" | AcX = "); Serial.println(AcX);
//Serial.print(" | AcY = "); Serial.println(AcY);
//Serial.print(" | AcZ = "); Serial.println(AcZ);
//Serial.print(" | GyX = "); 
//Serial.println(GyX); 
//Serial.print(" | GyY = "); 
//Serial.println(GyY); 
//Serial.print(" | GyZ = "); 
//Serial.println(GyZ);
//Serial.println();

delay(150);
//check for apple eaten from python
  while(Serial.available())
  {
    apple = Serial.read();
  }
  if(apple == '1')
  {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    apple = '0';
   }
//poll for joystick changes
   if(analogRead(X_Pin) == 0)//right
   {
    Serial.println("r");
   }
   else if(analogRead(X_Pin) == 1023)//left
   {
    Serial.println("l");
   }
   else if(analogRead(Y_Pin) == 0) //up
   {
    Serial.println("d");
   }
   else if(analogRead(Y_Pin) == 1023) //down
   {
    Serial.println("u");
   }
//poll for gyrometer changes
   if(GyX > 10000)//up in python
   {
    digitalWrite (blueLED, HIGH);
    Serial.println("u");
   }
   else if(GyX < -10000)//down in python
   { 
    digitalWrite (blueLED, LOW);
    Serial.println("d");
   }
   else if(GyY > 10000)//left in python
   {
    digitalWrite (yellowLED, HIGH);
    Serial.println("l");
   }
   else if(GyY < -10000)//right in python
   {
    digitalWrite (yellowLED, LOW);
    Serial.println("r");
   }
//poll for accelerometer changes
  if(abs(AcX) > 10000)
  {
   Serial.println("z");   
  }
}
