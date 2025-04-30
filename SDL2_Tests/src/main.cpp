#include "StaticTest.hpp"
#include "AlphaTest.hpp"
#include "RotateTest.hpp"
#include "MoveTest.hpp"
#include "definitions.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char* argv[]) {

    // Inicjalizujemy bibliotekê SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Blad inicjalizacji biblioteki SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Inicjalizujemy bibliotekê SDL_Image odpowiedzialn¹ za obs³ugê plików PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Blad inicjalizacji biblioteki SDL_Image: " << IMG_GetError() << std::endl;
        return 1;
    }

    // Tworzymy okno SDL
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Blad podczas tworzenia okna: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tworzymy renderer SDL, który bêdzie odpowiedzialny za rysowanie
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Blad podczas tworzenia renderera SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tworzymy wektor przechowuj¹cy ró¿ne testy wydajnoœciowe renderowania grafiki
    std::vector<std::unique_ptr<PerformanceTest>> tests;
    tests.push_back(std::make_unique<StaticTest>()); // Test renderowania statycznych obrazków
    tests.push_back(std::make_unique<AlphaTest>());  // Test renderowania obrazków z mieszaniem alfa
    tests.push_back(std::make_unique<RotateTest>()); // Test renderowania obrazków rotuj¹cych
    tests.push_back(std::make_unique<MoveTest>());   // Test renderowania obrazków poruszaj¹cych siê (wraz z animacj¹ poruszania siê)

    // W pêtli przechodzimy po wszystkich testach i uruchamiamy ka¿dy z nich po kolei
    for (const auto& test : tests) {
        std::cout << "Uruchomiony test: " << test->getName() << std::endl;
        test->run(renderer); // Uruchamiamy dany test
    }

    // Zwalniamy zasoby SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    // Na koniec informujemy o zakoñczeniu wszystkich testów
    std::cout << "Zakonczono wszystkie testy." << std::endl;
    return 0;
}