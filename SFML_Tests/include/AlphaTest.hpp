#pragma once
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicz¹ca po PerformanceTest reprezentuj¹ca test renderowania dla obiektów z mieszaniem alpha
class AlphaTest : public PerformanceTest {
public:
    void run(sf::RenderWindow& window) override; // Metoda uruchamiaj¹ca test
    std::string getName() const override { return "sfml_alpha.csv"; } // Metoda zwracaj¹ca nazwê testu (nazwa wykorzystywana do zapisywania wyników do pliku .csv)
};