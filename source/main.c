#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_STARS 500
#define SCREEN_W 640
#define SCREEN_H 480
#define STAR_MAX_SIZE_DEFAULT 1.0f

typedef struct {
    float x, y;
    float z;
} Star;

int main() {
    GRRLIB_Init();
    GRRLIB_2dMode();
    WPAD_Init();
    srand(time(NULL));

    int num_stars = 300;
    float star_max_size = STAR_MAX_SIZE_DEFAULT;
    float speed = 0.05f;

    Star stars[MAX_STARS];
    float centerX = SCREEN_W / 2.0f;
    float centerY = SCREEN_H / 2.0f;

    // inicializa estrelas
    for(int i = 0; i < MAX_STARS; i++) {
        stars[i].x = ((rand() % SCREEN_W) - centerX) / 2.0f;
        stars[i].y = ((rand() % SCREEN_H) - centerY) / 2.0f;
        stars[i].z = 1 + (rand() % 100) / 20.0f;
    }

    while(1) {
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        u32 held = WPAD_ButtonsHeld(0);

        if (pressed & WPAD_BUTTON_HOME) break;

        // velocidade controlada gradualmente
        if ((held & WPAD_BUTTON_PLUS) && !(held & (WPAD_BUTTON_A | WPAD_BUTTON_B))) speed += 0.001f;
        if ((held & WPAD_BUTTON_MINUS) && !(held & (WPAD_BUTTON_A | WPAD_BUTTON_B))) speed -= 0.001f;
        if(speed < 0.01f) speed = 0.01f;

        // tamanho controlado apenas se A estiver segurado
        if (held & WPAD_BUTTON_A) {
            if (held & WPAD_BUTTON_PLUS) star_max_size += 0.01f;
            if (held & WPAD_BUTTON_MINUS) star_max_size -= 0.01f;
            if(star_max_size < 0.1f) star_max_size = 0.1f;
        }

        // quantidade controlada apenas se B estiver segurado
        if (held & WPAD_BUTTON_B) {
            if (held & WPAD_BUTTON_PLUS) num_stars += 1;
            if (held & WPAD_BUTTON_MINUS) num_stars -= 1;
            if(num_stars < 10) num_stars = 10;
            if(num_stars > MAX_STARS) num_stars = MAX_STARS;
        }

        GRRLIB_FillScreen(0x000000FF);

        for(int i = 0; i < num_stars; i++) {
            float screenX = centerX + stars[i].x / stars[i].z;
            float screenY = centerY + stars[i].y / stars[i].z;

            float size = star_max_size / stars[i].z;
            GRRLIB_Circle(screenX, screenY, size, 0xFFFFFFFF, true);

            stars[i].z -= speed;

            if(screenX < 0 || screenX >= SCREEN_W || screenY < 0 || screenY >= SCREEN_H || stars[i].z <= 0.1f) {
                stars[i].x = ((rand() % SCREEN_W) - centerX) / 2.0f;
                stars[i].y = ((rand() % SCREEN_H) - centerY) / 2.0f;
                stars[i].z = 5 + (rand() % 100) / 20.0f;
            }
        }

        GRRLIB_Render();
    }

    GRRLIB_Exit();
    return 0;
}
