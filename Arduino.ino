#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27, 16 columns, 2 rows

#define NUM_SERVOS 4
#define DELAY_MS 20
const int MIN_ANGLE = 1;

Servo servos[NUM_SERVOS];
int servoPins[NUM_SERVOS] = {3, 5, 6, 9}; // Base, Shoulder, Elbow, Gripper
int currentAngles[NUM_SERVOS] = {90, 90, 90, 70}; // Initial positions

// Ultrasonic sensor
const int trigPin = 12;
const int echoPin = 13;
const int detectionThreshold = 6; // in cm

void setup() {
  Serial.begin(9600);
  Serial.println("Enter servo number (1-4) and angle (1-180)");

  // LCD Initialization
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Robot Arm Ready");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(currentAngles[i]);
  }
}

void moveServoSmoothly(int servoIndex, int targetAngle) {
  if (servoIndex < 0 || servoIndex >= NUM_SERVOS || targetAngle < MIN_ANGLE || targetAngle > 180) return;

  int currentAngle = currentAngles[servoIndex];
  int step = (targetAngle > currentAngle) ? 1 : -1;

  for (int angle = currentAngle; angle != targetAngle; angle += step) {
    servos[servoIndex].write(angle);
    delay(DELAY_MS);
  }

  servos[servoIndex].write(targetAngle);
  currentAngles[servoIndex] = targetAngle;
}

long readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

void autoMoveToTargetPose() {
  Serial.println("Object detected! Moving to pick-up position...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Auto Pick Object");

  moveServoSmoothly(0, 80);   // Base
  moveServoSmoothly(1, 138);  // Shoulder
  moveServoSmoothly(3, 60);   // Gripper
  moveServoSmoothly(2, 70);   // Elbow
  delay(500);
  moveServoSmoothly(3, 40);   // Close gripper
  delay(500);
  moveServoSmoothly(1, 90);   // Back to home
  moveServoSmoothly(2, 90);
  moveServoSmoothly(3, 38);
}

void loop() {
  long distance = readDistanceCM();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance <= detectionThreshold) {
    autoMoveToTargetPose();
    delay(1000);
  }

  if (Serial.available()) {
    char incoming = Serial.peek();

    // Color Detection Input
    if (incoming == 'R' || incoming == 'G' || incoming == 'B') {
      char color = Serial.read();
      lcd.clear();
      lcd.setCursor(0, 0);

      if (color == 'R') {
        Serial.println("Red detected!");
        lcd.print("Color: RED");
        moveServoSmoothly(0, 150);
        moveServoSmoothly(1, 125);
        moveServoSmoothly(2, 70);
        moveServoSmoothly(3, 70);
        delay(1000);
        moveServoSmoothly(1, 90);
        moveServoSmoothly(2, 90);
        moveServoSmoothly(3, 70);
        moveServoSmoothly(0, 90);
      } else if (color == 'G') {
        Serial.println("Green detected!");
        lcd.print("Color: GREEN");
        moveServoSmoothly(0, 115);
        moveServoSmoothly(2, 110);
        moveServoSmoothly(1, 140);
        moveServoSmoothly(3, 60);
           delay(1000);
        moveServoSmoothly(1, 90);
        moveServoSmoothly(2, 90);
        moveServoSmoothly(3, 70);
        moveServoSmoothly(0, 90);
      } else if (color == 'B') {
        Serial.println("Blue detected!");
        lcd.print("Color: BLUE");
        moveServoSmoothly(0, 180);
        moveServoSmoothly(1, 125);
        moveServoSmoothly(2, 70);
        moveServoSmoothly(3, 60);
           delay(1000);
        moveServoSmoothly(1, 90);
        moveServoSmoothly(2, 90);
        moveServoSmoothly(3, 70);
        moveServoSmoothly(0, 90);
      }
      else if(color == 'U') {
        Serial.println("Unknown color");
        lcd.print("Color: Unknown color");
        moveServoSmoothly(0, 90);
        moveServoSmoothly(1, 90);
        moveServoSmoothly(2, 90);
        moveServoSmoothly(3, 60);
      }
    }
    // Manual Servo Control
    else {
      int servoNumber = Serial.parseInt();
      int targetAngle = Serial.parseInt();

      if (servoNumber >= 1 && servoNumber <= NUM_SERVOS &&
          targetAngle >= MIN_ANGLE && targetAngle <= 180) {
        Serial.print("Moving Servo ");
        Serial.print(servoNumber);
        Serial.print(" to ");
        Serial.print(targetAngle);
        Serial.println(" degrees");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Servo ");
        lcd.print(servoNumber);
        lcd.print(": ");
        lcd.print(targetAngle);
        moveServoSmoothly(servoNumber - 1, targetAngle);
        Serial.println("Servo movement complete.");
      } else {
        Serial.println("Invalid input! Enter servo number (1-4) and angle (1-180).");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid Input!");
      }
    }
  }

  delay(200);
}
