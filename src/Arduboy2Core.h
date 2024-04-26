#ifndef ARDUBOY2_CORE_H
#define ARDUBOY2_CORE_H

#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>



#if !defined(ARDUBOY_10) && !defined(AB_DEVKIT)

#define ARDUBOY_10   
// #define AB_DEVKIT    //< compile for the official dev kit
#endif

#define RGB_ON LOW   
#define RGB_OFF HIGH 

// ----- Arduboy pins -----
#ifdef ARDUBOY_10

#define PIN_CS 12      
#define CS_PORT PORTD  
#define CS_BIT PORTD6   

#define PIN_DC 4       
#define DC_PORT PORTD  
#define DC_BIT PORTD4  

#define PIN_RST 6       
#define RST_PORT PORTD  
#define RST_BIT PORTD7  

#define RED_LED 10   /**< The pin number for the red color in the RGB LED. */
#define GREEN_LED 11 /**< The pin number for the greem color in the RGB LED. */
#define BLUE_LED 9   /**< The pin number for the blue color in the RGB LED. */

#define RED_LED_PORT PORTB
#define RED_LED_BIT PORTB6

#define GREEN_LED_PORT PORTB
#define GREEN_LED_BIT PORTB7

#define BLUE_LED_PORT PORTB
#define BLUE_LED_BIT PORTB5

// bit values for button states
// these are determined by the buttonsState() function
#define LEFT_BUTTON _BV(5)  /**< The Left button value for functions requiring a bitmask */
#define RIGHT_BUTTON _BV(6) /**< The Right button value for functions requiring a bitmask */
#define UP_BUTTON _BV(7)    /**< The Up button value for functions requiring a bitmask */
#define DOWN_BUTTON _BV(4)  /**< The Down button value for functions requiring a bitmask */
#define A_BUTTON _BV(3)     /**< The A button value for functions requiring a bitmask */
#define B_BUTTON _BV(2)     /**< The B button value for functions requiring a bitmask */

#define PIN_LEFT_BUTTON A2
#define LEFT_BUTTON_PORT PORTF
#define LEFT_BUTTON_PORTIN PINF
#define LEFT_BUTTON_DDR DDRF
#define LEFT_BUTTON_BIT PORTF5

#define PIN_RIGHT_BUTTON A1
#define RIGHT_BUTTON_PORT PORTF
#define RIGHT_BUTTON_PORTIN PINF
#define RIGHT_BUTTON_DDR DDRF
#define RIGHT_BUTTON_BIT PORTF6

#define PIN_UP_BUTTON A0
#define UP_BUTTON_PORT PORTF
#define UP_BUTTON_PORTIN PINF
#define UP_BUTTON_DDR DDRF
#define UP_BUTTON_BIT PORTF7

#define PIN_DOWN_BUTTON A3
#define DOWN_BUTTON_PORT PORTF
#define DOWN_BUTTON_PORTIN PINF
#define DOWN_BUTTON_DDR DDRF
#define DOWN_BUTTON_BIT PORTF4

#define PIN_A_BUTTON 7
#define A_BUTTON_PORT PORTE
#define A_BUTTON_PORTIN PINE
#define A_BUTTON_DDR DDRE
#define A_BUTTON_BIT PORTE6

#define PIN_B_BUTTON 8
#define B_BUTTON_PORT PORTB
#define B_BUTTON_PORTIN PINB
#define B_BUTTON_DDR DDRB
#define B_BUTTON_BIT PORTB4

#define PIN_SPEAKER_1 5  /**< The pin number of the first lead of the speaker */
#define PIN_SPEAKER_2 13 /**< The pin number of the second lead of the speaker */

#define SPEAKER_1_PORT PORTC
#define SPEAKER_1_DDR DDRC
#define SPEAKER_1_BIT PORTC6

#define SPEAKER_2_PORT PORTC
#define SPEAKER_2_DDR DDRC
#define SPEAKER_2_BIT PORTC7
// -----------------------

// ----- DevKit pins -----
#elif defined(AB_DEVKIT)

#define PIN_CS 6        
#define CS_PORT PORTD   
#define CS_BIT PORTD7   

#define PIN_DC 4        
#define DC_PORT PORTD   
#define DC_BIT PORTD4  

#define PIN_RST 12      
#define RST_PORT PORTD  
#define RST_BIT PORTD6  


#define RED_LED 17
#define GREEN_LED 17
#define BLUE_LED 17

#define BLUE_LED_PORT PORTB
#define BLUE_LED_BIT PORTB0


#define LEFT_BUTTON _BV(5)
#define RIGHT_BUTTON _BV(2)
#define UP_BUTTON _BV(4)
#define DOWN_BUTTON _BV(6)
#define A_BUTTON _BV(1)
#define B_BUTTON _BV(0)

#define PIN_LEFT_BUTTON 9
#define LEFT_BUTTON_PORT PORTB
#define LEFT_BUTTON_PORTIN PINB
#define LEFT_BUTTON_DDR DDRB
#define LEFT_BUTTON_BIT PORTB5

#define PIN_RIGHT_BUTTON 5
#define RIGHT_BUTTON_PORT PORTC
#define RIGHT_BUTTON_PORTIN PINC
#define RIGHT_BUTTON_DDR DDRC
#define RIGHT_BUTTON_BIT PORTC6

#define PIN_UP_BUTTON 8
#define UP_BUTTON_PORT PORTB
#define UP_BUTTON_PORTIN PINB
#define UP_BUTTON_DDR DDRB
#define UP_BUTTON_BIT PORTB4

#define PIN_DOWN_BUTTON 10
#define DOWN_BUTTON_PORT PORTB
#define DOWN_BUTTON_PORTIN PINB
#define DOWN_BUTTON_DDR DDRB
#define DOWN_BUTTON_BIT PORTB6

#define PIN_A_BUTTON A0
#define A_BUTTON_PORT PORTF
#define A_BUTTON_PORTIN PINF
#define A_BUTTON_DDR DDRF
#define A_BUTTON_BIT PORTF7

#define PIN_B_BUTTON A1
#define B_BUTTON_PORT PORTF
#define B_BUTTON_PORTIN PINF
#define B_BUTTON_DDR DDRF
#define B_BUTTON_BIT PORTF6

#define PIN_SPEAKER_1 A2
#define SPEAKER_1_PORT PORTF
#define SPEAKER_1_DDR DDRF
#define SPEAKER_1_BIT PORTF5

#endif
// --------------------

// ----- Pins common on Arduboy and DevKit -----

// Unconnected analog input used for noise by generateRandomSeed()
#define RAND_SEED_IN A4
#define RAND_SEED_IN_PORT PORTF
#define RAND_SEED_IN_BIT PORTF1
// Value for ADMUX to read the random seed pin: 2.56V reference, ADC1
#define RAND_SEED_IN_ADMUX (_BV(REFS0) | _BV(REFS1) | _BV(MUX0))

// SPI interface
#define SPI_MISO_PORT PORTB
#define SPI_MISO_BIT PORTB3

#define SPI_MOSI_PORT PORTB
#define SPI_MOSI_BIT PORTB2

#define SPI_SCK_PORT PORTB
#define SPI_SCK_BIT PORTB1

#define SPI_SS_PORT PORTB
#define SPI_SS_BIT PORTB0
// --------------------

// OLED hardware (SSD1306)

#define OLED_PIXELS_INVERTED 0xA7 
#define OLED_PIXELS_NORMAL 0xA6 

#define OLED_ALL_PIXELS_ON 0xA5 
#define OLED_PIXELS_FROM_RAM 0xA4 

#define OLED_VERTICAL_FLIPPED 0xC0 
#define OLED_VERTICAL_NORMAL 0xC8

#define OLED_HORIZ_FLIPPED 0xA0
#define OLED_HORIZ_NORMAL 0xA1

// -----

#define WIDTH 128 
#define HEIGHT 64 

#define COLUMN_ADDRESS_END (WIDTH - 1) & 127   // 128 pixels wide
#define PAGE_ADDRESS_END ((HEIGHT/8)-1) & 7    // 8 pages high


#define ARDUBOY_NO_USB int main() __attribute__ ((OS_main)); \
int main() { \
  Arduboy2NoUSB::mainNoUSB(); \
  return 0; \
}


class Arduboy2NoUSB
{
  friend int main();

  private:
    static void mainNoUSB();
};


#define SDA_PIN 2
#define SCL_PIN 3
class Arduboy2Core : public Arduboy2NoUSB
{
  friend class Arduboy2Ex;

  public:
    static void idle();
	
    static void LCDDataMode();
    static void LCDCommandMode();

    static void SPItransfer(uint8_t data);
    static uint8_t SPItransferAndRead(uint8_t data);

    static void displayOff();
    static void displayOn();


    static constexpr uint8_t width() { return WIDTH; }
    static constexpr uint8_t height() { return HEIGHT; }


    static uint8_t buttonsState();


    static void paint8Pixels(uint8_t pixels);
    static void paintScreen(const uint8_t *image);
    static void paintScreen(uint8_t image[], bool clear = false);

	
    static void blank();

    static void invert(bool inverse);

    static void allPixelsOn(bool on);

    static void flipVertical(bool flipped);
    static void flipHorizontal(bool flipped);


    static void sendLCDCommand(uint8_t command);


    static void setRGBled(uint8_t red, uint8_t green, uint8_t blue);
    static void setRGBled(uint8_t color, uint8_t val);

    static void freeRGBled();

    static void digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue);
    static void digitalWriteRGB(uint8_t color, uint8_t val);

    static void boot();

    static void safeMode();

    static unsigned long generateRandomSeed();

    static void delayShort(uint16_t ms) __attribute__ ((noinline));

    static void exitToBootloader();

	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// >>>>>>>>>>>> I2C & OLED TRASH >>>>>>>>>>>>
	// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	// I2C
	static void I2C_bootPins();
	
	static void I2C_startCondition();
	static void I2C_stopCondition();
	
	static void I2C_send_1();
	static void I2C_send_0();
	
	static void I2C_write(uint8_t data);
	
	// OLED
	static void I2C_bootOLED();
	
	static void I2C_paintScreen(const uint8_t *image);
	static void I2C_paintScreen(uint8_t image[], bool clear = false);
	
	static void I2C_clear();
	
	
  protected:
    static void setCPUSpeed8MHz();
    static void bootSPI();
    static void bootOLED();
    static void bootPins();
    static void bootPowerSaving();

    static const PROGMEM uint8_t lcdBootProgram[];
};

#endif
