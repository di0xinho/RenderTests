#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "PerformanceTest.hpp"
#include "definitions.hpp"

// Klasa dziedzicz¹ca po PerformanceTest reprezentuj¹ca test renderowania dla poruszaj¹cych siê obiektów
class MoveTest : public PerformanceTest {
public:
    void run(SDL_Renderer* renderer) override; // Metoda uruchamiaj¹ca test
    const char* getName() const override; // Metoda zwracaj¹ca nazwê testu
};
