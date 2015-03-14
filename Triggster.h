#ifndef Triggster_h
#define Triggster_h

#include "Arduino.h"

#define _maxStringSize 15
#define _amountOfValues 6

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

enum {up,down};
enum {MenuMode, SelectionMode, EditMode};

class Triggster
{
  public:
    Triggster(int extOutPin,int shutterPin, int encoderAPin, int encoderBPin, int encoderButtonPin);
    ~Triggster ();
    void encoderRotate(int direction);
    void amountOfMenus(char item);
    void setMenuSerial(char item);
    int  getAmountOfMenus();
    int  getActiveMenu();
    void onButtonPress();
    void trigger(bool ext);
    int  getActiveMode();
    int  getActiveSelection();

    void switchMenu(int direction);     //Change to next menu up or down
    char menuName[_maxStringSize];      // Menu Name
    //Menu Values
    int   n                ;            // Amount of pictures
    int   extDelay         ;            // Delay for external (i.e.Valve) Signal
    int   extHold          ;            // How long external pin is high
    int   delayBefore      ;            // Delay before Picture
    int   shutterHold      ;            // How long the shutter is high
    int   delayBetween     ;            // Delay between Pictures

  private:
    void switchSelection(int direction);//Change the selected Value up or down
    void changeValue(int direction);    //in or decrease the value
    //pins
    int _extOutPin;
    int _shutterPin;
    int _encoderAPin;
    int _encoderBPin;
    int _encoderButtonPin;

    //Menu Meta Part
    char tempString[_maxStringSize];

    int _activeMenu;
    int _activeMode;
    int _activeSelection;
    int _menus;
    int _valueToParse;
    int _menuParsing;
    int _tempStringPosition;

    //Menu Values
    int _nHigh;                         // High Limit
    int _nLow;                          // Low Limit
    int _nStep;                         // Step size

    int _extDelayHigh;                  // High Limit
    int _extDelayLow;                   // Low Limit
    int _extDelayStep;                  // Step size

    int _extHoldHigh;                   // High Limit
    int _extHoldLow;                    // Low Limit
    int _extHoldStep;                   // Step size

    int _delayBeforeHigh;               // High Limit
    int _delayBeforeLow;                // Low Limit
    int _delayBeforeStep;               // Step size

    int _shutterHoldHigh;               // High Limit
    int _shutterHoldLow;                // Low Limit
    int _shutterHoldStep;               // Step size

    int _delayBetweenHigh;              // High Limit
    int _delayBetweenLow;               // Low Limit
    int _delayBetweenStep;              // Step size
};

#endif
