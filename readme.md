# Testy wydajności renderowania grafiki: SFML, Raylib oraz SDL2

## Struktura Projektu

```
/ Główny katalog
    /SFML_Tests
    /Raylib_Tests
    /SDL2_Tests
```

Każdy katalog zawiera osobny projekt CMake.

---

## Budowanie projektu

W katalogu projektu (`SFML_Tests`, `Raylib_Tests`, `SDL2_Tests`) wykonaj:

```bash
cmake -B build
cmake --build build
```

Wszystkie zależności są pobierane automatycznie przez FetchContent (SFML, Raylib, SDL2, SDL2_image).

---

## Uruchamianie testów

Po zbudowaniu projektów, uruchamiaj pliki `.exe` ręcznie. Pliki mają tą samą nazwę: `main.exe`.
Znajdziesz je w odpowiednich katalogach build (np. out/build/x64-Debug/bin).

---

## Wyniki testów

Wyniki zapisywane są w plikach .csv w folderach Results/.

---

## Wymagania

- CMake 3.10+
- Kompilator C++ z obsługą C++17
- SDL2_image (dla SDL2_Tests)

---

## Informacje o testach

**Poziom obciążenia:** Liczba jednocześnie renderowanych obiektów

Przeprowadzone testy renderowania pozwalają określić, jak wymienione biblioteki radzą sobie z różnymi
typami obciążeń graficznych, takimi jak renderowanie:
- statycznych obrazów,
- obrazów z mieszaniem alfa,
- obrazów rotujących,
- obrazów poruszających się (zmieniających animacje).

Analiza wyników umożliwi ocenę wydajności w zależności od liczby jednocześnie wyświetlanych elementów,
co pozwala wskazać bibliotekę oferującą najlepszy balans między jakością renderingu a optymalizacją zasobów.

Wyniki badań mogą być pomocne dla twórców gier w wyborze odpowiedniego narzędzia do ich projektów.

---
