#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "main.h"
#include <stdint.h>

/* Bitmask constants for individual IR channels */
#define IR_CH1_MASK  (1U << 0)
#define IR_CH2_MASK  (1U << 1)
#define IR_CH3_MASK  (1U << 2)
#define IR_CH4_MASK  (1U << 3)
#define IR_CH5_MASK  (1U << 4)

/*
 * GPIO level that indicates a black surface detected.
 * Sensor output is HIGH when no IR is reflected back (black absorbs IR).
 * Change to GPIO_PIN_RESET if your sensor has opposite polarity.
 */
#define IR_BLACK_STATE  GPIO_PIN_SET

/**
 * Reads all 5 IR channels.
 * Returns a bitmask where a set bit means that channel detects black.
 */
uint8_t IR_Sensor_Read(void);

/**
 * Returns 1 if any channel in the bitmask detects black, 0 otherwise.
 */
uint8_t IR_Sensor_AnyBlack(uint8_t channels);

#endif /* IR_SENSOR_H */
