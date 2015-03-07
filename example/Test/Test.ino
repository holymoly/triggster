#include <Triggster.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SD.h>
#include <SPI.h>


//Encoder
#define encoderPinA 2
#define encoderPinB 5
//Display
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8
//SD
#define SD_CS 4

Adafruit_SSD1331 tft = Adafruit_SSD1331(cs, dc, rst);

Triggster menu(A6,A7);

File menuFile;

void switchToMenu(int direction){
  menu.switchMenu(direction);
  menuFile = SD.open("menu.dat");
  if (menuFile) {
    // read from the file until there's nothing else in it:
    while (menuFile.available()) {
      menu.setMenuSerial(char(menuFile.read()));
    }
  } else {
    // if the file didn't open, print an error:
    tft.println("error opening menu.dat");
  }
}

void printMenu(){
  tft.fillScreen(BLACK);
  tft.setCursor(0,0);
  tft.println(menu.menuName);
  //Values
  tft.setCursor(10,8);
  tft.print("N:");
  tft.setCursor(65,8);
  tft.print(menu.n);
  tft.setCursor(10,16);
  tft.print("DelayExt:");
  tft.setCursor(65,16);
  tft.print(menu.extDelay);
  tft.setCursor(10,24);
  tft.print("HoldExt:");
  tft.setCursor(65,24);
  tft.print(menu.extHold);
  tft.setCursor(10,32);
  tft.print("DelayBef:");
  tft.setCursor(65,32);
  tft.print(menu.delayBefore);
  tft.setCursor(10,40);
  tft.print("HoldShut:");
  tft.setCursor(65,40);
  tft.print(menu.shutterHold);
  tft.setCursor(10,48);
  tft.print("DelayBet:");
  tft.setCursor(65,48);
  tft.print(menu.delayBetween);
  tft.setCursor(10,56);
  if(menu.getActiveMode() == SelectionMode){
    tft.print("Exit");
  }
  //*************************
  //switchToMenu(up);
}


void setup() {
  //  | End of Menu Name
  //  ; End of value n
  //  > Low Limit for Value n
  //  < High Limit for value n
  //  _ Step for value n
  //  Test1| value1: 101.1;1.0> 100.0< 10.5_
  //Serial.begin(9600);      // open the serial port at 9600 bps:

  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);

  // initialize the OLED
  tft.begin();

  tft.fillScreen(BLACK);
  delay(500);

  if (!SD.begin(SD_CS)) {
    tft.println("SD failed!");
    return;
  }
  delay(500);
  tft.println("SD OK!");

  // Get Amount of Menus
  menuFile = SD.open("menu.dat");
  if (menuFile) {
    tft.println("menu.dat");

    // read from the file until there's nothing else in it:
    while (menuFile.available()) {
      menu.amountOfMenus(char(menuFile.read()));
    }
    tft.print("n: ");
    tft.println(menu.getAmountOfMenus());
    // close the file:
    menuFile.seek(0);
  } else {
    // if the file didn't open, print an error:
    tft.println("error opening menu.dat");
  }
  delay(1500);
  if (menuFile) {
    // read from the file until there's nothing else in it:
    while (menuFile.available()) {
      menu.setMenuSerial(char(menuFile.read()));
    }
  } else {
    // if the file didn't open, print an error:
    tft.println("error opening menu.dat");
  }
  menuFile.close();
  printMenu();
  delay(3000);
  switchToMenu(up);
  printMenu();
  menu.onButtonPress();
  menu.encoderRotate(up);
  menu.encoderRotate(up);
  menu.onButtonPress();
  menu.encoderRotate(up);
  menu.onButtonPress();
  delay(3000);
  printMenu();
}

void loop() {

}
