#include <Wire.h>
#include <Servo.h>
Servo myServo;
long accelX, accelY, accelZ;
float accelForceX, accelForceY, accelForceZ;
int addressMPU = 0x68;
float roll;
int servoPin = 3;
int offSet = 90;

void setup() {
// put your setup code here, to run once:

//Setting up the MPU6050
Serial.begin(9600);
Wire.begin();
Wire.beginTransmission(addressMPU);
Wire.write(0x6B);
Wire.write(0x10);
Wire.endTransmission();


//Setting up the accelerometer
Wire.beginTransmission(addressMPU);
Wire.write(0x1C);
Wire.write(0x00);
Wire.endTransmission();

myServo.attach(servoPin);

}


void loop() {
// put your main code here, to run repeatedly:

//Reading Accelromter values
Wire.beginTransmission(addressMPU);
Wire.write(0x3B);
Wire.endTransmission();
Wire.requestFrom(0b1101000, 6);
while(Wire.available() < 6);
accelX = Wire.read()<<8| Wire.read();
accelY = Wire.read()<<8| Wire.read();
accelZ = Wire.read()<<8| Wire.read();
accelForceX = (accelX/16384.0);
accelForceY = (accelY/16384.0);
accelForceZ = (accelZ/16384.0);

//Calculating Roll angle
roll = (atan(accelForceY / sqrt(pow(accelForceX, 2) + pow(accelForceZ, 2))) * 180 / PI);

//Setting Offset
if(Serial.available() != 0){
  offSet = Serial.parseInt();

  if(offSet > 0){
    myServo.write(offSet-roll);
  }
  else if (offSet < 0){
    offSet= 90 + (offSet*-1);
  }
  else{
    offSet = 90;
  }
}

//Moving the Servo
myServo.write(offSet-roll);

//Printing data to Serial
Serial.print("Roll angle: ");
Serial.print(roll);
Serial.print(" accelForceX: ");
Serial.print(accelForceX);
Serial.print(" accelForceY: ");
Serial.print(accelForceY);
Serial.print(" accelForceZ: ");
Serial.print(accelForceZ);
Serial.println();

delay(100);

}






