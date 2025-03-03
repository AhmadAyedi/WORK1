#include <LiquidCrystal.h>
#include <Stepper.h>

// Define the number of steps per revolution for your stepper motor (depends on your motor)
const int stepsPerRevolution = 2048;  // Full rotation for your motor

// Initialize the stepper motor. Pin numbers are chosen for the IN1, IN2, IN3, IN4 pins
Stepper stepperMotor(stepsPerRevolution, 8, 10, 9, 11);  // Pins connected to the driver

// Define the joystick and potentiometer pins
const int vrxPin = A0;  // Joystick X-axis connected to A0 (for controlling motor speed)
const int potPin = A1;  // Potentiometer connected to A1 (for controlling LCD brightness)

// Initialize the LCD (adjust pin numbers if necessary)
LiquidCrystal lcd(12, 6, 5, 4, 3, 2);

void setup() {
  // Start the LCD
  lcd.begin(16, 2);  // Initialize the LCD with 16 columns and 2 rows
  lcd.print("Motor Speed:");  // Display a default message on the LCD
  stepperMotor.setSpeed(10);  // Set initial speed for the motor
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

  // Read the potentiometer value (0-1023) to control LCD brightness
  int potValue = analogRead(potPin);

  // Map the potentiometer value (0-1023) to a suitable contrast range for the LCD
  int contrast = map(potValue, 0, 1023, 0, 255);

  // Adjust the LCD contrast using the potentiometer value
  analogWrite(6, contrast);  // Pin 6 is connected to the LCD's enable pin

  // Display the motor speed on the LCD
  lcd.setCursor(0, 1);  // Set the cursor to the second line
  lcd.print("Speed: ");
  lcd.print(speed);  // Display the speed value

  // Small delay for smooth updates
  delay(100);  
}
