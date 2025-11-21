#include <Arduino.h>
#include <Servo.h>
#include <math.h>

Servo SHOULDER;
Servo ELBOW;

float px;
float py;
float hypotenuse;
float hypotenuse_angle;

float rad_to_deg = 180.0/PI;

float L_2 = 10.0;
float L_1 = 10.0;

float inner_angle;
float outer_angle;
float shoulder_motor_angle;
float elbow_motor_angle;

float elbow_angle_absolute;
float shoulder_angle_absolute;


bool clampangles() {
  // Deprecated: this version used global radian values and was called before
  // angles were computed for the current (px,py). Use clampangles_deg instead.
  return true;
}

// Check angles in degrees (call after computing angles for the current point)
bool clampangles_deg(float shoulder_deg, float elbow_deg) {
  if (shoulder_deg < 0.0 || shoulder_deg > 180.0) {
    Serial.print("ERROR: shoulder_angle out of range! ");
    Serial.println(shoulder_deg);
    return false;
  }
  if (elbow_deg < 0.0 || elbow_deg > 180.0) {
    Serial.print("ERROR: elbow_angle out of range! ");
    Serial.println(elbow_deg);
    return false;
  }
  return true;
}



void calculate_hypotenuse(){
  hypotenuse = sqrt(sq(px) + sq(py));
  // hypotenuse_angle is the polar angle to (px,py): use atan2(y,x)
  // (previous code used atan2(px,py) which swaps arguments and gives wrong quadrant)
  hypotenuse_angle = atan2(py, px);
  // Serial.println(hypotenuse);
  // Serial.println(hypotenuse_angle * rad_to_deg);
}


float calculate_elbow_motor_angle(){
  // elbow_thetha2 = PI - acos((sq(L_1) + sq(L_2) - sq(P)) / (2 * L_1 * L_2));
  outer_angle = acos( (sq(L_1) + sq(L_2) - sq(hypotenuse)) / (2 * L_1 * L_2));
  elbow_motor_angle = PI - outer_angle;
  return elbow_motor_angle * rad_to_deg;
}

float calculate_shoulder_motor_angle(){
  // shoulder_thetha1 = atan(py / px) - acos((sq(L_1) + sq(P) - sq(L_2)) / (2 * L_1 * P));
  inner_angle = acos( (sq(hypotenuse) + sq(L_1) - sq(L_2)) / (2 * hypotenuse * L_1));
  // Serial.println(inner_angle * rad_to_deg);
  shoulder_motor_angle = hypotenuse_angle - inner_angle;
  // Serial.println(shoulder_motor_angle * rad_to_deg);
  return shoulder_motor_angle * rad_to_deg;
}

void drawRectangle(float x1, float y1, float x2, float y2) {

  py = y1;
  px = x1;

  delay(1000);

  // Left edge
  for (py = y1; py <= y2; py += 0.1) {
    Serial.print(x1);
    Serial.print(" ");
    Serial.println(py);
    calculate_hypotenuse();
    // compute angles (functions return degrees)
    {
      float elbow_deg = calculate_elbow_motor_angle();
      float shoulder_deg = calculate_shoulder_motor_angle();
      if (clampangles_deg(shoulder_deg, elbow_deg)) {
        elbow_angle_absolute = abs(elbow_deg);
        shoulder_angle_absolute = abs(90.0 - shoulder_deg);
        ELBOW.write(elbow_angle_absolute);
        SHOULDER.write(shoulder_angle_absolute);
      }
    }
    

    delay(100);
  }

  // Top edge
  for (px = x1; px <= x2; px += 0.1) {
    Serial.print(px);
    Serial.print(" ");
    Serial.println(y2);
    calculate_hypotenuse();
    {
      float elbow_deg = calculate_elbow_motor_angle();
      float shoulder_deg = calculate_shoulder_motor_angle();
      if (clampangles_deg(shoulder_deg, elbow_deg)) {
        elbow_angle_absolute = abs(elbow_deg);
        shoulder_angle_absolute = abs(90.0 - shoulder_deg);
        ELBOW.write(elbow_angle_absolute);
        SHOULDER.write(shoulder_angle_absolute);
      }
    }
    
    delay(100);
  }
  
  // Right edge
  for (py = y2; py >= y1; py -= 0.1) {
    Serial.print(x2);
    Serial.print(" ");
    Serial.println(py);
    calculate_hypotenuse();
      {
        float elbow_deg = calculate_elbow_motor_angle();
        float shoulder_deg = calculate_shoulder_motor_angle();
        if (clampangles_deg(shoulder_deg, elbow_deg)) {
          elbow_angle_absolute = abs(elbow_deg);
          shoulder_angle_absolute = abs(90.0 - shoulder_deg);
          ELBOW.write(elbow_angle_absolute);
          SHOULDER.write(shoulder_angle_absolute);
        }
      }

    delay(100);
  }

  // Bottom edge
  for (px = x2; px >= x1; px -= 0.1) {
    Serial.print(px);
    Serial.print(" ");
    Serial.println(y1);
    calculate_hypotenuse();
      {
        float elbow_deg = calculate_elbow_motor_angle();
        float shoulder_deg = calculate_shoulder_motor_angle();
        if (clampangles_deg(shoulder_deg, elbow_deg)) {
          elbow_angle_absolute = abs(elbow_deg);
          shoulder_angle_absolute = abs(90.0 - shoulder_deg);
          ELBOW.write(elbow_angle_absolute);
          SHOULDER.write(shoulder_angle_absolute);
        }
      }
    
    delay(100);
  }
}

void draw_circle(float center_x, float center_y, float radius) {
  for (float angle = 0; angle <= PI; angle += 0.1) {
    px = center_x + radius * cos(angle);
    py = center_y + radius * sin(angle);
    delay(1000);
    Serial.print(px);
    Serial.print(" ");
    Serial.println(py);
    calculate_hypotenuse();
    {
      float elbow_deg = calculate_elbow_motor_angle();
      float shoulder_deg = calculate_shoulder_motor_angle();
      if (clampangles_deg(shoulder_deg, elbow_deg)) {
        elbow_angle_absolute = abs(elbow_deg);
        shoulder_angle_absolute = abs(90.0 - shoulder_deg);
        ELBOW.write(elbow_angle_absolute);
        SHOULDER.write(shoulder_angle_absolute);
      }
    }

    delay(100);
  }
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

    // drawRectangle(-5, 7, 2, 15);

    // draw_circle(0,0,10);

    calculate_hypotenuse();
    {
      float elbow_deg = calculate_elbow_motor_angle();
      float shoulder_deg = calculate_shoulder_motor_angle();
      if (clampangles_deg(shoulder_deg, elbow_deg)) {
        elbow_angle_absolute = abs(elbow_deg);
        shoulder_angle_absolute = abs(90.0 - shoulder_deg);
        ELBOW.write(elbow_angle_absolute);
        SHOULDER.write(shoulder_angle_absolute);
      }
    }
    Serial.print("Elbow angle = ");
    Serial.println(elbow_angle_absolute);

    Serial.print("Shoulder angle = ");
    Serial.println(shoulder_angle_absolute);



    
  }
}

