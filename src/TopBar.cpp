#include "TopBar.hpp"
#include "Ray.hpp"
#include "SourceObject.hpp"
#include "BlockerObject.hpp"

//constructor, který se zavolá při spuštění aplikace
Button::Button()
{
    //                     x,  y, w, h
    addSourceButtonRect = {10, 5, 0, 0}; 
    addBlockerButtonRect = {100, 5, 0, 0}; 
    addMirrorButtonRect = {190, 5, 0, 0};

    deleteSourceButtonRect = {460, 5, 0, 0};
    deleteBlockerButtonRect = {280, 5, 0, 0};
    deleteMirrorButtonRect = {370, 5, 0, 0};

    addSourceButtonSurface = nullptr;
    addBlockerButtonSurface = nullptr;
    addMirrorButtonSurface = nullptr;

    addSourceButtonTexture = nullptr;
    addBlockerButtonTexture = nullptr;
    addMirrorButtonTexture = nullptr;

    deleteSourceButtonSurface = nullptr;
    deleteSourceButtonTexture = nullptr;

    deleteBlockerButtonSurface = nullptr;
    deleteBlockerButtonTexture = nullptr;
    
    deleteMirrorButtonSurface = nullptr;
    deleteMirrorButtonTexture = nullptr;
}

//destructor, ketrý se zavolá při ukončení aplikace
Button::~Button()
{
    if (addSourceButtonSurface) SDL_FreeSurface(addSourceButtonSurface);
    if (addBlockerButtonSurface) SDL_FreeSurface(addBlockerButtonSurface);
    if (addMirrorButtonSurface) SDL_FreeSurface(addMirrorButtonSurface);
    
    if (addSourceButtonTexture) SDL_DestroyTexture(addSourceButtonTexture);
    if (addBlockerButtonTexture) SDL_DestroyTexture(addBlockerButtonTexture);
    if (addMirrorButtonTexture) SDL_DestroyTexture(addMirrorButtonTexture);
    
    if (deleteSourceButtonSurface) SDL_FreeSurface(deleteSourceButtonSurface);
    if (deleteSourceButtonTexture) SDL_DestroyTexture(deleteSourceButtonTexture);
    
    if (deleteBlockerButtonSurface) SDL_FreeSurface(deleteBlockerButtonSurface);
    if (deleteBlockerButtonTexture) SDL_DestroyTexture(deleteBlockerButtonTexture);
    
    if (deleteMirrorButtonSurface) SDL_FreeSurface(deleteMirrorButtonSurface);
    if (deleteMirrorButtonTexture) SDL_DestroyTexture(deleteMirrorButtonTexture);
}

//constructor pro TopBar (místo kde se tlačítka nacházejí)
TopBar::TopBar()
{
    topBar = {0, 0, window_width, TOPBAR_HEIGHT}; 
}

//vykreslí TopBar
void TopBar::render_top_bar(SDL_Renderer* renderer) 
{
    topBar.w = window_width;//mění se podle toho jak je velký okno
    //změní barvu na šedou
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255); 
    //zaplní místo určené v topBar (rect)
    SDL_RenderFillRect(renderer, &topBar); 
}

//popsaný v BlockerObject.cpp
void Button::scan_directory()
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

//nastaví správný renderer
void AddSourceButton::set_renderer(SDL_Renderer* r) { renderer = r; }

//v BlockerObject.cpp
void AddSourceButton::create_add_source_button_texture(SDL_Renderer* renderer)
{
    scan_directory(); 
    addSourceButtonSurface = IMG_Load(fileMap["source.png"].c_str());

    if(!addSourceButtonSurface)
    {
        std::cerr << "Error creating the surface for the add source button SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    addSourceButtonTexture = SDL_CreateTextureFromSurface(renderer, addSourceButtonSurface);

    if (!addSourceButtonTexture)
    {
        std::cerr << "Failed to create the texture for the add source button! SDL_Error: " << SDL_GetError() << std::endl;
    }

    addSourceButtonRect.w = addSourceButtonSurface->w;
    addSourceButtonRect.h = addSourceButtonSurface->h;

    SDL_FreeSurface(addSourceButtonSurface);
    SDL_SetTextureBlendMode(addSourceButtonTexture, SDL_BLENDMODE_BLEND);
}

//taky v BlockerObject.cpp
void AddSourceButton::render_add_source_button(SDL_Renderer* renderer)
{
    if (addSourceButtonTexture)
    {
        SDL_RenderCopy(renderer, addSourceButtonTexture, nullptr, &addSourceButtonRect);
    }
}

//vytvoří nový zdroj
void AddSourceButton::add_source() 
{
    SourceObject* newSource = new SourceObject();
    newSource->sourceRect = {100,100,0,0};
    newSource->create_source_texture(renderer);
    newSource->generate_rays();
    //dá zdroj na konec vektrou sources
    sources.push_back(newSource);
}

//constructor
DeleteSourceButton::DeleteSourceButton(std::vector<SourceObject*>& srcs) : sources(srcs) {}

//nastaví správný renderer
void DeleteSourceButton::set_renderer(SDL_Renderer* r) { renderer = r; }

//taky v BlockerObject.cpp
void DeleteSourceButton::create_delete_source_button_texture(SDL_Renderer* renderer)
{
    scan_directory(); 
    deleteSourceButtonSurface = IMG_Load(fileMap["deletesource.png"].c_str());

    if(!deleteSourceButtonSurface)
    {
        std::cerr << "Error creating the surface for the delete source button SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    deleteSourceButtonTexture = SDL_CreateTextureFromSurface(renderer, deleteSourceButtonSurface);

    if (!deleteSourceButtonTexture)
    {
        std::cerr << "Failed to create the texture for the add source button! SDL_Error: " << SDL_GetError() << std::endl;
    }

    deleteSourceButtonRect.w = deleteSourceButtonSurface->w;
    deleteSourceButtonRect.h = deleteSourceButtonSurface->h;

    SDL_FreeSurface(deleteSourceButtonSurface);
    SDL_SetTextureBlendMode(deleteSourceButtonTexture, SDL_BLENDMODE_BLEND);
}

//taky v BlockerObject.cpp
void DeleteSourceButton::render_delete_source_button(SDL_Renderer* renderer)
{
    if (deleteSourceButtonTexture)
    {
        SDL_RenderCopy(renderer, deleteSourceButtonTexture, nullptr, &deleteSourceButtonRect);
    }
}

//vymaže zdroj z vektoru
void DeleteSourceButton::delete_source()
{
    if(!sources.empty())
    {
        //vymaže zdroj na konci toho vektoru
        delete sources.back();
        sources.pop_back();
    }
}


void AddBlockerButton::set_renderer(SDL_Renderer* r) { renderer = r; }

//taky v BlockerObject.cpp
void AddBlockerButton::create_add_blocker_button_texture(SDL_Renderer* renderer)
{
    scan_directory(); 
    addBlockerButtonSurface = IMG_Load(fileMap["blocker.png"].c_str()); 

    if(!addBlockerButtonSurface)
    {
        std::cerr << "Error creating the surface for the add blocker button SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    addBlockerButtonTexture = SDL_CreateTextureFromSurface(renderer, addBlockerButtonSurface);

    if (!addBlockerButtonTexture)
    {
        std::cerr << "Failed to create the texture for the add blocker button! SDL_Error: " << SDL_GetError() << std::endl;
    }

    addBlockerButtonRect.w = addBlockerButtonSurface->w;
    addBlockerButtonRect.h = addBlockerButtonSurface->h;

    SDL_FreeSurface(addBlockerButtonSurface);
    SDL_SetTextureBlendMode(addBlockerButtonTexture, SDL_BLENDMODE_BLEND);
}

//taky v BlockerObject.cpp
void AddBlockerButton::render_add_blocker_button(SDL_Renderer* renderer)
{
    if (addBlockerButtonTexture)
    {
        SDL_RenderCopy(renderer, addBlockerButtonTexture, nullptr, &addBlockerButtonRect);
    }
}

//vytvoří nový blocker
void AddBlockerButton::add_blocker()
{
    BlockerObject* newBlocker = new BlockerObject();
    newBlocker->blockerRect = {100, 600, 0, 0};
    newBlocker->create_blocker_texture(renderer);
    blockers.push_back(newBlocker);
    blockers.shrink_to_fit();
}

DeleteBlockerButton::DeleteBlockerButton(std::vector<BlockerObject*>& blks) : blockers(blks) {}

void DeleteBlockerButton::set_renderer(SDL_Renderer* r) { renderer = r; }

//taky v BlockerObject.cpp
void DeleteBlockerButton::create_delete_blocker_button_texture(SDL_Renderer* renderer)
{
    scan_directory(); 
    deleteBlockerButtonSurface = IMG_Load(fileMap["deleteblocker.png"].c_str()); 

    if(!deleteBlockerButtonSurface)
    {
        std::cerr << "Error creating the surface for the delete blocker button SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    deleteBlockerButtonTexture = SDL_CreateTextureFromSurface(renderer, deleteBlockerButtonSurface);

    if (!deleteBlockerButtonTexture)
    {
        std::cerr << "Failed to create the texture for the delete blocker button! SDL_Error: " << SDL_GetError() << std::endl;
    }

    deleteBlockerButtonRect.w = deleteBlockerButtonSurface->w;
    deleteBlockerButtonRect.h = deleteBlockerButtonSurface->h;

    SDL_FreeSurface(deleteBlockerButtonSurface);
    SDL_SetTextureBlendMode(deleteBlockerButtonTexture, SDL_BLENDMODE_BLEND);
}

//taky v BlockerObject.cpp
void DeleteBlockerButton::render_delete_blocker_button(SDL_Renderer* renderer)
{
    if (deleteBlockerButtonTexture)
    {
        SDL_RenderCopy(renderer, deleteBlockerButtonTexture, nullptr, &deleteBlockerButtonRect);
    }
}

//vymaže blocker z vektoru
void DeleteBlockerButton::delete_blocker()
{
    if(!blockers.empty())
    {
        //ten poslední (ten co je na konci toho vektoru)
        delete blockers.back();
        blockers.pop_back();
    }
}

//constructor
AddMirrorButton::AddMirrorButton() {}

void AddMirrorButton::set_renderer(SDL_Renderer* r) { renderer = r; }

void AddMirrorButton::create_add_mirror_button_texture(SDL_Renderer *renderer)
{
    scan_directory(); 
    addMirrorButtonSurface = IMG_Load(fileMap["mirror.png"].c_str()); 

    if(!addMirrorButtonSurface)
    {
        std::cerr << "Error creating the surface for the add mirror button SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    addMirrorButtonTexture = SDL_CreateTextureFromSurface(renderer, addMirrorButtonSurface); // Fixed assignment

    if (!addMirrorButtonTexture)
    {
        std::cerr << "Failed to create the texture for the add mirror button! SDL_Error: " << SDL_GetError() << std::endl;
    }

    addMirrorButtonRect.w = addMirrorButtonSurface->w;
    addMirrorButtonRect.h = addMirrorButtonSurface->h;

    SDL_FreeSurface(addMirrorButtonSurface);
    SDL_SetTextureBlendMode(addMirrorButtonTexture, SDL_BLENDMODE_BLEND);  
}

void AddMirrorButton::render_add_mirror_button(SDL_Renderer *renderer)
{
    if (addMirrorButtonTexture)
    {
        SDL_RenderCopy(renderer, addMirrorButtonTexture, nullptr, &addMirrorButtonRect);
    }
} 


void AddMirrorButton::add_mirror()
{
    MirrorObject* newMirror = new MirrorObject(renderer);
    newMirror->mirrorRect = {800, 400, 0, 0};
    newMirror->create_mirror_texture(renderer);
    mirrors.push_back(newMirror);
    mirrors.shrink_to_fit();
}

DeleteMirrorButton::DeleteMirrorButton(std::vector<MirrorObject*>& mirrs) : mirrors(mirrs) {}

void DeleteMirrorButton::set_renderer(SDL_Renderer *r) { renderer = r; }

void DeleteMirrorButton::create_delete_mirror_button_texture(SDL_Renderer *renderer)
{   
    scan_directory(); 
    deleteMirrorButtonSurface = IMG_Load(fileMap["deletemirror.png"].c_str()); 

    if(!deleteMirrorButtonSurface)
    {
        std::cerr << "Error creating the surface for the delete mirror button SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    deleteMirrorButtonTexture = SDL_CreateTextureFromSurface(renderer, deleteMirrorButtonSurface);

    if (!deleteMirrorButtonTexture)
    {
        std::cerr << "Failed to create the texture for the delete mirror button! SDL_Error: " << SDL_GetError() << std::endl;
    }

    deleteMirrorButtonRect.w = deleteMirrorButtonSurface->w;
    deleteMirrorButtonRect.h = deleteMirrorButtonSurface->h;

    SDL_FreeSurface(deleteMirrorButtonSurface);
    SDL_SetTextureBlendMode(deleteMirrorButtonTexture, SDL_BLENDMODE_BLEND);
}

void DeleteMirrorButton::render_delete_mirror_button(SDL_Renderer *renderer)
{
    if (deleteMirrorButtonTexture)
    {
        SDL_RenderCopy(renderer, deleteMirrorButtonTexture, nullptr, &deleteMirrorButtonRect);
    }
}

void DeleteMirrorButton::delete_mirror()
{
    if(!mirrors.empty())
    {
        delete mirrors.back();
        mirrors.pop_back();
    }
}