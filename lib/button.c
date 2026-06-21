#include "button.h"

// Hàm kh?i t?o các giá tr? ban d?u cho nút nh?n
void Button_Init(Button_t* btn, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    btn->GPIOx = GPIOx;
    btn->GPIO_Pin = GPIO_Pin;
    
    btn->btn_filter = 1;
    btn->btn_current = 1;
    btn->btn_last = 1;
    btn->is_debouncing = 0;
    btn->time_debouncing = 0;
    
    btn->is_press_timeout = 0;
    btn->press_start_time = 0;
    
    btn->pressing_cb = NULL;
    btn->release_cb = NULL;
    btn->press_short_cb = NULL;
    btn->press_timeout_cb = NULL;
}

// Hàm dang ký các hàm callback x? lý s? ki?n cho t?ng nút d?c l?p
void Button_Register_Callback(Button_t* btn, ButtonCallback pressing, ButtonCallback release, ButtonCallback short_press, ButtonCallback timeout) {
    btn->pressing_cb = pressing;
    btn->release_cb = release;
    btn->press_short_cb = short_press;
    btn->press_timeout_cb = timeout;
}

// Hàm c?t lõi x? lý máy tr?ng thái, truy?n con tr? nút nào vào thì x? lý nút dó
void Button_Process(Button_t* btn) {
    // 1. Ð?c tr?ng thái c?ng t? Port và Pin c?u hình trong struct
    uint8_t sta = HAL_GPIO_ReadPin(btn->GPIOx, btn->GPIO_Pin);
    
    // 2. B? l?c nhi?u (Debounce)
    if (sta != btn->btn_filter) {
        btn->btn_filter = sta;
        btn->is_debouncing = 1;
        btn->time_debouncing = HAL_GetTick();
    }
    
    if (btn->is_debouncing && (HAL_GetTick() - btn->time_debouncing >= 15)) {
        btn->btn_current = btn->btn_filter;
        btn->is_debouncing = 0;
    }
    
    // 3. X? lý tr?ng thái Nh?n / Nh?
    if (btn->btn_current != btn->btn_last) {
        if (btn->btn_current == 0) { // Khi nút du?c nh?n xu?ng (Tích c?c m?c TH?P)
            btn->is_press_timeout = 1;
            btn->press_start_time = HAL_GetTick();
            
            if (btn->pressing_cb != NULL) {
                btn->pressing_cb(); // G?i hàm callback tuong ?ng
            }
        } 
        else { // Khi nút du?c nh? ra
            if (HAL_GetTick() - btn->press_start_time <= 1000) {
                if (btn->press_short_cb != NULL) {
                    btn->press_short_cb();
                }
            }
            btn->is_press_timeout = 0; 
            
            if (btn->release_cb != NULL) {
                btn->release_cb();
            }
        }
        btn->btn_last = btn->btn_current;
    }

    // 4. Ki?m tra s? ki?n Nh?n gi? quá lâu (Timeout)
    if (btn->is_press_timeout && (HAL_GetTick() - btn->press_start_time >= 3000)) {
        if (btn->press_timeout_cb != NULL) {
            btn->press_timeout_cb();
        }
        btn->is_press_timeout = 0; // X? lý xong thì xóa c?
    }
}
