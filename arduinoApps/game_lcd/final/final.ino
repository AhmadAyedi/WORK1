#include <LiquidCrystal.h>
#include <EEPROM.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(11, 9, 6, 5, 4, 3);

#define PIN_BUTTON 2          // Main game start button
#define PIN_RESET_BUTTON 7     // Button to reset the high score

bool buttonPushed = false;
bool playing = false;
int playerPos = 1;            // 1 = on the ground, 0 = jumping
int score = 0;                // Player's score
int highScore = 0;            // High score stored in EEPROM
int gameSpeed = 400;          // Initial game speed (starts slow)
unsigned long lastSpeedIncreaseTime = 0;  // Time tracking for speed increase
unsigned long speedIncreaseInterval = 4000;  // Increase speed every 4 seconds

#define NUM_OBSTACLES 3       // Number of obstacles
int obstaclePositions[NUM_OBSTACLES] = {15, 18, 20};  // Initial positions of obstacles

// Interrupt function for main game button press
void buttonPush() {
  buttonPushed = true;
}

void setup() {
  lcd.begin(16, 2);
  pinMode(PIN_BUTTON, INPUT_PULLUP);      // Enable internal pull-up resistor for game button
  pinMode(PIN_RESET_BUTTON, INPUT_PULLUP); // Enable internal pull-up resistor for reset button

  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonPush, FALLING);  // Detect button press to start the game

  EEPROM.get(0, highScore); // Load high score from EEPROM

  lcd.print("Press to Start!");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Check reset button state (polling instead of interrupt)
  if (digitalRead(PIN_RESET_BUTTON) == LOW) {  // Button is pressed (LOW because of INPUT_PULLUP)
    delay(50);  // Simple debounce delay
    if (digitalRead(PIN_RESET_BUTTON) == LOW) {  // Confirm button is still pressed
      resetHighScore();
      delay(500);  // Prevent multiple resets due to button bounce
    }
  }

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

  // Display score
  lcd.setCursor(8, 0);  // Start from the 8th position on the first row
  lcd.print("Score:");
  lcd.print(score);  // Directly print the score next to the ":"

  // Display high score (just the value, no label) starting at the 5th position
  lcd.setCursor(5, 0);  // Starting at position 5 on the first row (5th position)
  lcd.print(highScore);  // Display the high score value

  // Increase difficulty every 4 seconds
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
        EEPROM.put(0, highScore); // Save new high score in EEPROM
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

// Function to reset the high score
void resetHighScore() {
  // Reset high score to 0
  highScore = 0;
  EEPROM.put(0, highScore);  // Store the new high score (0) in EEPROM

  // Clear the LCD and show reset message
  lcd.clear();
  lcd.print("High Score Reset!");
  delay(2000);  // Wait for a moment before restarting the game

  // Immediately clear the message and start the game screen again
  lcd.clear();
  lcd.print("Press to Start!");
  delay(2000);  // Wait for 2 seconds before proceeding to game screen
}