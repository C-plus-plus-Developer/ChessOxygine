#include "Game.h"

using namespace oxygine;
using namespace std;

int main(int argc, char* argv[])
{
    cout << "Program started" << endl;
    
    core::init_desc desc;
    desc.title = "My Chess";
    desc.w = 960;
    desc.h = 640;
    core::init(&desc);

    cout << "Core initialized" << endl;

    spStage stage = new Stage(true);
    stage->setSize(desc.w, desc.h);

    cout << "Stage created" << endl;

    ChessGame* game = new ChessGame(stage);

    cout << "Game created" << endl;

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) {
                cout << "Quit event received" << endl;
                done = true;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int gx = (event.button.x - BOARD_OFFSET_X) / TILE_SIZE;
                int gy = (event.button.y - BOARD_OFFSET_Y) / TILE_SIZE;
                if (gx >= 0 && gx < 8 && gy >= 0 && gy < 8) {
                    game->onClick(gx, gy);
                }
            }
        }

        core::update();
        stage->update();

        if (core::beginRendering())
        {
            Color clearColor(30, 30, 30, 255);
            Rect viewport(0, 0, desc.w, desc.h);
            stage->render(clearColor, viewport);
            core::swapDisplayBuffers();
        }
    }

    cout << "Deleting game..." << endl;
    delete game;
    cout << "Game deleted" << endl;
    
    stage = nullptr;
    cout << "Stage released" << endl;
    
    core::release();
    cout << "Core released" << endl;
    
    return 0;
}