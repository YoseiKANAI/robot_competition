# include <Servo.h>

// 車輪 (W)
const int WAIN1 = 2;  //pin assign of Arduino to motor_Driver
const int WAIN2 = 3;
const int WAPWM = 5;
const int WBIN1 = 4;
const int WBIN2 = 7;
const int WBPWM = 6;

// 吸引機構（M）
const int MIN1 = 13;
const int MIN2 = 12;
//const int MPWM = 11;

// 距離センサ
int trigPin = 9;  // Trigger
int echoPin = 10;  // Echo

// サーボモータ
Servo A;

// 時間計測
unsigned long starttime, nowtime;

//　吸引機構の時間制御
int time;
int flag = 0;

// 距離計測
long duration, cm, inches;

void setup() {
  // put your setup code here, to run once:
  pinMode(WAIN1, OUTPUT);
  pinMode(WAIN2, OUTPUT);
  pinMode(WAPWM, OUTPUT);
  pinMode(WBIN1, OUTPUT);
  pinMode(WBIN2, OUTPUT);
  pinMode(WBPWM, OUTPUT);
  pinMode(MIN1, OUTPUT);
  pinMode(MIN2, OUTPUT);
//  pinMode(MPWM, OUTPUT);
//  pinMode(MSTBY, OUTPUT);
  Serial.begin(9600);  //Com speed
//  digitalWrite(MSTBY, HIGH);
  analogWrite(WAPWM, 180);  //control speed of A-side of motor (max 255)
  analogWrite(WBPWM, 130);   //control speed of B-side of motor (max 255)
//  analogWrite(MPWM, 255);  //control speed of B-side of motor (max 255)

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  A.attach(8);
  A.write(180);
}


void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  cm = (duration / 2) / 29.1;  // Divide by 29.1 or multiply by 0.0343

  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  int key_s = 0;
  // cmが6cm以内の時フラグを立てる
  if (flag == 0) {
    if (cm <= 5) {
      key_s = 1;
      flag = 1;
      starttime = millis();
    }
  } else {
    key_s = 1;
    nowtime = millis();
    int diff = nowtime - starttime;
    if (diff > 10000) {
      flag = 0;
    }
  }

  switch (key_s) {
    case 0:  //Stop
      digitalWrite(MIN1, LOW);
      digitalWrite(MIN2, LOW);
      break;
    case 1:  //
      digitalWrite(MIN1, HIGH);
      digitalWrite(MIN2, LOW);
      break;
  }

  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {  //Recieve control-commands from COM port
    char key_w = Serial.read();
    switch (key_w) {
      case '0':  //Stop
        digitalWrite(WAIN1, LOW);
        digitalWrite(WBIN1, LOW);
        digitalWrite(WAIN2, LOW);
        digitalWrite(WBIN2, LOW);
        break;
      case '1':  //Forward
        digitalWrite(WAIN1, HIGH);
        digitalWrite(WBIN1, HIGH);
        digitalWrite(WAIN2, LOW);
        digitalWrite(WBIN2, LOW);
        break;
      case '2':  //Back
        digitalWrite(WAIN1, LOW);
        digitalWrite(WBIN1, LOW);
        digitalWrite(WAIN2, HIGH);
        digitalWrite(WBIN2, HIGH);
        break;
      case '3':  //Turn1
        digitalWrite(WAIN1, HIGH);
        digitalWrite(WBIN1, LOW);
        digitalWrite(WAIN2, LOW);
        digitalWrite(WBIN2, HIGH);
        break;
      case '4':  //Turn2
        digitalWrite(WAIN1, LOW);
        digitalWrite(WBIN1, HIGH);
        digitalWrite(WAIN2, HIGH);
        digitalWrite(WBIN2, LOW);
        break;
      case '5':  //Servo
        digitalWrite(WAIN1, LOW);
        digitalWrite(WBIN1, LOW);
        digitalWrite(WAIN2, LOW);
        digitalWrite(WBIN2, LOW);
        digitalWrite(MIN1, LOW);
        digitalWrite(MIN2, HIGH);
        A.write(55);
        delay(7000);
        A.write(180);
        break;
    }   
  }
  delay(250);
}