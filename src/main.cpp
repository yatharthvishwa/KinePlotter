#include <Arduino.h>
#include <Servo.h>

Servo SHOULDER;
Servo ELBOW;

float shoulder_thetha1;
float elbow_thetha2;

float px;
float py;
float P;

float rad_to_deg = 180.0/PI;

float L_1 = 10.0;
float L_2 = 10.0;

void calculate_P(){
  P = sqrt(sq(px) + sq(py));
  Serial.print("P=");
  Serial.println(P);
}

void calculate_thetha2(){
  elbow_thetha2 = PI - acos((sq(L_1) + sq(L_2) - sq(P)) / (2 * L_1 * L_2));
  Serial.print("ELBOW=");
  Serial.println(elbow_thetha2 * rad_to_deg);
}

void calculate_thetha1(){
  shoulder_thetha1 = atan(py / px) - acos((sq(L_1) + sq(P) - sq(L_2)) / (2 * L_1 * P));
  Serial.print("SHOULDER=");
  Serial.println(elbow_thetha2 * rad_to_deg);
}



void setup() {
  Serial.begin(9600);
  SHOULDER.attach(3);
  ELBOW.attach(9);
}


void loop() {
  if(Serial.available()>0) {

    px = Serial.parseFloat();
    Serial.print("px =");
    Serial.println(px);

    py = Serial.parseFloat();
    Serial.print("py =");
    Serial.println(py);

    while (Serial.available() > 0) {
      Serial.read(); // - Buffer contains: `['\n']` , `while` loop reads and discards `'\n'`
                      
    }

    calculate_P();

    calculate_thetha2();
    calculate_thetha1();

    SHOULDER.write(shoulder_thetha1 * rad_to_deg);
    ELBOW.write(elbow_thetha2 * rad_to_deg);


  }
}

