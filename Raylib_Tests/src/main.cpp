#include "StaticTest.hpp"
#include "AlphaTest.hpp"
#include "RotateTest.hpp"
#include "MoveTest.hpp"
#include "definitions.hpp" // Wszystkie makra zdefiniowane są w pliku "include/definitions.hpp"
#include <memory>
#include <vector>
#include <iostream>

int main() {

    // Inicjalizujemy okno aplikacji przy użyciu biblioteki Raylib
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    // Ustawiamy brak ograniczeń FPS (0 oznacza brak limitu)
    SetTargetFPS(0);

    // Tworzymy wektor przechowujący różne testy wydajnościowe renderowania grafiki
    std::vector<std::unique_ptr<PerformanceTest>> tests;

    tests.push_back(std::make_unique<StaticTest>()); // Test renderowania statycznych obrazków
    tests.push_back(std::make_unique<AlphaTest>()); // Test renderowania obrazków z mieszaniem alfa
    tests.push_back(std::make_unique<RotateTest>()); // Test renderowania obrazków rotujących
    tests.push_back(std::make_unique<MoveTest>()); // Test renderowania obrazków poruszających się (wraz z animacją poruszania się)

    // W pętli przechodzimy po wszystkich testach i uruchamiamy każdy z nich po kolei
    for (const auto& test : tests) {
        std::cout << "Uruchomiony test: " << test->getName() << std::endl;

        // Uruchamiamy dany test
        test->run();
    }

    // Zamykamy okno aplikacji po zakończeniu testów
    CloseWindow();
    return 0;
}
