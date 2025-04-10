#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "BlockerObject.hpp"

//vytvoří texturu bro blocker
void BlockerObject::create_blocker_texture(SDL_Renderer* renderer)
{
    //projde složku s texturami
    scan_directory();

    //načte obrázek jménem "BlockerImage.png"
    blockerSurface = IMG_Load(fileMap["BlockerImage.png"].c_str());

    if(!blockerSurface)
    {
        std::cerr << "[ERROR] Error cryeating the BLOCKER surface! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    //vytvoří texturu s povrchu, který jsme nahráli ten obrázek
    blockerTexture = SDL_CreateTextureFromSurface(renderer, blockerSurface);

    if (!blockerTexture) 
    {
        std::cerr << "[ERROR] Failed to create blocker texture! SDL_Error: " << SDL_GetError() << std::endl;
    }

    //povrch je tak velký jako obrázek co do něj nahrajeme -> potom ty velikosti přiřadíme tomu objektu
    blockerRect.w = blockerSurface->w;
    blockerRect.h = blockerSurface->h;

    //uvolní povrch, protože už není potřeba
    SDL_FreeSurface(blockerSurface);
    //jelikož by se textura jinak vykreslovala i s pozadím tak tímhle (pokud má ten obrázek jako průhledný pozadí) se ho zbavíme
    SDL_SetTextureBlendMode(blockerTexture, SDL_BLENDMODE_BLEND);
}

void BlockerObject::render_blocker(SDL_Renderer* renderer)
{   
    //je textura v pořádku a funguje tak se do renderu zkopíruje textura a blockerRect(x, y, w, h)
    if (blockerTexture)
    {
        SDL_RenderCopy(renderer, blockerTexture, nullptr, &blockerRect);
    }
}

void BlockerObject::move_blocker(SDL_Point mousePoint)
{
    //zajistí aby se blocker pohyboval s kurzorem
    blockerRect.x = mousePoint.x - blockerRect.w / 2;
    blockerRect.y = mousePoint.y - blockerRect.h / 2;
}

void BlockerObject::scan_directory()
{
    try
    {
        //zjistí working directory (odkud byla aplikace spuštěna) a přidá k tomu "Desktop/RTC" aby ta basePath vedla do tý základní složky
        fs::path basePath = fs::current_path() / "Desktop/RTC";
        //přidá k basePath ještě jméno tý složky s obrázky aby to vědělo kde to je má hledat
        fs::path resourcesPath = basePath / "resources";

        std::cout << "Working directory: " << basePath << std::endl;
        std::cout << "Looking for 'resources' at: " << resourcesPath << std::endl;

        //zkontroluje jestli soubor existuje (resources)
        if (!fs::exists(resourcesPath))
        {
            std::cerr << "[ERROR] Directory does not exist: " << resourcesPath << std::endl;
            return;
        }

        //zkontroluje jestli je resources složka opravdu složka
        if (!fs::is_directory(resourcesPath))
        {
            std::cerr << "[ERROR] Path is not a directory: " << resourcesPath << std::endl;
            return;
        }

        //projde všechny soubory ve složce se pokud jsou to normální soubory tak jejich jméno a tu adresu (v souborech např.: Desktop/RTC/resources/image.png) do mapy
        for (const auto& entry : fs::recursive_directory_iterator(resourcesPath))
        {
            if (fs::is_regular_file(entry))
            {
                //vytváří nový body v mapě (jméno -> path k souboru)
                std::string name = entry.path().filename().string();
                fileMap[name] = entry.path();
            }
        }
    }
    catch (const std::exception& e)
    {
        //pokud se něco nepovede tak tohle vypíše co se nepovedlo
        std::cerr << "[EXCEPTION] " << e.what() << std::endl;
    }
}