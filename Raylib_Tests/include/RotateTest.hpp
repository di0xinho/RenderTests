#pragma once
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicz�ca po PerformanceTest reprezentuj�ca test renderowania dla obiekt�w rotuj�cych
class RotateTest : public PerformanceTest {
public:
    void run() override; // Metoda uruchamiaj�ca test
    const char* getName() const override; // Metoda zwracaj�ca nazw� testu
};