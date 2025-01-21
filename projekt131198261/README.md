# Projekt FILLOMINO

- [Opis projektu na zaliczenie programowania w języku C/C++](#projekt-fillomino)
  - [Zasady łamigłówki Fillomino](#zasady-łamigłówki-fillomino)
    - [Rekwizyty](#rekwizyty)
    - [Podstawowe definicje](#podstawowe-definicje)
    - [Cel gry](#cel-gry)
  - [Warunki zaliczenia](#warunki-zaliczenia)

## Zasady łamigłówki Fillomino

### Rekwizyty

Kwadratowa plansza z 64 polami (8x8), liczby całkowite od 1 do 8. Plansza początkowo jest częściowo wypełniona liczbami, w taki sposób, że pole może być puste lub zawierać jedną liczbę.

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

Pole _przyległe_ oznacza pole położone bezpośrednio nad, pod, z lewej, lub z prawej strony danego pola. _Spójny blok pól_ to grupa pól (co najmniej jedno pole), w której drugie pole jest przyległe do pierwszego, trzecie do pierwszego lub drugiego, czwarte do pierwszego lub drugiego lub trzeciego itd., aż do ostatniego pola danej grupy.

Przykład (pola oznaczone cyfrą 6 stanowią spójny blok pól, pola oznaczone cyfrą 2 nie stanowią spójnego bloku pól):
```
. . . . . . . .
. . . 6 6 . . .
. . 6 6 6 . . .
. . . 6 . . . .
. . . . . . . .
. 2 . . . . . .
. . 2 . . . . .
. . . . . . . .
```

### Cel gry

Wypełnić puste pola za pomocą liczb całkowitych (do dyspozycji mamy tylko liczby od 1 do 8, każda z nich może być użyta wielokrotnie lub ani razu) w taki sposób, aby każdy maksymalny spójny blok pól zawierających tę samą liczbę _n_ składał się dokładnie z _n_ pól.

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

Napisać program konsolowy w języku C/C++, który ze standardowego wejścia odczytuje początkową planszę (proszę przyjąć formatowanie wg powyższych przykładów), a na standardowym wyjściu wypisuje rozwiązanie. Limit czasowy wynosi 10 sekund.

Program może korzystać z dodatkowych (ogólnie dostępnych w Internecie) bibliotek pod warunkiem, że mamy dostęp do ich kodu źródłowego. Jeśli program ma postać więcej niż jednego pliku, to powinien być zorganizowany w projekt, który da się skompilować za pomocą ogólnie dostępnego narzędzia (`dotnet build`, `make` itp. w zależności od systemu operacyjnego i polecanego kompilatora).

Programy będą oceniane na podstawie trzech plansz: pierwsza będzie wypełniona w stopniu od 75% do 100%,
druga w stopniu od 50% do 75%, a trzecia w stopniu od 25% do 50%. Rozwiązanie w limicie czasowym pierwszej planszy daje ocenę dostateczną, drugiej – dobrą, trzeciej – bardzo dobrą.
