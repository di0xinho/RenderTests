#include "AlphaTest.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj�ca sprite 
struct AlphaSprite {
    sf::Sprite sprite; 
};

// Implementacja metody run uruchamiaj�cej test
void AlphaTest::run(sf::RenderWindow& window) {

    // Progi dla liczby obiekt�w do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog pod wyniki test�w
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultat�w
    std::ofstream out(std::string(RESULTS_PATH) + "/" + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // �adujemy tekstur� 
    sf::Texture texture;
    if (!texture.loadFromFile(IDLE_TEXTURE)) {
        // W przypadku niepowodzenia wy�wietlamy odpowiedni komunikat
        std::cerr << "B��d wczytania tekstury" << std::endl;
        return;
    }

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wynik�w
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - texture.getSize().x);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - texture.getSize().y);

    // Iterujemy po r�nych progach liczby obiekt�w
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj�cy sprite'y
        std::vector<AlphaSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprity z losow� pozycj� i przezroczysto�ci�
        for (int i = 0; i < count; ++i) {

            sf::Sprite sprite(texture);

            // Ustawiamy losow� pozycji sprite'a
            float x = static_cast<float>(xDist(rng));
            float y = static_cast<float>(yDist(rng));

            sprite.setPosition(x, y);

            // Ustawiamy 50% przezroczysto�ci (czyli warto�� alfa na 128)
            sprite.setColor(sf::Color(255, 255, 255, 128));
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

            // Obs�uga zdarze� okna
            sf::Event event;

            // W momencie zamkni�cia okna, test jest wtedy ko�czony
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    return; 
            }

            // Czyszczenie okna i renderowanie ka�dego sprite'a z przezroczysto�ci�
            window.clear(sf::Color::Black);
            for (const auto& as : sprites)
                window.draw(as.sprite);
            window.display();

            ++frames; // Zliczamy ka�d� klatk�
        }

        // Obliczamy liczb� FPS i zapisujemy wyniki do pliku
        float fps = static_cast<float>(frames) / TEST_DURATION_SECONDS;

        std::cout << "Ilosc obiektow: " << count << ", FPS: " << fps << std::endl;
        out << count << "," << fps << std::endl;
    }

    // Na koniec zamykamy plik wynikowy
    out.close();
}