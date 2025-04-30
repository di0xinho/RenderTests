#include <SFML/Graphics.hpp>
#include <iostream>
#include "definitions.hpp" // Wszystkie makra zdefiniowane s� w pliku "include/definitions.hpp"
#include "StaticTest.hpp"
#include "AlphaTest.hpp"
#include "RotateTest.hpp"
#include "MoveTest.hpp"

int main() {

    // Stworzenie okna do renderowania grafiki
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    // Tworzymy wektor przechowuj�cy r�ne testy wydajno�ciowe renderowania grafiki
    std::vector<std::unique_ptr<PerformanceTest>> tests;

    tests.push_back(std::make_unique<StaticTest>()); // Test renderowania statycznych obrazk�w
    tests.push_back(std::make_unique<AlphaTest>()); // Test renderowania obrazk�w z mieszaniem alfa
    tests.push_back(std::make_unique<RotateTest>()); // Test renderowania obrazk�w rotuj�cych
    tests.push_back(std::make_unique<MoveTest>()); // Test renderowania obrazk�w poruszaj�cych si� (wraz z animacj� poruszania si�)

    // W p�tli przechodzimy po wszystkich testach i uruchamiamy ka�dy z nich po kolei
    for (auto& test : tests) {
        std::cout << "Uruchomiony test: " << test->getName() << std::endl;

        // Uruchamiamy test, a jako parametr przekazujemy okno renderowania
        test->run(window);
    }

    return 0;
}
