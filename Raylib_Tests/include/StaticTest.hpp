#pragma once
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicząca po PerformanceTest reprezentująca test renderowania dla obiektów statycznych
class StaticTest : public PerformanceTest {
public:
    void run() override; // Metoda uruchamiająca test
    const char* getName() const override; // Metoda zwracająca nazwę testu
};