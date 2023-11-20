#ifndef GAME_COMMON_H
#define GAME_COMMON_H

/*
    Enums
    when we have researched more the game,
    we can group all these in different header files
*/

enum Levels {
    SUMMER_FOREST = 0,
    GLIMMER = 1,
    IDOL_SPRINGS = 2,
    COLOSSUS = 3,
    HURRICOS = 4,
    AQUARIA_TOWERS = 5,
    SUNNY_BEACH = 6,
    OCEAN_SPEEDWAY = 7,
    CRUSHS_DUNGEON = 8,
    AUTUMN_PLAINS = 9,
    SKELOS_BADLANDS = 10,
    CRYSTAL_GLACIER = 11,
    BREEZE_HARBOR = 12,
    ZEPHYR = 13,
    METRO_SPEEDWAY = 14,
    SCORCH = 15,
    SHADY_OASIS = 16,
    MAGMA_CONE = 17,
    FRACTURE_HILLS = 18,
    ICY_SPEEDWAY = 19,
    GULPS_OVERLOOK = 20,
    WINTER_TUNDRA = 21,
    MYSTIC_MARSH = 22,
    CLOUD_TEMPLES = 23,
    CANYON_SPEEDWAY = 24,
    ROBOTICA_FARMS = 25,
    METROPOLIS = 26,
    DRAGON_SHORES = 27,
    RIPTOS_ARENA = 28
};

enum GameState {
    PLAYING = 0, // Normal gameplay
    TALKING = 1, // Interactions with Zoe, Moneybags...
    TRANSITION_LOAD_TO_PLAYING = 3, // Happens briefly after a loading screen
    PAUSED = 4,
    LOADING_HUBS = 5, // Book load
    CUTSCENE = 6,
    LOADING_LEVELS = 7 // Spyro flying skybox animation
};

/*
    Structs
    when we have researched more the game,
    we can group all these in different header files
*/

typedef struct {
    unsigned int * tail;
    unsigned int * head;
} LinkedList;

typedef struct {
    int vramYmin;
    int vramYmax;
    int length;
} TextConfig;

/*
    Functions
    when we have researched more the game,
    we can group all these in different header files
*/

void GAME_main();
void GAME_InitSound();
void GAME_InitGeometry();

void GAME_memcpyWord(int *dst,int *src,int size);
void GAME_memsetWord(int *buffer,int n,int size);

void GAME_DrawText(char * text, int x, int y, int colorIndex, TextConfig * textConfig);
// Semi-Transparent, Flat, RGB = (8, 8, 8)
void GAME_DrawSTBlackRect(short x1, short x2, short y1, short y2);
// Opaque, Flat, RGB = (0, 0, 0)
void GAME_DrawOpaqueBlackRect(short x1, short x2, short y1, short y2);
void GAME_DrawUnknownRect(short x1, short x2, short y1, short y2);
// Used during the pause menu
void GAME_DrawGameScreenshot();
void GAME_DrawShadowedOrangeLine(short p0x,short p0y,short p1x,short p1y);


/*
    Data
    when we have researched more the game,
    we can group all these in different header files
*/

extern int GAME_gemCount;
extern int GAME_gemCountHUD;
extern int GAME_gameState;

#endif