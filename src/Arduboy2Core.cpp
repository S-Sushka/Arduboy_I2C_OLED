#include "Arduboy2Core.h"

#include <avr/wdt.h>


//========================================
//========== class Arduboy2Core ==========
//========================================

const PROGMEM uint8_t Arduboy2Core::lcdBootProgram[] = { 0xD5, 0xF0, 0x8D, 0x14, 0xA1, 0xC8, 0x81, 0xCF, 0xD9, 0xF1, 0xAF, 0x20, 0x00 };


void Arduboy2Core::boot()
{
  #ifdef ARDUBOY_SET_CPU_8MHZ
  setCPUSpeed8MHz();
  #endif

  ADMUX = RAND_SEED_IN_ADMUX;

  bootPins();
  bootSPI();
  bootOLED();
  bootPowerSaving();
}

#ifdef ARDUBOY_SET_CPU_8MHZ
void Arduboy2Core::setCPUSpeed8MHz()
{
  uint8_t oldSREG = SREG;
  cli();                // suspend interrupts
  PLLCSR = _BV(PINDIV); // dissable the PLL and set prescale for 16MHz)
  CLKPR = _BV(CLKPCE);  // allow reprogramming clock
  CLKPR = 1;            // set clock divisor to 2 (0b0001)
  PLLCSR = _BV(PLLE) | _BV(PINDIV); // enable the PLL (with 16MHz prescale)
  SREG = oldSREG;       // restore interrupts
}
#endif

void Arduboy2Core::bootPins()
{
#ifdef ARDUBOY_10

  // Port B INPUT_PULLUP or HIGH
  PORTB |= _BV(RED_LED_BIT) | _BV(GREEN_LED_BIT) | _BV(BLUE_LED_BIT) |
           _BV(B_BUTTON_BIT);
  // Port B INPUT or LOW (none)
  // Port B inputs
  DDRB &= ~(_BV(B_BUTTON_BIT) | _BV(SPI_MISO_BIT));
  // Port B outputs
  DDRB |= _BV(RED_LED_BIT) | _BV(GREEN_LED_BIT) | _BV(BLUE_LED_BIT) |
          _BV(SPI_MOSI_BIT) | _BV(SPI_SCK_BIT) | _BV(SPI_SS_BIT);

  // Port C
  // Speaker: Not set here. Controlled by audio class

  // Port D INPUT_PULLUP or HIGH
  PORTD |= _BV(CS_BIT);
  // Port D INPUT or LOW
  PORTD &= ~(_BV(RST_BIT));
  // Port D inputs (none)
  // Port D outputs
  DDRD |= _BV(RST_BIT) | _BV(CS_BIT) | _BV(DC_BIT);

  // Port E INPUT_PULLUP or HIGH
  PORTE |= _BV(A_BUTTON_BIT);
  // Port E INPUT or LOW (none)
  // Port E inputs
  DDRE &= ~(_BV(A_BUTTON_BIT));
  // Port E outputs (none)

  // Port F INPUT_PULLUP or HIGH
  PORTF |= _BV(LEFT_BUTTON_BIT) | _BV(RIGHT_BUTTON_BIT) |
           _BV(UP_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT);
  // Port F INPUT or LOW
  PORTF &= ~(_BV(RAND_SEED_IN_BIT));
  // Port F inputs
  DDRF &= ~(_BV(LEFT_BUTTON_BIT) | _BV(RIGHT_BUTTON_BIT) |
            _BV(UP_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT) |
            _BV(RAND_SEED_IN_BIT));
  // Port F outputs (none)

#elif defined(AB_DEVKIT)

  // Port B INPUT_PULLUP or HIGH
  PORTB |= _BV(LEFT_BUTTON_BIT) | _BV(UP_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT) |
           _BV(BLUE_LED_BIT);
  // Port B INPUT or LOW (none)
  // Port B inputs
  DDRB &= ~(_BV(LEFT_BUTTON_BIT) | _BV(UP_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT) |
            _BV(SPI_MISO_BIT));
  // Port B outputs
  DDRB |= _BV(SPI_MOSI_BIT) | _BV(SPI_SCK_BIT) | _BV(SPI_SS_BIT) |
          _BV(BLUE_LED_BIT);

  // Port C INPUT_PULLUP or HIGH
  PORTC |= _BV(RIGHT_BUTTON_BIT);
  // Port C INPUT or LOW (none)
  // Port C inputs
  DDRC &= ~(_BV(RIGHT_BUTTON_BIT));
  // Port C outputs (none)

  // Port D INPUT_PULLUP or HIGH
  PORTD |= _BV(CS_BIT);
  // Port D INPUT or LOW
  PORTD &= ~(_BV(RST_BIT));
  // Port D inputs (none)
  // Port D outputs
  DDRD |= _BV(RST_BIT) | _BV(CS_BIT) | _BV(DC_BIT);

  // Port E (none)

  // Port F INPUT_PULLUP or HIGH
  PORTF |= _BV(A_BUTTON_BIT) | _BV(B_BUTTON_BIT);
  // Port F INPUT or LOW
  PORTF &= ~(_BV(RAND_SEED_IN_BIT));
  // Port F inputs
  DDRF &= ~(_BV(A_BUTTON_BIT) | _BV(B_BUTTON_BIT) | _BV(RAND_SEED_IN_BIT));
  // Port F outputs (none)
  // Speaker: Not set here. Controlled by audio class

#endif
}

void Arduboy2Core::bootOLED()
{
  I2C_bootOLED();									// <<<<<<<<<<<<<<<< I2C
	
  delayShort(5); 
  bitSet(RST_PORT, RST_BIT); 
  delayShort(5); 

  bitClear(CS_PORT, CS_BIT);

  LCDCommandMode();
  for (uint8_t i = 0; i < sizeof(lcdBootProgram); i++) 
    SPItransfer(pgm_read_byte(lcdBootProgram + i));
  LCDDataMode();
}

void Arduboy2Core::LCDDataMode()
{
  bitSet(DC_PORT, DC_BIT);
}

void Arduboy2Core::LCDCommandMode()
{
  bitClear(DC_PORT, DC_BIT);
}

void Arduboy2Core::bootSPI()
{
  SPCR = _BV(SPE) | _BV(MSTR);
  SPSR = _BV(SPI2X);
}


void Arduboy2Core::SPItransfer(uint8_t data)
{
  SPDR = data;
  asm volatile("nop");
  while (!(SPSR & _BV(SPIF))) { } // wait
}


uint8_t Arduboy2Core::SPItransferAndRead(uint8_t data)
{
  SPItransfer(data);
  return SPDR;
}

void Arduboy2Core::safeMode()
{
  if (buttonsState() == UP_BUTTON)
  {
    digitalWriteRGB(RED_LED, RGB_ON);

#ifndef ARDUBOY_CORE 
    power_timer0_disable();
#endif

    while (true) { }
  }
}


/* Power Management */

void Arduboy2Core::idle()
{
  SMCR = _BV(SE);
  sleep_cpu();
  SMCR = 0;
}

void Arduboy2Core::bootPowerSaving()
{
  PRR0 = _BV(PRTWI) | _BV(PRADC);
  // disable USART1
  PRR1 |= _BV(PRUSART1);
}

// Shut down the display
void Arduboy2Core::displayOff()
{
  LCDCommandMode();
  SPItransfer(0xAE); // display off
  SPItransfer(0x8D); // charge pump:
  SPItransfer(0x10); //   disable
  delayShort(250);
  bitClear(RST_PORT, RST_BIT); // set display reset pin low (reset state)
}


void Arduboy2Core::displayOn()
{
  bootOLED();
}


/* Drawing */

void Arduboy2Core::paint8Pixels(uint8_t pixels)
{
  SPItransfer(pixels);
}



void Arduboy2Core::paintScreen(const uint8_t *image)
{
  I2C_paintScreen(image);															// <<<<<<<<<<<<<<<< I2C
  
/*   for (int i = 0; i < (HEIGHT*WIDTH)/8; i++)
    SPItransfer(pgm_read_byte(image + i));   */
}

#if 0
void Arduboy2Core::paintScreen(uint8_t image[], bool clear)
{
  uint16_t count;

  asm volatile (
    "   ldi   %A[count], %[len_lsb]               \n\t" //for (len = WIDTH * HEIGHT / 8)
    "   ldi   %B[count], %[len_msb]               \n\t"
    "1: ld    __tmp_reg__, %a[ptr]      ;2        \n\t" //tmp = *(image)
    "   out   %[spdr], __tmp_reg__      ;1        \n\t" //SPDR = tmp
    "   cpse  %[clear], __zero_reg__    ;1/2      \n\t" //if (clear) tmp = 0;
    "   mov   __tmp_reg__, __zero_reg__ ;1        \n\t"
    "2: sbiw  %A[count], 1              ;2        \n\t" //len --
    "   sbrc  %A[count], 0              ;1/2      \n\t" //loop twice for cheap delay
    "   rjmp  2b                        ;2        \n\t"
    "   st    %a[ptr]+, __tmp_reg__     ;2        \n\t" //*(image++) = tmp
    "   brne  1b                        ;1/2 :18  \n\t" //len > 0
    "   in    __tmp_reg__, %[spsr]                \n\t" //read SPSR to clear SPIF
    : [ptr]     "+&e" (image),
      [count]   "=&w" (count)
    : [spdr]    "I"   (_SFR_IO_ADDR(SPDR)),
      [spsr]    "I"   (_SFR_IO_ADDR(SPSR)),
      [len_msb] "M"   (WIDTH * (HEIGHT / 8 * 2) >> 8),   // 8: pixels per byte
      [len_lsb] "M"   (WIDTH * (HEIGHT / 8 * 2) & 0xFF), // 2: for delay loop multiplier
      [clear]   "r"   (clear)
  );
}
#endif

void Arduboy2Core::paintScreen(uint8_t image[], bool clear)
{
  uint8_t c;
  int i = 0;

  I2C_paintScreen(image, clear);								// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< I2C
  
  // if (clear)
  // {
    // SPDR = image[i]; 
    // image[i++] = 0;  
  // }
  // else
    // SPDR = image[i++];

  // while (i < (HEIGHT * WIDTH) / 8)
  // {
    // if (clear)
    // {
      // c = image[i];
      // image[i++] = 0;
    // }
    // else
      // c = image[i++];

    // while (!(SPSR & _BV(SPIF)));

    // SPDR = c;
  // }
  // while (!(SPSR & _BV(SPIF)));
}



void Arduboy2Core::blank()
{
  for (int i = 0; i < (HEIGHT*WIDTH)/8; i++)
    SPItransfer(0x00);
}

void Arduboy2Core::sendLCDCommand(uint8_t command)
{
  LCDCommandMode();
  SPItransfer(command);
  LCDDataMode();
}


void Arduboy2Core::invert(bool inverse)
{
  sendLCDCommand(inverse ? OLED_PIXELS_INVERTED : OLED_PIXELS_NORMAL);
}

void Arduboy2Core::allPixelsOn(bool on)
{
  sendLCDCommand(on ? OLED_ALL_PIXELS_ON : OLED_PIXELS_FROM_RAM);
}


void Arduboy2Core::flipVertical(bool flipped)
{
  sendLCDCommand(flipped ? OLED_VERTICAL_FLIPPED : OLED_VERTICAL_NORMAL);
}

// flip the display horizontally or set to normal
void Arduboy2Core::flipHorizontal(bool flipped)
{
  sendLCDCommand(flipped ? OLED_HORIZ_FLIPPED : OLED_HORIZ_NORMAL);
}

/* RGB LED */

void Arduboy2Core::setRGBled(uint8_t red, uint8_t green, uint8_t blue)
{
#ifdef ARDUBOY_10 
  TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
  OCR0A = 255 - green;

  TCCR1A = _BV(COM1A1) | _BV(COM1A0) | _BV(COM1B1) | _BV(COM1B0) | _BV(WGM10);
  OCR1AL = blue;
  OCR1BL = red;
#elif defined(AB_DEVKIT)
  (void)red;    // parameter unused
  (void)green;  // parameter unused
  bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, blue ? RGB_ON : RGB_OFF);
#endif
}

void Arduboy2Core::setRGBled(uint8_t color, uint8_t val)
{
#ifdef ARDUBOY_10
  if (color == RED_LED)
  {
    OCR1BL = val;
  }
  else if (color == GREEN_LED)
  {
    OCR0A = 255 - val;
  }
  else if (color == BLUE_LED)
  {
    OCR1AL = val;
  }
#elif defined(AB_DEVKIT)
  if (color == BLUE_LED)
  {
    bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, val ? RGB_ON : RGB_OFF);
  }
#endif
}

void Arduboy2Core::freeRGBled()
{
#ifdef ARDUBOY_10
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  TCCR1A = _BV(WGM10);
#endif
}

void Arduboy2Core::digitalWriteRGB(uint8_t red, uint8_t green, uint8_t blue)
{
#ifdef ARDUBOY_10
  bitWrite(RED_LED_PORT, RED_LED_BIT, red);
  bitWrite(GREEN_LED_PORT, GREEN_LED_BIT, green);
  bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, blue);
#elif defined(AB_DEVKIT)
  // only blue on DevKit
  (void)red;    // parameter unused
  (void)green;  // parameter unused
  bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, blue);
#endif
}

void Arduboy2Core::digitalWriteRGB(uint8_t color, uint8_t val)
{
#ifdef ARDUBOY_10
  if (color == RED_LED)
  {
    bitWrite(RED_LED_PORT, RED_LED_BIT, val);
  }
  else if (color == GREEN_LED)
  {
    bitWrite(GREEN_LED_PORT, GREEN_LED_BIT, val);
  }
  else if (color == BLUE_LED)
  {
    bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, val);
  }
#elif defined(AB_DEVKIT)
  // only blue on DevKit
  if (color == BLUE_LED)
  {
    bitWrite(BLUE_LED_PORT, BLUE_LED_BIT, val);
  }
#endif
}

/* Buttons */

uint8_t Arduboy2Core::buttonsState()
{
  uint8_t buttons;

#ifdef ARDUBOY_10
  // up, right, left, down
  buttons = ((~PINF) &
              (_BV(UP_BUTTON_BIT) | _BV(RIGHT_BUTTON_BIT) |
               _BV(LEFT_BUTTON_BIT) | _BV(DOWN_BUTTON_BIT)));
  // A
  if (bitRead(A_BUTTON_PORTIN, A_BUTTON_BIT) == 0) { buttons |= A_BUTTON; }
  // B
  if (bitRead(B_BUTTON_PORTIN, B_BUTTON_BIT) == 0) { buttons |= B_BUTTON; }
#elif defined(AB_DEVKIT)
  // down, left, up
  buttons = ((~PINB) &
              (_BV(DOWN_BUTTON_BIT) | _BV(LEFT_BUTTON_BIT) | _BV(UP_BUTTON_BIT)));
  // right
  if (bitRead(RIGHT_BUTTON_PORTIN, RIGHT_BUTTON_BIT) == 0) { buttons |= RIGHT_BUTTON; }
  // A
  if (bitRead(A_BUTTON_PORTIN, A_BUTTON_BIT) == 0) { buttons |= A_BUTTON; }
  // B
  if (bitRead(B_BUTTON_PORTIN, B_BUTTON_BIT) == 0) { buttons |= B_BUTTON; }
#endif

  return buttons;
}

unsigned long Arduboy2Core::generateRandomSeed()
{
  unsigned long seed;

  power_adc_enable(); // ADC on

  ADCSRA |= _BV(ADSC); 
  while (bit_is_set(ADCSRA, ADSC)) { } 

  seed = ((unsigned long)ADC << 16) + micros();

  power_adc_disable(); // ADC off

  return seed;
}

// delay in ms with 16 bit duration
void Arduboy2Core::delayShort(uint16_t ms)
{
  delay((unsigned long) ms);
}

void Arduboy2Core::exitToBootloader()
{
  cli();

  *(uint8_t *)MAGIC_KEY_POS = lowByte(MAGIC_KEY);
  *(uint8_t *)(MAGIC_KEY_POS + 1) = highByte(MAGIC_KEY);

  wdt_reset();
  WDTCSR = (_BV(WDCE) | _BV(WDE));
  WDTCSR = _BV(WDE);
  while (true) { }
}


//=========================================
//========== class Arduboy2NoUSB ==========
//=========================================

void Arduboy2NoUSB::mainNoUSB()
{
  // disable USB
  UDCON = _BV(DETACH);
  UDIEN = 0;
  UDINT = 0;
  USBCON = _BV(FRZCLK);
  UHWCON = 0;
  power_usb_disable();

  init();

  TX_RX_LED_INIT;
  TXLED0;
  RXLED0;

  bitSet(DOWN_BUTTON_PORT, DOWN_BUTTON_BIT);
  bitClear(DOWN_BUTTON_DDR, DOWN_BUTTON_BIT);

  Arduboy2Core::delayShort(10);

  // if the DOWN button is pressed
  if (bitRead(DOWN_BUTTON_PORTIN, DOWN_BUTTON_BIT) == 0) {
    Arduboy2Core::exitToBootloader();
  }

  setup();

  for (;;) {
    loop();
//    if (serialEventRun) serialEventRun();
  }

//  return 0;
}





// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>> I2C & OLED TRASH >>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// >>>>>>>>>>>> I2C

void Arduboy2Core::I2C_bootPins() 
{
	pinMode(SDA_PIN, OUTPUT);
	pinMode(SCL_PIN, OUTPUT);
}

void Arduboy2Core::I2C_startCondition() 
{
	// RESET LINES
	digitalWrite(SCL_PIN, 0);
	digitalWrite(SDA_PIN, 0);

	digitalWrite(SCL_PIN, 1);
	
	digitalWrite(SDA_PIN, 1);
	digitalWrite(SDA_PIN, 0);
	
	digitalWrite(SCL_PIN, 0);
}


// SDA - 0b00000010
// SCL - 0b00000001
void Arduboy2Core::I2C_stopCondition() 
{
	// RESET LINES
	digitalWrite(SCL_PIN, 0);
	digitalWrite(SDA_PIN, 0);
	
	
	digitalWrite(SCL_PIN, 1);
	
	digitalWrite(SDA_PIN, 0);
	digitalWrite(SDA_PIN, 1);
	
	digitalWrite(SCL_PIN, 0); 
	digitalWrite(SDA_PIN, 0); // !
}


void Arduboy2Core::I2C_send_0() 
{
	PORTD &= ~0b00000010;
	//digitalWrite(SDA_PIN, 0);
	
	// LATCH
	PORTD |= 0b00000001;
	PORTD &= ~0b00000001;
	//digitalWrite(SCL_PIN, 1);
	//digitalWrite(SCL_PIN, 0);
}

void Arduboy2Core::I2C_send_1() 
{	
	PORTD |= 0b00000010;
	//digitalWrite(SDA_PIN, 1);
	
	// LATCH
	PORTD |= 0b00000001;
	PORTD &= ~0b00000001;
	//digitalWrite(SCL_PIN, 1);
	//digitalWrite(SCL_PIN, 0);
}

void Arduboy2Core::I2C_write(uint8_t data) 
{
	uint8_t mask = 0x80;
	
	for (uint8_t i = 0; i < 8; i++) 
	{	
		if ((data & mask) == 0) 
			I2C_send_0(); 
		else 
			I2C_send_1();
		
		mask >>= 1;
	} 
	
	// ACK
	PORTD &= ~0b00000010;
	//digitalWrite(SDA_PIN, 0);
	
	PORTD |= 0b00000001;
	PORTD &= ~0b00000001;
	//digitalWrite(SCL_PIN, 1);
	//digitalWrite(SCL_PIN, 0); 
}


// >>>>>>>>>>>> OLED


void Arduboy2Core::I2C_bootOLED() 
{
    I2C_bootPins();

    I2C_startCondition();
    I2C_write(0x78);
    I2C_write(0);

  for (uint8_t i = 0; i < sizeof(lcdBootProgram); i++) 
    I2C_write(pgm_read_byte(lcdBootProgram + i));
  
    I2C_stopCondition();  	
}


void Arduboy2Core::I2C_paintScreen(const uint8_t *image) 
{
    I2C_startCondition();
    I2C_write(0x78);
    I2C_write(0b01000000);
	
	for (int i = 0; i < 1024; i++)
		I2C_write(0x00);  
	
	I2C_stopCondition();
}

void Arduboy2Core::I2C_paintScreen(uint8_t image[], bool clear) 
{
	uint8_t c;
	int i = 0;

	I2C_startCondition();
    I2C_write(0x78);
	I2C_write(0b01000000);
  
  if (clear)
  {
    I2C_write(image[i]); 
    image[i++] = 0;  
  }
  else
    I2C_write(image[i++]);

  while (i < (HEIGHT * WIDTH) / 8)
  {
    if (clear)
    {
      c = image[i];
      image[i++] = 0;
    }
    else
      c = image[i++];

    I2C_write(c);
  }
  
  
  I2C_stopCondition();
}


void Arduboy2Core::I2C_clear() 
{
    I2C_startCondition();
    I2C_write(0x78);
    I2C_write(0b01000000);

    for (int i = 0; i < 512; i++)
        I2C_write(0xAA);
    
    I2C_stopCondition();
}