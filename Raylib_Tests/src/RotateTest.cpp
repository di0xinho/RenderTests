#include "RotateTest.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj�ca sprite 
struct RotateSprite {
    Vector2 position; // Pozycja sprite'a
    float rotation;   // K�t o jaki obraca si� sprite
};

// Implementacja metody run uruchamiaj�cej test
void RotateTest::run() {
    // Definiujemy progi dla liczby obiekt�w do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog na wyniki test�w
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultat�w
    std::ofstream out(std::string(RESULTS_PATH) + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // �adujemy tekstur� dla sprite'�w
    Texture2D texture = LoadTexture(IDLE_TEXTURE);

    // Ustawiamy punkt obrotu sprite'�w na ich �rodek
    Vector2 origin = { (float)texture.width / 2, (float)texture.height / 2 };

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wynik�w
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - texture.width);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - texture.height);

    // Iterujemy po progach liczby obiekt�w
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj�cy sprite'y
        std::vector<RotateSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprite'y z losow� pozycj� i k�tem obrotu r�wnym 0
        for (int i = 0; i < count; ++i) {

            // Ustawiamy losow� pozycj� sprite'a
            float x = static_cast<float>(xDist(rng));
            float y = static_cast<float>(yDist(rng));

            sprites.push_back({ Vector2{ x, y }, 0.0f });
        }

        
        int frames = 0; // Licznik klatek
        auto startTime = std::chrono::high_resolution_clock::now(); // Czas rozpocz�cia testuu

        // G��wna p�tla testu - dzia�amy tak d�ugo jak ustawiony jest czas trwania testu, czyli 10s
        while (!WindowShouldClose()) {

            // Rozpoczynamy mierzenie czasu od pobrania aktualnego czasu
            auto now = std::chrono::high_resolution_clock::now();

            // Liczymy ile sekund min�o od rozpocz�cia testu
            float delta = std::chrono::duration<float>(now - startTime).count();

            // W momencie up�yni�cia wymaganego czasu przerywamy test
            if (delta >= TEST_DURATION_SECONDS)
                break;
            
            // Rozpoczynamy rysowanie
            BeginDrawing();
            ClearBackground(BLACK); // Czarne t�o

            // Obracamy sprite'y i renderujemy je
            for (auto& s : sprites) {
                s.rotation += 1.0f; // Obracamy sprite o 1 stopie�
                DrawTexturePro(
                    texture,
                    { 0, 0, (float)texture.width, (float)texture.height }, // �r�d�o tekstury
                    { s.position.x, s.position.y, (float)texture.width, (float)texture.height }, // Miejsce renderowania
                    origin, // Punkt obrotu
                    s.rotation, // K�t obrotu
                    WHITE // Kolor
                );
            }

            // Ko�czymy rysowanie
            EndDrawing();

            ++frames; // Zliczamy klatki
        }

        // Obliczamy FPS i zapisujemy wyniki do pliku
        float fps = static_cast<float>(frames) / TEST_DURATION_SECONDS;
        std::cout << "Ilosc obiektow: " << count << ", FPS: " << fps << std::endl;
        out << count << "," << fps << std::endl;
    }

    // Zwalniamy tekstur�
    UnloadTexture(texture);

    // Zamykamy plik wynikowy
    out.close();
}

// Zwracamy nazw� pliku wynikowego
const char* RotateTest::getName() const {
    return "raylib_rotate.csv";
}