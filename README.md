# RC Car – Line Follower

Projekt samochodu zdalnie sterowanego (RC), który porusza się **autonomicznie po linii** wyznaczonej na podłożu. Pojazd sam wykrywa i śledzi ścieżkę bez udziału operatora – całe sterowanie odbywa się w czasie rzeczywistym bezpośrednio na mikrokontrolerze.

---

## O projekcie

Samochód wykorzystuje 8-kanałowy czujnik podczerwieni (IR) do wykrywania linii na podłożu. Na podstawie odczytów z czujnika mikrokontroler oblicza korektę toru jazdy i steruje silnikami napędowymi oraz serwem skrętu.

Projekt jest realizowany na platformie **STM32 NUCLEO-C051C8** (ARM Cortex-M0+) i pisany w języku C z użyciem biblioteki HAL generowanej przez STM32CubeMX.

---

## Dokumentacja

Szczegółowe informacje o połączeniach elektrycznych, pinach mikrokontrolera i konfiguracji timerów znajdują się w pliku [`HARDWARE.md`](HARDWARE.md).
