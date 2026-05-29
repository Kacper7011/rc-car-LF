## Cel projektu

Budowa samochodu zdalnie sterowanego RC poruszającego się autonomicznie po linii (5-kanałowy czujnik IR). Projekt oparty na mikrokontrolerze STM32 NUCLEO-C051C8.

---

## Główne funkcje

- Jazda po linii (line follower) – 5-kanałowy czujnik IR

---

## Użyte komponenty

| Komponent | Ilość |
|---|---|
| STM32 NUCLEO-C051C8 | 1x |
| Moduł sterownika silników L298N | 1x |
| Silnik DC ARD-6292 | 2x |
| 5-kanałowy czujnik IR (5 Channel Infrared Reflective Sensor) | 1x |

---

## Platforma

Mikrokontroler **STM32 NUCLEO-C051C8** (ARM Cortex-M0+, 48 MHz HSI). Szczegółowe parametry płytki: [`HARDWARE.md`](HARDWARE.md).
---

## Dokumentacja techniczna

Przed rozpoczęciem pracy nad jakimkolwiek modułem sprzętowym Claude **zawsze** czyta odpowiednie pliki dokumentacji:

| Plik | Kiedy czytać |
|------|--------------|
| [`HARDWARE.md`](./HARDWARE.md) | Przed pisaniem kodu dla silników (L298N), czujnika IR, UART |
| `Project_CAR.ioc` | Jako ostateczne źródło prawdy dla pinów i timerów |

**Reguła weryfikacji pinów:** Claude **nigdy nie przyjmuje pinów ani parametrów timerów z pamięci** — każda wartość musi być zweryfikowana w `Project_CAR.ioc` lub `Docs/HARDWARE.md` przed użyciem w kodzie.

---

## Zasady pracy z Claude

### Konfiguracja przez plik .ioc (STM32CubeMX)

- Claude **nigdy nie edytuje pliku `.ioc` samodzielnie** – wszelkie zmiany konfiguracji peryferiów (timery, GPIO, UART, PWM, DMA, zegary itp.) muszą być wykonane ręcznie przez użytkownika w STM32CubeMX.
- Gdy dana funkcjonalność wymaga zmian w `.ioc`, Claude **zatrzymuje się** i przed napisaniem jakiegokolwiek kodu przedstawia użytkownikowi dokładną instrukcję konfiguracji, np.:

```
⚙️ Wymagana konfiguracja w STM32CubeMX (.ioc):

1. Otwórz plik .ioc w STM32CubeMX.
2. Przejdź do zakładki „Pinout & Configuration".
3. W sekcji „Timers" wybierz <TIMER>.
4. Ustaw „Clock Source" na „Internal Clock".
5. W „Channel N" wybierz „PWM Generation CHN".
6. Przejdź do „Parameter Settings" i ustaw:
   - Prescaler: <wartość>
   - Counter Period: <wartość>
   - Pulse: <wartość>
7. W zakładce „Pinout" upewnij się, że <PIN> jest przypisany do <TIMER_CHN>.
8. Kliknij „Generate Code".

Daj znać gdy gotowe – wtedy napiszę kod korzystający z tej konfiguracji.
```

- Claude czeka na potwierdzenie przez użytkownika że konfiguracja została zastosowana i kod wygenerowany, zanim przejdzie do implementacji.

---

### Styl pisania kodu

- Kod jest pisany w małych, czytelnych fragmentach – każda funkcja robi jedną rzecz.
- Nazwy zmiennych, funkcji i plików są opisowe i jednoznaczne.
- Każdy fragment kodu zawiera komentarze wyjaśniające cel i działanie.
- Unika się „magicznych liczb" – stałe są definiowane przez `#define` lub `const` z nazwą.
- Kod jest dzielony na logiczne moduły (pliki `.c` / `.h`) odpowiadające komponentom sprzętowym.

### Praca z gałęziami Git

- Cały kod projektu jest przechowywany na GitHubie.
- Każdy nowy feature (funkcjonalność) dostaje **osobny branch** o nazwie odpowiadającej tej funkcjonalności, np.:
  - `feature/line-follower`
- Claude pracuje **wyłącznie na branchu danego feature'a** – nigdy bezpośrednio na `main`.

### Przepływ pracy (workflow)

1. **Rozpoczęcie feature'a** – Claude tworzy nowy branch i informuje użytkownika o jego nazwie.
2. **Implementacja** – kod jest pisany i commitowany stopniowo, w małych logicznych krokach.
3. **Zakończenie pracy** – gdy implementacja jest gotowa, Claude przedstawia użytkownikowi podsumowanie w trzech punktach:
   - **Co zostało zrobione** – lista zmian i dodanych plików.
   - **Jak to ręcznie przetestować** – konkretne kroki weryfikacji na sprzęcie lub w debuggerze.
   - **Jak to łączy się z resztą projektu** – zależności od innych modułów, piny, peryferia.
4. **Oczekiwanie na zatwierdzenie** – Claude czeka na potwierdzenie od użytkownika, że wszystko działa poprawnie. Nie wykonuje żadnych dalszych działań bez tej zgody. Dopiero gdy otrzyma zatwierdzenie robi claude robi commit.
5. **Merge i push** – po otrzymaniu zatwierdzenia Claude merguje branch do `main` i wypycha zmiany do GitHuba (`git push origin main`).

### Przykładowy komunikat po zakończeniu feature'a

```
✅ Feature gotowy: feature/line-follower

Co zrobiłem:
- Dodałem plik ir_sensor.c / ir_sensor.h z odczytem 5 kanałów IR.
- Dodałem plik line_follower.c / line_follower.h z logiką sterowania silnikami.
- Kanały IR odczytywane są jako GPIO_Input (IR_CH1 … IR_CH5).

Jak przetestować:
1. Wgraj firmware na Nucleo.
2. Połóż pojazd na linii (czarna taśma na białym tle).
3. Sprawdź, czy pojazd podąża za linią i koryguje kierunek przy jej utracie.

Jak to łączy się z projektem:
- Moduł wywoływany jest w pętli głównej co 10 ms.
- Wynik trafia do logiki sterowania silnikami (motor_control.c).
- Nie ma konfliktu z timerami PWM.

Czy mogę zmergować do main?
```
