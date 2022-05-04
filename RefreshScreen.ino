#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(P6_7, P2_3, P2_6, P2_4, P5_6, P6_6);

byte hero[8] = {
    B00000,
    B00100,
    B11111,
    B00100,
    B01110,
    B01010,
    B11111,
    B01010
};
    //Flip Phase 1
byte heroFlip1[8] = {
    B00100,
    B11111,
    B01110,
    B01010,
    B00000,
    B11111,
    B11111,
    B00000
};
    //Flip Phase 2
byte heroFlip2[8] = {
    B00100,
    B11111,
    B01110,
    B01010,
    B00000,
    B01010,
    B11111,
    B00000
};
byte obsticle[8] = {
    B00100,
    B00100,
    B01110,
    B01110,
    B01110,
    B11111,
    B11111,
    B11111
};


int jumpCount = 0;  //Counting number of successful jumps, which increase dificulty
int level = 1;

//OneMsTaskTimer_t wallTask = {250, wallISR, 0, 0}; //Takes Analog Reading of Joystick every second


enum screenTasks {START_SCREEN, DRAW, GAME_OVER};
screenTasks tasks;

void setupRefreshScreen(){
    //Set up LCD's number of rows and columns
  lcd.begin(16, 2);
  
    //Set up Hero Character
  lcd.createChar(0, hero);
  lcd.createChar(1, heroFlip1);
  lcd.createChar(2, heroFlip2);
  lcd.createChar(3, obsticle);

  lcd.setCursor(12,0);
  lcd.print("L:");

//  OneMsTaskTimer::add(&wallTask);
//  OneMsTaskTimer::start();
  
}

void drawScreen(){
    //SWITCH TRANSITIONS
    switch(tasks){
        case START_SCREEN:
          if((heroLocation.x == prevHeroLocation.x) && (heroLocation.y == prevHeroLocation.y)){
            //tasks = DRAW;
          }
          else{
            tasks = DRAW;
          }
          break;

        case DRAW:
          if((heroLocation.x == obsticleLocation.x) && (heroLocation.y == obsticleLocation.y)){
            tasks = GAME_OVER;
          }
          break;

        case GAME_OVER:
          break;
    }
    
    //SWITCH ACTIONS
    switch(tasks){
        case START_SCREEN:
          break;

        case DRAW:
          lcd.setCursor(14,0);
          lcd.print(level);
          drawHero();
          drawObsticle();
          break;

        case GAME_OVER:
          displayGameOver();
          break;
    }
}

void loopRefreshScreen(){
  while(timerFlag2 == 0){
    delay(10);
  }
  timerFlag2 = 0;
  drawScreen();
  
}

void eraseOldHero(){
    lcd.setCursor(prevHeroLocation.x, prevHeroLocation.y);
    lcd.print(" ");
}

void drawHero(){
    eraseOldHero();
    prevHeroLocation = heroLocation;
    lcd.setCursor(heroLocation.x, heroLocation.y);
    if(heroLocation.y == 1){
        lcd.write(byte(0)); //While this custome character
    }
    else{
        lcd.setCursor(heroLocation.x, heroLocation.y);
        lcd.write(byte(1)); //Flip Phase 1 test
        delay(100);
        lcd.setCursor(heroLocation.x, heroLocation.y);
        lcd.write(byte(2)); //Flip Phase 2 test
        delay(100);
    }
}

void eraseOldObsticle(){
  lcd.setCursor(prevObsticleLocation.x, prevObsticleLocation.y);
  lcd.print(" ");
}

void drawObsticle(){
  eraseOldObsticle();
  
  obsticleLocation.x--;
  prevObsticleLocation = obsticleLocation;

  if(obsticleLocation.x < 0){
    obsticleLocation.x = 16;
    jumpCount++;
  }

  if(jumpCount == 2){
      level++;
      jumpCount = 0;
  }
  
  lcd.setCursor(obsticleLocation.x, obsticleLocation.y);
  lcd.write(byte(3));
}

void displayGameOver(){
//  eraseOldObsticle();
//  eraseOldHero();
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("GAME OVER");
  lcd.setCursor(3,1);
  lcd.print("HIGH SCORE:");
  lcd.print(level);
}

//void wallISR(){
//  timerFlag2 = 1;
//
//}
