#include "AudioPlayer.h"
#include "AudioAnalyzer.h"
#include "Renderer.h"
#include <SDL2/SDL.h>
#include <iostream>

int main() {
    // Initialize audio player
    AudioPlayer player("assets/audio.wav");
    if (!player.init()) {
        return -1;
    }
    player.play();

    // Set up analyzer (FFT size = 1024 as an example)
    const size_t FFT_SIZE = 1024;
    AudioAnalyzer analyzer(FFT_SIZE);

    // Set up renderer
    Renderer renderer(800, 600);
    if (!renderer.init()) {
        return -1;
    }

    // Main loop
    bool running = true;
    Uint32 startTicks = SDL_GetTicks();

    std::vector<float> timeDomain(FFT_SIZE, 0.0f);
    std::vector<float> fftOutput;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        // Get current samples (for this example, we try to fetch samples from the file)
        if (player.getCurrentSamples(timeDomain, FFT_SIZE)) {
            // Perform FFT
            analyzer.performFFT(timeDomain, fftOutput);

            // Update texture with FFT data
            renderer.updateFFTTexture(fftOutput);
        }

        float currentTime = (SDL_GetTicks() - startTicks) / 1000.0f;
        renderer.render(currentTime);
    }

    return 0;
}
