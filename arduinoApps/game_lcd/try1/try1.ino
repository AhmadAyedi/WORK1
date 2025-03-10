#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(11, 9, 6, 5, 4, 3);

#define PIN_BUTTON 2   // Push button pin
bool buttonPushed = false;
bool playing = false;
int playerPos = 1;  // 1 = on the ground, 0 = jumping
int obstaclePos = 15;  // Initial obstacle position

// Interrupt function for button press
void buttonPush() {
  buttonPushed = true;
}

void setup() {
  lcd.begin(16, 2);
  pinMode(PIN_BUTTON, INPUT_PULLUP);  // Enable internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), buttonPush, FALLING);
  
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
    obstaclePos = 15;
  }

  // Clear screen and redraw game
  lcd.clear();

  // Draw player
  lcd.setCursor(1, playerPos);
  lcd.print('o');

  // Draw obstacle
  if (obstaclePos >= 0) {
    lcd.setCursor(obstaclePos, 1);
    lcd.print('X');
  }

  // Check collision
  if (playerPos == 1 && obstaclePos == 1) {
    lcd.clear();
    lcd.print("Game Over!");
    playing = false;
    delay(2000);
    lcd.clear();
  }

  delay(300);  // Game speed
}
