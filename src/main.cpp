#include <Arduino.h>
#include <Servo.h>

Servo SHOULDER;
Servo ELBOW;

float shoulder_thetha1;
float elbow_thetha2;

float px;
float py;
float hypotenuse;
float hypotenuse_angle;

float rad_to_deg = 180.0/PI;

float L_1 = 10.0;
float L_2 = 10.0;

float inner_angle;
float outer_angle;
float shoulder_motor_angle;
float elbow_motor_angle;


void calculate_hypotenuse(){
  hypotenuse = sqrt(sq(px) + sq(py));
  hypotenuse_angle = asin(px / hypotenuse);
  Serial.print("P=");
  Serial.println(hypotenuse);
}

void calculate_elbow_motor_angle(){
  // elbow_thetha2 = PI - acos((sq(L_1) + sq(L_2) - sq(P)) / (2 * L_1 * L_2));
  outer_angle = acos( sq(L_1) + sq(L_2) - sq(hypotenuse) ) / (2 * L_1 * L_2);
  elbow_motor_angle = PI - outer_angle;
}

void calculate_shoulder_motor_angle(){
  // shoulder_thetha1 = atan(py / px) - acos((sq(L_1) + sq(P) - sq(L_2)) / (2 * L_1 * P));
  inner_angle = acos( sq(hypotenuse) + sq(L_1) - sq(L_2) ) / (2 * hypotenuse * L_1);
  shoulder_motor_angle = hypotenuse_angle - inner_angle
}

void setup() {
  Serial.begin(9600);
  SHOULDER.attach(3);
  ELBOW.attach(9);
}


void loop() {
  if(Serial.available()>0) {

    px = Serial.parseFloat(); //retrieves the first valid floating point number from the Serial buffer
    Serial.print("px =");
    Serial.println(px);

    py = Serial.parseFloat();
    Serial.print("py =");
    Serial.println(py);

    while (Serial.available() > 0) {
      Serial.read(); // - Buffer contains: `['\n']` , `while` loop reads and discards `'\n'`
                      
    }

    calculate_hypotenuse();
    calculate_elbow_motor_angle();
    calculate_shoulder_motor_angle();

    SHOULDER.write(shoulder_motor_angle * rad_to_deg);
    ELBOW.write(elbow_motor_angle * rad_to_deg);
  }
}

