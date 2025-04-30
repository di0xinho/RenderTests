#pragma once
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicz�ca po PerformanceTest reprezentuj�ca test renderowania dla obiekt�w rotuj�cych
class RotateTest : public PerformanceTest {
public:
    void run(sf::RenderWindow& window) override; // Metoda uruchamiaj�ca test
    std::string getName() const override { return "sfml_rotate.csv"; } // Metoda zwracaj�ca nazw� testu (nazwa wykorzystywana do zapisywania wynik�w do pliku .csv)
};