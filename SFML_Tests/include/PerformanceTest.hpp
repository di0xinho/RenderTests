#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "definitions.hpp"

// Dla ka�dego testu wydajno�ci utworzona zosta�a klasa bazowa;
// Wszystkie klasy testowe musz� dziedziczy� po klasie 'PerformanceTest' i implementowa� jej metody wirtualne
class PerformanceTest {
public:
    virtual void run(sf::RenderWindow& window) = 0; // Metoda wirtualna uruchamiaj�ca test
    virtual std::string getName() const = 0; // Metoda wirtualna zwracaj�ca nazw� testu
    virtual ~PerformanceTest() = default; // Destruktor
};