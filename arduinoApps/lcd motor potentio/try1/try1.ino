#include <Stepper.h>
#include <LiquidCrystal.h>

// Define the number of steps per revolution for your stepper motor (depends on your motor)
const int stepsPerRevolution = 2048;  // Full rotation for your motor

// Initialize the stepper motor. Pin numbers are chosen for the IN1, IN2, IN3, IN4 pins
Stepper stepperMotor(stepsPerRevolution, 8, 10, 9, 11);  // Pins connected to the driver

// Define the LCD pins (using 3k resistor for contrast)
const int rs = 12, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define the potentiometer pin for reading speed
const int potPin = A0;

void setup() {
  // Initialize the LCD (16 columns and 2 rows)
  lcd.begin(16, 2);  
  lcd.print("Motor Speed: ");  // Initial text

  // Set the stepper motor speed to an initial value
  stepperMotor.setSpeed(10);  // Start with a speed of 10 RPM
}

void loop() {
  // Read the potentiometer value (0-1023)
  int potValue = analogRead(potPin);

  // Map the potentiometer value to a motor speed range (e.g., 1 to 100 RPM)
  int speed = map(potValue, 0, 1023, 1, 100); 

  // Set the stepper motor speed to the mapped value
  stepperMotor.setSpeed(speed);

  // Move the stepper motor slightly (in small steps for smooth control)
  stepperMotor.step(stepsPerRevolution / 100);  // Step a small increment

  // Display the motor speed on the LCD
  lcd.setCursor(0, 1);  // Set the cursor to the second row of the LCD
  lcd.print("Speed: ");
  lcd.print(speed);      // Display the current speed
  lcd.print(" RPM  ");   // Add extra spaces to overwrite old values

  // Add a small delay to stabilize the motor and LCD
  delay(100);  // Small delay for smoother updates
}
