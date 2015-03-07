#include "Arduino.h"
#include "Triggster.h"

//Constructor
Triggster::Triggster(int extOutPin,int shutterPin)
{
  _activeMenu         = 0;
  _activeMode         = MenuMode;
  _activeSelection    = 0;
  _menus              = 0;
  _valueToParse       = 0;
  _menuParsing        = 0;
  _tempStringPosition = 0;

  _extOutPin          = extOutPin;
  _shutterPin         = shutterPin;
}

//Deconstructor
Triggster::~Triggster()
{
}

//Check if requested menu is higher
void Triggster::encoderRotate(int direction){
  switch(_activeMode){
    case MenuMode:
        switchMenu(direction);
      break;
    case SelectionMode:
        switchSelection(direction);
      break;
    case 2:
        changeValue(direction);
      break;
  }
}

// switch Menu
void Triggster::switchMenu(int direction)
{
  if(direction == up){
    _activeMenu++;
  }else{
    _activeMenu--;
  }
  if(_activeMenu == _menus){
    _activeMenu = 0;
  }
  if(_activeMenu < 0){
    _activeMenu = _menus -1;
  }
  _menuParsing = 0;
};

// switch Selection
void Triggster::switchSelection(int direction)
{
  if (direction == up)
  {
    if(_activeSelection == _amountOfValues){
      _activeSelection = 0;
    }else{
       _activeSelection++;
    }
  }else{
    if(_activeSelection == 0){
      _activeSelection = _amountOfValues;
    }else{
       _activeSelection++;
    }
  }
};

// Returns Active Mode
int Triggster::getActiveMode()
{
 return _activeMode;
}
// Change Value
void Triggster::changeValue(int direction)
{
  //Check if chang is positive or negative
  int factor = -1;
  if (direction == up)
  {
    factor = 1;
  }

  //Setthe value
  switch(_activeSelection){
    case 0:
      n = n + (_nStep * factor);
      break;
    case 1:
      extDelay = extDelay + (_extDelayStep * factor);
      break;
    case 2:
      extHold = extHold + (_extHoldStep * factor);
    case 3:
      delayBefore = delayBefore + (_delayBeforeStep * factor);
      break;
    case 4:
      shutterHold = shutterHold + (_shutterHoldStep * factor);
      break;
    case 5:
      delayBetween = delayBetween + (_delayBetweenStep * factor);
      break;
  }
};

// Menu Change up or down
void Triggster::onButtonPress()
{
  //Menu->Selection
  if(_activeMode == MenuMode){
    _activeMode = SelectionMode;
    return;
  }
  //Selection-> Menu(Exit)
  if((_activeMode == SelectionMode) && (_activeSelection == _amountOfValues)){
    _activeMode = MenuMode;
    return;
  }
  //Selection-> Edit
  if(_activeMode == SelectionMode){
    _activeMode = EditMode;
    return;
  }
  //Edit->Selection
  if(_activeMode == EditMode){
    _activeMode = SelectionMode;
    return;
  }

};

//Get Amount of menus
void Triggster::amountOfMenus(char item){
  if(item == '\n'){
    _menus++;
  }
}

//Get Amount of menus
int Triggster::getAmountOfMenus(){
  return _menus;
}
//Get Amount of menus
int Triggster::getActiveMenu(){
  return _activeMenu;
}

//Read Menu and Set Variables of Active Menu
void Triggster::setMenuSerial(char item){
  if(_activeMenu == _menuParsing){
    tempString[_tempStringPosition] = item;
    _tempStringPosition++;
    //Menu Name
    if (item == '|'){
      tempString[_tempStringPosition-1] = '\0';
      strncpy(menuName,tempString,_maxStringSize);
      _tempStringPosition = 0;
    }
    //Value
    if (item == ';'){
      tempString[_tempStringPosition-1] = '\0';
      switch(_valueToParse){
        case 0:
          n = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 1:
          extDelay = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 2:
          extHold = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 3:
          delayBefore = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 4:
          shutterHold = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 5:
          delayBetween = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
      }
    }
    // Highlimit
    if (item == '>'){
      tempString[_tempStringPosition-1] = '\0';
      switch(_valueToParse){
        case 0:
          _nHigh = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 1:
          _extDelayHigh = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 2:
          _extHoldHigh = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 3:
          _delayBeforeHigh = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
       case 4:
          _extHoldHigh = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 5:
          _delayBetweenHigh = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
      }
    }
    // Lowlimit
    if (item == '<'){
      tempString[_tempStringPosition-1] = '\0';
      switch(_valueToParse){
        case 0:
          _nLow = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 1:
          _extDelayLow = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 2:
          _extHoldLow = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 3:
          _delayBeforeLow = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 4:
          _extHoldLow = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
        case 5:
          _delayBetweenLow = String(tempString).toInt();
          _tempStringPosition = 0;
          break;
      }
    }
    // Stepsize
    if (item == '_'){
      tempString[_tempStringPosition-1] = '\0';
      switch(_valueToParse){
        case 0:
          _nStep = String(tempString).toInt();
          _tempStringPosition = 0;
          _valueToParse++;
          break;
        case 1:
          _extDelayStep = String(tempString).toInt();
          _tempStringPosition = 0;
          _valueToParse++;
          break;
        case 2:
          _extHoldStep = String(tempString).toInt();
          _tempStringPosition = 0;
          _valueToParse++;
          break;
        case 3:
          _delayBeforeStep = String(tempString).toInt();
          _tempStringPosition = 0;
          _valueToParse++;
          break;
        case 4:
          _extHoldStep = String(tempString).toInt();
          _tempStringPosition = 0;
          _valueToParse++;
          break;
        case 5:
          _delayBetweenStep = String(tempString).toInt();
          _tempStringPosition = 0;
          _valueToParse++;
          break;
      }
    }
    if (item == ('\r')){
      _tempStringPosition = 0;
      _valueToParse = 0;

    }
    if (item == ('\n')){
      _tempStringPosition = 0;
      _valueToParse = 0;
      _menuParsing++;
    }
  }else{
    if (item == ('\r')){
      _tempStringPosition = 0;
      _valueToParse = 0;
    }
    if (item == ('\n')){
      _tempStringPosition = 0;
      _valueToParse = 0;
      _menuParsing++;
    }
  }
}

//Make photos
void Triggster::trigger(bool ext)
{
  if(ext = true)
  { delay(extDelay);
    digitalWrite(_extOutPin, HIGH);
    delay(extHold);
    digitalWrite(_extOutPin, LOW);
  }
  delay(delayBefore);
  for(int i = 0; i < n; i++ )
  {
    digitalWrite(_shutterPin, HIGH);
    delay(shutterHold);
    digitalWrite(_shutterPin, LOW);
    delay(delayBetween);
  }
}
