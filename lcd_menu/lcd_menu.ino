
//Import LCD library
#include <LiquidCrystal.h>

//These values won't change
#define BUTTON_DOWN_RIGHT A2
#define BUTTON_UP_LEFT A3

#define NONE 0
#define LEFT 1
#define RIGHT 2
#define UP 4
#define DOWN 8
#define ALL 16

int buttonPressed = 0;  //Have any buttons been pressed, used for leaving the front screen
int lastButton = 0; //Not used at the moment
int currentMenu = 1; //Keep track on the current menu number
int intervalometerMenu = 0; //Has this menu been accessed
int lightMenu = 0;  //Has this menu been accessed
int soundMenu = 0;  //Has this menu been accessed
int x = 0;  //Cursor position
int y = 0;  //Cursor position
int button;  //Not used at the moment

//LCD I/O Pins
LiquidCrystal lcd(12,11,8,7,6,5);

//Custom Characters
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

//Function used to rest certain parameters
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
  //Check for button input
  button = checkButton();
  //Go to Frontscreen
  frontscreen ();
  delay(250);
/*
  //menuposmenupos(button, x, y);
  lcd.cursor();
  lcd.setCursor(x,y);
  delay(100);
*/
}

//Check for button input
byte checkButton()
{
  //Assign variable for reading input
  int a1 = analogRead(BUTTON_UP_LEFT);
  int a2 = analogRead(BUTTON_DOWN_RIGHT);

  //Check Input and return value based on buttons pressed
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

//Work out menu position based on button pressed
void menupos(int state, int& pos)
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

//Front screen
void frontscreen()
{
  //series of if statements to check for submenu access and if buttons have been pressed
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

//Display menu items
void displaymenu()
{
  //use mempos function to update the current menu based on button pressed
  menupos(button, currentMenu);
  
  //If statement to make sure that the currentmenu doesn't got out of scope
  if (currentMenu < 1){
    currentMenu = 1;
  } else if (currentMenu > 3 && currentMenu < 10){
    currentMenu = 3;
  }
  Serial.println(currentMenu);
  //switchcase statements to display text and set variables based on currentmenu
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

//intervalometer submenu
void intervalometer()
{
  //use mempos function to update the current menu based on button pressed
  menupos(button, currentMenu);
  
  //If statement to make sure that the currentmenu doesn't got out of scope
  if (currentMenu <= 10){
    currentMenu = 10;
  } else if (currentMenu >= 16 && currentMenu < 20){
    currentMenu = 16;
  }
  Serial.println(currentMenu);
  //switchcase statements to display text and set variables based on currentmenu
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

//Test function... future use
void light()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Light Sensor");
}

//Test function... future use
void sound()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sound Sensor");
}
