#pragma once

#include "cmrc/cmrc.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <thread>

#define VERSION         "1.0.0"
#define M_PI            3.14159265358979323846
#define LUIGGI_WIDTH    32
#define LUIGGI_HEIGHT   32
#define LUIGGI_WINDOW_FLAGS SDL_WINDOW_UTILITY | \
                            SDL_WINDOW_ALWAYS_ON_TOP | \
                            SDL_WINDOW_TRANSPARENT | \
                            SDL_WINDOW_BORDERLESS \
// #define WINDOW_WIDTH 640
// #define WINDOW_HEIGHT 320


// initialize the embedded resource library using chosen namespace
CMRC_DECLARE(da_luiggi);


SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Surface *surface;
SDL_Event event;
SDL_Tray *tray;
SDL_TrayMenu *menu;
SDL_TrayEntry *entry;

bool running = false;
double speed = 5;
double maxX = 1920, maxY = 1080;
double posX = 0, posY = 0;
double rads = 7 * M_PI / 5;
double xVel = speed * std::cos(rads);
double yVel = speed * std::sin(rads);
std::random_device rd;
std::mt19937 gen{rd()};
const int targetFPS = 60;
const Uint32 frameDelay = 1000 / targetFPS; // ~33 ms per frame


void quitApp(void *userdata, SDL_TrayEntry *invoker) {
    event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&event);
}


void initApp() {
    // set up the SDL window and renderer
    SDL_SetAppMetadata("Luiggi", VERSION, "com.dadams05.luiggi");
    SDL_Init(SDL_INIT_VIDEO), SDL_GetError();
    SDL_CreateWindowAndRenderer("Luiggi", LUIGGI_WIDTH, LUIGGI_HEIGHT, LUIGGI_WINDOW_FLAGS, &window, &renderer);
    SDL_SetRenderLogicalPresentation(renderer, LUIGGI_WIDTH, LUIGGI_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    // get display size to use for bouncing
    SDL_Rect usableBounds;
    if (SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &usableBounds)) {
        maxX = usableBounds.x + usableBounds.w;
        maxY = usableBounds.y + usableBounds.h;
    } else {
        std::cerr << "Failed to get display usable bounds: " << SDL_GetError() << std::endl;
    }
    // load in the png image
    auto fs = cmrc::da_luiggi::get_filesystem();
    if (fs.exists("res/luiggi.png")) {
        auto file = fs.open("res/luiggi.png");
        // create an SDL_IOStream from the embedded memory buffer
        SDL_IOStream* io = SDL_IOFromMem((void*)file.begin(), file.size());
        // load the PNG surface from memory (true = automatically close/free the stream)
        surface = SDL_LoadPNG_IO(io, true);
    } else { exit(EXIT_FAILURE); }
    // go ahead and draw the png onto the window (only need to do it once)
    SDL_BlitSurface(surface, NULL, SDL_GetWindowSurface(window), NULL);
    SDL_UpdateWindowSurface(window);
    // set up the system tray
    tray = SDL_CreateTray(surface, "My tray"); // create the entry in the system tray
    menu = SDL_CreateTrayMenu(tray); // create a context menu for the tray
    entry = SDL_InsertTrayEntryAt(menu, -1, "Quit", SDL_TRAYENTRY_BUTTON); // create a button in the context menu
    SDL_SetTrayEntryCallback(entry, quitApp, NULL); // set the callback for the button
    // set up ImGui
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
    // start running the app
    running = true;
    SDL_zero(event);
}


void shutdownApp() {
	// safely shutdown ImGui
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
	// safely shutdown SDL3
    if (surface) { SDL_DestroySurface(surface); surface = nullptr; }
	if (renderer) { SDL_DestroyRenderer(renderer); renderer = nullptr; }
	if (window) { SDL_DestroyWindow(window); window = nullptr; }
	SDL_Quit();
}


void appEventHandler() {
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL3_ProcessEvent(&event);
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
                // forces Luiggi back above the taskbar layer when focus returns from the tray
                SDL_SetWindowAlwaysOnTop(window, true);
                break;
        }
	}
}


void drawApp() {
    // ImGui_ImplSDLRenderer3_NewFrame();
	// ImGui_ImplSDL3_NewFrame();
	// ImGui::NewFrame();

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	// SDL_RenderClear(renderer);

    // int posx, posy;
    // SDL_GetWindowPosition(window, &posx, &posy);
    // SDL_SetWindowPosition(window, posx+1, posy);

    // Set texture position
    // SDL_FRect dstRect{ 0, 0, static_cast<float>( 32 ), static_cast<float>( 32 ) };

    // Render texture
    // SDL_RenderTexture( renderer, texture, nullptr, &dstRect );

	// ImGui::SetNextWindowPos(ImVec2(100, 100));
	// ImGui::SetNextWindowSize(ImVec2(100, 100));
	// ImGui::Begin("Stack", nullptr);
	// for (int i = 0; i < 16; i++) {
	// 	ImGui::Text("0x%01X", i);
	// 	ImGui::SameLine();
	// }
	// ImGui::End();

    // if (ImGui::Button("summon luiggi")) {
    //     std::cout << "pressed chink" << std::endl;
    //     SDL_Window* window = SDL_CreateWindow("My SDL3 Window", 800, 600, 0);
    //     if (!window) {
    //         SDL_Log("Window creation failed: %s", SDL_GetError());
    //         SDL_Quit();
    //     }

    //     // Keep window open for 3 seconds
    //     SDL_Delay(3000);

    //     // Clean up
    //     SDL_DestroyWindow(window);
    // }

	// ImGui::Render();

	// ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    // SDL_RenderPresent(renderer);  /* put it all on the screen! */
}


void moveApp() {
    if (posX + xVel < 0 || posX + xVel > maxX - LUIGGI_WIDTH) xVel = -xVel;
    if (posY + yVel < 0 || posY + yVel > maxY - LUIGGI_HEIGHT) yVel = -yVel;
    posX += xVel;
    posY += yVel;
    SDL_SetWindowPosition(window, static_cast<int>(posX), static_cast<int>(posY));
    std::cout << "xvel: " << xVel << "\t\tyvel: " << yVel << std::endl;
}


int main(int argc, char* args[]) {
    Uint32 frameStart;
    Uint32 frameTime;

    initApp();
    while (running) {
        frameStart = SDL_GetTicks();
        appEventHandler();
        moveApp();
        frameTime = SDL_GetTicks() - frameStart;
        // if we finished the frame faster than 33ms, delay for the remainder
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    shutdownApp();
	return EXIT_SUCCESS;
}






// if (!SDL_Init(SDL_INIT_VIDEO)) {
//     printf("SDL_Init Error: %s\n", SDL_GetError());
//     return 1;
// }

// // Get the list of all connected displays
// int num_displays = 0;
// SDL_DisplayID *displays = SDL_GetDisplays(&num_displays); //

// std::cout << "num of displays: " << num_displays << std::endl;

// if (displays && num_displays > 0) {
//     for (int i = 0; i < num_displays; i++) {
//         const SDL_DisplayMode *dis = SDL_GetCurrentDisplayMode(displays[i]);
//         int screen_width = dis->w;
//         int screen_height = dis->h;
//         printf("Screen %d Size: %d x %d\n", (i+1), screen_width, screen_height);
//     }

//     // You must free the array allocated by SDL_GetDisplays
//     SDL_free(displays); //
// }

// SDL_Quit();




// #include <SDL3/SDL.h>
// #include <SDL3/SDL_main.h>

// void callback_quit(void *userdata, SDL_TrayEntry *invoker)
// {
//     SDL_Event e;
//     e.type = SDL_EVENT_QUIT;
//     SDL_PushEvent(&e);
// }

// int main(int argc, char *argv[])
// {
//     SDL_Tray *tray;
//     SDL_TrayMenu *menu;
//     SDL_TrayEntry *entry;
//     SDL_Event e;

//     SDL_Init(SDL_INIT_VIDEO);

//     // Create the entry in the system tray. A regular app will want to provide
//     // an SDL_Surface instead of NULL.
//     tray = SDL_CreateTray(NULL, "My tray");

//     // Create a context menu for the tray.
//     menu = SDL_CreateTrayMenu(tray);

//     // Create a button in the context menu.
//     entry = SDL_InsertTrayEntryAt(menu, -1, "Quit", SDL_TRAYENTRY_BUTTON);

//     // Set the callback for the button
//     SDL_SetTrayEntryCallback(entry, callback_quit, NULL);

//     // Run the main loop...
//     while (SDL_WaitEvent(&e)) {
//         if (e.type == SDL_EVENT_QUIT) {
//             break;
//         }
//     }

//     // No need to destroy anything other than the tray itself - the rest is
//     // destroyed automatically.
//     SDL_DestroyTray(tray);

//     SDL_Quit();

//     return 0;
// }
