# HARDWARE – Połączenia i konfiguracja STM32CubeMX

Źródło prawdy dla pinów i parametrów timerów: `Project_CAR.ioc`.

---

## Platforma

### STM32 NUCLEO-C051C8

| Parametr | Wartość |
|---|---|
| Mikrokontroler | STM32C051C8T6 |
| Rdzeń | ARM Cortex-M0+ |
| Taktowanie | 12 MHz (HSI 48 MHz / DIV4) |
| Pamięć Flash | 64 KB |
| Pamięć RAM | 12 KB |
| Zasilanie | 3,3V (logika) / 5V (CN7/CN10 U5V) |
| Złącza rozszerzeń | CN7 (Morpho lewy), CN10 (Morpho prawy), CN5/CN6 (Arduino) |
| Interfejs debugowania | ST-LINK/V2-1 (wbudowany, USB) |

---

## 1. Sterownik silników L298N (silniki ARD-6292)

### Opis modułu

| Parametr | Wartość |
|---|---|
| Sterownik | L298N Dual H-Bridge |
| Napięcie silników (VS) | 5–35V |
| Napięcie logiki (VSS) | 5V (wbudowany regulator 5V) |
| Prąd wyjściowy | maks. 2A na kanał |
| Kanały | 2 (silnik A i silnik B) |
| Sterowanie kierunkiem | IN1/IN2 (silnik A), IN3/IN4 (silnik B) |
| Sterowanie prędkością | ENA – PWM silnik A, ENB – PWM silnik B |

### Fizyczne połączenie

| Pin L298N | Pin Nucleo | Złącze      | Funkcja              | User Label |
|-----------|------------|-------------|----------------------|------------|
| ENA       | PA8        | CN10 pin 23 | TIM1 CH1 AF2 (PWM)   | `M_ENA`    |
| IN1       | PB0        | CN10 pin 33 | GPIO Output          | `M_IN1`    |
| IN2       | PB1        | CN8 A3      | GPIO Output          | `M_IN2`    |
| IN3       | PB3        | CN7 pin 34  | GPIO Output          | `M_IN3`    |
| IN4       | PB5        | CN9 D6      | GPIO Output          | `M_IN4`    |
| ENB       | PB4        | CN9 D4      | TIM3 CH1 AF1 (PWM)   | `M_ENB`    |
| GND       | GND        | —           | Masa wspólna         | —          |
| 5V (VSS)  | +5V        | CN7 pin 18  | Zasilanie logiki     | —          |

> Zasilanie silników (VS – 5–12V) podłączone bezpośrednio do zewnętrznego źródła, nie do Nucleo.

### Konfiguracja CubeMX (skonfigurowane w .ioc)

**TIM1 CH1 (ENA – PA8):**
1. Przejdź do: **Timers → TIM1**.
2. Clock Source: **Internal Clock**.
3. Channel 1: **PWM Generation CH1**.
4. Parameter Settings: Prescaler `47`, Counter Period `999`.
5. Pin PA8 → `TIM1_CH1` (AF2) – przypisywany automatycznie.
6. User Label pinu PA8: `M_ENA`.

**TIM3 CH1 (ENB – PB4):**
1. Przejdź do: **Timers → TIM3**.
2. Clock Source: **Internal Clock**.
3. Channel 1: **PWM Generation CH1**.
4. Parameter Settings: Prescaler `47`, Counter Period `999`.
5. Pin PB4 → `TIM3_CH1` (AF1) – przypisywany automatycznie.
6. User Label pinu PB4: `M_ENB`.

**GPIO Output (PB0, PB1, PB3, PB5):**
- GPIO output level: Low, Push Pull, No pull, Speed: Low.
- User Labels: PB0 → `M_IN1`, PB1 → `M_IN2`, PB3 → `M_IN3`, PB5 → `M_IN4`.

> Wynikowa częstotliwość PWM: 12 MHz / 48 / 1000 = **250 Hz**.

---

## 2. Czujnik IR – 5 Channel Infrared Reflective Sensor

### Fizyczne połączenie

Zasilanie: **5V**

| Pin Nucleo | Kanał  | Złącze      |
|------------|--------|-------------|
| PA0        | IR_CH1 | CN8 A0      |
| PA1        | IR_CH2 | CN8 A1      |
| PA2        | IR_CH3 | CN7 pin 37  |
| PA3        | IR_CH4 | CN10 pin 9  |
| PC6        | IR_CH5 | CN7 pin 17  |

### Konfiguracja CubeMX (skonfigurowane w .ioc)

Wszystkie 5 pinów skonfigurowane jako `GPIO_Input`, Pull-up, etykiety `IR_CH1` … `IR_CH5`.

---

## 3. Przycisk użytkownika

### Fizyczne połączenie

| Komponent | Pin Nucleo | Złącze | Funkcja             |
|-----------|------------|--------|---------------------|
| B1 (USER) | PC13       | CN7    | GPIO Input, Pull-up |

### Konfiguracja CubeMX (skonfigurowane w .ioc)

- PC13 → `GPIO_Input`, Pull-up, User Label: `BTN_USER`.

---

## 4. Podsumowanie zajętych timerów


| Timer | Kanał | Pin | Label   | Zastosowanie             | Prescaler | Period |
|-------|-------|-----|---------|--------------------------|-----------|--------|
| TIM1  | CH1   | PA8 | M_ENA   | L298N silnik A (PWM)     | 47        | 999    |
| TIM3  | CH1   | PB4 | M_ENB   | L298N silnik B (PWM)     | 47        | 999    |
