#include <Stepper.h>

const int stepsPerRevolution = 2048; // Change based on your motor specs
Stepper stepperMotor(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  stepperMotor.setSpeed(16); // Adjust speed (RPM)
}

void loop() {
  stepperMotor.step(stepsPerRevolution);  // Rotate 360° clockwise
  // delay(1000);
  
  // stepperMotor.step(-stepsPerRevolution); // Rotate 360° counterclockwise
  // delay(1000);
}
