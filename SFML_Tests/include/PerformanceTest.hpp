#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "definitions.hpp"

// Dla ka¿dego testu wydajnoœci utworzona zosta³a klasa bazowa;
// Wszystkie klasy testowe musz¹ dziedziczyæ po klasie 'PerformanceTest' i implementowaæ jej metody wirtualne
class PerformanceTest {
public:
    virtual void run(sf::RenderWindow& window) = 0; // Metoda wirtualna uruchamiaj¹ca test
    virtual std::string getName() const = 0; // Metoda wirtualna zwracaj¹ca nazwê testu
    virtual ~PerformanceTest() = default; // Destruktor
};