#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
int window_width = 720, window_height = window_width;

// depth/degree of recursion
int degree = 5;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Menger Sponge", "1.0", "");
    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, "60");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Menger Sponge", window_width, window_height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    } else if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        if (event->wheel.y > 0){ // unlimited power
            ++degree;
        }else if (event->wheel.y < 0) {
            if(degree - 1 >= 0) {
                --degree;
            }
        }
    } else if (event->type == SDL_EVENT_KEY_DOWN) {
        if (SDL_SCANCODE_1 <= event->key.scancode && event->key.scancode <= SDL_SCANCODE_0){
            degree = event->key.scancode==SDL_SCANCODE_0? 0: event->key.scancode - SDL_SCANCODE_1 + 1;
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // get current window width, height
    SDL_GetWindowSize(window, &window_width, &window_height);

    // choose color
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float) (0.5 + 0.5 * SDL_sin(now));
    const float green = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2. / 3.));
    const float blue = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4. / 3.));
    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */

    // base color
    SDL_Texture * image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, window_width, window_height);
    SDL_SetRenderTarget(renderer, image);
    SDL_RenderClear(renderer);

    // erase middle
    SDL_FRect rect{ .x = window_width/3.0f, .y = window_height/3.0f, .w = window_width/3.0f, .h = window_height/3.0f};
    constexpr float magic_scale = 4.f;
    SDL_SetRenderDrawColorFloat(renderer, red/magic_scale, green/magic_scale, blue/magic_scale, SDL_ALPHA_OPAQUE_FLOAT);// middle color
    SDL_RenderFillRect(renderer, &rect);

    // create fractals
    {   
        SDL_Texture * copy_image = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, window_width, window_height);
        for(int repetition_index=0; repetition_index<degree; ++repetition_index){
            SDL_SetRenderTarget(renderer, copy_image);
            SDL_RenderTexture(renderer, image, nullptr, nullptr);
            SDL_SetRenderTarget(renderer, image);
            rect.x=rect.y=0.f;
            for(int i=0;i<3;++i){
                for(int j=0;j<3;++j){
                    if(i!=1||j!=1){
                        SDL_RenderTexture(renderer, copy_image, nullptr, &rect);
                    }
                    rect.x += rect.w;
                }
                rect.x=0;
                rect.y+=rect.h;
            }
            // rect.y = 0; // optional
        }
        SDL_DestroyTexture(copy_image);
    }

    // draw result image
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderTexture(renderer, image, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(image);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
