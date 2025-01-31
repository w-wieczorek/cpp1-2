# Projekt krzyżówki

## Format danych wejściowych i wyjściowych

Celem projektu jest wypełnienie zadanej krzyżówki za pomocą podzbioru słów z zadanego zbioru słów. Na wejściu znajdują się informacje o rozmiarze krzyżówki (zawsze będzie to kratownica NxN) i liczbie słów, z których wybieramy słowa do uzupełnienia krzyżówki. Następnie, od wiersza drugiego, mamy diagram z zaznaczonymi pustymi i zacieniowanymi polami. Na końcu znajduje się zbiór słów; każde słowo zapisane jest w oddzielnym wierszu. Dane wejściowe nie będą zawierały polskich znaków diakrytycznych. Słowa nie będą krótsze niż 3 znaki.

Przykład:
```
4 10
1 . 2 X
. X . X
3 . . .
. X . X
pies
kot
ryba
lura
drut
tata
rata
kara
luty
bak 
```

Liczby w diagramie (od 1 do maksymalnie 40) pomagają w przedstawieniu rozwiązania, kropki to puste pola a znak X oznacza pole zacieniowane, do którego nie wpisujemy litery.

W rozwiązaniu żadne słowo nie może pojawić się dwa razy i powinno ono mieć postać, jak pokazano poniżej:
```
Poziomo:
1) kot
3) rata
Pionowo:
1) kara
2) tata
```

## Warunki zaliczenia

Napisać program konsolowy w języku C, który ze standardowego wejścia odczytuje dane (proszę przyjąć formatowanie wg powyższego przykładu), a na standardowym wyjściu wypisuje rozwiązanie. Limit czasowy wynosi 10 sekund.

Program może korzystać z dodatkowych (ogólnie dostępnych w Internecie) bibliotek pod warunkiem, że mamy dostęp do ich kodu źródłowego w języku C. Jeśli program ma postać więcej niż jednego pliku, to powinien być zorganizowany w projekt, który da się skompilować za pomocą ogólnie dostępnego narzędzia (`dotnet build`, `make` itp. w zależności od systemu operacyjnego i polecanego kompilatora).

Programy będą oceniane na podstawie trzech diagramów: 4x4 oraz do 10 słów w zbiorze; 8x8 oraz do 300 słów w zbiorze, 16x16 oraz do 2000 słów w zbiorze. Rozwiązanie w limicie czasowym pierwszej krzyżówki daje ocenę dostateczną, drugiej – dobrą, trzeciej – bardzo dobrą.

Przykładowe dane testowe: [`we4.txt`](https://w-wieczorek.github.io/cpp1-2/konkurs/we4.txt), [`we8.txt`](https://w-wieczorek.github.io/cpp1-2/konkurs/we8.txt), [`we16.txt`](https://w-wieczorek.github.io/cpp1-2/konkurs/we16.txt). 
