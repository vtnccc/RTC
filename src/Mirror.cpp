#include "Ray.hpp"
#include "definitions.hpp"
#include <iostream>
#include <SDL.h>
#include <cmath>
#include <SDL_image.h>
#include "Mirror.hpp"

// pokud je zjištěná kolize paprsku a zrcadla, tak se zavolá tahle funkce, která ho "odrazí"
void MirrorObject::reflect_light()
{
    // pokud byl špatně vytvořen renderer nebo v hittingRays vektoru nic není (žádné paprsky se neodráži) tak se to rovnou quitne
    if(renderer == nullptr || hittingRays.empty()) return;
    
    for(auto& ray : hittingRays)
    {
        if(!ray->hasMirrorCollision) continue;
        
       // akorát přebýrá původní angle paprsku a jeho x a y koordinaci při kolizi
        float incidentAngle = ray->angle;
        float collisionX = ray->collisionX;
        float collisionY = ray->collisionY;
        
       // výpočet odraženýho úhlu pro ploché zrcadlo
        float normalAngle = -M_PI/2; 
        float reflectionAngle = 2 * normalAngle - incidentAngle;
        
        // vypočítá kde bude odražený paprsek končit
        float reflectedEndX = collisionX + 2000 * cos(reflectionAngle);
        float reflectedEndY = collisionY + 2000 * sin(reflectionAngle);
        
        // vyrenderuje paprsek
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // žlutá
        SDL_RenderDrawLine(renderer, collisionX, collisionY, reflectedEndX, reflectedEndY);
    }
    
    // vyčistí vektor pro další použití
    hittingRays.clear();
}

//znova vytvoří texturu jako u minulího obejktu (blocker)
void MirrorObject::create_mirror_texture(SDL_Renderer *renderer)
{   
    scan_directory();

    mirrorSurface = IMG_Load(fileMap["MirrorImage.png"].c_str());
    
    if(!mirrorSurface)
    {
        std::cerr << "Error loading the image on the mirror surface! IMG_Error: " << IMG_GetError() << std::endl;
    }

    mirrorTexture = SDL_CreateTextureFromSurface(renderer, mirrorSurface);

    if(!mirrorTexture)
    {
        std::cerr << "Error creating the mirror texture from its surface! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    mirrorRect.w = mirrorSurface->w - 40;
    mirrorRect.h = mirrorSurface->h;

    SDL_FreeSurface(mirrorSurface);
    SDL_SetTextureBlendMode(mirrorTexture, SDL_BLENDMODE_BLEND);
}

//to stejný (už popsaný v BlockerObject.cpp)
void MirrorObject::render_mirror(SDL_Renderer *renderer)
{
    if(mirrorTexture)
    {
        SDL_RenderCopy(renderer, mirrorTexture, NULL, &mirrorRect);
    }
}

//to stejný (už popsaný v BlockerObject.cpp)
void MirrorObject::move_mirror(SDL_Point mousePoint)
{
    mirrorRect.x = mousePoint.x - mirrorRect.w / 2;
    mirrorRect.y = mousePoint.y - mirrorRect.h / 2;
}

//to stejný (už popsaný v BlockerObject.cpp)
void MirrorObject::scan_directory()
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
                fileMap[name] = entry.path();
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "[EXCEPTION] " << e.what() << std::endl;
    }
}

void MirrorObject::set_renderer(SDL_Renderer *r) { renderer = r; }