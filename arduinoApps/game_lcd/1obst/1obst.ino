#include <LiquidCrystal.h>
#include <EEPROM.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(11, 9, 6, 5, 4, 3);

#define PIN_BUTTON 2   // Push button pin
bool buttonPushed = false;
bool playing = false;
int playerPos = 1;      // 1 = on the ground, 0 = jumping
int obstaclePos = 15;   // Initial obstacle position (right side)
int score = 0;          // Player's score
int highScore;          // High score stored in EEPROM
int gameSpeed = 400;    // Initial game speed (starts slow)

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
      obstaclePos = 15;
      score = 0;  // Reset score
      gameSpeed = 400; // Reset speed
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

  // Move the obstacle
  obstaclePos--;

  // Reset obstacle if it moves off-screen
  if (obstaclePos < 0) {
    obstaclePos = 15;  // Reset to right side
    score++;  // Increase score when an obstacle resets
  }

  // Clear screen and redraw game
  lcd.clear();

  // Draw player
  lcd.setCursor(1, playerPos);
  lcd.print('o');

  // Draw obstacle (obstacle is always at ground level)
  if (obstaclePos >= 0) {
    lcd.setCursor(obstaclePos, 1);
    lcd.print('X');
  }

  // Display the word "Score"
  lcd.setCursor(10, 0);
  lcd.print("Score:");

  // Increase difficulty by reducing delay (speeding up the game)
  if (score % 5 == 0 && gameSpeed > 100) {
    gameSpeed -= 20;  // Increase difficulty by speeding up the game
  }

  // Check collision (if player is on the ground and obstacle is in the same position)
  if (playerPos == 1 && obstaclePos == 1) {
    // Collision detected
    lcd.clear();
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
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
  }

  delay(gameSpeed);  // Adjust game speed dynamically
}
