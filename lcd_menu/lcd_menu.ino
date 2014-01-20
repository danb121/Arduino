#include <LiquidCrystal.h>

#define BUTTON_DOWN_RIGHT A2
#define BUTTON_UP_LEFT A3

#define NONE 0
#define LEFT 1
#define RIGHT 2
#define UP 4
#define DOWN 8
#define ALL 16

int buttonPressed = 0;
int lastButton = 0;
int currentmenu = 1;
int submenu = 0;
int x = 0;
int y = 0;
int button;

LiquidCrystal lcd(12,11,8,7,6,5);

byte down[8] = {
  B00000,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

byte up[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00000
};
#include <LiquidCrystal.h>

#define BUTTON_DOWN_RIGHT A2
#define BUTTON_UP_LEFT A3

#define NONE 0
#define LEFT 1
#define RIGHT 2
#define UP 4
#define DOWN 8
#define ALL 16

int buttonPressed = 0;
int lastButton = 0;
int currentMenu = 1;
int intervalometerMenu = 0;
int lightMenu = 0;
int soundMenu = 0;
int x = 0;
int y = 0;
int button;

LiquidCrystal lcd(12,11,8,7,6,5);

byte down[8] = {
  B00000,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

byte up[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00000
};

byte right[8] ={
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000
};

byte left[8] ={
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000
};

void setup()
{
  pinMode(BUTTON_UP_LEFT, INPUT);
  pinMode(BUTTON_DOWN_RIGHT, INPUT);
  lcd.begin(16,2);
  Serial.begin(9600);
  lcd.createChar(1,down);
  lcd.createChar(2,up);
  lcd.createChar(3,right);
  lcd.createChar(4,left);
  reset();
}

void reset()
{
  lcd.clear();
  delay(500);
  lcd.noCursor();
  lcd.setCursor(0,0);
  lcd.print("Welcome");
  delay(3000);
  lcd.clear();
  buttonPressed = 0;
  lastButton = 0;
  currentMenu = 1;
  intervalometerMenu = 0;
  lightMenu = 0;
  soundMenu = 0;
  x = 0;
  y = 0;
}

void loop()
{
  button = checkButton();
  frontscreen ();
  delay(250);
/*
  //cursorPos(button, x, y);
  lcd.cursor();
  lcd.setCursor(x,y);
  delay(100);
*/
}


byte checkButton()
{
  int a1 = analogRead(BUTTON_UP_LEFT);
  int a2 = analogRead(BUTTON_DOWN_RIGHT);

  if (a1 < 700 && a2 < 700){
    reset();
  }
  if (a1 > 500 && a2 > 500)
    return NONE;
  if (a1 < 10)
    return LEFT;
  if (a1 > 10 && a1 < 500)
    return UP;
  if (a2 < 10)
    return DOWN;
  if (a2 > 10 && a2 < 500)
    return RIGHT;
}

void cursorpos(int state, int& pos)
{
  if (state == 2) {
    pos = pos * 10;
    }
  if (state == 4) {
    pos = pos - 1;
    }
  if (state == 8) {
    pos = pos + 1;
    }
}

void frontscreen()
{
  if (intervalometerMenu == 1){
    intervalometer();
  } else if (lightMenu == 1){
    light();
  } else if (soundMenu == 1){
    sound();
  } else if (buttonPressed == 1){
    displaymenu();
  } else if (button == 1 || button == 2 || button == 4 || button == 8){
    buttonPressed = 1;
  } else {
    lcd.setCursor(0,0);
    lcd.print("Use Arrows To");
    lcd.setCursor(0,1);
    lcd.print("Select Menu");
    lcd.setCursor(12,1);
    lcd.write(1);
    lcd.setCursor(13,1);
    lcd.write(2);
    lcd.setCursor(14,1);
    lcd.write(3);
    lcd.setCursor(15,1);
    lcd.write(4);
  }
}

void displaymenu()
{
  cursorpos(button, currentMenu);
  
  if (currentMenu < 1){
    currentMenu = 1;
  } else if (currentMenu > 3 && currentMenu < 10){
    currentMenu = 3;
  }
  Serial.println(currentMenu);
  switch(currentMenu) {
  case 1:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Intervalomter");
    break;
  case 2:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Light Trigger");
    break;
  case 3:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sound Trigger");
    break;
  case 10:
    intervalometerMenu = 1;
    break;
  case 20:
    lightMenu = 1;
    break;
  case 30:
    soundMenu = 1;
    break;
  }
}

void intervalometer()
{
  cursorpos(button, currentMenu);
  
  if (currentMenu <= 10){
    currentMenu = 10;
  } else if (currentMenu >= 16 && currentMenu < 20){
    currentMenu = 16;
  }
  Serial.println(currentMenu);
  switch(currentMenu) {
  case 10:
    lcd.clear();
    lcd.cursor();
    lcd.setCursor(0,0);
    lcd.print("Shutter Duration");
    lcd.setCursor(0,1);
    lcd.print("00h 00m 00s");
    break;
  case 11:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shutter Duration");
    lcd.cursor();
    lcd.setCursor(0,1);
    lcd.print("00h 00m 00s");
    break;
   case 12:
    lcd.clear();
    lcd.cursor();
    lcd.setCursor(0,0);
    lcd.print("Shot Delay");
    lcd.setCursor(0,1);
    lcd.print("00h 00m 00s");
    break;
  case 13:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shot Delay");
    lcd.cursor();
    lcd.setCursor(0,1);
    lcd.print("00h 00m 00s");
    break;
    case 14:
    lcd.clear();
    lcd.cursor();
    lcd.setCursor(0,0);
    lcd.print("Num Shots");
    lcd.setCursor(0,1);
    lcd.print("00000");
    break;
  case 15:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Num Shots");
    lcd.cursor();
    lcd.setCursor(0,1);
    lcd.print("00000");
    break;
   case 16:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Back");
    break;
   case 160:
     currentMenu = 0;
     buttonPressed = 1;
     intervalometerMenu = 0;
     frontscreen();
     break;
  }
}

void light()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Light Sensor");
}

void sound()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sound Sensor");
}
byte right[8] ={
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000
};

byte left[8] ={
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000
};

void setup()
{
  pinMode(BUTTON_UP_LEFT, INPUT);
  pinMode(BUTTON_DOWN_RIGHT, INPUT);
  lcd.begin(16,2);
  Serial.begin(9600);
  lcd.createChar(1,down);
  lcd.createChar(2,up);
  lcd.createChar(3,right);
  lcd.createChar(4,left);
  reset();
}

void reset()
{
  lcd.clear();
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("Welcome");
  delay(3000);
  lcd.clear();
  buttonPressed = 0;
  lastButton = 0;
  currentmenu = 1;
  x = 0;
  y = 0;
}

void loop()
{
  button = checkButton();
  frontscreen ();
  delay(250);
/*
  //cursorPos(button, x, y);
  lcd.cursor();
  lcd.setCursor(x,y);
  delay(100);
*/
}


byte checkButton()
{
  int a1 = analogRead(BUTTON_UP_LEFT);
  int a2 = analogRead(BUTTON_DOWN_RIGHT);

  if (a1 < 700 && a2 < 700){
    reset();
  }
  if (a1 > 500 && a2 > 500)
    return NONE;
  if (a1 < 10)
    return LEFT;
  if (a1 > 10 && a1 < 500)
    return UP;
  if (a2 < 10)
    return DOWN;
  if (a2 > 10 && a2 < 500)
    return RIGHT;
}

void cursorpos(int state, int& pos)
{
  if (state == 2) {
    pos = pos * 10;
    }
  if (state == 4) {
    pos = pos - 1;
    }
  if (state == 8) {
    pos = pos + 1;
    }
}

void frontscreen()
{
  if (buttonPressed == 1){
    displaymenu();
  } else if (button == 1 || button == 2 || button == 4 || button == 8){
    buttonPressed = 1;
  } else {
    lcd.setCursor(0,0);
    lcd.print("Use Arrows To");
    lcd.setCursor(0,1);
    lcd.print("Select Menu");
    lcd.setCursor(12,1);
    lcd.write(1);
    lcd.setCursor(13,1);
    lcd.write(2);
    lcd.setCursor(14,1);
    lcd.write(3);
    lcd.setCursor(15,1);
    lcd.write(4);
  }
}

void displaymenu()
{
  cursorpos(button, currentmenu);
  
  if (currentmenu < 1){
    currentmenu = 0;
  } else if (currentmenu > 3 && currentmenu < 10){
    currentmenu = 3;
  }
  
  switch(currentmenu) {
  case 1:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Intervalomter");
    break;
  case 2:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Light Trigger");
    break;
  case 3:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sound Trigger");
    break;
  }
}
