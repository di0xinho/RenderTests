#include "StaticTest.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj¹ca sprite
struct StaticSprite {
    Vector2 position; // Pozycja sprite'a
};

// Implementacja metody run uruchamiaj¹cej test
void StaticTest::run() {
    // Definiujemy progi dla liczby obiektów do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog na wyniki testów
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultatów
    std::ofstream out(std::string(RESULTS_PATH) + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // £adujemy teksturê dla sprite'ów
    Texture2D texture = LoadTexture(IDLE_TEXTURE);

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wyników
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - texture.width);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - texture.height);

    // Iterujemy po progach liczby obiektów
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj¹cy sprite'y
        std::vector<StaticSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprite'y z losow¹ pozycj¹
        for (int i = 0; i < count; ++i) {

            // Ustawiamy losow¹ pozycjê sprite'a
            float x = static_cast<float>(xDist(rng));
            float y = static_cast<float>(yDist(rng));

            sprites.push_back({ Vector2{ x, y } });
        }

        int frames = 0; // Licznik klatek
        auto startTime = std::chrono::high_resolution_clock::now(); // Czas rozpoczêcia testu

        // G³ówna pêtla testu - dzia³amy tak d³ugo jak ustawiony jest czas trwania testu, czyli 10s
        while (!WindowShouldClose()) {

            // Rozpoczynamy mierzenie czasu od pobrania aktualnego czasu
            auto now = std::chrono::high_resolution_clock::now();

            // Liczymy ile sekund minê³o od rozpoczêcia testu
            float delta = std::chrono::duration<float>(now - startTime).count();

            // W momencie up³yniêcia wymaganego czasu przerywamy test
            if (delta >= TEST_DURATION_SECONDS)
                break;

            // Rozpoczynamy rysowanie
            BeginDrawing();
            ClearBackground(BLACK); // Czarne t³o

            // Renderujemy wszystkie sprite'y
            for (const auto& s : sprites)
                DrawTextureV(texture, s.position, WHITE);

            // Koñczymy rysowanie
            EndDrawing();

            ++frames; // Zliczamy klatki
        }

        // Obliczamy FPS i zapisujemy wyniki do pliku
        float fps = static_cast<float>(frames) / TEST_DURATION_SECONDS;
        std::cout << "Ilosc obiektow: " << count << ", FPS: " << fps << std::endl;
        out << count << "," << fps << std::endl;
    }

    // Zwalniamy teksturê
    UnloadTexture(texture);

    // Zamykamy plik wynikowy
    out.close();
}

// Zwracamy nazwê pliku wynikowego
const char* StaticTest::getName() const {
    return "raylib_static.csv";
}