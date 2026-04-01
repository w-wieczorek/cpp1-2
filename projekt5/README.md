# 🔐 BreakLock solver — konkurs programistyczny

## Opis zadania

Twoim zadaniem jest napisanie **programu klienckiego w języku C++**, który łączy się z serwerem gry **BreakLock** i w jak najmniejszej liczbie prób odgaduje ukryty wzór blokady Androida.

Wzór składa się z **_n_ różnych kropek** wybranych z planszy _n×n_. Po każdej próbie serwer zwraca dwie liczby informujące, jak blisko jesteś rozwiązania. Zadanie zostaje zaliczone, gdy program odgadnie ukryty wzór w limicie czasowym 3 sekund.

---

## Plansza i numeracja kropek

Kropki numerujemy od `1` do `n*n` w kolejności czytania (wierszami, od lewej do prawej):

```
  1  |  2  | ... |  n
----------------------
 n+1 | ... | ... | ...
----------------------
 ... | ... | ... | ...
----------------------
 ... | ... | ... | n*n
```

Tajny wzór to **ciąg** _n_ różnych numerów, np. `1 5 9 8` (dla _n_ = 4). Kolejność ma znaczenie. Dopuszczamy także kolejność (taką jak np. 1 7 4 5), która w rzeczywistym androidowym breaklocku byłaby niemożliwa do uzyskania.

---

## Zasady gry

- Klient w każdej turze zgłasza **_n_ różnych liczb** z zakresu `1..n*n` jako propozycję wzoru.
- Serwer odpowiada dwiema liczbami:
  - **`hits`** — ile spośród zgłoszonych kropek w ogóle należy do tajnego wzoru (niezależnie od pozycji),
  - **`exact`** — ile zgłoszonych kropek stoi jednocześnie na właściwej pozycji.
- Gra kończy się, gdy `hits = n` i `exact = n`.

### Przykład (dla _n_ = 4)

| Sekret   | Próba    | hits | exact | Wyjaśnienie                                   |
|----------|----------|------|-------|-----------------------------------------------|
| `1 5 9 8`| `1 9 2 8`| 3    | 2     | Trafione: 1, 9, 8 — na dobrym miejscu: 1 i 8 |
| `1 5 9 8`| `1 5 9 8`| 4    | 4     | Rozwiązanie!                                  |

---

## Protokół komunikacyjny TCP/IP

Protokół jest **tekstowy i liniowy** — każda wiadomość kończy się znakiem `\n`. Zalecana biblioteka: [**Asio**](https://think-async.com/Asio/) (standalone lub Boost.Asio), np. do odczytu można użyć `asio::read_until(socket, buf, '\n')`.

### 1. Nagłówek sesji (serwer → klient)

Po nawiązaniu połączenia serwer wysyła:

```
HELLO BREAKLOCK 1
BOARD 4 4
LENGTH 4
NUMBERS 1 16
DISTINCT YES
ENDHELLO
```

| Pole        | Znaczenie                                   |
|-------------|---------------------------------------------|
| `HELLO BREAKLOCK 1` | Nazwa gry i wersja protokołu      |
| `BOARD 4 4` | Plansza 4 wiersze × 4 kolumny               |
| `LENGTH 4`  | Długość szukanego wzoru                     |
| `NUMBERS 1 16`| Dozwolone numery kropek (zakres domknięty)  |
| `DISTINCT YES` | Brak powtórzeń w jednej próbie           |
| `ENDHELLO`  | Koniec nagłówka                             |

### 2. Wiadomości klienta

#### Próba odgadnięcia

```
TRY a b c d
```

gdzie `a b c d` to cztery różne liczby całkowite z zakresu `1..16`.

#### Zakończenie sesji

```
QUIT
```

### 3. Wiadomości serwera

#### Odpowiedź na poprawną próbę (nierozwiązującą)

```
RESULT hits exact
```

Przykład:
```
RESULT 3 2
```

#### Rozwiązanie odgadnięte

```
SOLVED attempts
```

Przykład:
```
SOLVED 5
```

#### Błąd składni lub niepoprawne dane

```
ERROR code message
```

| Kod | Znaczenie              |
|-----|------------------------|
| 100 | `BAD_FORMAT`           |
| 101 | `DUPLICATE_DOTS`       |
| 102 | `OUT_OF_RANGE`         |
| 103 | `WRONG_LENGTH`         |

> Błędna wiadomość **nie zużywa próby** — serwer czeka na poprawny komunikat `TRY`.

### 4. Przykładowa sesja

```
S: HELLO BREAKLOCK 1
S: BOARD 4 4
S: LENGTH 4
S: DIGITS 1 16
S: DISTINCT YES
S: ENDHELLO

C: TRY 1 2 3 4
S: RESULT 2 1

C: TRY 1 5 3 8
S: RESULT 3 2

C: TRY 1 5 9 8
S: SOLVED 3
```

### 5. Implementacja serwera

[Github](https://github.com/w-wieczorek/breaklock_server/)

---

## Wymagania techniczne

- **Język:** C++17 lub C++20.
- **Sieć:** TCP/IP; zalecana biblioteka **Asio** (standalone, bez Boost) lub **Boost.Asio**.
- **Kompilacja:** program może korzystać z dodatkowych (ogólnie dostępnych w Internecie) bibliotek. Jeśli program ma postać więcej niż jednego pliku, to powinien być zorganizowany w projekt, który da się skompilować za pomocą ogólnie dostępnego narzędzia (`bazel`, `conan`, `cmake`, `make`, `meson`, projekt MSBuild/Visual Studio itp. w zależności od systemu operacyjnego i polecanego kompilatora).

- **Wywołanie:**
  ```bash
  breaklock_client.exe <host> <port>
  ```

---

## Ocenianie

Ocena zależy od wielkości rozwiązania planszy w wyznaczonym limicie czasowym:

| Rozmiar planszy                  | Ocena |
|----------------------------------|-------|
| _n_ = 3                          | 3,5   |
| _n_ = 4                          | 4,0   |
| _n_ = 5                          | 4,5   |
| _n_ = 6                          | 5,0   |

> 💡 **Podpowiedź algorytmiczna:** Dobre strategie odgadywania bazują na podejściu podobnym do gry Mastermind — po każdej odpowiedzi usuń z przestrzeni możliwych rozwiązań wszystkie wzory niezgodne z otrzymanymi `hits` i `exact`, a następną próbę wybierz tak, by maksymalnie zmniejszyć tę przestrzeń (np. algorytm min-max Knutha).

---

## Harmonogram

| Etap               | Termin     |
|--------------------|------------|
| Publikacja zadania | 18.04      |
| Ostateczny termin  | 10.05      |
| Ogłoszenie wyników | 10.05      |

---

## Przesyłanie rozwiązań

Rozwiązanie należy umieścić w prywatnym repozytorium GitHub i udostępnić je organizatorom przed upływem terminu. Repozytorium powinno zawierać wszystkie pliki umożliwiające skompilowanie projektu,np. dla cmake:

```
/
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── README.md   ← krótki opis strategii
```

---

## Kontakt

W razie pytań dotyczących protokołu lub środowiska testowego prosimy o kontakt na platformie **Teams**.
