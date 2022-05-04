
#include "OneMsTaskTimer.h"

int selectPin = P3_3;
int jumpFlag = 0;
int timerFlag = 0;
int timerFlag2 = 0;
int forwardFlag = 0;
int backwardFlag = 0;

int dirReadPin = P6_0;
int leftThreshold = 540;
int rightThreshold = 950;

int count = 0;


OneMsTaskTimer_t timerTask = {250, timerISR, 0, 0}; //Takes Analog Reading of Joystick every second

typedef struct xy_struct{
  int x;
  int y;
} XY;
    //Current Hero Position
XY heroLocation;
    //Previous Hero Position
XY prevHeroLocation;

    //Current Obsticle Position
XY obsticleLocation;
    //Previous Obsticle Position
XY prevObsticleLocation;

enum heroStatus {onBoot, START, waitForAction, moveBack, moveForward, jump};
heroStatus hero_state;

void setupHeroActions() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Baud Rate

  obsticleLocation.x = 16;
  obsticleLocation.y = 1;

  hero_state = onBoot;

  pinMode(selectPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(selectPin), jumpISR, RISING);

  OneMsTaskTimer::add(&timerTask);
  OneMsTaskTimer::start();
  
}

void heroStatusFunction(){
  //State Transitions   ARROWS ON THE DIAGRAMS
  //Switch
  switch(hero_state){
    case onBoot:
        hero_state = START;
        break;
    
    case START:
        hero_state = waitForAction;
        break;

    case waitForAction:
        if(forwardFlag == 1){
          hero_state = moveForward;
        }
        else if(backwardFlag == 1){
          hero_state = moveBack;
        }
        else if(jumpFlag == 1){
          hero_state = jump;
          count = 0;
        }
    break;

    case moveForward:
        hero_state = waitForAction;
        break;

    case moveBack:
        hero_state = waitForAction;
        break;

    case jump:
        if(count > 2){
            hero_state = waitForAction;
            heroLocation.y = 1;
        }
        break;
  } //END TRANSITIONS

  //State Actions   WHAT'S HAPPENING AT THAT STATE LOCATION (UPDATED HERO LOCATION)
  //Swich
  switch(hero_state){
    case onBoot:
        break;
    
    case START:
        heroLocation.x = 0;
        heroLocation.y = 1;

        obsticleLocation.x = 16;
        obsticleLocation.y = 1;
        
        Serial.println(heroLocation.x);
        Serial.println(heroLocation.y);
        break;

    case waitForAction:
        break;

    case moveForward:
        if(heroLocation.x == 15){
          heroLocation.x = 15;
        }
        else{
          heroLocation.x++;
        }
        //heroLocation.x++;
        Serial.print("Hero Position: ");
        Serial.print(heroLocation.x);
        Serial.print(" ");
        Serial.println(heroLocation.y);
        forwardFlag = 0;  //reset forwardFlag
        break;

    case moveBack:
        if(heroLocation.x == 0){
          heroLocation.x = 0;
        }
        else{
          heroLocation.x--;
        }
        Serial.print("Hero Position: ");
        Serial.print(heroLocation.x);
        Serial.print(" ");
        Serial.println(heroLocation.y);
        backwardFlag = 0; //reset backwardFlag
        break;

    case jump:
        heroLocation.y = 0;
        Serial.print("Hero Position: ");
        Serial.print(heroLocation.x);
        Serial.print(" ");
        Serial.println(heroLocation.y);
        count++;
        jumpFlag = 0; //reset jumpFlag
        break;
  } //END ACTIONS
}

void loopHeroActions() {
  while(timerFlag == 0){
    delay(10); //Delay 10ms
  }
  //Serial.println("Clock Tick");
  pollJoyStick();
  heroStatusFunction(); //Calling switch statements
  timerFlag = 0;
  delay(10);

}

void jumpISR(){
  jumpFlag = 1;
}

void timerISR(){
  timerFlag = 1;
  timerFlag2 = 1;
}

void pollJoyStick(){
    int joystickX = analogRead(dirReadPin);
    //Detect moving forward or backwards

    if(joystickX < leftThreshold){
      backwardFlag = 1;
    }
    if(joystickX > rightThreshold){
      forwardFlag = 1;
    }
}
