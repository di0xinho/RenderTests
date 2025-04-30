#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicz�ca po PerformanceTest reprezentuj�ca test renderowania dla obiekt�w rotuj�cych
class RotateTest : public PerformanceTest {
public:
    void run(SDL_Renderer* renderer) override;  // Metoda uruchamiaj�ca test
    const char* getName() const override; // Metoda zwracaj�ca nazw� testu
};
