#include <Servo.h>

// Define motor pins
const int motor1Pin1 = 2; // Motor 1 control pin 1
const int motor1Pin2 = 3; // Motor 1 control pin 2
const int motor2Pin1 = 4; // Motor 2 control pin 1
const int motor2Pin2 = 5; // Motor 2 control pin 2

// Define servo objects
Servo servo1; // Servo 1 for the robotic arm
Servo servo2; // Servo 2 for the robotic arm
Servo servo3; // Servo 3 for the robotic arm
Servo moistureServo; // Servo for the moisture sensor

// Servo angles
int servo1Angle = 90; // Initial angle for Servo 1
int servo2Angle = 90; // Initial angle for Servo 2
int servo3Angle = 90; // Initial angle for Servo 3
int moistureServoAngle = 90; // Initial angle for Moisture Sensor Servo

void setup() {
  // Set motor pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Attach servos to pins
  servo1.attach(6); // Servo 1 on pin 6
  servo2.attach(7); // Servo 2 on pin 7
  servo3.attach(8); // Servo 3 on pin 8
  moistureServo.attach(9); // Moisture Sensor Servo on pin 9

  // Initialize servos to default positions
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  servo3.write(servo3Angle);
  moistureServo.write(moistureServoAngle);

  // Start serial communication for Bluetooth
  Serial.begin(9600); // HC-05 default baud rate
}

void loop() {
  // Check for incoming Bluetooth data
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read the command
    processCommand(command); // Process the command
  }
}

void processCommand(String command) {
  // Rover movement commands
  if (command == "FORWARD") {
    moveForward();
  } else if (command == "BACKWARD") {
    moveBackward();
  } else if (command == "LEFT") {
    turnLeft();
  } else if (command == "RIGHT") {
    turnRight();
  } else if (command == "STOP") {
    stopMotors();
  }

  // Servo commands
  else if (command.startsWith("SERVO1:")) {
    servo1Angle = command.substring(7).toInt(); // Extract angle
    servo1.write(servo1Angle); // Move Servo 1
  } else if (command.startsWith("SERVO2:")) {
    servo2Angle = command.substring(7).toInt(); // Extract angle
    servo2.write(servo2Angle); // Move Servo 2
  } else if (command.startsWith("SERVO3:")) {
    servo3Angle = command.substring(7).toInt(); // Extract angle
    servo3.write(servo3Angle); // Move Servo 3
  } else if (command.startsWith("MOISTURE:")) {
    moistureServoAngle = command.substring(9).toInt(); // Extract angle
    moistureServo.write(moistureServoAngle); // Move Moisture Sensor Servo
  }
}

// Motor control functions
void moveForward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void moveBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void turnLeft() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void turnRight() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}