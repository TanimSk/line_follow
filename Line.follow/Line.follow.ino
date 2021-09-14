#include <Servo.h>

#define RightForward 13
#define RightBackward 12

#define LeftForward 9
#define LeftBackward 8

#define IR_Right3 7
#define IR_Right2 A0
#define IR_Right1 A1

#define IR_Left1 A2
#define IR_Left2 A3
#define IR_Left3 6

/*Servo config*/
#define ServoPin 3
Servo srv; /*PWM pin --> orange*/

/*ultrasonic sensor*/
#define pingPin 4
#define echoPin 5

#define SPEED 100  /*0-255 range*/
#define SPEED_PIN 11
//#define SPEED_PIN_L 11

bool ir_right3,ir_left3;
float ir_right1, ir_right2, ir_left1, ir_left2;
unsigned long startMillis;
unsigned long currentMillis;
bool picked_obj = false;

int cmd_index = 0;
char cmd[] = {'L', 'F', 'F', 'L', 'D', 'G', 'F', 'R', 'F', 'L', 'G'};
/** L - left, R - right
 S - stop, F - forward
G - 180 degree rotation
D - drop obj release()
**detect white surface!!**

*/

float distance = 0.0;

void setup(){
//  Serial.begin(9600);
  pinMode(RightForward, OUTPUT);
  pinMode(RightBackward, OUTPUT);
  pinMode(LeftForward, OUTPUT);
  pinMode(LeftBackward, OUTPUT);
  pinMode(SPEED_PIN, OUTPUT);
//  pinMode(SPEED_PIN_L, OUTPUT);

  pinMode(IR_Right3, INPUT);
  pinMode(IR_Right2, INPUT);
  pinMode(IR_Right1, INPUT);
  pinMode(IR_Left1, INPUT);
  pinMode(IR_Left2, INPUT);
  pinMode(IR_Left3, INPUT);

  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);

  srv.attach(ServoPin);
  srv.write(90);

  delay(1000);
  analogWrite(SPEED_PIN, SPEED);
//  analogWrite(SPEED_PIN_L, SPEED);
  
}

void left(){
  digitalWrite(RightForward, HIGH);
  digitalWrite(LeftBackward, LOW);

  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, LOW);
  //  delay(10);**
}

void hard_left(){
  digitalWrite(RightForward, HIGH);
  digitalWrite(LeftBackward, HIGH);

  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, LOW);
  delay(100);
}

void right(){
  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftForward, HIGH);

  digitalWrite(RightForward, LOW);
  digitalWrite(LeftBackward, LOW);
//  delay(10);**
}

void hard_right(){
  digitalWrite(RightBackward, HIGH);
  digitalWrite(LeftForward, HIGH);

  digitalWrite(RightForward, LOW);
  digitalWrite(LeftBackward, LOW);
  delay(100);
}

void forward(){
  digitalWrite(RightForward, HIGH);
  digitalWrite(LeftForward, HIGH);

  digitalWrite(RightBackward, LOW);
  digitalWrite(LeftBackward, LOW);
//  delay(20);
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

void pick_up(){
    _stop_();
    delay(100);
    srv.write(0);
    analogWrite(SPEED_PIN, 102);
    delay(700);
    forward();
}

void release(){
    analogWrite(SPEED_PIN, SPEED);
    _stop_();
    delay(200);
    srv.write(90);
    delay(400);
    picked_obj = false;
}

void run_until(void (*func)(), int milli_sec){
  startMillis = millis();
  while(true){
    func();
    currentMillis = millis() - startMillis;
    if(currentMillis >= milli_sec){
      break;
    }
  }
}


void stay_on_line(){
  ir_right1 = analogRead(IR_Right1);
  ir_right2 = analogRead(IR_Right2);
  ir_left1 = analogRead(IR_Left1);
  ir_left2 = analogRead(IR_Left2);

  if (ir_right1 > 180 && ir_left1 > 180){
    forward();
  }
  else if(ir_right1 > 170){
    right();
  }
  else if(ir_left1 > 170){
    left();
  }
  else if(ir_right2 > 170){
    right();
  }
  else if(ir_left2 > 170){
    left();
  }
//  else{
//    forward();
//  }
}


void exe_cmd(char dir){
  if(dir == 'L'){
    left();
    delay(390);
  }
  else if(dir == 'R'){
    right();
    delay(390);
  }
  else if(dir == 'S'){
    _stop_();
    delay(5000);
  }
  else if(dir == 'F'){
    forward();
  }
  else if(dir == 'D'){
    run_until(&stay_on_line, 920);
    hard_left();
    delay(450);
    release();
    hard_right();
    delay(420);
    _stop_();
    delay(200);
  }
  else if(dir == 'G'){
    hard_right();
    delay(400);
    forward(); 
  }
  delay(350);
}

void loop(){

  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  distance = pulseIn(echoPin, HIGH) * 0.034 / 2;

  if (!picked_obj && distance < 4 && distance != 0){
    pick_up();
    picked_obj = true;
  }

  /* 1 -> black, 0 -> white */
  ir_right3 = digitalRead(IR_Right3);
  ir_left3 = digitalRead(IR_Left3);

  /*intersection detected*/
  if(ir_right3 == 1 || ir_left3 == 1){
    if(sizeof(cmd) > cmd_index){
      exe_cmd(cmd[cmd_index]);
      cmd_index++;
    }
    else{
      _stop_();
      exit(0);
    }
  }
  else{
    stay_on_line();
  }
}
