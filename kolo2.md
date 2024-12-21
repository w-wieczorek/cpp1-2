## Zadanie 1.

Zdefiniujmy ciąg liczbowy $f_n$ ($n = 0, 1, 2, \ldots$) za pomocą równania rekurencyjnego o postaci $f_{n + k} = a_0 f_{n + k - 1} + a_1 f_{n + k - 2} + \ldots + a_{k - 1} f_n$, gdzie $a_i$ ($i = 0, 1, \ldots, k - 1$) są całkowite. Napisz funkcję zwracającą liczbę $f_n$.

```cpp
// Wymagane pliki nagłówkowe

int f(int n, int a[], int w[], int k)
{
    // Tu wstaw swój kod
}

int main()
{
    int a[] = {2, 0, -1}  // f_{n + 3} = 2 f_{n + 2} - f_n
    int w[] = {1, 2, 4}   // f_0 = 1, f_1 = 2, f_2 = 4
    printf("%d", f(7, a, w, 3))  // f_7 czyli 54
    return 0
}
```

---

## Zadanie 2.

Napisz funkcję sortującą tablicę łańcuchów w następującym porządku. Łańcuch $u$ jest mniejszy niż łańcuch $v$, jeśli $|u| < |v|$ lub $|u| = |v| \, \wedge \, u \prec v$. 

```cpp
// Wymagane pliki nagłówkowe oraz ewentualnie dodatkowe funkcje

void my_sort(char* tab[], int n)
{
    // Tu wstaw swój kod
}

int main()
{
    char* a[] = {"ala", "ma", "kota", "i", "psa"};
    my_sort(a, 5);
    for (int i = 0; i < 5; ++i)
        printf("%s ", a[i]);
    // i ma ala psa kota
    return 0;
}
```

---

## Zadanie 3.

Dane są dwie tablice, $w$ i $s$, liczb całkowitych dodatnich. Napisz w języku C funkcję, która szereguje jak największy podzbiór indeksów w ciąg, w którym jednocześnie $w[i]$ rośnie, a $s[i]$ maleje.

```cpp
// Wymagane pliki nagłówkowe oraz ewentualnie dodatkowe funkcje

void write(int w[], int s[], int n)
{
    // Tu wstaw swój kod
}

int main()
{
    int w[] = {70, 40, 50, 80, 100, 110};
    int s[] = {20, 40, 30, 20, 10, 100}; 
    write(w, s, 6);  // 1 2 0 4
    return 0;
}
```
