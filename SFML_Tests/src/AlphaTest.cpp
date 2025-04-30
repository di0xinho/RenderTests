#include "AlphaTest.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj¹ca sprite 
struct AlphaSprite {
    sf::Sprite sprite; 
};

// Implementacja metody run uruchamiaj¹cej test
void AlphaTest::run(sf::RenderWindow& window) {

    // Progi dla liczby obiektów do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog pod wyniki testów
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultatów
    std::ofstream out(std::string(RESULTS_PATH) + "/" + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // £adujemy teksturê 
    sf::Texture texture;
    if (!texture.loadFromFile(IDLE_TEXTURE)) {
        // W przypadku niepowodzenia wyœwietlamy odpowiedni komunikat
        std::cerr << "B³¹d wczytania tekstury" << std::endl;
        return;
    }

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wyników
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - texture.getSize().x);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - texture.getSize().y);

    // Iterujemy po ró¿nych progach liczby obiektów
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj¹cy sprite'y
        std::vector<AlphaSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprity z losow¹ pozycj¹ i przezroczystoœci¹
        for (int i = 0; i < count; ++i) {

            sf::Sprite sprite(texture);

            // Ustawiamy losow¹ pozycji sprite'a
            float x = static_cast<float>(xDist(rng));
            float y = static_cast<float>(yDist(rng));

            sprite.setPosition(x, y);

            // Ustawiamy 50% przezroczystoœci (czyli wartoœæ alfa na 128)
            sprite.setColor(sf::Color(255, 255, 255, 128));
            sprites.push_back({ sprite });
        }

        int frames = 0; // Licznik klatek
        auto startTime = std::chrono::high_resolution_clock::now(); // Czas rozpoczêcia testu

        // G³ówna pêtla testu - dzia³amy tak d³ugo jak ustawiony jest czas trwania testu, czyli 10s
        while (true) {

            // Rozpoczynamy mierzenie czasu od pobrania aktualnego czasu
            auto now = std::chrono::high_resolution_clock::now();

            // Liczymy ile sekund minê³o od rozpoczêcia testu
            float delta = std::chrono::duration<float>(now - startTime).count();

            // W momencie up³yniêcia wymaganego czasu przerywamy test
            if (delta >= TEST_DURATION_SECONDS)
                break;

            // Obs³uga zdarzeñ okna
            sf::Event event;

            // W momencie zamkniêcia okna, test jest wtedy koñczony
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    return; 
            }

            // Czyszczenie okna i renderowanie ka¿dego sprite'a z przezroczystoœci¹
            window.clear(sf::Color::Black);
            for (const auto& as : sprites)
                window.draw(as.sprite);
            window.display();

            ++frames; // Zliczamy ka¿d¹ klatkê
        }

        // Obliczamy liczbê FPS i zapisujemy wyniki do pliku
        float fps = static_cast<float>(frames) / TEST_DURATION_SECONDS;

        std::cout << "Ilosc obiektow: " << count << ", FPS: " << fps << std::endl;
        out << count << "," << fps << std::endl;
    }

    // Na koniec zamykamy plik wynikowy
    out.close();
}