/* #include <stdint.h> */
/* #include <stdio.h> */
/* #include <stdbool.h> */
#include <stdlib.h>
/* #include <assert.h> */
/* #include <string.h> */
#include <ctype.h>
/* #include <fcntl.h> */
#include <unistd.h>
/* #include <limits.h> */

#include "core/common.h"
#include "core/spi_lcd.h"

#include "helpware/display.h"

#ifdef EXTENDED_DISPLAY_DEBUGGING
#define ddprintf printf
#else
#define ddprintf(f,...)
#endif

void core_common_on_exit(void) {
  lcd_shutdown();
}

static void animate(void* frame)
{
    lcd_write_buffer((uint16_t*)frame, LCD_FRAME_WIDTH * LCD_FRAME_HEIGHT * sizeof(uint16_t));
}

#define FRAME_WORDS (LCD_FRAME_WIDTH*LCD_FRAME_HEIGHT)
unsigned short usColors[8] = {0xf800, 0x7e0, 0x1f, 0xffff, 0xffe0, 0x7ff, 0xf81f, 0x7bef};

int main(int argc, const char* argv[])
{
  size_t i;

  ddprintf("Initializing\n");

  lcd_init();

  display_init();

  LcdFrame frame;

  if (argc > 1)
  {
    int brightness = atoi(argv[1]);
    lcd_set_brightness(brightness);
  }


  while (true) {
    for (i=0; i<FRAME_WORDS; ++i) frame.data[i] = lcd_RED;
    printf("Drawn red\n");
//     spilcdFills(frame.data);
    lcd_write_buffer(frame.data, FRAME_WORDS*sizeof(uint16_t));
usleep(200000);

    for (i=0; i<FRAME_WORDS; ++i) frame.data[i] = lcd_GREEN;
    printf("Drawn green\n");
    lcd_write_buffer(frame.data, FRAME_WORDS*sizeof(uint16_t));
usleep(200000);

    for (i=0; i<FRAME_WORDS; ++i) frame.data[i] = lcd_BLUE;
    printf("Drawn blue\n");
    lcd_write_buffer(frame.data, FRAME_WORDS*sizeof(uint16_t));
usleep(200000);

    for (i=0; i<FRAME_WORDS; ++i) frame.data[i] = lcd_GRAY;
    printf("Drawn gray\n");
    spilcdFills(frame.data);
//     lcd_write_buffer(frame.data, FRAME_WORDS*sizeof(uint16_t));
usleep(200000);
        
  }



  return 0;
}
