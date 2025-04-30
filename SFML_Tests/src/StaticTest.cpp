#include "StaticTest.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj�ca Sprite
struct StaticSprite {
    sf::Sprite sprite; 
};

// Implementacja metody run uruchamiaj�cej test
void StaticTest::run(sf::RenderWindow& window) {

    // Definiujemy progi dla liczby obiekt�w do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog na wyniki test�w
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultat�w
    std::ofstream out(std::string(RESULTS_PATH) + "/" + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // �adujemy tekstur� dla sprite'�w
    sf::Texture texture;
    if (!texture.loadFromFile(IDLE_TEXTURE)) {
        // Je�li nie uda si� wczyta� tekstury, wy�wietlamy komunikat o b��dzie i ko�czymy test
        std::cerr << "B��d wczytania tekstury" << std::endl;
        return;
    }

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wynik�w
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - texture.getSize().x);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - texture.getSize().y);

    // Iterujemy po progach liczby obiekt�w
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj�cy sprite'y
        std::vector<StaticSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprite'y z losow� pozycj�
        for (int i = 0; i < count; ++i) {

            sf::Sprite sprite(texture);

            // Losujemy pozycje sprite'�w w oknie
            float x = static_cast<float>(xDist(rng));
            float y = static_cast<float>(yDist(rng));

            sprite.setPosition(x, y);
            sprites.push_back({ sprite });
        }

        int frames = 0; // Licznik klatek
        auto startTime = std::chrono::high_resolution_clock::now(); // Czas rozpocz�cia testu

        // G��wna p�tla testu - dzia�amy tak d�ugo jak ustawiony jest czas trwania testu, czyli 10s
        while (true) {

            // Rozpoczynamy mierzenie czasu od pobrania aktualnego czasu
            auto now = std::chrono::high_resolution_clock::now();

            // Liczymy ile sekund min�o od rozpocz�cia testu
            float delta = std::chrono::duration<float>(now - startTime).count();

            // W momencie up�yni�cia wymaganego czasu przerywamy test
            if (delta >= TEST_DURATION_SECONDS)
                break;

            // Obs�uga zdarzenia okna
            sf::Event event;
            while (window.pollEvent(event)) {
                // Ko�czymy test, je�li u�ytkownik zamknie okno
                if (event.type == sf::Event::Closed)
                    return;
            }

            // Czy�cimy okno
            window.clear();

            // Renderujemy sprite'y
            for (const auto& ss : sprites)
                window.draw(ss.sprite);

            window.display();
            ++frames; // Zliczamy klatki
        }

        // Obliczamy FPS i zapisujemy wyniki do pliku
        float fps = static_cast<float>(frames) / TEST_DURATION_SECONDS;
        std::cout << "Ilosc obiektow: " << count << ", FPS: " << fps << std::endl;
        out << count << "," << fps << std::endl;
    }

    // Zamykamy plik wynikowy
    out.close();
}