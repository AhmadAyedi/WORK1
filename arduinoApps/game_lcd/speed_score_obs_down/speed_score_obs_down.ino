#include <LiquidCrystal.h>
#include <EEPROM.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(11, 9, 6, 5, 4, 3);

#define PIN_BUTTON 2   // Push button pin
bool buttonPushed = false;
bool playing = false;
int playerPos = 1;      // 1 = on the ground, 0 = jumping
int score = 0;          // Player's score
int highScore;          // High score stored in EEPROM
int gameSpeed = 400;    // Initial game speed (starts slow)
unsigned long lastSpeedIncreaseTime = 0;  // Time tracking for speed increase
unsigned long speedIncreaseInterval = 3000;  // Increase speed every 3 seconds

#define NUM_OBSTACLES 3  // Number of obstacles
int obstaclePositions[NUM_OBSTACLES] = {15, 18, 20};  // Initial positions of obstacles

// Interrupt function for button press
void buttonPush() {
  buttonPushed = true;
}

void setup() {
  lcd.begin(16, 2);
  pinMode(PIN_BUTTON, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonPush, FALLING);

  EEPROM.get(0, highScore); // Load high score from EEPROM

  lcd.print("Press to Start!");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (!playing) {
    if (buttonPushed) {
      playing = true;
      buttonPushed = false;
      playerPos = 1;
      score = 0;  // Reset score
      gameSpeed = 400; // Reset speed
      lastSpeedIncreaseTime = millis();  // Reset the timer
      // Reset obstacle positions
      for (int i = 0; i < NUM_OBSTACLES; i++) {
        obstaclePositions[i] = 15 + i * 3; // Set initial positions for obstacles
      }
    } else {
      return;
    }
  }

  // Player jump logic
  if (buttonPushed) {
    playerPos = 0;  // Jump
    buttonPushed = false;
  } else {
    playerPos = 1;  // Stay on the ground
  }

  // Move obstacles
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    obstaclePositions[i]--;  // Move obstacle left

    // Reset obstacle if it moves off-screen
    if (obstaclePositions[i] < 0) {
      obstaclePositions[i] = 15 + i * 3;  // Reset to right side, with varied start positions
      score++;  // Increase score when an obstacle resets
    }
  }

  // Clear screen and redraw game
  lcd.clear();

  // Draw player
  lcd.setCursor(1, playerPos);
  lcd.print('o');

  // Draw obstacles
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    if (obstaclePositions[i] >= 0) {
      lcd.setCursor(obstaclePositions[i], 1);
      lcd.print('X');
    }
  }

  // Display score without space between ":" and the actual value
  lcd.setCursor(8, 0);  // Start from the 8th position on the first row
  lcd.print("Score:");
  lcd.print(score);  // Directly print the score next to the ":"

  // Increase difficulty every 3 seconds
  if (millis() - lastSpeedIncreaseTime >= speedIncreaseInterval) {
    if (gameSpeed > 100) { // Minimum speed limit
      gameSpeed -= 40;  // Remarkably speed up the game
    }
    lastSpeedIncreaseTime = millis();  // Reset the timer for the next speed increase
  }

  // Check collision (if player is on the ground and any obstacle is in the same position)
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    if (playerPos == 1 && obstaclePositions[i] == 1) {
      // Collision detected
      lcd.clear();
      lcd.print("Game Over!");
      lcd.setCursor(0, 1);
      lcd.print("Score:");
      lcd.print(score);

      // Update high score if new record
      if (score > highScore) {
        highScore = score;
        EEPROM.put(0, highScore);
        lcd.setCursor(10, 1);
        lcd.print("NEW!");
      }

      playing = false;
      delay(2000);
      lcd.clear();
      return;
    }
  }

  delay(gameSpeed);  // Adjust game speed dynamically
}
