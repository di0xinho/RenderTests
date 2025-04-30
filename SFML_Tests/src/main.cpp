#include <SFML/Graphics.hpp>
#include <iostream>
#include "definitions.hpp" // Wszystkie makra zdefiniowane s¹ w pliku "include/definitions.hpp"
#include "StaticTest.hpp"
#include "AlphaTest.hpp"
#include "RotateTest.hpp"
#include "MoveTest.hpp"

int main() {

    // Stworzenie okna do renderowania grafiki
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    // Tworzymy wektor przechowuj¹cy ró¿ne testy wydajnoœciowe renderowania grafiki
    std::vector<std::unique_ptr<PerformanceTest>> tests;

    tests.push_back(std::make_unique<StaticTest>()); // Test renderowania statycznych obrazków
    tests.push_back(std::make_unique<AlphaTest>()); // Test renderowania obrazków z mieszaniem alfa
    tests.push_back(std::make_unique<RotateTest>()); // Test renderowania obrazków rotuj¹cych
    tests.push_back(std::make_unique<MoveTest>()); // Test renderowania obrazków poruszaj¹cych siê (wraz z animacj¹ poruszania siê)

    // W pêtli przechodzimy po wszystkich testach i uruchamiamy ka¿dy z nich po kolei
    for (auto& test : tests) {
        std::cout << "Uruchomiony test: " << test->getName() << std::endl;

        // Uruchamiamy test, a jako parametr przekazujemy okno renderowania
        test->run(window);
    }

    return 0;
}
