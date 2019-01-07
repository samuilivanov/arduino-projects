#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte gameBoard[16][80] = {};
int ballUpdateTime = 100;
byte ballXDirection = 0;
byte ballYDirection = -1;
byte ballX = 7;
byte ballY = 37;
byte ballCharArray[8] = {};
byte playerScore = 0;
byte aiScore = 0;
byte charNum = 0;

byte aiPaddle[2][8] = {0};
byte playerPaddle[2][8] = {0};

byte aiColPaddle[16] = {0};
byte playerColPaddle[16] = {0};

const int moveRightButton = 8;
const int moveLeftButton = 7;

int rightButtonState = 0;
int leftButtonState = 0;

int rightLastButtonState = 0;
int leftLastButtonState = 0;

byte playerPaddlePos = 6;
// TODO :: change ai start pos
byte aiPaddlePos = 6;

// Prototypes
void ClearPaddles();
void PrintPaddles();
void SetupPaddles();
void MovePaddleUp();
void MovePaddleDown();
void UpdatePaddle();
void AwardAPoint();
void PrintStuff();
void Update();
void UpdateBall();
void SetupBall();
void GenerateBallArray();

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(moveRightButton, INPUT);
  pinMode(moveLeftButton, INPUT);
  SetupPaddles();
  SetupBall();
}

void loop()
{
  rightButtonState = digitalRead(moveRightButton);
  leftButtonState = digitalRead(moveLeftButton);
  
  if (rightButtonState == HIGH) {
    MovePaddleUp();
    rightButtonState = 0;
  }
  
  if (leftButtonState == HIGH) {
    MovePaddleDown();
    leftButtonState = 0;
  }
  Update();
  PrintStuff();
  delay(50);
}

void ClearPaddles()
{
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 8; ++j) {
      aiPaddle[i][j] = 0;
      playerPaddle[i][j] = 0;
    }
  }
}

void PrintPaddles()
{
  lcd.createChar(0, playerPaddle[0]);
  lcd.createChar(1, playerPaddle[1]);
  
  lcd.createChar(14, aiPaddle[0]);
  lcd.createChar(15, aiPaddle[1]);
  
  lcd.setCursor(14, 0);
  lcd.write(byte(0));
  
  lcd.setCursor(14, 1);
  lcd.write(byte(1));
  
  lcd.setCursor(1, 0);
  lcd.write(byte(14));
  
  lcd.setCursor(1, 1);
  lcd.write(byte(15));
}

void SetupPaddles()
{
  ClearPaddles();
  
  aiPaddle[0][5] = 1;
  aiPaddle[0][6] = 1;
  aiPaddle[0][7] = 1;
  
  playerPaddle[0][5] = 16;
  playerPaddle[0][6] = 16;
  playerPaddle[0][7] = 16;
  
  PrintPaddles();
  
}

void MovePaddleUp()
{
  if (playerPaddlePos != 1) {
    playerPaddlePos--;
  }
}

void MovePaddleDown()
{
  if (playerPaddlePos != 14) {
    playerPaddlePos++;
  }
}

void UpdatePaddle()
{
  for (int i = 0 ; i < 16; ++i) {
    if (i == (playerPaddlePos - 1) || i == playerPaddlePos || i == (playerPaddlePos + 1)) {
      playerColPaddle[i] = 16;
    } else {
      playerColPaddle[i] = 0;
    }
  }
  for (int i = 0; i < 8; ++i) {
    playerPaddle[0][i] = playerColPaddle[i];
  }
  for (int i = 8; i < 16; ++i) {
    playerPaddle[1][i - 8] = playerColPaddle[i];
  }
}

int GetLEDRowValue(byte ledRow, byte maxColumn)
{
  int minColumn = maxColumn - 4;
  int ledValue = 0;
  int multiplier = 1;
  for (int i = maxColumn; i >= minColumn; --i) {
    ledValue += (gameBoard[ledRow][i] * multiplier);
	multiplier *= 2;
  }
  return ledValue;
}

void GenerateBallArray()
{
  byte maxColumn = ((ballY / 5) * 5) + 4;
  byte minColumn = maxColumn - 4;
  byte startRow = (ballX <= 7) ? 0 : 8;
  byte endRow = startRow + 8;
  if (startRow == 0) {
    for (int i = startRow; i < endRow; ++i) {
      ballCharArray[i] = GetLEDRowValue(i, maxColumn);
    }
  } else {
    for (int i = startRow; i < endRow; ++i) {
      ballCharArray[i - 8] = GetLEDRowValue(i, maxColumn);
    }
  }
}

void PrintBall()
{
  byte LCDColumn = ballY / 5;
  byte LCDRow = (ballX <= 7) ? 0 : 1;
  charNum = ballY / 5;
  lcd.createChar(charNum, ballCharArray);
  lcd.setCursor(LCDColumn, LCDRow);
  lcd.write(byte(charNum));
}

void UpdateBall()
{
  delay(ballUpdateTime);
  if ((ballY <= 8) || (ballY >= 71)) {
    AwardAPoint();
  } else if ((ballX == 0) ||(ballX == 15)){
    ballXDirection *= -1;
  } else if ((ballY == 69) && (ballX == playerPaddlePos)) {
    ballYDirection *= -1;
  } else if ((ballY == 69) && (ballX == (playerPaddlePos + 1))) {
    ballYDirection *= -1;
    ballXDirection = 1;
  } else if ((ballY == 69) && (ballX == (playerPaddlePos - 1))) {
    ballYDirection *= -1;
    ballXDirection = -1;
  }
  gameBoard[ballX][ballY] = false;
  ballX += ballXDirection;
  ballY += ballYDirection;
  gameBoard[ballX][ballY] = true;
  GenerateBallArray();
}

void SetupBall()
{
  ballYDirection *= -1;
  gameBoard[7][35] = true;
}

void AwardAPoint()
{
  if (ballY <= 8) {
    playerScore++;
  } else {
    aiScore++;
  }
  delay(ballUpdateTime);
  ballYDirection *= -1;
}

void PrintStuff()
{
  lcd.clear();
  PrintPaddles();
  if ((ballY > 9) && (ballY < 70)) {
    PrintBall();
  }
}

void Update()
{
  UpdatePaddle();
  UpdateBall();
}