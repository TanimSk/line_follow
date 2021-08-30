
#include <Servo.h>

#define RightForward A0
#define RightBackward A1

#define LeftForward A2
#define LeftBackward A3

#define IR_Right4 13
#define IR_Right3 12
#define IR_Right2 11
#define IR_Right1 10

#define IR_Left1 9
#define IR_Left2 8
#define IR_Left3 7
#define IR_Left4 6

/*Servo config*/
#define ServoPin 5
Servo srv; /*PWM pin --> orange*/

/*ultrasonic sensor*/
#define pingPin 4
#define echoPin 2

#define SPEED 95  /*0-255 range*/
#define SPEED_PIN_R 3
#define SPEED_PIN_L 3

bool ir_right1, ir_right2, ir_right3, ir_right4, ir_left1, ir_left2, ir_left3, ir_left4;

int cmd_index = 0;
char cmd[] = {'L', 'R', 'S'};

float distance;

void setup(){
//  Serial.begin(9600);

  pinMode(RightForward, OUTPUT);
  pinMode(RightBackward, OUTPUT);
  pinMode(LeftForward, OUTPUT);
  pinMode(LeftBackward, OUTPUT);
  pinMode(SPEED_PIN_R, OUTPUT);
  pinMode(SPEED_PIN_L, OUTPUT);

  pinMode(IR_Right4, INPUT);
  pinMode(IR_Right3, INPUT);
  pinMode(IR_Right2, INPUT);
  pinMode(IR_Right1, INPUT);
  pinMode(IR_Left1, INPUT);
  pinMode(IR_Left2, INPUT);
  pinMode(IR_Left3, INPUT);
  pinMode(IR_Left4, INPUT);

  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);

  srv.attach(ServoPin);
  srv.write(90);

  delay(1000);
  analogWrite(SPEED_PIN_R, SPEED);
  analogWrite(SPEED_PIN_L, SPEED);
}

void left(){
  digitalWrite(RightForward, HIGH);
  digitalWrite(LeftBackward, LOW);

  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, LOW);
  delay(20);
}

void hard_left(){
  digitalWrite(RightForward, HIGH);
  digitalWrite(LeftBackward, HIGH);

  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, LOW);
  delay(110);
}

void right(){
  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, HIGH);

  digitalWrite(RightForward, LOW);
  digitalWrite(LeftBackward, LOW);
  delay(20);
}

void hard_right(){
  digitalWrite(RightBackward, HIGH);
  digitalWrite(LeftForward, HIGH);

  digitalWrite(RightForward, LOW);
  digitalWrite(LeftBackward, LOW);
  delay(110);
}

void forward(){
  digitalWrite(RightForward, HIGH);
  digitalWrite(LeftForward, HIGH);

  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftBackward, LOW);
  delay(20);
}

void backward(){
  digitalWrite(RightBackward, HIGH);
  digitalWrite(LeftBackward, HIGH);

  digitalWrite(RightForward, LOW);
  digitalWrite(LeftForward, LOW);
  delay(150);
}

void _stop_(){
  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftBackward, LOW);

  digitalWrite(RightForward, LOW);
  digitalWrite(LeftForward, LOW);
}

void exe_cmd(char dir){
  if(dir == 'L'){
    hard_left();
  }
  else if(dir == 'R'){
    hard_right();
  }
}

void stay_on_line(bool l3, l2, l1, r1, r2, r3){
  /*write from here*/
}


void loop(){
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  distance = pulseIn(echoPin, HIGH) * 0.034 / 2;

  if (distance < 5){
    _stop_();
    delay(2000);
    srv.write(180);
    delay(100);
    backward();
    delay(300);
  }

  /* 1 -> black, 0 -> white */

  ir_right1 = digitalRead(IR_Right1);
  ir_right2 = digitalRead(IR_Right2);
  ir_right3 = digitalRead(IR_Right3);
  ir_right4 = digitalRead(IR_Right4);

  ir_left1 = digitalRead(IR_Left1);
  ir_left2 = digitalRead(IR_Left2);
  ir_left3 = digitalRead(IR_Left3);
  ir_left4 = digitalRead(IR_Left4);
//  Serial.print();
  if(ir_right4 == 1 || ir_left4 == 1){
    if(sizeof(cmd) > cmd_index){
      exe_cmd(cmd[cmd_index]);
      cmd_index++;
    }
  }

  else{
    stay_on_line(ir_left3, ir_left2, ir_left1, ir_right1, ir_right2, ir_right3);
  }

}
