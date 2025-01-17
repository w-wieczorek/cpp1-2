# Projekt FILLOMINO

- [Opis projektu na zaliczenie programowania w języku C/C++](#projekt-fillomino)
  - [Zasady łamigłówki Fillomino](#zasady-łamigłówki-fillomino)
    - [Rekwizyty](#rekwizyty)
    - [Podstawowe definice](#podstawowe-definicje)
    - [Cel gry](#cel-gry)
  - [Warunki zaliczenia](#warunki-zaliczenia)

## Zasady łamigłówki Fillomino

### Rekwizyty

Kwadratowa plansza z 64 polami (8x8), liczby całkowite od 1 do 9. Plansza początkowo jest częściowo wypełniona liczbami, w taki sposób, że pole może być puste lub zawierać jedną liczbę.

Przykład (kropka oznacza puste pole):
```
2 1 . 1 . . . .
. . 4 . . 1 6 .
. . 1 . . . 7 1
. . 3 3 7 7 . .
5 1 . 1 . 8 . .
. . 1 . 3 . 1 .
1 . . . 1 . . .
. 7 . 1 . 4 1 2
```

### Podstawowe definicje

Pole _przyległe_ oznacza pole położone bezpośrednio nad, pod, z lewej, lub z prawej strony danego pola. _Spójny blok pól_ to grupa pól (co najmniej jedno pole), w której pierwsze pole jest przyległe do drugiego, drugie do trzeciego itd., aż do ostatniego pola danej grupy.

Przykład (pola oznaczone cyfrą 6 stanowią spójny blok pól, pola oznaczone cyfrą 3 nie stanowią spójnego bloku pól):
```
. . . . . . . .
. . . 6 6 . . .
. . 6 6 6 . . .
. . . 6 . . . .
. . . . . . . .
. 3 . . . . . .
. . 3 . . . . .
. . . . . . . .
```

### Cel gry

Wypełnić puste pola za pomocą liczb całkowitych (do dyspozycji mamy tylko liczby od 1 do 9, każda z nich może być użyta wielokrotnie lub ani razu) w taki sposób, aby każdy maksymalny spójny blok pól zawierających tę samą liczbę _n_ składał się dokładnie z _n_ pól.

Przykład planszy początkowej:
```
. . . 5 1 . 6 6
2 5 5 1 7 6 6 6
. . 2 . . 3 . .
. 3 1 . 7 7 7 7
. . . 2 3 . . .
5 1 4 3 2 6 4 .
. . 2 . . . . 4
5 5 2 . 1 6 . 6
```

Dopuszczalne rozwiązanie:
```
2 5 5 5 1 6 6 6
2 5 5 1 7 6 6 6
3 2 2 7 7 3 3 3
3 3 1 2 7 7 7 7
4 4 4 2 3 3 3 4
5 1 4 3 2 6 4 4
5 5 2 3 2 6 6 4
5 5 2 3 1 6 6 6
```

## Warunki zaliczenia

Napisać program konsolowy w języku C/C++, który ze standardowego wejścia odczytuje początkową planszę (proszę przyjąć formatowanie wg powyższych przykładów), a na standardowym wyjściu wypisuje rozwiązanie. Limit czasowy wynosi 30 sekund.

Program nie może korzystać z żadnych dodatkowych bibliotek dynamicznych (pliki `.dll`, `.so`), ani statycznych (pliki `.lib`, `.a`, `.o`), a kod źródłowy powinien mieć postać jednego pliku tekstowego (plik `.c` lub `.cpp`) o długości do 1000 wierszy.

Programy będą oceniane na podstawie trzech plansz: pierwsza będzie wypełniona w 75%,
druga w 50%, a trzecia w 25%. Rozwiązanie w limicie czasowym pierwszej planszy daje ocenę dostateczną, drugiej —— dobrą, trzeciej —— bardzo dobrą.
