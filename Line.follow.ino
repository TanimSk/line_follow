uint8_t a;
uint8_t b;
uint8_t c;

void setup(){
  pinMode(8, INPUT);//  IR sensor
  pinMode(9, INPUT);
  pinMode(12, INPUT);// IR sensor

  pinMode(2, OUTPUT);//IN1
  pinMode(3, OUTPUT);//IN2
  pinMode(4, OUTPUT);//IN3
  pinMode(5, OUTPUT);//IN4
  pinMode(10, OUTPUT);//ENAB

  //initial state
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  
  analogWrite(10, 95);//pwm
}

void loop(){
   a = digitalRead(8);
   b = digitalRead(9);
   c = digitalRead(12);
   
  if(a == 0 && b == 0 && c == 0){
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW); 
    digitalWrite(2, LOW);  // left  
    delay(50);
  }
  
   else if(a == 0  && c == 0){
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH); // forward
    digitalWrite(2, LOW);
    digitalWrite(5, LOW);
  }
  
  else if(a == 0  && c == 1){
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);  // right
    digitalWrite(2, HIGH);
    delay(50);
    
  }
  
  else if(a == 1  && c == 0){
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH); //left
    digitalWrite(2, LOW);
    delay(50);
    
  }
   else if(a == 1  && c == 1){
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH); // forward
    digitalWrite(2, LOW);
    digitalWrite(5, LOW);
   }
  
  delay(1);
}
