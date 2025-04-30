#pragma once
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicz¹ca po PerformanceTest reprezentuj¹ca test renderowania dla obiektów statycznych
class StaticTest : public PerformanceTest {
public:
    void run() override; // Metoda uruchamiaj¹ca test
    const char* getName() const override; // Metoda zwracaj¹ca nazwê testu
};