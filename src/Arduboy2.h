#ifndef ARDUBOY2_H
#define ARDUBOY2_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Arduboy2Core.h"
#include "Arduboy2Audio.h"
#include "Arduboy2Beep.h"
#include "Sprites.h"
#include "SpritesB.h"
#include <Print.h>


#define ARDUBOY_LIB_VER 60000

#define ARDUBOY_UNIT_NAME_LEN 6
#define ARDUBOY_UNIT_NAME_BUFFER_SIZE (ARDUBOY_UNIT_NAME_LEN + 1)


#define EEPROM_STORAGE_SPACE_START 16


#define BLACK 0 
#define WHITE 1  

#define INVERT 2

#define CLEAR_BUFFER true


//=============================================
//========== Rect (rectangle) object ==========
//=============================================

struct Rect
{
  int16_t x;      
  int16_t y;      
  uint8_t width;  
  uint8_t height;

  Rect() = default;

  constexpr Rect(int16_t x, int16_t y, uint8_t width, uint8_t height)
    : x(x), y(y), width(width), height(height)
  {
  }
};

//==================================
//========== Point object ==========
//==================================

struct Point
{
  int16_t x; 
  int16_t y;

  Point() = default;

  constexpr Point(int16_t x, int16_t y)
    : x(x), y(y)
  {
  }
};


//==================================
//========== Arduboy2Base ==========
//==================================

class Arduboy2Base : public Arduboy2Core
{
 friend class Arduboy2Ex;
 friend class Arduboy2Audio;

 public:

  static Arduboy2Audio audio;


  static void begin();
  static void beginDoFirst();


  static void flashlight();


  static void systemButtons();


  static void bootLogo();
  static void bootLogoCompressed();
  static void bootLogoSpritesSelfMasked();
  static void bootLogoSpritesOverwrite();
  static void bootLogoSpritesBSelfMasked();
  static void bootLogoSpritesBOverwrite();
  static bool bootLogoShell(void (&drawLogo)(int16_t));

 
  static void waitNoButtons();


  static void clear();

  static void fillScreen(uint8_t color = WHITE);

  
  static void display();
  static void display(bool clear);

  
  static void drawPixel(int16_t x, int16_t y, uint8_t color = WHITE);
  static uint8_t getPixel(uint8_t x, uint8_t y);


  static void drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color = WHITE);
  static void fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color = WHITE);

  static void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color = WHITE);
  
  static void drawFastVLine(int16_t x, int16_t y, uint8_t h, uint8_t color = WHITE);
  static void drawFastHLine(int16_t x, int16_t y, uint8_t w, uint8_t color = WHITE);

  static void drawRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color = WHITE);
  static void fillRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color = WHITE);

  static void drawRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color = WHITE);
  static void fillRoundRect(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color = WHITE);

  static void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color = WHITE);
  static void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color = WHITE);

  static void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color = WHITE);

  static void drawSlowXYBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color = WHITE);

  static void drawCompressed(int16_t sx, int16_t sy, const uint8_t *bitmap, uint8_t color = WHITE);

  
  static uint8_t* getBuffer();

  static void initRandomSeed();

  static void setFrameRate(uint8_t rate);
  static void setFrameDuration(uint8_t duration);


  static bool nextFrame();
  static bool nextFrameDEV();

  
  static bool everyXFrames(uint8_t frames);


  static int cpuLoad();


  static bool pressed(uint8_t buttons);
  static bool anyPressed(uint8_t buttons);
  static bool notPressed(uint8_t buttons);

  static void pollButtons();

  static bool justPressed(uint8_t button);
  static bool justReleased(uint8_t button);

  static bool collide(Point point, Rect rect);
  static bool collide(Rect rect1, Rect rect2);

  
  static uint16_t readUnitID();
  static void writeUnitID(uint16_t id);


  static uint8_t readUnitName(char* name);
  static void writeUnitName(const char* name);


  static bool readShowBootLogoFlag();
  static void writeShowBootLogoFlag(bool val);


  static bool readShowUnitNameFlag();
  static void writeShowUnitNameFlag(bool val);

  static bool readShowBootLogoLEDsFlag();
  static void writeShowBootLogoLEDsFlag(bool val);

  
  
  static uint16_t frameCount;
  static uint8_t currentButtonState;

  static uint8_t previousButtonState;

  static uint8_t sBuffer[(HEIGHT*WIDTH)/8];

  static const PROGMEM uint8_t arduboy_logo[];
  static const PROGMEM uint8_t arduboy_logo_compressed[];
  static const PROGMEM uint8_t arduboy_logo_sprite[];

 protected:
  static void sysCtrlSound(uint8_t buttons, uint8_t led, uint8_t eeVal);

  static void drawLogoBitmap(int16_t y);
  static void drawLogoCompressed(int16_t y);
  static void drawLogoSpritesSelfMasked(int16_t y);
  static void drawLogoSpritesOverwrite(int16_t y);
  static void drawLogoSpritesBSelfMasked(int16_t y);
  static void drawLogoSpritesBOverwrite(int16_t y);

  static void drawCircleHelper(int16_t x0, int16_t y0, uint8_t r, uint8_t corners,
                        uint8_t color = WHITE);

  static void fillCircleHelper(int16_t x0, int16_t y0, uint8_t r,
                        uint8_t sides, int16_t delta, uint8_t color = WHITE);


  class BitStreamReader;

  static void swapInt16(int16_t& a, int16_t& b);


  static uint8_t eachFrameMillis;
  static uint8_t thisFrameStart;
  static uint8_t lastFrameDurationMs;
  static bool justRendered;

  static constexpr uint16_t eepromSysFlags = 1;
  static constexpr uint16_t eepromAudioOnOff = 2;
  static constexpr uint16_t eepromUnitID = 8;
  static constexpr uint16_t eepromUnitName = 10;

  static constexpr uint8_t sysFlagUnameBit = 0;
  static constexpr uint8_t sysFlagUnameMask = _BV(sysFlagUnameBit);

  static constexpr uint8_t sysFlagShowLogoBit = 1;
  static constexpr uint8_t sysFlagShowLogoMask = _BV(sysFlagShowLogoBit);

  static constexpr uint8_t sysFlagShowLogoLEDsBit = 2;
  static constexpr uint8_t sysFlagShowLogoLEDsMask = _BV(sysFlagShowLogoLEDsBit);
};


//==============================
//========== Arduboy2 ==========
//==============================

class Arduboy2 : public Print, public Arduboy2Base
{
 friend class Arduboy2Ex;

 public:
  using Print::write;

  void begin();
  
  void bootLogo();
  void bootLogoCompressed();
  void bootLogoSpritesSelfMasked();
  void bootLogoSpritesOverwrite();
  void bootLogoSpritesBSelfMasked();
  void bootLogoSpritesBOverwrite();
  void bootLogoText();
  void bootLogoExtra();


  virtual size_t write(uint8_t);

  static void drawChar(int16_t x, int16_t y, uint8_t c, uint8_t color, uint8_t bg, uint8_t size);


  static void setCursor(int16_t x, int16_t y);
  
  static void setCursorX(int16_t x);
  static void setCursorY(int16_t y);
  
  static int16_t getCursorX();
  static int16_t getCursorY();


  static void setTextColor(uint8_t color);
  static uint8_t getTextColor();

  static void setTextBackground(uint8_t bg);
  static uint8_t getTextBackground();

  static void setTextSize(uint8_t s);
  static uint8_t getTextSize();

  static void setTextWrap(bool w);
  static bool getTextWrap();

  static void setTextRawMode(bool raw);
  static bool getTextRawMode();

  static void clear();

  
  static constexpr uint8_t getCharacterWidth(uint8_t textSize = 1) { return characterWidth * textSize; }
  static constexpr uint8_t getCharacterSpacing(uint8_t textSize = 1) { return characterSpacing * textSize; }
  static constexpr uint8_t getCharacterHeight(uint8_t textSize = 1) { return characterHeight * textSize; }
  static constexpr uint8_t getLineSpacing(uint8_t textSize = 1) { return lineSpacing * textSize; }

  static const PROGMEM uint8_t font5x7[];

 protected:
  static int16_t cursor_x;
  static int16_t cursor_y;
  static uint8_t textColor;
  static uint8_t textBackground;
  static uint8_t textSize;
  static bool textWrap;
  static bool textRaw;


  static constexpr uint8_t characterWidth = 5;
  static constexpr uint8_t characterHeight = 8;
  static constexpr uint8_t characterSpacing = 1;
  static constexpr uint8_t lineSpacing = 0;
  static constexpr uint8_t fullCharacterWidth = characterWidth + characterSpacing;
  static constexpr uint8_t fullCharacterHeight = characterHeight + lineSpacing;
};

#endif
