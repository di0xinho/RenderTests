#pragma once
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicząca po PerformanceTest reprezentująca test renderowania dla obiektów z mieszaniem alpha
class AlphaTest : public PerformanceTest {
public:
    void run(sf::RenderWindow& window) override; // Metoda uruchamiająca test
    std::string getName() const override { return "sfml_alpha.csv"; } // Metoda zwracająca nazwę testu (nazwa wykorzystywana do zapisywania wyników do pliku .csv)
};