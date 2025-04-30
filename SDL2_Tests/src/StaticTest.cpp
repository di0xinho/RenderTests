#include "StaticTest.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj¹ca sprite
struct StaticSprite {
    SDL_Rect destRect; // Prostok¹t okreœlaj¹cy pozycjê i rozmiar sprite'a
};

// Implementacja metody run uruchamiaj¹cej test
void StaticTest::run(SDL_Renderer* renderer) {

    // Definiujemy progi dla liczby obiektów do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog pod wyniki testów
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultatów
    std::ofstream out(std::string(RESULTS_PATH) + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // £adujemy teksturê z pliku
    SDL_Surface* surface = IMG_Load(IDLE_TEXTURE);
    if (!surface) {
        std::cerr << "B³¹d podczas ³adowania tekstur: " << IMG_GetError() << std::endl;
        return;
    }

    // Tworzymy teksturê z powierzchni
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Pobieramy wymiary tekstury
    int texWidth, texHeight; // Zmienne pod szerokoœæ i wysokoœæ
    SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wyników
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - texWidth);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - texHeight);

    // Iterujemy przez progi liczby obiektów
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj¹cy sprite'y
        std::vector<StaticSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprite'y z losow¹ pozycj¹
        for (int i = 0; i < count; ++i) {

            // Ustawiamy losow¹ pozycjê sprite'a
            int x = xDist(rng);
            int y = yDist(rng);
            sprites.push_back({ SDL_Rect{ x, y, texWidth, texHeight } });
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

            // Obs³uga zdarzeñ (np. zamkniêcie okna)
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    return;
            }

            // Czyœcimy ekran
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Renderujemy wszystkie sprite'y
            for (const auto& s : sprites)
                SDL_RenderCopy(renderer, texture, nullptr, &s.destRect);

            // Prezentujemy wyrenderowan¹ klatkê
            SDL_RenderPresent(renderer);
            ++frames; // Zliczamy klatki
        }

        // Obliczamy FPS i zapisujemy wyniki
        float fps = static_cast<float>(frames) / TEST_DURATION_SECONDS;
        std::cout << "Ilosc obiektow: " << count << ", FPS: " << fps << std::endl;
        out << count << "," << fps << std::endl;
    }

    // Zwalniamy teksturê
    SDL_DestroyTexture(texture);

    // Zamykamy plik wynikowy
    out.close();
}

// Zwracamy nazwê pliku wynikowego
const char* StaticTest::getName() const {
    return "sdl2_static.csv";
}