#include <Triggster.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SD.h>
#include <SPI.h>

//Encoder
#define encoderPinA 2
#define encoderPinB 5
#define encoderButton 3
//Output
#define externalOut A6
#define shutterOut  A7
//Display
#define sclk        13
#define mosi        11
#define cs          10
#define rst         9
#define dc          8
//SD
#define SD_CS       4
//Meta
#define debounce    200

Adafruit_SSD1331 tft = Adafruit_SSD1331(cs, dc, rst);
Triggster menu(externalOut, shutterOut, encoderPinA, encoderPinB, encoderButton);
File menuFile;

void switchToMenu(){
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
  menuFile.close();
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

void printValue(){
  tft.fillScreen(BLACK);
  tft.setCursor(10,32);
  //Values
  switch(menu.getActiveSelection()){
    case 0:
      tft.print("N: ");
      tft.print(menu.n);
      break;
    case 1:
      tft.print("DelayExt: ");
      tft.print(menu.extDelay);
      break;
    case 2:
      tft.print("HoldExt: ");
      tft.print(menu.extHold);
      break;
    case 3:
      tft.print("DelayBef: ");
      tft.print(menu.delayBefore);
      break;
    case 4:
      tft.print("HoldShut: ");
      tft.print(menu.shutterHold);
      break;
    case 5:
      tft.print("DelayBet: ");
      tft.print(menu.delayBetween);
      break;
  }
  //*************************
  //switchToMenu(up);
}

void printSelection(){
  tft.fillRect( 0,  8,  9,  55, BLACK);
  tft.fillRect( 4,  (menu.getActiveSelection() * 8) + 8 + 4,  1,  1, WHITE);
}
void clearSelection(){
  tft.fillRect( 0,  8,  9,  55, BLACK);
}

void doEncoderButtonISR(){
  noInterrupts();
  Serial.println("Button pressed");
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  //Timer for debounce
  if (interrupt_time - last_interrupt_time > debounce)
  {
    menu.onButtonPress();
    if(menu.getActiveMode() == MenuMode){
      printMenu();
      clearSelection();
    }
    if(menu.getActiveMode() == SelectionMode){
      printMenu();
      printSelection();
    }
    if(menu.getActiveMode() == EditMode){
      printValue();
    }
    last_interrupt_time = interrupt_time;
  }
  interrupts();
}

void doEncoderISR(){
  noInterrupts();
  bool B = digitalRead(encoderPinB);
  bool A = digitalRead(encoderPinA);

  //Timer for debounce
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > debounce)
  {
    int direction = down;
    if (A == B) {
      direction = up;
    }
    menu.encoderRotate(direction);
    if(menu.getActiveMode() == MenuMode){
      switchToMenu();
      printMenu();
    }
    if(menu.getActiveMode() == SelectionMode){
      printSelection();
    }
    if(menu.getActiveMode() == EditMode){
      printValue();
    }
    last_interrupt_time = interrupt_time;
  }
  interrupts();
}

void setup() {
  //  | End of Menu Name
  //  ; End of value n
  //  > Low Limit for Value n
  //  < High Limit for value n
  //  _ Step for value n
  //  Test1|101.1;1.0>100.0<10.5_
  Serial.begin(9600);

  attachInterrupt(0, doEncoderISR, CHANGE);  // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, doEncoderButtonISR, FALLING);  // encoder pin on interrupt 0 - pin 2

  Serial.begin(9600);      // open the serial port at 9600 bps:

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
  delay(15);
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
  //*******Init Display************
  printMenu();
  //******************************
}

void loop() {
  interrupts();
}
