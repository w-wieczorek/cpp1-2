# Projekt krzyżówki

## Format danych wejściowych i wyjściowych

Celem projektu jest wypełnienie zadanej krzyżówki za pomocą podzbioru słów z zadanego zbioru słów. Na wejściu znajdują się informacje o rozmiarze krzyżówki (zawsze będzie to kratownica NxN) i liczbie słów, z których wybieramy słowa do uzupełnienia krzyżówki, następnie diagram z zaznaczonymi pustymi i zacieniowanymi polami, a na końcu każde słowo w oddzielnym wierszu. Dane wejściowe są kodowane w utf8.

Przykład:
```
4 10
 1  .  2  X
 .  X  .  X
 3  .  .  .
 .  X  .  X
pies
kot
ryba
but
śrut
tata
rata
kara
luty
bąk
```

Liczby w diagramie (od 1 do maksymalnie 99) pomagają w przedstawieniu rozwiązania, kropki to puste pola a znak X oznacza pole zacieniowane, do którego nie wpisujemy litery.

Rozwiązanie powinno mieć postać:
```
Poziomo:
1) kot
3) rata
Pionowo:
1) kara
2) tata
```

## Warunki zaliczenia

Napisać program konsolowy w języku C, który ze standardowego wejścia odczytuje dane (proszę przyjąć formatowanie wg powyższego przykładu), a na standardowym wyjściu wypisuje rozwiązanie. Limit czasowy wynosi 10 sekund. Układ pustych pól oraz zbiór słów będą tak dobrane, żeby istniało co najmniej jedno rozwiązanie.

Program może korzystać z dodatkowych (ogólnie dostępnych w Internecie) bibliotek pod warunkiem, że mamy dostęp do ich kodu źródłowego w języku C. Jeśli program ma postać więcej niż jednego pliku, to powinien być zorganizowany w projekt, który da się skompilować za pomocą ogólnie dostępnego narzędzia (`dotnet build`, `make` itp. w zależności od systemu operacyjnego i polecanego kompilatora).

Programy będą oceniane na podstawie trzech diagramów: 4x4, 8x8 i 12x12. Rozwiązanie w limicie czasowym pierwszej planszy daje ocenę dostateczną, drugiej – dobrą, trzeciej – bardzo dobrą.
