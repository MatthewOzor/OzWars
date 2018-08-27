#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

static const unsigned char PROGMEM ship_bmp[] =

{ B10000000, B00000000,
  B11000000, B00000000,
  B11100000, B00000000,
  B11110000, B00000000,
  B10011000, B00000000,
  B10001110, B00001000,
  B11111111, B00010100,
  B11111111, B11111111,
  B10000111, B11111111,
  B10001100, B00001110,
  B10111000, B00000000,
  B11110000, B00000000,
  B11100000, B00000000,
  B11000000, B00000000,
  B10000000, B00000000,
  B10000000, B00000000 };

static const unsigned char PROGMEM missile_bmp[] =

{ B10000000,
  B11110000,
  B11110000,
  B10000000 };

const byte button1Pin = 7;   // LEFT
const byte button2Pin = 8;   // RIGHT
const byte button3Pin = 9;   // UP
const byte button4Pin = 10;  // DOWN
const byte button5Pin = 6;   // FIRE

const byte sound = 3;

const byte starNum = 15;

class cstar
    {
      public:
      byte x;
      byte y; 
      byte s;
};

class cmissile
    {
      public:
      byte x;
      byte y;
      boolean fired;
};

cstar starField[starNum];

cmissile missile;

byte button1State = 0 ;
byte button2State = 0 ;
byte button3State = 0 ;
byte button4State = 0 ;
byte button5State = 0 ;

byte shipx = 128;
byte shipy = 32;
byte shipSpeed = 4;

void setup() {

  //Serial.begin(9600);
  
  pinMode(sound, OUTPUT); 
  
  pinMode(button1Pin, INPUT);    
  pinMode(button2Pin, INPUT);  
  pinMode(button3Pin, INPUT);   
  pinMode(button4Pin, INPUT); 
  pinMode(button5Pin, INPUT);    

  digitalWrite(button1Pin, HIGH);
  digitalWrite(button2Pin, HIGH);  
  digitalWrite(button3Pin, HIGH);
  digitalWrite(button4Pin, HIGH);  
  digitalWrite(button5Pin, HIGH); 
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();

  starsInit();

  gameIntro();
  
}

void loop() {
  
  button1State = digitalRead(button1Pin);  
  button2State = digitalRead(button2Pin);  
  button3State = digitalRead(button3Pin);  
  button4State = digitalRead(button4Pin);  
  button5State = digitalRead(button5Pin);  
  
  if (button1State == LOW) {  
    shipx = shipx - shipSpeed;
    tone(sound,1600,5);
 }

  if (button2State == LOW) {  
    shipx = shipx + shipSpeed;
    tone(sound,1600,5);
 }

   if (button3State == LOW) {  
    shipy = shipy - shipSpeed;
    tone(sound,1600,5);
 }

   if (button4State == LOW) {  
    shipy = shipy + shipSpeed;
        tone(sound,1600,5);
 }

   if (button5State == LOW && !missile.fired) {  
    missile.x = shipx + 5;
    missile.y = shipy + 5;
    missile.fired = true;
    tone(sound,500,10);
 }

  //delay(10);
  starsMove();
  gameDisplay();
  
}

void starsInit() {
 for (byte i=0; i < starNum; i++) {
  starField[i].x = random(1, 128);
  starField[i].y = random(1, 64);
  starField[i].s = random(1, 3);
 }
}

void starsDraw() {
 for (byte i=0; i < starNum; i++) {
  display.drawPixel(starField[i].x, starField[i].y, WHITE);
 }
}

void starsMove() {
 for (byte i=0; i < starNum; i++) {
  starField[i].x = starField[i].x - starField[i].s;
  if (starField[i].x < 1) {
      starField[i].x = 127;
      starField[i].y = random(1, 64);
      starField[i].s = random(1, 3);
  }
 }
}

void missileDraw() {
    display.drawBitmap(missile.x, missile.y,  missile_bmp, 8, 4, 1);
}

void missileMove() {
  missile.x = missile.x + 5;
  if (missile.x > 132) { 
      missile.fired = false;
  }
}

void gameDisplay() {
    display.clearDisplay();
    starsDraw();
    display.drawBitmap(shipx, shipy,  ship_bmp, 16, 16, 1);
    if (missile.fired) {
      missileDraw();
      missileMove();
    }
    display.display();
}

void gameIntro() {
  
  display.setTextColor(WHITE);
  display.setTextSize(3);
  
  for (byte i = 0; i < 64; i++){
    starsMove();
    display.clearDisplay();
    starsDraw();
    display.display();
  }  

    display.setCursor(12, 10);
    display.print("OzWars");
    display.display(); 
  
  for (byte i = 0; i < 128; i++){
    starsMove();
    display.clearDisplay();
    starsDraw();
    display.setCursor(12, 10);
    display.print("OzWars");
    display.display();    
  }  

  for (byte i = 0; i < 28; i++){
    starsMove();
    display.clearDisplay();
    starsDraw();
    shipx = shipx - 4;
    display.drawBitmap(shipx, shipy,  ship_bmp, 16, 16, 1);
    display.display();    
  }  
  
}

