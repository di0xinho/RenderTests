#pragma once
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicząca po PerformanceTest reprezentująca test renderowania dla obiektów statycznych
class StaticTest : public PerformanceTest {
public:
    void run(sf::RenderWindow& window) override; // Metoda uruchamiająca test
    std::string getName() const override { return "sfml_static.csv"; } // Metoda zwracająca nazwę testu (nazwa wykorzystywana do zapisywania wyników do pliku .csv)
};
