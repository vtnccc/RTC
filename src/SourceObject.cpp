#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <string>
#include "SourceObject.hpp"
#include "TopBar.hpp"
#include "definitions.hpp"
#include "Ray.hpp"
#include "Mirror.hpp" 
#include "BlockerObject.hpp" 

void SourceObject::clear_reflected_rays() 
{
    for (auto ray : reflectedRays) 
    {
        delete ray;
    }
    reflectedRays.clear();
}

SourceObject::~SourceObject()
{
    clear_reflected_rays();
}

//vytvoří texturu pro zdroj (vysvětleno v BlockerObejct.cpp)
void SourceObject::create_source_texture(SDL_Renderer* renderer)
{
    scan_directory();

    sourceSurface = IMG_Load(fileMap["SourceImage.png"].c_str());
    
    if(!sourceSurface)
    {
        std::cerr << "Error creating the SOURCE surface SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    
    sourceTexture = SDL_CreateTextureFromSurface(renderer, sourceSurface);
    
    if (!sourceTexture)
    {
        std::cerr << "Failed to create source texture! SDL_Error: " << SDL_GetError() << std::endl;
    }
    
    sourceRect.w = sourceSurface->w;
    sourceRect.h = sourceSurface->h;
    
    SDL_FreeSurface(sourceSurface);
    SDL_SetTextureBlendMode(sourceTexture, SDL_BLENDMODE_BLEND);
}

//tky v BlockerObject.cpp
void SourceObject::render_source(SDL_Renderer* renderer)
{
    if(sourceTexture)
    {
        SDL_RenderCopy(renderer, sourceTexture, nullptr, &sourceRect);
    }
}


void SourceObject::generate_rays() 
{
    rays.clear(); // smaže staré paprsky
        
    const float angleIncrement = 2 * M_PI / RAY_AMOUNT; // o kolik se zvětší úhel mezi paprsky
        
    for (int i = 0; i < RAY_AMOUNT; ++i)
    {
        float angle = i * angleIncrement;
        rays.emplace_back(this, angle);
    }
}

void SourceObject::update_rays(const std::vector<BlockerObject*>& blockers, const std::vector<MirrorObject*>& mirrors)
{
    
    //když se pohne se zdrojem pro všechna paprsky se updatuje pozice a zkontroluje se kolize
    for (auto& ray : rays)
    {
        ray.reset_collision(); // Reset collision state before checking again
        ray.update_position();
        ray.check_for_ray_collision(blockers, mirrors);
    }
    
    // Update any reflected rays
    for (auto ray_ptr : reflectedRays) {
        ray_ptr->check_for_ray_collision(blockers, mirrors);
    }
}

//vysvětleno v BlockerObject.cpp
void SourceObject::scan_directory()
{
    try
    {
        fs::path basePath = fs::current_path() / "Desktop/RTC";
        fs::path resourcesPath = basePath / "resources";

        std::cout << "Working directory: " << basePath << std::endl;
        std::cout << "Looking for 'resources' at: " << resourcesPath << std::endl;

        if (!fs::exists(resourcesPath))
        {
            std::cerr << "[ERROR] Directory does not exist: " << resourcesPath << std::endl;
            return;
        }

        if (!fs::is_directory(resourcesPath))
        {
            std::cerr << "[ERROR] Path is not a directory: " << resourcesPath << std::endl;
            return;
        }

        for (const auto& entry : fs::recursive_directory_iterator(resourcesPath))
        {
            if (fs::is_regular_file(entry))
            {
                std::string name = entry.path().filename().string();
                fileMap[name] = entry.path().string(); // Convert to string to avoid issues
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "[EXCEPTION] " << e.what() << std::endl;
    }
}

//tky v BlockerObject.cpp
void SourceObject::move_source(SDL_Point mousePoint) 
{
    sourceRect.x = mousePoint.x - sourceRect.w / 2;
    sourceRect.y = mousePoint.y - sourceRect.h / 2;
    for (auto& ray : rays)
    {
        ray.update_position();
    }
}