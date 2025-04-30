#pragma once
#include <SDL.h>

// Dla ka�dego testu wydajno�ci utworzona zosta�a klasa bazowa;
// Wszystkie klasy testowe musz� dziedziczy� po klasie 'PerformanceTest' i implementowa� jej metody wirtualne
class PerformanceTest {
public:
    virtual void run(SDL_Renderer* renderer) = 0; // Metoda wirtualna uruchamiaj�ca test
    virtual const char* getName() const = 0; // Metoda wirtualna zwracaj�ca nazw� testu
    virtual ~PerformanceTest() = default; // Destruktor
};
