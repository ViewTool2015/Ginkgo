#ifndef _LCD5510_H_
#define _LCD5510_H_

void LCD_Init(void);
void LCD_WriteString(uint8_t x,uint8_t y,const char *str);
void LCD_Clear(void);

#endif
