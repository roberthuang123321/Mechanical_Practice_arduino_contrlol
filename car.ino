#include <SoftwareSerial.h>

#include <Servo.h>
#include <Arduino.h>
//创建气泵和电磁阀控制对象（Servo myservo0）为气泵 （ Servo myservo1）为电磁阀  
// 该变量用气泵电磁阀开关（0为关闭，180为开启）
Servo myservo0;
Servo myservo1;
Servo myservo2; //door

// 定义蓝牙模块连接的脚位
SoftwareSerial bleSerial(2, 3); // RX, TX

// 定义马达控制脚位
const int motorAPin1 = 6;
const int motorAPin2 = 7;
const int motorBPin1 = 8;
const int motorBPin2 = 9;
const int motorCPin1 = 10;
const int motorCPin2 = 11;
const int motorDPin1 = 12;
const int motorDPin2 = 13;

void setup() {
  Serial.begin(9600);           // 初始化主串行端口
  bleSerial.begin(9600);        // 初始化蓝牙串口
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(motorBPin1, OUTPUT);
  pinMode(motorBPin2, OUTPUT);
  pinMode(motorCPin1, OUTPUT);
  pinMode(motorCPin2, OUTPUT);
  pinMode(motorDPin1, OUTPUT);
  pinMode(motorDPin2, OUTPUT);
  myservo0.attach(A0);
  myservo1.attach(A1);
  myservo2.attach(5);
 
  myservo0.write(0);  
  myservo1.write(0);
  myservo2.write(0); 
}

void loop() {
  if (bleSerial.available()) {
    char cmd = bleSerial.read(); // 读取命令
    controlMotors(cmd);

    Serial.print("Received command: ");  // 在串行监视器输出接收到的命令
    Serial.println(cmd);
  }
}

void controlMotors(char command) {
  switch (command) {
    case 'a': // 前进
      driveMotors(HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW);
      delay(100);
      
      break;
    case 'c': // 后退
      driveMotors(LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH);
      delay(100);
      driveMotors(LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW);
      break;
    case 'd': // 左转
      driveMotors(LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW);
      delay(100);
      driveMotors(LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW);
      break;
    case 'b': // 右转
      driveMotors(HIGH, LOW, LOW, HIGH, HIGH, LOW , LOW, HIGH);
      delay(100);
      driveMotors(LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW);
      break;
    case 'G': // 停止
      driveMotors(LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW);
      delay(500);
      break;
    case 'F':
      myservo0.write(180);  //吸 
      myservo1.write(0);   //吸
      delay(2000);                           
      myservo0.write(0);   //停止
      myservo1.write(0);
      break; 
    case 'H':
      myservo0.write(0);   //放
      myservo1.write(180); //放
      delay(1000);                           
      myservo0.write(0);   //停止
      myservo1.write(0);
      break;
    case 'E':
      myservo2.write(90);
      delay(3000);
      myservo2.write(0);
      break;


  }
}

void driveMotors(bool a1, bool a2, bool b1, bool b2, bool c1, bool c2, bool d1, bool d2) {
  digitalWrite(motorAPin1, a1);
  digitalWrite(motorAPin2, a2);
  digitalWrite(motorBPin1, b1);
  digitalWrite(motorBPin2, b2);
  digitalWrite(motorCPin1, c1);
  digitalWrite(motorCPin2, c2);
  digitalWrite(motorDPin1, d1);
  digitalWrite(motorDPin2, d2);
}
