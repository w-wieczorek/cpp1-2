# Projekt programistyczny 48h

## Sformułowanie problemu

Dany jest tekst *T*. Jeśli jego wszystkie różne niepuste podsłowa uporządkujemy leksykograficznie, to które z nich będzie *k*-tym najmniejszym podsłowem? Jeśli na przykład *T* = `babaacbaab` oraz *k* = 10, to szukanym podsłowem będzie `aba`. Wyjaśnienie: dziesięcioma pierwszymi różnymi podsłowami są w kolejności leksykograficznej: `a`, `aa`, `aab`, `aac`, `aacb`, `aacba`, `aacbaa`, `aacbaab`, `ab` oraz `aba`.

## Format danych wejściowych i wyjściowych

Na wejściu dostajemy plik tekstowy zawierający dwa wiersze. W pierwszym wierszu znajduje się test *T* złożony z liter `a`–`z` o długości nie przekraczającej 500K znaków. W drugim wierszu znajduje się dodatnia liczba całkowita *k* mieszcząca się w typie `long long int`. Na przykład:
```txt
bcaaaccbaca
49
```

Na wyjściu należy wypisać *k*-te najmniejsze podsłowo:
```txt
cbac
```

## Warunki zaliczenia

Napisać program konsolowy w języku C/C++, który odczytuje dane z pliku tekstowego wskazanego jako parametr wywołania programu (proszę przyjąć formatowanie danych wg powyższego przykładu), a na standardowym wyjściu wypisuje rozwiązanie. Limit czasowy wynosi 5 sekund.

Program może korzystać z dodatkowych (ogólnie dostępnych w Internecie) bibliotek pod warunkiem, że mamy dostęp do ich kodu źródłowego w języku C/C++. Jeśli program ma postać więcej niż jednego pliku, to powinien być zorganizowany w projekt, który da się skompilować za pomocą ogólnie dostępnego narzędzia (`dotnet build`, `make` itp. w zależności od systemu operacyjnego i polecanego kompilatora).

Programy będą oceniane na podstawie trzech zestawów danych wejściowych: (i) z tekstem o długości 50 znaków, (ii) z tekstem o długości 5000 znaków i (iii) z tekstem o długości 500 tysięcy znaków. Rozwiązanie w limicie czasowym pierwszego zestawu daje ocenę dostateczną, drugiego – dobrą, trzeciego – bardzo dobrą.
