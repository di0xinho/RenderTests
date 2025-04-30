#include "MoveTest.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj�ca sprite 
struct MovingSprite {
    Vector2 position;      // Pozycja sprite'a
    Vector2 velocity;      // Pr�dko�� ruchu sprite'a (wektor pr�dko�ci - precyzyjniej)
    int currentFrame = 0;  // Obecna klatka animacji
    float animationTimer = 0.0f; // Zegar animacji
};

// Implementacja metody run uruchamiaj�cej test
void MoveTest::run() {

    // Definiujemy progi dla liczby obiekt�w do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog na wyniki test�w
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultat�w
    std::ofstream out(std::string(RESULTS_PATH) + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // �adujemy tekstury dla animacji ruchu
    Texture2D walkTextures[3];
    if (!(
        walkTextures[0] = LoadTexture(RUN_TEXTURE_1),
        walkTextures[1] = LoadTexture(RUN_TEXTURE_2),
        walkTextures[2] = LoadTexture(RUN_TEXTURE_3),
        walkTextures[0].id && walkTextures[1].id && walkTextures[2].id)) {
        // Je�li tekstury nie zostan� poprawnie za�adowane, wy�wietlamy komunikat o b��dzie i ko�czymy test
        std::cerr << "Blad podczas ladowania tekstury" << std::endl;
        return;
    }

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wynik�w
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - walkTextures[0].width);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - walkTextures[0].height);
    std::uniform_real_distribution<float> velDist(-5.0f, 5.0f);

    // Iterujemy po progach liczby obiekt�w
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj�cy sprite'y
        std::vector<MovingSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprite'y z losow� pozycj� i pr�dko�ci�
        for (int i = 0; i < count; ++i) {

            // Ustawiamy losow� pozycj� sprite'a
            float x = static_cast<float>(xDist(rng));
            float y = static_cast<float>(yDist(rng));

            // Ustawiamy losow� pr�dko�� sprite'a
            float vx = velDist(rng);
            float vy = velDist(rng);

            sprites.push_back({ Vector2{ x, y }, Vector2{ vx, vy } });
        }

        int frames = 0; // Licznik klatek
        auto startTime = std::chrono::high_resolution_clock::now(); // Czas rozpocz�cia testu

        // G��wna p�tla testu - dzia�amy tak d�ugo jak ustawiony jest czas trwania testu, czyli 10s
        while (!WindowShouldClose()) {

            // Rozpoczynamy mierzenie czasu od pobrania aktualnego czasu
            auto now = std::chrono::high_resolution_clock::now();

            // Liczymy ile sekund min�o od rozpocz�cia testu
            float delta = std::chrono::duration<float>(now - startTime).count();

            // W momencie up�yni�cia wymaganego czasu przerywamy test
            if (delta >= TEST_DURATION_SECONDS)
                break;

            // Aktualizujemy pozycj� i animacj� ka�dego sprite'a
            for (auto& s : sprites) {

                // Aktualizujemy pozycj� na podstawie pr�dko�ci
                s.position.x += s.velocity.x;
                s.position.y += s.velocity.y;

                // Odbijamy sprite'y od kraw�dzi okna
                if (s.position.x < 0 || s.position.x + walkTextures[0].width > WINDOW_WIDTH) s.velocity.x *= -1;
                if (s.position.y < 0 || s.position.y + walkTextures[0].height > WINDOW_HEIGHT) s.velocity.y *= -1;

                // Aktualizujemy klatk� animacji
                s.animationTimer += delta;
                if (s.animationTimer >= 0.1f) {
                    s.animationTimer = 0.0f;
                    s.currentFrame = (s.currentFrame + 1) % 3;
                }
            }

            // Rozpoczynamy rysowanie
            BeginDrawing();
            ClearBackground(BLACK);

            // Renderujemy sprite'y
            for (const auto& s : sprites)
                DrawTextureV(walkTextures[s.currentFrame], s.position, WHITE);

            // Ko�czymy rysowanie
            EndDrawing();

            ++frames; // Zliczamy klatki
        }

        // Obliczamy FPS i zapisujemy wyniki do pliku
        float fps = static_cast<float>(frames) / TEST_DURATION_SECONDS;
        std::cout << "Ilosc obiektow: " << count << ", FPS: " << fps << std::endl;
        out << count << "," << fps << std::endl;
    }

    // Zwalniamy tekstury
    for (int i = 0; i < 3; ++i)
        UnloadTexture(walkTextures[i]);

    // Zamykamy plik wynikowy
    out.close();
}

// Zwracamy nazw� pliku wynikowego
const char* MoveTest::getName() const {
    return "raylib_move.csv";
}