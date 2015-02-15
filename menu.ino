//Display pins
#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

//Encoder pins
#define encoder0PinA  2
#define encoder0PinB  5

#define encoder0PinButton  3

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF
#define HEADLINE        0xCCCC

#define menuStartPositonX 0
#define menuStartPositonY 0

#define valueTextXoffset    10
#define valueTextYoffset    25
#define valueXoffset        65

#define selectionClearRectangleX 0
#define selectionClearRectangleY 24
#define selectionClearRectangleWidth 10
#define selectionClearRectangleHeigth 40

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

// Option 1: use any pins but a little slower
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);
//Strings for Menu
String  menu[8] = { "manual", "delayed", "interval","bulb", "bulb interv", "extern dig", "extern ana", "general"};
int     manualValues[0];  //none
String  manualValuesNames[0];
int     delayedValues[1] = {1}; //delay
String  delayedValuesNames[1] = {"delay"};
int     intervalValues[2] = {1,2}; //step(between two photos), amount
String  intervalValuesNames[2] = {"wait", "amount"};
int     bulbValues[1] = {30}; //duration
String  bulbValuesNames[1] = {"duration"};
int     bulbIntervalValues[3] = {30,30,2}; //duration, step(between two photos), amount
String  bulbIntervalValuesNames[3] = {"duration", "wait", "amount"};
int     externalDigValues[1] = {1}; //delay
String  externalDigValuesNames[1] = {"delay"};
int     externalAnaValues[2] = {1,512}; //delay, threshold
String  externalAnaValuesNames[2] = {"delay", "threshold"};
int     generalValues [2] = {500,500}; //focus time, shutter time
String  generalValuesNames[2] = {"f time", "s time"};

//Parameter stored for parameter changing
int *tempValues;
String *tempValuesNames;

//amount of Values in tempValues and tempValuesNames
int availableValues = 0;

// Store the active menu
int activeMenu = 0;

// Store active Value
int activeValue = 0;

// Store selection mode
bool selection = false;

// Store edit mode
bool edit = false;

void initMenu(){
  display.begin();
  printMenu(0);

  pinMode(encoder0PinA, INPUT);
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT);
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinButton, INPUT);
  digitalWrite(encoder0PinButton, HIGH);       // turn on pullup resistor

  //activate interrupt when pin 2 changes
  attachInterrupt(0, doEncoder,CHANGE);  // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, doEncoderButton,FALLING);  // encoder pin on interrupt 0 - pin 2
}

void printMenu(int menuNumber){
  if (menuNumber > 7){
    menuNumber = 0;
  }
  if (menuNumber < 0){
    menuNumber = 7;
  }
  //Print mode description to display
  display.fillScreen(BLACK);
  display.setTextSize(1);
  display.setCursor(menuStartPositonX, menuStartPositonY);
  display.setTextColor(HEADLINE);
  display.println("Mode: ");
  display.setTextColor(WHITE);
  display.print(menuNumber);
  display.print(". ");

  switch (menuNumber) {
    case 0:
      display.println(menu[menuNumber]);
      break;
    case 1:
      display.println(menu[menuNumber]);
      break;
    case 2:
      display.println(menu[menuNumber]);
      break;
    case 3:
      display.println(menu[menuNumber]);
      break;
    case 4:
      display.println(menu[menuNumber]);
      break;
    case 5:
      display.println(menu[menuNumber]);
      break;
    case 6:
      display.println(menu[menuNumber]);
      break;
    case 7:
      display.println(menu[menuNumber]);
      break;
  };

  //print mode values
  display.setTextColor(HEADLINE);
  display.println("Values:");
  display.setTextColor(WHITE);
  switch (menuNumber) {
    case 0:
      printValues(manualValuesNames,manualValues, 0, false);
      break;
    case 1:
      printValues(delayedValuesNames,delayedValues, 1, false);
      break;
    case 2:
      printValues(intervalValuesNames,intervalValues, 2, false);
      break;
    case 3:
      printValues(bulbValuesNames,bulbValues, 1, false);
      break;
    case 4:
      printValues(bulbIntervalValuesNames,bulbIntervalValues, 3, false);
      break;
    case 5:
      printValues(externalDigValuesNames,externalDigValues, 1, false);
      break;
    case 6:
      printValues(externalAnaValuesNames,externalAnaValues, 2, false);
      break;
    case 7:
      printValues(generalValuesNames,generalValues, 2, false);
      break;
  };
  activeMenu = menuNumber;
}

//Checks how much values exist per menu and prints them
void printValues(String *Names,int *Values, int length, bool exit){

  for (int i = 0; i < length; i++){
    display.setCursor(valueTextXoffset, valueTextYoffset + (i * 8));
    display.print(Names[i] + ": ");
    display.setCursor(valueTextXoffset + valueXoffset, valueTextYoffset + (i * 8));
    display.println(Values[i]);
  }
  if (exit){
    display.setCursor(valueTextXoffset, valueTextYoffset + (length * 8));
    display.print("Exit");
  }
  //stere available values for later usage
  tempValues = Values;
  tempValuesNames = Names;
  availableValues = length;
}

// Encoder button interrupt routine
// Priority 1: Check if exit is selected
//          2: Check if selection should be edited
//          3: Check if editing should be ended
//          4: Check if selection should be started
void doEncoderButton() {
  noInterrupts();
  //End value selection
  // 1
  if (activeValue == availableValues){
    selection = false;
    edit = false;
    activeValue = 0;
    display.fillRect(selectionClearRectangleX, selectionClearRectangleY, selectionClearRectangleWidth, selectionClearRectangleHeigth, BLACK);
    printMenu(activeMenu);
    return;
  }

  //Enter edit mode
  // 2
  if (selection){
    edit = true;
    selection = false;
    printValue();
    return;
  }

  //End edit mode
  // 3
  if (edit){
    edit = false;
    selection = true;
    display.fillRect(selectionClearRectangleX, selectionClearRectangleY, 95, 40, BLACK);
    setSelection();
    printValues(tempValuesNames,tempValues, availableValues, true);
    return;
  }

  //Start value selection
  // 4
  if (!selection){
    activeValue = 0;
    selection = true;
    display.fillRect(selectionClearRectangleX, selectionClearRectangleY, 95, 40, BLACK);
    setSelection();
    printValues(tempValuesNames,tempValues, availableValues, true);
  }
}

//Encoder interrupt routine
void doEncoder() {
  noInterrupts();
  bool B = digitalRead(encoder0PinB);
  bool A = digitalRead(encoder0PinA);

  // Debounce
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 50)
  {
    //Priority  1. Edit Mode
    //          2. Selection Mode
    //          3. Menu Mode
    //Up
    if (A == B) {
      // 1
      if(edit){
        tempValues[activeValue] = tempValues[activeValue] + 1;
        printValue();
      } else {
        // 2
        if (!selection){
          printMenu(activeMenu + 1);
        } else {
          // 3
          activeValue += 1;
          if (activeValue > availableValues - 0){
            activeValue = 0;
          }
          setSelection();
        }
      }
    }
    //Priority  1. Edit Mode
    //          2. Selection Mode
    //          3. Menu Mode
    //Down
    if (A != B) {
      // 1
      if(edit){
        tempValues[activeValue] = tempValues[activeValue] - 1;
        printValue();
      } else {
        // 2
        if (!selection){
          printMenu(activeMenu - 1);
        } else {
          // 3
          activeValue -= 1;
          if (activeValue < 0){
            activeValue = availableValues;
          }
          setSelection();
        }
      }
    }
    last_interrupt_time = interrupt_time;
  }
}

//Set point before selection
void setSelection(){
  display.fillRect(selectionClearRectangleX, selectionClearRectangleY, selectionClearRectangleWidth, selectionClearRectangleHeigth, BLACK);
  display.fillRect(5, valueTextYoffset + (activeValue * 8) + 3, 2, 2,WHITE);
}

//Print sected value on screen for editing
void printValue(){
  display.setTextSize(2);
  display.fillRect(selectionClearRectangleX, selectionClearRectangleY, 95, 40, BLACK);
  display.setCursor(valueTextXoffset + 15, valueTextYoffset);
  display.print(tempValues[activeValue]);
  display.setTextSize(1);
}

int getFocusDelay(){
  return generalValues[0];
}

int getShutterDelay(){
  return generalValues[1];
}

int getDelayModeDelay(){
  return delayedValues[0];
}

int getIntervalModeWait(){
  return intervalValues[0];
}

int getIntervalModeAmount(){
  return intervalValues[1];
}

int getBulbModeDuration(){
  return bulbValues[0];
}

int getBulIntervalbModeDuration(){
  return bulbIntervalValues[0];
}
int getBulIntervalbModeWait(){
  return bulbIntervalValues[1];
}

int getBulIntervalbModeAmaount(){
  return bulbIntervalValues[2];
}

int getExternalDigModeDelay(){
  return externalDigValues[0];
}

int getExternalAnaModeDelay(){
  return externalAnaValues[0];
}

int getExternalAnaModeThreshold(){
  return externalAnaValues[1];
}

int getActiveMenu(){
  return activeMenu;
}
