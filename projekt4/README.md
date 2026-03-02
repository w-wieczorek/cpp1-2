# Konkurs Programistyczny: Zgadnij Automat (DFA Challenge)

> **Edycja:** 2026  
> **Język implementacji:** C++17 lub nowszy  
> **Biblioteka sieciowa:** [Asio (standalone)](https://think-async.com/Asio/) ≥ 1.28  
> **Serwer konkursu:** `89.167.87.83`, port **9000**

---

## 1. Cel konkursu

Celem zadania jest napisanie programu klienckiego w języku C++, który — komunikując
się z serwerem przez sieć TCP — zidentyfikuje **ukryty deterministyczny automat
skończony (DFA)** i dostarczy poprawne odpowiedzi na zestaw weryfikacyjnych zapytań
słownych wysyłanych przez serwer.

---

## 2. Definicja problemu

### 2.1 Ukryty automat

Serwer przechowuje stały (przez czas trwania rundy) automat DFA zdefiniowany jako:

$$
\mathcal{A} = (Q,\, \Sigma,\, \delta,\, q_0,\, F)
$$

| Składnik | Wartość |
|---|---|
| Zbiór stanów $Q$ | $\{0, 1, 2, \dots, 9\}$ — **10 stanów** |
| Alfabet $\Sigma$ | $\{a, b\}$ |
| Stan początkowy $q_0$ | $0$ |
| Stany akceptujące $F$ | dokładnie **5 stanów** wybranych losowo z $Q$ |
| Funkcja przejścia $\delta$ | $\delta : Q \times \Sigma \to Q$, generowana losowo (automat zupełny) |

Automat jest **generowany losowo** na początku każdej rundy konkursowej
i pozostaje niezmieniony przez cały czas jej trwania.

### 2.2 Zadanie uczestnika

Program kliencki ma za zadanie:

1. **Fazę uczenia** — wysyłać do serwera słowa $w \in \{a,b\}^*$ i na podstawie
   odpowiedzi `YES`/`NO` odtworzyć strukturę ukrytego automatu.
2. **Fazę weryfikacji** — odpowiedzieć poprawnie na **wszystkie** słowa testowe
   wysłane przez serwer (potwierdzenie, że odtworzony automat jest równoważny
   ukrytemu).

---

## 3. Protokół komunikacyjny

### 3.1 Warstwa transportowa

| Parametr | Wartość |
|---|---|
| Protokół | TCP / IPv4 |
| Port | **9000** |
| Kodowanie | UTF-8 |
| Separator wiadomości | `\n` (LF, znak 0x0A) |
| Maksymalna długość wiersza | 4096 bajtów |

Każda wiadomość to **jeden wiersz** zakończony znakiem `\n`.
Białe znaki wewnątrz wiersza oddzielają tokeny; wiodące i kończące spacje
są ignorowane przez serwer.

### 3.2 Przegląd faz sesji

```

KLIENT                          SERWER
|                               |
|---- HELLO <nazwa> ----------->|   Faza 1: Rejestracja
|<--- WELCOME <sid> LIMIT <k> --|
|                               |
|---- QUERY <słowo> ----------->|   Faza 2: Zapytania (≤ k razy)
|<--- RESULT YES|NO ------------|
|         ...                   |
|                               |
|---- SUBMIT ------------------>|   Faza 3: Weryfikacja
|<--- VERIFY_START <n> ---------|
|<--- TEST <słowo_1> -----------|
|---- ANSWER YES|NO ----------->|
|<--- ACK ----------------------|
|         ...                   |
|<--- TEST <słowo_n> -----------|
|---- ANSWER YES|NO ----------->|
|<--- ACK ----------------------|
|<--- SUCCESS SCORE <pts> ------|   lub FAILURE WORD <słowo>
|                               |
|---- QUIT -------------------->|   Zamknięcie sesji
|<--- BYE ----------------------|

```

### 3.3 Faza 1 — Rejestracja

#### Komunikat klienta

```

HELLO <nazwa_zespołu>

```

- `<nazwa_zespołu>` — ciąg znaków alfanumerycznych i `_`, długość 1–32 znaki.
- Przykład: `HELLO alice_team`

#### Odpowiedź serwera

```

WELCOME <session_id> LIMIT <k>

```

- `<session_id>` — unikalny identyfikator sesji (ciąg szesnastkowy, 8 znaków).
- `<k>` — limit zapytań słownych w fazie 2 (domyślnie **1000**).
- Przykład: `WELCOME a3f10c2e LIMIT 1000`

W przypadku błędu (np. zduplikowana nazwa):

```

ERROR <kod> <opis>

```

Możliwe kody błędów:

| Kod | Znaczenie |
|---|---|
| `NAME_TAKEN` | Podana nazwa jest już zajęta w tej rundzie |
| `INVALID_NAME` | Niedozwolone znaki w nazwie |
| `SESSION_FULL` | Osiągnięto maksymalną liczbę jednoczesnych sesji |

### 3.4 Faza 2 — Zapytania o przynależność

#### Komunikat klienta

```

QUERY <słowo>

```

- `<słowo>` — niepusty ciąg nad $\{a,b\}^*$; dla słowa pustego $\varepsilon$ użyj
  tokenu `eps`.
- Przykłady: `QUERY aabba`, `QUERY eps`, `QUERY b`

#### Odpowiedź serwera

```

RESULT YES
RESULT NO

```

W przypadku błędu:

```

ERROR LIMIT_EXCEEDED
ERROR INVALID_WORD

```

Uwagi:
- Po wyczerpaniu limitu `k` każde kolejne `QUERY` skutkuje odpowiedzią
  `ERROR LIMIT_EXCEEDED`.
- Słowa dłuższe niż **512 znaków** są odrzucane odpowiedzią `ERROR INVALID_WORD`.

### 3.5 Faza 3 — Weryfikacja

Po wysłaniu `SUBMIT` klient inicjuje weryfikację.

#### Odpowiedź serwera na SUBMIT

```

VERIFY_START <n>

```

- `<n>` — liczba słów testowych w zestawie weryfikacyjnym (typowo 280–350).

#### Wymiana testów

Dla każdego $i = 1, \dots, n$:

1. Serwer wysyła:
```

TEST <słowo_i>

```
2. Klient odpowiada natychmiastowo (przyjmijmy, że w ciągu **2 sekund**):
```

ANSWER YES

```
lub
```

ANSWER NO

```
3. Serwer potwierdza:
```

ACK

```

#### Zakończenie weryfikacji

Jeśli wszystkie odpowiedzi są poprawne:

```

SUCCESS SCORE <pts>

```

W przypadku pierwszej błędnej odpowiedzi serwer **natychmiast** przerywa
weryfikację i wysyła:

```

FAILURE WORD <słowo_i>

```

gdzie `<słowo_i>` to słowo, na którym nastąpił błąd. W takim wypadku klient powinien się rozłączyć.

> **Ważne:** Zestaw weryfikacyjny jest generowany metodą **W** (Chow, 1978) oraz
> uzupełniony o losowe słowa. Dla dowolnych dwóch DFA z co najwyżej 10 stanami
> zestaw ten gwarantuje wykrycie każdej różnicy — jeśli klient odpowiada poprawnie
> na wszystkie testy, jego model jest **językowo równoważny** ukrytemu automatowi.

### 3.6 Dodatkowe komunikaty

| Komunikat klienta | Odpowiedź serwera | Opis |
|---|---|---|
| `STATUS` | `STATUS QUERIES_USED <u> QUERIES_LEFT <l>` | Bieżący stan licznika zapytań |
| `QUIT` | `BYE` | Zamknięcie sesji |

---

## 4. Zasady punktacji

### 4.1 Podstawowa punktacja rundy

$$
S = S_{\text{base}} + B_q
$$

| Składnik | Wartość |
|---|---|
| $S_{\text{base}}$ — za pomyślną weryfikację | **1000 pkt** |
| $B_q$ — bonus za efektywność zapytań | $\max\!\bigl(0,\ 500 - \lfloor q / 2 \rfloor\bigr)$ pkt, gdzie $q$ = liczba wysłanych zapytań QUERY |

Maksymalna punktacja za jedną rundę: **1500 pkt**.

Nieudana weryfikacja: **0 pkt** za daną rundę (sesję można powtórzyć).

### 4.2 Rundy i wynik końcowy

- Konkurs składa się z **3 rund**, w każdej inny losowo wygenerowany automat.
- Każda drużyna może wykonać **maksymalnie 3 próby** w danej rundzie;
  liczy się **najlepszy wynik** z każdej rundy.
- Wynik końcowy = suma punktów z 3 rund (maks. 4500 pkt).

---

## 5. Wymagania techniczne

### 5.1 Środowisko

- Język: **C++17** lub C++20.
- Biblioteka sieciowa: **standalone Asio** (nie Boost.Asio) —
  `#define ASIO_STANDALONE`, nagłówek `<asio.hpp>`.
- Kompilator: GCC ≥ 11, Clang ≥ 14, MSVC 2022 lub MSVC 2026.

### 5.2 Instalacja Asio (standalone)

**vcpkg:**
```bash
vcpkg install asio
```

**Scoop (Windows):**

```bash
scoop install asio
```

**Ręcznie:**

```bash
# Pobierz asio-1.30.x.tar.bz2 z https://think-async.com/Asio/
# Skopiuj katalog include/asio/ i plik include/asio.hpp do projektu
```


### 5.3 Minimalna konfiguracja CMake

```cmake
cmake_minimum_required(VERSION 3.20)
project(dfa_client CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(asio CONFIG REQUIRED)      # lub dodaj ścieżkę ręcznie

add_executable(dfa_client main.cpp)
target_link_libraries(dfa_client PRIVATE asio::asio)

if(WIN32)
    target_link_libraries(dfa_client PRIVATE ws2_32 wsock32)
endif()
```


### 5.4 Szkielet klienta

```cpp
#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Użycie: " << argv[0] << " <adres_ip> <port>\n";
        return 1;
    }
    const std::string host = argv[1];
    const std::string port = argv[2];

    asio::io_context io;
    asio::ip::tcp::resolver resolver(io);
    asio::ip::tcp::socket sock(io);

    auto endpoints = resolver.resolve(host, port);
    asio::connect(sock, endpoints);

    // Pomocnicze funkcje I/O
    auto send = [&](const std::string& msg) {
        asio::write(sock, asio::buffer(msg + "\n"));
    };
    auto recv = [&]() -> std::string {
        asio::streambuf buf;
        asio::read_until(sock, buf, '\n');
        std::istream is(&buf);
        std::string line;
        std::getline(is, line);
        return line;
    };

    // Faza 1: Rejestracja
    send("HELLO imie_nazwisko");
    std::cout << recv() << "\n";   // WELCOME ...

    // Faza 2: przykładowe zapytania
    send("QUERY aab");
    std::cout << recv() << "\n";   // RESULT YES|NO

    send("QUERY eps");
    std::cout << recv() << "\n";

    // --- Tu umieść algorytm uczący ---

    // Faza 3: Weryfikacja
    send("SUBMIT");
    std::string line = recv();     // VERIFY_START <n>
    int n = std::stoi(line.substr(line.rfind(' ') + 1));

    for (int i = 0; i < n; ++i) {
        std::string test = recv();             // TEST <słowo>
        std::string word = test.substr(5);     // wytnij "TEST "

        // --- Tu zapytaj swój model DFA ---
        bool accepted = /* twój_model.accepts(word) */ false;

        send(accepted ? "ANSWER YES" : "ANSWER NO");
        std::cout << (accepted ? "ANSWER YES" : "ANSWER NO") << "\n";
        std::string response = recv(); // ACK lub FAILURE
        std::cout << response << "\n"; 
        if (response == "FAILURE") {
            std::cerr << "Błąd weryfikacji\n";
            return 1;
        }
    }

    std::cout << recv() << "\n";   // SUCCESS SCORE

    send("QUIT");
    recv();  // BYE
    return 0;
}
```


---

## 6. Wskazówki algorytmiczne

### 6.1 Algorytm L* Angluin (zalecany)

Klasyczny algorytm aktywnego uczenia się DFA — Angluin (1987) — odtwarza
**minimalny DFA** dla nieznanego języka regularnego, korzystając wyłącznie
z zapytań o przynależność (ang. *membership queries*). Wymagana liczba zapytań
wynosi $O(n^2 \cdot |\Sigma|)$, gdzie $n$ — liczba stanów docelowego automatu.
Dla naszego przypadku ($n = 10$, $|\Sigma| = 2$) to teoretycznie **ok. 200 zapytań**,
co jest znacznie poniżej limitu 1000.

Kroki algorytmu L*:

1. Zainicjuj **tablicę obserwacji** $T: (S \cup S \cdot \Sigma) \times E \to \{0,1\}$,
gdzie $S = \{\varepsilon\}$, $E = \{\varepsilon\}$.
2. **Zapełnij** brakujące komórki tablicy zapytaniami do serwera.
3. Sprawdź **zamkniętość** (closedness) i **spójność** (consistency) tablicy.
Jeśli nie są spełnione, rozszerz $S$ lub $E$ i wróć do kroku 2.
4. Zbuduj **hipotezę** DFA $\mathcal{H}$ na podstawie tablicy.
5. Sprawdź równoważność $\mathcal{H}$ z ukrytym automatem.
6. Powtarzaj od kroku 2 aż do uzyskania właściwego automatu.

> **Uwaga:** Oryginalna wersja L* zakłada dostęp do wyroczni równoważności
> (ang. *equivalence oracle*). W naszym protokole rolę tę pełni komenda `SUBMIT` —
> jeśli weryfikacja się nie powiedzie, słowo podane w `FAILURE WORD` jest
> gotowym kontrprzykładem.

### 6.2 Algorytm naiwny (podejście wyczerpujące)

Dla małych automatów ($n = 10$) wystarczy systematyczne odpytywanie wszystkich
słów o długości $0, 1, 2, \dots$ i budowanie tablicy przejść metodą BFS.
Liczba rozróżnialnych klas równoważności Myhilla-Nerode'a to co najwyżej
$n = 10$, więc do ich pełnej identyfikacji wystarczą słowa długości $\leq 9$.

Łączna liczba słów nad $\{a, b\}$ o długości $\leq 9$:

$$
\sum_{k=0}^{9} 2^k = 2^{10} - 1 = 1023
$$

Zmieści się to w limicie 1000 zapytań jedynie przy ostrożnym planowaniu
— stąd przewaga podejścia L*.

### 6.3 Reprezentacja DFA w pamięci klienta (to tylko propozycja)

```cpp
struct DFA {
    static constexpr int N = 10;
    int delta[N];   // delta[stan][0=a, 1=b] = stan docelowy
    bool accept[N];
    int start = 0;

    bool run(const std::string& word) const {
        int q = start;
        for (char c : word)
            q = delta[q][c == 'a' ? 0 : 1];
        return accept[q];
    }
};
```


---

## 7. Ograniczenia i reguły

1. **Jeden klient jednocześnie** — każda drużyna może mieć co najwyżej 1 aktywną sesję na rundę.
2. **Limit zapytań:** 1000 `QUERY` na sesję.
3. **Faza uczenia** powinna zakończyć się w ciągu **30 sekund**.
4. **Timeout odpowiedzi w fazie weryfikacji:** 2 sekundy na każde `ANSWER`.
5. **Zakaz ataku na serwer** — zabrania się prób obejścia protokołu, skanowania
portów i wszelkich działań zakłócających pracę serwera.
1. **Samodzielność implementacji** — użycie gotowych bibliotek do uczenia automatów
(np. `libalf`, `LearnLib`) jest **niedozwolone**. Dozwolone są biblioteki
ogólnego zastosowania (STL, Asio, boost, fmt, scippp, spdlog itp.).
1. **Format kodu źródłowego** — należy dostarczyć kompilujący się projekt (CMake, bazel, conan itp.); kod musi być czytelny i zawierać krótki opis zastosowanego algorytmu.

---

## 8. Dostarczone materiały

Organizatorzy udostępniają:

- **Serwer testowy** ([`dfa_server.cpp`](https://w-wieczorek.github.io/cpp1-2/konkurs/serwer/dfa_serwer.cpp)) — działa przez cały czas trwania konkursu.
- **Przykładowy klient** w C++ (minimalna implementacja, bez algorytmu uczącego).
- **Specyfikację formalną protokołu** (ten dokument).
- **Generator DFA** — narzędzie do lokalnego generowania losowych automatów
(zawarty w kodzie serwera).

---

## 9. Przykładowa sesja

```
C→S: HELLO team_kowalski
S→C: WELCOME 3d9f1a00 LIMIT 1000

C→S: QUERY eps
S→C: RESULT NO

C→S: QUERY a
S→C: RESULT YES

C→S: QUERY b
S→C: RESULT NO

C→S: QUERY aa
S→C: RESULT NO

C→S: QUERY ab
S→C: RESULT YES

  ... (kolejne zapytania budujące model) ...

C→S: STATUS
S→C: STATUS QUERIES_USED 87 QUERIES_LEFT 913

C→S: SUBMIT
S→C: VERIFY_START  tests=332

S→C: TEST eps
C→S: ANSWER NO
S→C: ACK

S→C: TEST a
C→S: ANSWER YES
S→C: ACK

S→C: TEST aabba
C→S: ANSWER YES
S→C: ACK

  ... (łącznie 332 testy) ...

S→C: SUCCESS  score=1208  queries=285

C→S: QUIT
S→C: BYE
```


---

## 10. Harmonogram

| Etap | Termin |
| :-- | :-- |
| Udostępnienie dokumentacji i serwera testowego | T+0 |
| Otwarcie serwera konkursowego (runda 1–5) | T+14 dni |
| Zamknięcie serwera konkursowego | T+14 dni |
| Ogłoszenie wyników | T+14 dni |


---

## 11. Literatura pomocnicza

1. D. Angluin, *Learning Regular Sets from Queries and Counterexamples*,
Information and Computation, 75(2):87–106, 1987.
2. T. S. Chow, *Testing Software Design Modeled by Finite-State Machines*,
IEEE Trans. Softw. Eng., 4(3):178–187, 1978.
3. Asio C++ Library — dokumentacja: [https://think-async.com/Asio/](https://think-async.com/Asio/)
4. M. J. Kearns, U. V. Vazirani, *An Introduction to Computational Learning Theory*,
MIT Press, 1994, rozdz. 8.
