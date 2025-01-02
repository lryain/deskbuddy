#ifndef LRYA_HELPER_TEXT_H_
#define LRYA_HELPER_TEXT_H_


int helper_lcdset_command_parse(const char* command, int linelen);
int helper_lcdshow_command_parse(const char* command, int linelen);

void helper_lcd_busy_spinner(void);


#endif//LRYA_HELPER_TEXT_H_
