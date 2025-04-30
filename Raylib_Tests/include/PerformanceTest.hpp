#pragma once
#include <raylib.h>
#include <string>
#include "definitions.hpp"

// Dla ka¿dego testu wydajnoœci utworzona zosta³a klasa bazowa;
// Wszystkie klasy testowe musz¹ dziedziczyæ po klasie 'PerformanceTest' i implementowaæ jej metody wirtualne
class PerformanceTest {
public:
    virtual void run() = 0; // Metoda wirtualna uruchamiaj¹ca test
    virtual const char* getName() const = 0; // Metoda wirtualna zwracaj¹ca nazwê testu
    virtual ~PerformanceTest() = default; // Destruktor
};
