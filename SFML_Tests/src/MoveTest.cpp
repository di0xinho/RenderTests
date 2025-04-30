#include "MoveTest.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj¹ca Sprite
struct MovingSprite {
    sf::Sprite sprite; // Obiekt Sprite
    sf::Vector2f velocity; // Wektor prêdkoœci Sprite'a
    int currentFrame = 0; // Aktualna klatka animacji
    float animationTimer = 0.0f; // Timer do zarz¹dzania animacji
};

// Implementacja metody run uruchamiaj¹cej test
void MoveTest::run(sf::RenderWindow& window) {

    // Progi dla liczby obiektów do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog pod wyniki testów
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultatów
    std::ofstream out(std::string(RESULTS_PATH) + "/" + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // Wczytujemy tekstury do animacji
    sf::Texture runTextures[3];
    if (!runTextures[0].loadFromFile(RUN_TEXTURE_1) ||
        !runTextures[1].loadFromFile(RUN_TEXTURE_2) ||
        !runTextures[2].loadFromFile(RUN_TEXTURE_3)) {

        // W przypadku b³êdu zwracamy odpowiedni komunikat
        std::cerr << "Blad wczytania tekstur" << std::endl;
        return;
    }

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wyników
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - runTextures[0].getSize().x);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - runTextures[0].getSize().y);
    std::uniform_real_distribution<float> velDist(-5.0f, 5.0f);

    // Dokonujemy iteracji po ró¿nych prograch liczby obiektów
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj¹cy sprite'y
        std::vector<MovingSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprite'y z losow¹ pozycj¹ i prêdkoœci¹
        for (int i = 0; i < count; ++i) {

            sf::Sprite sprite(runTextures[0]); // Ustawiamy sprite'om pierwsz¹ teksturkê z tablicy

            // Losowanie pozycji
            float x = static_cast<float>(xDist(rng));
            float y = static_cast<float>(yDist(rng));

            // Losowanie prêdkoœci
            float vx = velDist(rng);
            float vy = velDist(rng);

            sprite.setPosition(x, y);
            sprites.push_back({ sprite, {vx, vy}, 0, 0.0f });
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

            // W iteracji po wszystkich sprite'ach aktualizujemy ich pozycje i animacje
            for (auto& ms : sprites) {

                // Aktualizujemy pozycje i odbicia od krawêdzi okna
                sf::Vector2f pos = ms.sprite.getPosition();
                pos += ms.velocity;

                // Zapewniamy mechanizm odbicia siê od krawêdzi okna
                if (pos.x < 0 || pos.x + ms.sprite.getTexture()->getSize().x > WINDOW_WIDTH)
                    ms.velocity.x = -ms.velocity.x;
                if (pos.y < 0 || pos.y + ms.sprite.getTexture()->getSize().y > WINDOW_HEIGHT)
                    ms.velocity.y = -ms.velocity.y;

                ms.sprite.move(ms.velocity);

                // Aktualizujemy animacje
                ms.animationTimer += delta;
                if (ms.animationTimer >= 0.1f) {
                    ms.animationTimer = 0.0f;
                    ms.currentFrame = (ms.currentFrame + 1) % 3;
                    ms.sprite.setTexture(runTextures[ms.currentFrame]);
                }
            }

            // Czyszczenie okna i renderowanie ka¿dego sprite'a
            window.clear();

            for (const auto& ms : sprites)
                window.draw(ms.sprite);

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
