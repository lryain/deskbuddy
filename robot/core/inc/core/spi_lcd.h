#ifndef SPI_LCD_H
#define SPI_LCD_H
#include <stdint.h>  // 添加这行来支持 uint16_t 类型
//
// Treat the LCD as a 240x320 portrait-mode image
// or a 320x240 landscape mode image
// This affects the coordinate system and rotates the
// drawing direction of fonts and tiles
//
#define LCD_ORIENTATION_NATIVE 1
#define LCD_ORIENTATION_ROTATED 2

#define LCD_GC9A01 1
#define LCD_ST7735 2
#define LCD_SSD1351 3   // OLED
#define LCD_ST7789 4
#define LCD_ILI9341 5

// touch panel types
#define TOUCH_XPT2046 1

// #define LCD LCD_ST7789
// #define LCD LCD_ILI9341
#define LCD LCD_GC9A01

static int MAX_TRANSFER = 0x1000;
static const int PWM_FREQUENCY = 1000; // PWM频率 (1kHz，适合背光调节)
static const int RPM_MAX             = 200;  // Noctua Specs: Max=5000
static const int RPM_MIN             = 50;  // Noctua Specs: Min=1000 [Kept 1500 as Min]

#ifndef LCD_ILI9341
#define LCD_FRAME_WIDTH    240
#define LCD_FRAME_HEIGHT   240
#endif

#ifdef LCD_ILI9341
#define LCD_FRAME_WIDTH    240
#define LCD_FRAME_HEIGHT   320
#endif

#define width    LCD_FRAME_WIDTH
#define height   LCD_FRAME_HEIGHT
#define FRAME_WORDS (LCD_FRAME_WIDTH*LCD_FRAME_HEIGHT)
#define FALSE 0
#define TRUE (!FALSE)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LcdFrame_t {
  uint16_t data[LCD_FRAME_WIDTH*LCD_FRAME_HEIGHT];
} LcdFrame;

enum LcdColor {
  lcd_BLACK   = 0x0000,
  lcd_BLUE    = 0x001F,
  lcd_GREEN   = 0x07E0,
  lcd_CYAN    = 0x7FFF,
  lcd_GRAY    = 0x8430,
  lcd_RED     = 0xF800,
  lcd_MAGENTA = 0xF81F,
  lcd_YELLOW  = 0xFFE0,
  lcd_WHITE   = 0xFFFF,
};

typedef enum
{
 MODE_DATA = 0,
 MODE_COMMAND
} DC_MODE;

// for vector compatibility
int lcd_init(void);
static void spilcdSetPosition(int x, int y, int w, int h);
static void spilcdWriteDataBlock(unsigned char *ucBuf, int iLen);
static void spilcdWriteData8(unsigned char c);
static void spilcdWriteData16(unsigned short us);
static void spilcdWriteCommand(unsigned char c);
void spilcdWriteDataBlock(unsigned char *pData, int iLen);
int spilcdFill(unsigned short usData);
void draw_animate(void* frame);
void spilcdWriteDataBlock2(unsigned char *ucBuf, int iLen);
void lcd_draw_frame(const LcdFrame* frame);
void lcd_draw_frame2(const uint16_t* frame, size_t size);
void lcd_draw_frame1(const uint16_t* frame, size_t size);
int lcd_write_buffer(uint16_t *buffer, int size);
// void lcd_spi_transfer(const void *data, int bytes);
static void lcd_spi_transfer(int cmd, int bytes, const void* data);
// int spilcdFills(unsigned short* usData);
// 
int lcd_set_brightness(const int brightness);
void lcd_shutdown(void);
void lcd_clear_screen(void);
static void myPinWrite(int iPin, int iValue);
int setLCDLight(int pin, int pwm);
static int _lcd_set_brightness(const int brightness);
int spilcdFill(unsigned short usData);
int spilcdFills(uint16_t *usData);
// end of vector compatibility

// Sets the D/C pin to data or command mode
void spilcdSetMode(int iMode);

//
// Choose the gamma curve between 2 choices (0/1)
// ILI9341 only
//
int spilcdSetGamma(int iMode);

// Initialize the library
int spilcdInit(int iLCDType, int bFlipped, int iSPIChannel, int iSPIFreq, int iDCPin, int iResetPin, int iLEDPin);

// Turns off the display and frees the resources
void spilcdShutdown(void);

// Fills the display with the byte pattern
int spilcdFill(unsigned short usPattern);

//
// Draw a rectangle and optionally fill it
//
void spilcdRectangle(int x, int y, int w, int h, unsigned short usColor, int bFill);

//
// Reset the scroll position to 0
//
void spilcdScrollReset(void);

// Configure a GPIO pin for input
// Returns 0 if successful, -1 if unavailable
int spilcdConfigurePin(int iPin);

// Read from a GPIO pin
int spilcdReadPin(int iPin);

//
// Scroll the screen N lines vertically (positive or negative)
// This is a delta which affects the current hardware scroll offset
// If iFillcolor != -1, the newly exposed lines will be filled with that color
//
void spilcdScroll(int iLines, int iFillColor);

//
// Draw a NxN tile scaled 150% in both directions
int spilcdDrawTile150(int x, int y, int iTileWidth, int iTileHeight, unsigned char *pTile, int iPitch);

// Draw a NxN tile
int spilcdDrawTile(int x, int y, int iTileWidth, int iTileHeight, unsigned char *pTile, int iPitch);

// Draw a 16x16 tile with variable cols/rows removed
int spilcdDrawMaskedTile(int x, int y, unsigned char *pTile, int iPitch, int iColMask, int iRowMask);

// Draw a NxN tile scaled to 2x width, 1.5x height with pixel averaging
int spilcdDrawScaledTile(int x, int y, int cx, int cy, unsigned char *pTile, int iPitch);

int spilcdDraw53Tile(int x, int y, int cx, int cy, unsigned char *pTile, int iPitch);

// Draw a 16x16 tile as 16x13 (with priority to non-black pixels)
int spilcdDrawRetroTile(int x, int y, unsigned char *pTile, int iPitch);

// Draw a 16x16 tile scaled to 16x14 with pixel averaging
int spilcdDrawSmallTile(int x, int y, unsigned char *pTile, int iPitch);

// Write a text string to the display at x (column 0-83) and y (row 0-5)
// bLarge = 0 - 8x8 font, bLarge = 1 - 16x24 font
int spilcdWriteString(int x, int y, char *szText, unsigned short usFGColor, unsigned short usBGColor, int bLarge);

// Write a text string of 8x8 characters
// quickly to the LCD with a single data block write.
// This is necessary because there is a lot of latency between
// writes when using the spidev kernel driver
int spilcdWriteStringFast(int x, int y, char *szText, unsigned short usFGColor, unsigned short usBGColor);

// Sets a pixel to the given color
// Coordinate system is pixels, not text rows (0-239, 0-319)
int spilcdSetPixel(int x, int y, unsigned short usPixel);

// Set the software orientation
int spilcdSetOrientation(int iOrientation);

#ifdef __cplusplus
}
#endif

#endif // SPI_LCD_H
