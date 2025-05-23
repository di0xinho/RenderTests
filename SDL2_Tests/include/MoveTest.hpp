#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicząca po PerformanceTest reprezentująca test renderowania dla poruszających się obiektów
class MoveTest : public PerformanceTest {
public:
    void run(SDL_Renderer* renderer) override; // Metoda uruchamiająca test
    const char* getName() const override; // Metoda zwracająca nazwę testu
};
