#ifndef MIRROR_HPP
#define MIRROR_HPP

#include "Ray.hpp"
#include <iostream>
#include "definitions.hpp"
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <SDL.h>

namespace fs = std::filesystem;

class MirrorObject
{
private:
    std::unordered_map<std::string, fs::path> fileMap;
    SDL_Renderer* renderer = nullptr;
public:
    SDL_Rect mirrorRect = {800, 400, 0, 0};
    SDL_Surface* mirrorSurface = nullptr;
    SDL_Texture* mirrorTexture = nullptr;    
    std::vector<Ray*> hittingRays;
    
    MirrorObject(SDL_Renderer* r = nullptr) : renderer(r) {}

    void reflect_light();
    void create_mirror_texture(SDL_Renderer* renderer);
    void render_mirror(SDL_Renderer* renderer);
    void move_mirror(SDL_Point mousePoint);
    void scan_directory();
    void set_renderer(SDL_Renderer* r);
};

#endif