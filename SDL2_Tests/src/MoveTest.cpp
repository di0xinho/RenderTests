#include "MoveTest.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <filesystem>

// Struktura reprezentuj¹ca sprite
struct MovingSprite {
    SDL_Rect destRect;       // Prostok¹t okreœlaj¹cy pozycjê i rozmiar sprite'a
    SDL_FPoint velocity;     // Prêdkoœæ ruchu sprite'a (x, y)
    int currentFrame = 0;    // Obecna klatka animacji
    float animationTimer = 0.0f; // Zegar animacji
};

// Implementacja metody run uruchamiaj¹cej test
void MoveTest::run(SDL_Renderer* renderer) {

    // Definiujemy progi dla liczby obiektów do testowania
    std::vector<int> thresholds = { 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000 };

    // Tworzymy katalog pod wyniki testów
    std::filesystem::create_directories(RESULTS_PATH);

    // Otwieramy plik wynikowy do zapisu rezultatów
    std::ofstream out(std::string(RESULTS_PATH) + getName(), std::ios::app);
    out << "objects,fps" << std::endl;

    // £adujemy tekstury animacji ruchu
    SDL_Surface* surfaces[3] = {
        IMG_Load(RUN_TEXTURE_1),
        IMG_Load(RUN_TEXTURE_2),
        IMG_Load(RUN_TEXTURE_3),
    };

    bool loadFailed = false; // Flaga okreœlaj¹ca stan za³adowania tekstur

    for (int i = 0; i < 3; ++i) {
        if (!surfaces[i]) {
            std::cerr << "Blad podczas ladowania tekstur: " << IMG_GetError() << std::endl;
            loadFailed = true;
        }
    }
    if (loadFailed) return;

    // Tworzymy tekstury z za³adowanych powierzchni
    SDL_Texture* runTextures[3];
    for (int i = 0; i < 3; ++i) {
        runTextures[i] = SDL_CreateTextureFromSurface(renderer, surfaces[i]);
        SDL_FreeSurface(surfaces[i]);
    }

    // Pobieramy wymiary tekstury
    int texWidth, texHeight; // Zmienne pod szerokoœæ i wysokoœæ
    SDL_QueryTexture(runTextures[0], nullptr, nullptr, &texWidth, &texHeight);

    // Inicjalizujemy RNG z ustalonym seedem dla powtarzalnych wyników
    std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> xDist(0, WINDOW_WIDTH - texWidth);
    std::uniform_int_distribution<int> yDist(0, WINDOW_HEIGHT - texHeight);
    std::uniform_real_distribution<float> velDist(-5.0f, 5.0f);

    // Iterujemy przez progi liczby obiektów
    for (int count : thresholds) {

        // Tworzymy wektor przechowuj¹cy sprite'y
        std::vector<MovingSprite> sprites;
        sprites.reserve(count);

        // Inicjalizujemy sprite'y z losow¹ pozycj¹ i prêdkoœci¹
        for (int i = 0; i < count; ++i) {

            // Ustawiamy losow¹ pozycjê sprite'a
            int x = xDist(rng);
            int y = yDist(rng);

            // Ustawiamy losow¹ prêdkoœæ sprite'a
            float vx = velDist(rng);
            float vy = velDist(rng);

            // Na koniec wszystkie wylosowane (tak naprawdê mamy tutaj zawsze pseudolosowoœæ) wartoœci uzupe³niamy jako parametry obiektu SDL_Rect, po czym dodajemy go do wektora reprezentuj¹cego sprite'y
            sprites.push_back({ SDL_Rect{ x, y, texWidth, texHeight }, {vx, vy} });
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

            // Aktualizacja pozycji i animacji sprite'ów
            for (auto& s : sprites) {
                s.destRect.x += static_cast<int>(s.velocity.x);
                s.destRect.y += static_cast<int>(s.velocity.y);

                // Odbicie od krawêdzi okna
                if (s.destRect.x < 0 || s.destRect.x + texWidth > WINDOW_WIDTH) s.velocity.x *= -1;
                if (s.destRect.y < 0 || s.destRect.y + texHeight > WINDOW_HEIGHT) s.velocity.y *= -1;

                // Zmieniamy klatkê animacji
                s.animationTimer += delta;
                if (s.animationTimer >= 0.1f) {
                    s.animationTimer = 0.0f;
                    s.currentFrame = (s.currentFrame + 1) % 3;
                }
            }

            // Czyœcimy ekran
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Renderujemy sprite'y
            for (const auto& s : sprites)
                SDL_RenderCopy(renderer, runTextures[s.currentFrame], nullptr, &s.destRect);

            // Prezentujemy wyrenderowan¹ klatkê
            SDL_RenderPresent(renderer);
            ++frames; // Zliczamy klatki
        }

        // Obliczamy FPS i zapisujemy wyniki
        float fps = static_cast<float>(frames) / TEST_DURATION_SECONDS;
        std::cout << "Ilosc obiektow: " << count << ", FPS: " << fps << std::endl;
        out << count << "," << fps << std::endl;
    }

    // Zwalniamy tekstury
    for (int i = 0; i < 3; ++i)
        SDL_DestroyTexture(runTextures[i]);

    // Zamykamy plik wynikowy
    out.close();
}


const char* MoveTest::getName() const {
    return "sdl2_move.csv";
}