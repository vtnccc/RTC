#ifndef BLOCKER_OBJECT_H
#define BLOCKER_OBJECT_H

#include <SDL.h>
#include "definitions.hpp"
#include <filesystem>
#include <fstream>
#include <unordered_map>

namespace fs = std::filesystem;

class BlockerObject
{
private:
    std::unordered_map<std::string, fs::path> fileMap;
public:
    SDL_Rect blockerRect = {100, 600, 0, 0};
    SDL_Surface* blockerSurface = nullptr;
    SDL_Texture* blockerTexture = nullptr;
    std::vector<BlockerObject*> blockers;
    void create_blocker_texture(SDL_Renderer* renderer);
    void render_blocker(SDL_Renderer* renderer);
    void move_blocker(SDL_Point mousePoint);
    void scan_directory();
};

#endif // BLOCKER_OBJECT_H