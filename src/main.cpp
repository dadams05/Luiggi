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




#pragma once

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "SDL3_image/SDL_image.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include <iostream>
#include <string>
#include <thread>


#define VERSION "1.0.0"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Texture *texture = nullptr;
SDL_Event event;
SDL_Tray *tray;
SDL_TrayMenu *menu;
SDL_TrayEntry *entry;

std::string path = "res/luigi.png";
bool running = false;


void callback_quit(void *userdata, SDL_TrayEntry *invoker)
{
    SDL_Event e;
    e.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&e);
}



void checkError(bool cond, std::string msg) {
	if (cond) {
		std::cerr << "[Slower] " << msg << std::endl;
		exit(EXIT_FAILURE);
	}
}

// SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_BORDERLESS
void initApp() {
    SDL_SetAppMetadata("Luiggi", VERSION, "com.dadams05.luiggi");

    checkError(!SDL_Init(SDL_INIT_VIDEO), SDL_GetError());

    checkError(
        !SDL_CreateWindowAndRenderer("Slower", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_UTILITY | SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_BORDERLESS, &window, &renderer),
        SDL_GetError()
    );

    SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);


    texture = IMG_LoadTexture(renderer, path.c_str());


    // Create the entry in the system tray. A regular app will want to provide
    // an SDL_Surface instead of NULL.
    tray = SDL_CreateTray(NULL, "My tray");

    // Create a context menu for the tray.
    menu = SDL_CreateTrayMenu(tray);

    // Create a button in the context menu.
    entry = SDL_InsertTrayEntryAt(menu, -1, "Quit", SDL_TRAYENTRY_BUTTON);

    // Set the callback for the button
    SDL_SetTrayEntryCallback(entry, callback_quit, NULL);



    IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);

    running = true;
    SDL_zero(event);
}


void shutdownApp() {
	// safely shutdown ImGui
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	// safely shutdown SDL3
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
	if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
	if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
	SDL_Quit();
}


void appEventHandler() {
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL3_ProcessEvent(&event);
        switch (event.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                //SDL_Log("We got a motion event.");
                //SDL_Log("Current mouse position is: (%f, %f)", event.motion.x, event.motion.y);
                break;
        }

	}
}


void drawApp() {
    ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

    // int posx, posy;
    // SDL_GetWindowPosition(window, &posx, &posy);
    // SDL_SetWindowPosition(window, posx+1, posy);

    //Set texture position
    SDL_FRect dstRect{ 0, 0, static_cast<float>( 32 ), static_cast<float>( 32 ) };

    //Render texture
    SDL_RenderTexture( renderer, texture, nullptr, &dstRect );

	//ImGui::SetNextWindowPos(ImVec2(100, 100));
	//ImGui::SetNextWindowSize(ImVec2(100, 100));
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

	ImGui::Render();

	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);  /* put it all on the screen! */


}

////////////////////////////////////////////////////////////////////////////

#define M_PI 3.14159265358979323846
#include <cmath>
#include <random>
double speed = 0.1;
double maxX = 2560, maxY = 1600;
double posX = 0, posY = 0;
double rads = 7 * M_PI / 5;
double xVel = speed * std::cos(rads);
double yVel = speed * std::sin(rads);
int width = 32, height = 32;
std::random_device rd;
std::mt19937 gen{rd()};

void move() {
    if (posX + xVel < 0 || posX + xVel > maxX - width) xVel = -xVel;
    if (posY + yVel < 0 || posY + yVel > maxY - height) yVel = -yVel;
    posX += xVel;
    posY += yVel;
    SDL_SetWindowPosition(window, static_cast<int>(posX), static_cast<int>(posY));
    std::cout << "xvel: " << xVel << "\t\tyvel: " << yVel << std::endl;
}

/////////////////////////////////////////////////////////////////////////

int main(int argc, char* args[]) {
    initApp();

    while(running) {
        appEventHandler();
        drawApp();
        move();
    }

    shutdownApp();

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

	return EXIT_SUCCESS;
}
