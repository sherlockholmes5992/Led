#ifndef __BUTTON_H
#define __BUTTON_H

#include "main.h" // C?n nhúng d? l?y d?nh nghia GPIO_TypeDef, uint32_t, vv.

// Ð?nh nghia ki?u con tr? hàm cho các s? ki?n (callback)
typedef void (*ButtonCallback)(void);

// C?u trúc d? li?u dóng gói toàn b? tr?ng thái c?a M?T nút nh?n
typedef struct {
    // C?u hình ph?n c?ng
    GPIO_TypeDef* GPIOx;       // Port c?a nút (VD: GPIOA, GPIOB)
    uint16_t GPIO_Pin;         // Pin c?a nút (VD: GPIO_PIN_5)
    
    // Các bi?n ph?c v? l?c nhi?u (Debounce)
    uint8_t btn_filter;
    uint8_t btn_current;
    uint8_t btn_last;
    uint8_t is_debouncing;
    uint32_t time_debouncing;
    
    // Các bi?n ph?c v? d?m th?i gian nh?n gi?
    uint8_t is_press_timeout;
    uint32_t press_start_time;
    
    // Các hàm callback x? lý s? ki?n (gán linh ho?t cho t?ng nút)
    ButtonCallback pressing_cb;
    ButtonCallback release_cb;
    ButtonCallback press_short_cb;
    ButtonCallback press_timeout_cb;
} Button_t;

// Khai báo các hàm mà main.c có th? g?i
void Button_Init(Button_t* btn, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void Button_Register_Callback(Button_t* btn, ButtonCallback pressing, ButtonCallback release, ButtonCallback short_press, ButtonCallback timeout);
void Button_Process(Button_t* btn);

#endif /* __BUTTON_H */
