#include <Stepper.h>

// Define the number of steps per revolution for your stepper motor (depends on your motor)
const int stepsPerRevolution = 2048;  // Full rotation for your motor

// Initialize the stepper motor. Pin numbers are chosen for the IN1, IN2, IN3, IN4 pins
Stepper stepperMotor(stepsPerRevolution, 8, 10, 9, 11);  // Pins connected to the driver

// Define the joystick pins
const int vrxPin = A0;  // Joystick X-axis connected to A0 (for controlling motor speed)

void setup() {
  // Initialize the stepper motor speed to a default value
  stepperMotor.setSpeed(15);  // Start with a speed of 10 RPM
}

void loop() {
  // Read the joystick X-axis (VRX) value (0-1023) to control motor speed
  int joystickX = analogRead(vrxPin);

  // Map the joystick X-axis value to motor speed (e.g., 1 to 100 RPM)
  int speed = map(joystickX, 0, 1023, 1, 100);

  // Set the stepper motor speed to the mapped value
  stepperMotor.setSpeed(speed);

  // Move the stepper motor slightly (in small steps for smooth control)
  stepperMotor.step(stepsPerRevolution / 100);  // Step a small increment

  // Small delay for smooth updates
  delay(100);  
}
