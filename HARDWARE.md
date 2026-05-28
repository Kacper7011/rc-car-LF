# HARDWARE – Połączenia i konfiguracja STM32CubeMX

Źródło prawdy dla pinów i parametrów timerów: `LineFollower.ioc`.

---

## Platforma

### STM32F446ZET6U

| Parametr | Wartość |
|---|---|
| Mikrokontroler | STM32F446ZET6U |
| Rdzeń | ARM Cortex-M4 (FPU) |
| Taktowanie | 16 MHz HSI |
| Pamięć Flash | 512 KB |
| Pamięć RAM | 128 KB |
| Interfejs debugowania | ST-LINK/V2-1 (wbudowany, USB) |

---

## 1. Sterownik silników L298N

| Parametr | Wartość |
|---|---|
| Sterownik | L298N Dual H-Bridge |
| Napięcie silników (VS) | 5–35V |
| Prąd wyjściowy | maks. 2A na kanał |

### Silnik lewy

| Pin L298N | Pin STM32 | Funkcja | User Label |
|-----------|-----------|---------|------------|
| IN1 | PG2 | GPIO Output (kierunek) | MOT_L_IN1 |
| IN2 | PG3 | GPIO Output (kierunek) | MOT_L_IN2 |
| ENA | PB4 | TIM3_CH1 AF2 (PWM) | MOT_L_PWM |

### Silnik prawy

| Pin L298N | Pin STM32 | Funkcja | User Label |
|-----------|-----------|---------|------------|
| IN3 | PD0 | GPIO Output (kierunek) | MOT_R_IN3 |
| IN4 | PD1 | GPIO Output (kierunek) | MOT_R_IN4 |
| ENB | PA0 | TIM2_CH1 AF1 (PWM) | MOT_R_PWM |

### Konfiguracja timerów PWM

| Timer | Kanał | Pin | Prescaler | ARR | Częstotliwość PWM |
|-------|-------|-----|-----------|-----|-------------------|
| TIM3 | CH1 | PB4 | 839 | 999 | ~19 Hz |
| TIM2 | CH1 | PA0 | 839 | ARR=999 | ~19 Hz |

> Zakres duty cycle: 0–999 (0% – 100%)

---

## 2. Czujnik IR – 5 Channel Infrared Reflective Sensor

Zasilanie: **5V** | Logika: aktywna LOW (GPIO_PIN_RESET = wykrycie linii)

| Pin STM32 | Kanał | User Label |
|-----------|-------|------------|
| PC8 | S1 (lewy skrajny) | IR_S1 |
| PC9 | S2 (lewy) | IR_S2 |
| PC10 | S3 (środkowy) | IR_S3 |
| PC11 | S4 (prawy) | IR_S4 |
| PC12 | S5 (prawy skrajny) | IR_S5 |

Wszystkie piny: `GPIO_Input`, Pull-up wyłączony.

---

## 3. LED diagnostyczna

| LED | Pin STM32 | Funkcja |
|-----|-----------|---------|
| LD1 (zielona) | PB0 | GPIO Output — świeci gdy linia wykryta |

---

## 4. Podsumowanie zajętych zasobów

| Zasób | Pin | Funkcja |
|-------|-----|---------|
| TIM2 CH1 | PA0 | PWM silnik prawy |
| TIM3 CH1 | PB4 | PWM silnik lewy |
| GPIO OUT | PG2, PG3 | Kierunek silnik lewy |
| GPIO OUT | PD0, PD1 | Kierunek silnik prawy |
| GPIO IN | PC8–PC12 | Czujniki IR S1–S5 |
| GPIO OUT | PB0 | LED diagnostyczna |
