#include "raylib.h"

const int SIM_WIDTH = 128;
const int SIM_HEIGHT = 128;
const int SIM_CELL_SIZE = 5;
const int SIM_SIZE = SIM_WIDTH * SIM_HEIGHT;

const int LINE_WIDTH = 1.0;
const Color LINE_COLOR = LIGHTGRAY;

// time delay between each update
const double TIME_DELAY = 0.1;

bool grid_visible = true;
bool sim_pause = true;

typedef struct {
        Rectangle bounds;
        bool state;
        int lt;     // number of neighbours
        Color color;
} Tile;

// array containing all the tiles
Tile tiles[SIM_SIZE];

// get the tile from (x,y) position
Tile* getTile(int x, int y) {
    if (x < 0 || y < 0 || x >= SIM_WIDTH || y >= SIM_HEIGHT) {
        
    }
    int index = x + y * SIM_WIDTH;
    return &tiles[index];
}

// get the state of a tile on the (x,y) position
bool getState(int x, int y) {
    if (x < 0 || y < 0 || x >= SIM_WIDTH || y >= SIM_HEIGHT) {
        
    }
    int index = x + y * SIM_WIDTH;
    return tiles[index].state;
}

void changeCellState(Vector2 position, bool state) {
    Tile *tile = getTile(position.x, position.y);
    if (state == true) {
        tile->state = true;
    } else {
        tile->state = false;
    }
}

void reset() {
    for (int x = 0; x < SIM_WIDTH; x++) {
        for (int y = 0; y < SIM_HEIGHT; y++) {
            Tile* tile = getTile(x,y);
            tile->state = false;
        }
    }
}

void updateInput() {
    // add cell
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        changeCellState({GetMousePosition().x/ SIM_CELL_SIZE, GetMousePosition().y / SIM_CELL_SIZE}, true);
    }
    // remove cell
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        changeCellState({GetMousePosition().x/SIM_CELL_SIZE, GetMousePosition().y/SIM_CELL_SIZE}, false);
    }
    if (IsKeyPressed(KEY_P)) {
        sim_pause = !sim_pause;
    }
    if (IsKeyPressed(KEY_G)) {
        grid_visible = !grid_visible;
    }
    if (IsKeyPressed(KEY_R)) {
        reset();
    }
}

void updateSim() {
    if (!sim_pause) {
        for (int x = 0; x < SIM_WIDTH; x++) {
            for (int y = 0; y < SIM_HEIGHT; y++) {
                // check all 8 neighbouring positions to get the number of neighbours
                bool neighbours[8] = { getState(x-1, y), getState(x-1, y-1), getState(x, y-1), getState(x+1, y-1), getState(x+1, y), getState(x+1, y+1), getState(x, y+1), getState(x-1, y+1) };
                Tile* tile = getTile(x, y);
                tile->lt = 0;
                for (int i = 0; i < sizeof(neighbours); i++) {
                    // if the neighbours state is true then increase number of neighbour
                    if (neighbours[i] == true) {
                        tile->lt += 1;
                    }
                }
            }
        }
        for (int x = 0; x < SIM_WIDTH; x++) {
            for (int y = 0; y < SIM_HEIGHT; y++) {
                Tile* tile = getTile(x, y);
                switch (tile->lt) {
                    // In case of 2 neighbours the cell lives
                    case 2:
                        continue;
                    // In case of 3 neighbours new cell is born
                    case 3:
                        tile->state = true;
                        break;
                    // In all other cases the cell dies
                    default:
                        tile->state = false;
                        break;
                }
            }
        }
    }
}

int main() {
    InitWindow(SIM_WIDTH * SIM_CELL_SIZE, SIM_HEIGHT * SIM_CELL_SIZE, "Game Of Life");

    // ready
    for (int y = 0; y < SIM_HEIGHT; y++) {
        for (int x = 0; x < SIM_WIDTH; x++) {
            Tile tile = Tile();
            tile.state = false;
            tile.lt = 0;
            tile.bounds = {x * SIM_CELL_SIZE, y * SIM_CELL_SIZE, SIM_CELL_SIZE, SIM_CELL_SIZE};

            int index = x + y * SIM_WIDTH;
            tiles[index] = tile;
        }
    }

    double lastTime = GetTime();

    while (!WindowShouldClose()){
        // update ---
        // ----------------
        updateInput();
        if (GetTime() > lastTime + TIME_DELAY) {
            updateSim();
            lastTime = GetTime();
        }
        // ----------------


        BeginDrawing();
            ClearBackground(RAYWHITE);
            if (grid_visible) {
                for (int x = 0; x < SIM_WIDTH; x++) {
                    DrawLine(x * SIM_CELL_SIZE, 0, x * SIM_CELL_SIZE,SIM_CELL_SIZE * SIM_HEIGHT, LINE_COLOR);
                }
                for (int y = 0; y < SIM_HEIGHT; y++) {
                    DrawLine(0, y * SIM_CELL_SIZE, SIM_WIDTH * SIM_CELL_SIZE, y * SIM_CELL_SIZE, LINE_COLOR);
                }
            }
            for (int tile = 0; tile < SIM_SIZE; tile++) {
                if (tiles[tile].state == true) 
                    DrawRectangle(tiles[tile].bounds.x, tiles[tile].bounds.y, tiles[tile].bounds.width, tiles[tile].bounds.height, DARKGRAY);
            }
        EndDrawing();
    }

    CloseWindow();
}
