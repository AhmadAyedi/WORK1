#include <LiquidCrystal.h>
#include <EEPROM.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(11, 9, 6, 5, 4, 3);

#define PIN_BUTTON 2   // Push button pin
bool buttonPushed = false;
bool playing = false;
int playerPos = 1;      // 1 = on the ground, 0 = jumping
int obstaclePos[3];     // Positions for multiple obstacles (3 obstacles)
int obstacleHeight[3];  // Heights for obstacles (random heights)
int score = 0;          // Player's score
int highScore;          // High score stored in EEPROM
int gameSpeed = 400;    // Initial game speed (starts slow)

String scoreText = "Score: ";  // Text that will be shown during the game
String scoreValue = "000";     // Actual score value displayed at the end of the game

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

      // Initialize obstacle positions
      for (int i = 0; i < 3; i++) {
        obstaclePos[i] = 15 + i * 5;  // Set different starting positions for each obstacle
        obstacleHeight[i] = random(0, 2);  // Randomize height (0 or 1) for obstacle
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

  // Move the obstacles
  for (int i = 0; i < 3; i++) {
    obstaclePos[i]--;
    
    // Reset obstacle if it moves off-screen
    if (obstaclePos[i] < 0) {
      obstaclePos[i] = 15 + (i * 5);  // Reset to starting positions
      obstacleHeight[i] = random(0, 2);  // Randomize height of obstacle
      score++;  // Increase score when an obstacle is passed
    }
  }

  // Clear screen and redraw game
  lcd.clear();

  // Draw player
  lcd.setCursor(1, playerPos);
  lcd.print('o');

  // Draw obstacles
  for (int i = 0; i < 3; i++) {
    if (obstaclePos[i] >= 0) {
      lcd.setCursor(obstaclePos[i], obstacleHeight[i]);
      lcd.print('X');
    }
  }

  // Display the word "Score"
  lcd.setCursor(10, 0);
  lcd.print(scoreText);

  // Increase difficulty by reducing delay (speeding up the game)
  if (score % 5 == 0 && gameSpeed > 150) {
    gameSpeed -= 20;  // Faster speed increase
  }

  // Check collision with obstacles
  for (int i = 0; i < 3; i++) {
    if (playerPos == obstacleHeight[i] && obstaclePos[i] == 1) {
      // Collision detected
      lcd.clear();
      lcd.print("Game Over!");
      lcd.setCursor(0, 1);
      lcd.print(scoreText);
      
      // Format and display score
      scoreValue = String(score);
      if (scoreValue.length() < 3) {
        scoreValue = "0" + scoreValue;  // Ensure 3 digits
      }
      lcd.print(scoreValue);

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
  }

  delay(gameSpeed);  // Adjust game speed dynamically
}
