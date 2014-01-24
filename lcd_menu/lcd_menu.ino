//
//Camera Controller with LCD
//
//Daniel Borg
//22 Jan 2014
//
//

//Import LCD library
#include <LiquidCrystal.h>

//These values won't change

#define DEBUG
#define BUTTON_MENU A2
#define BUTTON_IP A3

#define NONE 0
#define LEFT 1
#define RIGHT 2
#define UP 4
#define DOWN 8
#define ENTER 16
#define M_UP 32
#define M_DOWN 64


int menuEntered = 0;  //Have any buttons been pressed, used for leaving the front screen
int currentMenu = 1; //Keep track on the current menu number
int lastMenu = 0; //Used to see if the menu has changed
int intervalometerMenu = 0; //Has this menu been accessed
int lightMenu = 0;  //Has this menu been accessed
int soundMenu = 0;  //Has this menu been accessed
int button;  //Used to hold button state
int lastButton = 0; //Not used at the moment
int cursorMoved = 0; //Used to hold if the cursor has been adjusted state
int x = 0;  // Left to Right Cursor position
int y = 0;  //Up and Down Cursor position
char shutterTime[6];  //Array for storing Shutter time
char delayTime[6];  //Array for storing shot delay
char numShots[4]; //Array for storing the number of photos.
int shut = 0;  //used to keep track of shutterTime array position
int del = 0;  //used to keep track of delayTime array position
int shot = 0; //used to keep track of delayTime array position
int shutVal = 0;  //used for storing and checking that the shutter time value hasn't gone out of scope
int delVal = 0;  //used for storing and checking that the delay time value hasn't gone out of scope
int shotVal = 0;  //used for storing and checking that the shot value hasn't gone out of scope

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
  pinMode(BUTTON_MENU, INPUT);
  pinMode(BUTTON_IP, INPUT);
  lcd.begin(16,2);
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  
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
  menuEntered = 0;
  lastButton = 0;
  currentMenu = 1;
  lastMenu = 0;
  intervalometerMenu = 0;
  lightMenu = 0;
  soundMenu = 0;
  shut = 0; 
  del = 0;
  shot = 0;
  shutVal = 0;
  delVal = 0;
  shotVal = 0;
  x = 0;
  y = 0;
  for (int i=0; i < 6; i++){
    shutterTime[i] = 0;
  }
  for (int i=0; i < 6; i++){
    delayTime[i] = 0;
  }
  for (int i=0; i < 4; i++){
    numShots[i] = 0;
  }
}

void loop()
{
  //Check for button input
  button = checkButton();
  
  //Go to Frontscreen
  frontscreen ();
  delay(150);
}

//Check for button input
byte checkButton()
{
  //Assign variable for reading input
  int a1 = analogRead(BUTTON_IP);
  int a2 = analogRead(BUTTON_MENU);
  
  #ifdef DEBUG
  Serial.print ("a1 = ");
  Serial.println (a1);
  Serial.print ("a2 = ");
  Serial.println (a2);
  #endif
  
  //Check Input and return value based on buttons pressed
  if (a1 > 245 && a1 < 275){
    reset();
  }
  if (a1 > 950)
    return NONE;
  if (a1 > 160 && a1 < 190)
    return RIGHT;
  if (a1 > 330 && a1 < 360)
    return DOWN;
  if (a1 > 500 && a1 < 530)
    return ENTER;
  if (a1 > 670 && a1 < 700)
    return LEFT;
  if (a1 > 840 && a1 < 870)
    return UP;
  if (a2 < 20)
    return M_UP;
  if (a2 > 70 && a2 < 120)
    return M_DOWN;
}

//Work out menu position based on button pressed
void menupos(int state, int& pos)
{
  if (state == 16) {
    pos = pos * 10;
    }
  if (state == 32) {
    pos = pos - 1;
    }
  if (state == 64) {
    pos = pos + 1;
    }
}

void time(int state, char& num)
{
  if (state == 4) {
    num = num +1;
    if (num >= 9){
      num = 9;
    }  
  }
  if (state == 8) {
    num = num -1;
    if (num <= 0){
      num = 0;
    }
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
  } else if (menuEntered == 1){
    displaymenu();
  } else if (button == 16){
    menuEntered = 1;
  } else {
    lcd.setCursor(0,0);
    lcd.print("Press Enter to");
    lcd.setCursor(0,1);
    lcd.print("Continue");
  }
}

//Display menu items
void displaymenu()
{
  //use mempos function to update the current menu based on button pressed
  menupos(button, currentMenu);
  
    //If statement to make sure that the currentmenu doesn't got out of scope
  if (currentMenu <= 1){
    currentMenu = 1;
  } else if (currentMenu > 3 && currentMenu < 10){
    currentMenu = 3;
  }
  
    #ifdef DEBUG
    Serial.print ("button = ");
    Serial.println (button);  
    Serial.print ("Current Menu = ");
    Serial.println(currentMenu);
    Serial.print ("Last Menu = ");
    Serial.println(lastMenu);
    #endif

  //Check to see if the menu has changed
  if (currentMenu != lastMenu){
  
    //switchcase statements to display text and set variables based on currentmenu
    switch(currentMenu) {
      case 1:
        lastMenu = currentMenu;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Intervalomter");
        break;
      case 2:
        lastMenu = currentMenu;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Light Trigger");
        break;
      case 3:
        lastMenu = currentMenu;
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
}

//intervalometer submenu
void intervalometer()
{
  //use mempos function to update the current menu based on button pressed
  menupos(button, currentMenu);
  
  //Create buffer for printing values to LCD
  char lcd_buffer[11];
  
  //If statement to make sure that the currentmenu doesn't got out of scope
  if (currentMenu <= 10 || currentMenu == 100){
    currentMenu = 10;
  } else if (currentMenu >= 13 && currentMenu < 20){
    currentMenu = 13;
  }
  
    #ifdef DEBUG
    Serial.print ("Current Menu = ");
    Serial.println(currentMenu);
    Serial.print ("Last Menu = ");
    Serial.println(lastMenu);
    #endif
      
  //quick test to see if directional buttons have been pressed
  if (button == 1 || button == 2 || button == 4 || button == 8){
    cursorMoved = 1;
  }
    //Check to see if the menu has changed
  if (currentMenu != lastMenu || cursorMoved == 1){
    
    //switchcase statements to display text and set variables based on currentmenu
    switch(currentMenu) {
      case 10:
        if (currentMenu != lastMenu){
          x = 0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Shutter Duration");
        }
          lcd.setCursor(0,1);
          //Load data into buffer
          sprintf(lcd_buffer,"%d%dh %d%dm %d%ds", shutterTime[0],shutterTime[1],shutterTime[2],shutterTime[3],shutterTime[4],shutterTime[5]);
          lcd.print(lcd_buffer);
          lcd.setCursor(x,1);
          lcd.cursor();
      
        if (cursorMoved == 1){
          switch (button) {
            case RIGHT:
              //increment cursor position by one
              x++;
              
              //skip spaces where data entry isn't required and make sure that position doesn't go out of scope
              if (x == 2 || x == 4 || x == 6){
                x = x+2;
              } else if (x >= 9){
                x = 9;
              }
              //increment array position by one and make sure that it doesn't go out of scope
              shut++;
              if (shut >= 5){
                shut = 5;
              }
              lcd.setCursor(x,1);
              lcd.cursor();
              break;
            
            case LEFT:
              //decrement cursor position by one
              x--;
              
              //skip spaces where data entry isn't required and make sure that position doesn't go out of scope
              if (x == 3 || x == 7){
                x = x-2;
              } else if (x <= 0){
                x = 0;
              }
              //deccrement array position by one and make sure that it doesn't go out of scope
              shut--;
              if (shut <= 0){
                shut = 0;
              }
              lcd.setCursor(x,1);
              lcd.cursor();
              break;

            case UP:
              time(button,shutterTime[shut]);
              break;
            
            case DOWN:
              time(button,shutterTime[shut]);
              break;
          }
          lastMenu = currentMenu;
          //lastButton = button;
        }
        break;
      case 11:
        if (currentMenu != lastMenu){
          x = 0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Shot Delay");
         }
          lcd.setCursor(0,1);
          sprintf(lcd_buffer,"%d%dh %d%dm %d%ds", delayTime[0],delayTime[1],delayTime[2],delayTime[3],delayTime[4],delayTime[5]);
          lcd.print(lcd_buffer);
          lcd.setCursor(x,1);
          lcd.cursor();
      
        if (cursorMoved == 1){
          switch (button) {
            case RIGHT:
            //increment cursor position by one
              x++;
              
              //skip spaces where data entry isn't required and make sure that position doesn't go out of scope
              if (x == 2 || x == 4 || x == 6){
                x = x+2;
              } else if (x >= 9){
                x = 9;
              }
              //increment array position by one and make sure that it doesn't go out of scope
              del++;
              if (del >= 5){
                del = 5;
              }
              lcd.setCursor(x,1);
              lcd.cursor();
              break;
            
            case LEFT:
              //decrement cursor position by one
              x--;
              
              //skip spaces where data entry isn't required and make sure that position doesn't go out of scope
              if (x == 3 || x == 7){
                x = x-2;
              } else if (x <= 0){
                x = 0;
              }
              //deccrement array position by one and make sure that it doesn't go out of scope
              del--;
              if (del <= 0){
                del = 0;
              }
              lcd.setCursor(x,1);
              lcd.cursor();
              break;

            case UP:
              time(button,delayTime[del]);
              break;
            
            case DOWN:
              time(button,delayTime[del]);
              break;
          }
          lastMenu = currentMenu;
        }
        break;
      case 12:
        if (currentMenu != lastMenu){
          x = 0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Number of Shots");
         }
          lcd.setCursor(0,1);
          sprintf(lcd_buffer,"%d%d%d%d", numShots[0],numShots[1],numShots[2],numShots[3]);
          lcd.print(lcd_buffer);
          lcd.setCursor(x,1);
          lcd.cursor();
      
        if (cursorMoved == 1){
          switch (button) {
            case RIGHT:
            //increment cursor position by one
              x++;
              
              //skip spaces where data entry isn't required and make sure that position doesn't go out of scope
              if (x >= 3){
                x = 3;
              }
              //increment array position by one and make sure that it doesn't go out of scope
              shot++;
              if (shot >= 3){
                shot = 3;
              }
              lcd.setCursor(x,1);
              lcd.cursor();
              break;
            
            case LEFT:
              //decrement cursor position by one
              x--;
              
              //skip spaces where data entry isn't required and make sure that position doesn't go out of scope
              if (x <= 0){
                x = 0;
              }
              //deccrement array position by one and make sure that it doesn't go out of scope
              shot--;
              if (shot <= 0){
                shot = 0;
              }
              lcd.setCursor(x,1);
              lcd.cursor();
              break;

            case UP:
              time(button,numShots[shot]);
              break;
            
            case DOWN:
              time(button,numShots[shot]);
              break;
          }
          lastMenu = currentMenu;
        }
        break;
      case 13:
        lastMenu = currentMenu;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Back");
        lcd.setCursor(0,0);
        lcd.cursor();
        break;
      case 130:
        lcd.noCursor();
        currentMenu = 1;
        button = NONE;
        menuEntered = 1;
        intervalometerMenu = 0;
        frontscreen();
        break;
    }
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
