#include "ir_sensor.h"

uint8_t IR_Sensor_Read(void)
{
    uint8_t result = 0;

    if (HAL_GPIO_ReadPin(IR_CH1_GPIO_Port, IR_CH1_Pin) == IR_BLACK_STATE) result |= IR_CH1_MASK;
    if (HAL_GPIO_ReadPin(IR_CH2_GPIO_Port, IR_CH2_Pin) == IR_BLACK_STATE) result |= IR_CH2_MASK;
    if (HAL_GPIO_ReadPin(IR_CH3_GPIO_Port, IR_CH3_Pin) == IR_BLACK_STATE) result |= IR_CH3_MASK;
    if (HAL_GPIO_ReadPin(IR_CH4_GPIO_Port, IR_CH4_Pin) == IR_BLACK_STATE) result |= IR_CH4_MASK;
    if (HAL_GPIO_ReadPin(IR_CH5_GPIO_Port, IR_CH5_Pin) == IR_BLACK_STATE) result |= IR_CH5_MASK;

    return result;
}

uint8_t IR_Sensor_AnyBlack(uint8_t channels)
{
    return channels != 0;
}
