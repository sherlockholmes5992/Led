#ifndef __LED_EFFECT_H
#define __LED_EFFECT_H

#include "main.h" // Nhúng main.h để sử dụng được các hàm HAL_GPIO

// Khai báo các hàm để file khác (như main.c) có thể gọi được
void LED_Chase_Init(uint32_t speed_ms);
void LED_Chase_Execute(void);
void button_handle(void);
void led_handle(void);

typedef enum
{
	LED_OFF = 0,
	LED1_BLINK_1HZ,
	LED2_BLINK_5HZ,
}LedStatus;
extern LedStatus led_status;

#endif /* __LED_EFFECT_H */
