// Test git 
#include "led_effect.h"
#include <stdio.h>
#include <string.h>

// Biến "static" giúp các file khác không thể nhìn thấy hay sửa đổi lung tung
static uint32_t t_led_step = 0;
static uint8_t led_state = 0;
static uint32_t delay_time = 0;
char uart_buf[50];           // Bộ đệm chứa chuỗi ký tự truyền UART
// Thêm dòng này ở đầu file led_effect.c
extern UART_HandleTypeDef huart1;
extern uint8_t sta_button;
extern uint8_t count;
// Thêm dòng này vào đầu file led_effect.c (phía ngoài tất cả các hàm)
LedStatus led_status = LED_OFF;


// Hàm khởi tạo tốc độ đuổi
void LED_Chase_Init(uint32_t speed_ms) {
    delay_time = speed_ms;
    t_led_step = HAL_GetTick();
}

// Hàm chạy thuật toán (Đem toàn bộ switch-case từ main vào đây)
void LED_Chase_Execute(void) {
    if (HAL_GetTick() - t_led_step >= delay_time) {
        t_led_step = HAL_GetTick();

        // Tắt hết LED
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

        // Bật 1 LED theo trạng thái
        switch (led_state) {
            case 0:
            	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
            	sprintf(uart_buf, "LED PC13 is ON\r\n");
            	HAL_UART_Transmit(&huart1, (uint8_t*)uart_buf, strlen(uart_buf), 10);
            	led_state = 1; break;
            case 1:
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,  GPIO_PIN_RESET);
            	sprintf(uart_buf, "LED PA3 is ON\r\n");
            	HAL_UART_Transmit(&huart1, (uint8_t*)uart_buf, strlen(uart_buf), 10);
            	led_state = 0; break;
            default: led_state = 0; break;
        }
    }
}

void led1Blink1Hz (void)
{
	static uint32_t t_led1_blink; // Bien toan cuc chi cho ham nay, khong bi mat gia tri moi khi goi lai ham
	if(HAL_GetTick() - t_led1_blink >= 500)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);
		t_led1_blink = HAL_GetTick();
	}
}

void led2Blink5Hz (void)
{
	static uint32_t t_led2_blink; // Bien toan cuc chi cho ham nay, khong bi mat gia tri moi khi goi lai ham
	if(HAL_GetTick() - t_led2_blink >= 100)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
		t_led2_blink = HAL_GetTick();
	}
}

void led_off()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
}

void led_handle(void) {
		switch(led_status)
	{
		case LED_OFF:
			led_off();
		break;
		case LED1_BLINK_1HZ:
			led1Blink1Hz();
		break;
		case LED2_BLINK_5HZ:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
			led2Blink5Hz();
		break;
	}

}
