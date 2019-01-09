#include <LiquidCrystal.h>
#include <math.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// My random number holder
byte randNumber;


// Game board boundries later will change it to take the whole LCD
const int maxRow = 16;
const int maxCol = 20;

// This will be 0 and 1
bool gameBoard[maxRow][maxCol] = {0};
byte golArray[8][8];

// Prototypes
void GenerateRandomBoard();
void GenerateGoLArray();
void PrintToLCD();
int GetLCDRowValue(int i, int j, int mult);
int ConvertToDecimal(int row, int col);
void UpdateBoard();
void CopyArray(bool gameBoard2[maxRow][maxCol]);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  randomSeed(analogRead(0));
  GenerateRandomBoard();
  GenerateGoLArray();
  PrintToLCD();
}

void loop()
{
  UpdateBoard();
  GenerateGoLArray();
  PrintToLCD();
}

void GenerateRandomBoard()
{
  for (int i = 0; i < maxRow; ++i) {
    for (int j = 0; j < maxCol; ++j) {
      randNumber = random(0, 2);
      gameBoard[i][j] = randNumber;
      
      //Serial.println(j);
    }
  }
}

void GenerateGoLArray()
{
  int mult = 0;
  int step = 0;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 8; ++j) {
      golArray[i][j] = GetLCDRowValue(step, j, mult);
      step += 5;
      //Serial.println(golArray[i][j]);
    }
  }
  mult = 8;
  for (int i = 4; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      golArray[i][j] = GetLCDRowValue(i, j, mult);
      //Serial.println(golArray[i][j]);
    }
  }
}

int GetLCDRowValue(int i, int j, int mult)
{
  
  for (int row = j + mult; row < 16; ++row) {
      return ConvertToDecimal(row, i);
  }
}

void PrintToLCD()
{
  for (int i = 0; i < 4; ++i) {
    lcd.createChar(i, golArray[i]);
    lcd.setCursor(i, 0);
    lcd.write(byte(i));
  }
 
  for (int i = 4; i < 8; ++i) {
    lcd.createChar(i, golArray[i]);
    lcd.setCursor(i - 4, 1);
    lcd.write(byte(i));
  }
}

int ConvertToDecimal(int row, int col)
{
  int result = 0;
  int j = 4;
  for (int i = 0; i < 5; ++i) {
    result += gameBoard[row][i + col] * pow(2, j--);
  }
  return result;
}

void CopyArray(bool gameBoard2[maxRow][maxCol]){
    for(int a =0; a < maxRow; a++)
    {
        for(int b = 0; b < maxCol; b++)
        {
          gameBoard2[a][b] = gameBoard[a][b];
        }
    }
}

void UpdateBoard(){
  bool gameBoard2[maxRow][maxCol] = {};
  CopyArray(gameBoard2);
  
  for(int a = 0; a < maxRow; a++) {
    for(int b = 0; b < maxCol; b++) {
      int numOfTrues = 0;
      for(int c = -1; c < 2; c++) {
        for(int d = -1; d < 2; d++) {
          if(!(c == 0 && d == 0)) {
            if(gameBoard2[a+c][b+d]) {
              ++numOfTrues;
            }
          }
        }
      }
      if(numOfTrues < 2){gameBoard[a][b] = false;}
      else if(numOfTrues == 3){gameBoard[a][b] = true;}
      else if(numOfTrues > 3){gameBoard[a][b] = false;}
    }
  }
}