#ifndef SOURCE_OBJECT_H
#define SOURCE_OBJECT_H

#include <SDL.h>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <cmath> // For M_PI

class Ray;
class MirrorObject;
class BlockerObject;

namespace fs = std::filesystem;

class SourceObject
{
private:
    std::unordered_map<std::string, fs::path> fileMap;

public:
    SDL_Rect sourceRect = {400, 400, 0, 0};
    SDL_Surface* sourceSurface = nullptr;
    SDL_Texture* sourceTexture = nullptr;
    std::vector<Ray> rays;
    std::vector<Ray*> reflectedRays;

    SourceObject() = default;
    ~SourceObject();
    void update_rays(const std::vector<BlockerObject*>& blockers, const std::vector<MirrorObject*>& mirrors);
    void generate_rays();
    void create_source_texture(SDL_Renderer* renderer);
    void render_source(SDL_Renderer* renderer);
    void move_source(SDL_Point mousePoint);
    void scan_directory();
    void clear_reflected_rays();
};

#endif