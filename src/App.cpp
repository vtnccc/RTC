#include "App.hpp"
#include "TopBar.hpp"

int window_width = WINDOW_WIDTH;

// hlavní render funkce
void Renderer1::render(SDL_Renderer *renderer,
    SDL_Window *window, 
    TopBar *topBar, 
    AddSourceButton *addSourceButton, 
    AddBlockerButton *addBlockerButton, 
    AddMirrorButton *addMirrorButton, 
    DeleteSourceButton *deleteSourceButton, 
    DeleteBlockerButton *deleteBlockerButton, 
    DeleteMirrorButton *deleteMirrorButton)
{
    // povolí resizing okny
    SDL_SetWindowResizable(window, SDL_TRUE);

    // vyčistí scénu a nastaví černý pozadí
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // vygeneruje a vykreslý všechny paprsky a zdroje
    for (auto& src : addSourceButton->sources)
    {
        // vygeneruje paprsky znova
        src->generate_rays();

        // zkontroluje kolizi paprsků
        for(auto& ray : src->rays)
        {
            ray.check_for_ray_collision(addBlockerButton->blockers, addMirrorButton->mirrors);
        }

        // vyrenderuje održený paprsky
        for (auto& mirror : addMirrorButton->mirrors)
        {
            mirror->reflect_light();
        }

        // vyrenderuje paprsky
        for (auto& ray : src->rays) 
        {
            ray.draw_ray(renderer);
        }

        // vyrenderuje odražený paprsky
        for (auto* reflectedRay : src->reflectedRays) 
        {
            reflectedRay->draw_ray(renderer);
        }

        // vyrenderuje zdroje
        src->render_source(renderer);
    }

    // vyrenderuje všechny blockery
    for (auto& blk : addBlockerButton->blockers)
    {
        blk->render_blocker(renderer);
    }

    // všechny zrcadla
    for (auto& mirror : addMirrorButton->mirrors)
    {
        mirror->render_mirror(renderer);
    }

    // všechny tlačítka
    topBar->render_top_bar(renderer);
    addSourceButton->render_add_source_button(renderer);
    addBlockerButton->render_add_blocker_button(renderer);
    addMirrorButton->render_add_mirror_button(renderer);
    deleteSourceButton->render_delete_source_button(renderer);
    deleteBlockerButton->render_delete_blocker_button(renderer);
    deleteMirrorButton->render_delete_mirror_button(renderer);
    
    SDL_RenderPresent(renderer);
}

//hlavní funkce na detekci inputů
void Update::process_input(bool &running, AddSourceButton* addSourceButton, AddBlockerButton* addBlockerButton,
                   DeleteSourceButton* deleteSourceButton, DeleteBlockerButton* deleteBlockerButton, 
                   AddMirrorButton* addMirrorButton, DeleteMirrorButton* deleteMirrorButton)
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                //vypnutí aplikace
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
            //pokud zmáčknete levý tlačítku na myši tak se pro všechny objekty zkontroluje jestli nebyly kliknuty
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    SDL_Point mousePoint = {event.button.x, event.button.y};

                    // zkontroluje zdroje
                    for (auto& src : addSourceButton->sources)
                    {
                        if (SDL_PointInRect(&mousePoint, &src->sourceRect))
                        {
                            isDraggingSource = true;
                            draggedSource = src;
                            break;
                        }
                    }
                    
                    //zkontroluje blockery
                    for (auto& blk : addBlockerButton->blockers)
                    {
                        if (SDL_PointInRect(&mousePoint, &blk->blockerRect))
                        {
                            isDraggingBlocker = true;
                            draggedBlocker = blk;
                            break;
                        }
                    }

                    //zkontroluje zrcadla
                    for (auto& mirror : addMirrorButton->mirrors)
                    {
                        if (SDL_PointInRect(&mousePoint, &mirror->mirrorRect))
                        {
                            isDraggingMirror = true;
                            draggedMirror = mirror;
                            break;
                        }
                    }
                    
                    //zkontroluje všechny tlačítka
                    if(SDL_PointInRect(&mousePoint, &addSourceButton->addSourceButtonRect))
                    {
                        addSourceButton->add_source();
                    }
                    else if(SDL_PointInRect(&mousePoint, &addBlockerButton->addBlockerButtonRect))
                    {
                        addBlockerButton->add_blocker();
                    }
                    else if(SDL_PointInRect(&mousePoint, &addMirrorButton->addMirrorButtonRect))
                    {
                        addMirrorButton->add_mirror();
                    }
                    else if(SDL_PointInRect(&mousePoint, &deleteSourceButton->deleteSourceButtonRect))
                    {
                        deleteSourceButton->delete_source();
                    }
                    else if(SDL_PointInRect(&mousePoint, &deleteBlockerButton->deleteBlockerButtonRect))
                    {
                        deleteBlockerButton->delete_blocker();
                    }
                    else if(SDL_PointInRect(&mousePoint, &deleteMirrorButton->deleteMirrorButtonRect))
                    {
                        deleteMirrorButton->delete_mirror();
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
            //pokud tlačítku pustíte tak se všechno resetuje a přeruší se pohyb objektů
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isDraggingSource = false;
                    isDraggingBlocker = false;
                    isDraggingMirror = false;
                    draggedSource = nullptr;
                    draggedBlocker = nullptr;
                    draggedMirror = nullptr;
                }
                break;

            case SDL_MOUSEMOTION:
            //pokud se splní podmínky = pohybujete objektem
            if (isDraggingSource && draggedSource)
            {
                SDL_Point mousePoint = {event.motion.x, event.motion.y};
                draggedSource->move_source(mousePoint);
            }
            else if (isDraggingBlocker && draggedBlocker)
            {
                SDL_Point mousePoint = {event.motion.x, event.motion.y};
                draggedBlocker->move_blocker(mousePoint);
            }
            else if (isDraggingMirror && draggedMirror)
            {
                SDL_Point mousePoint = {event.motion.x, event.motion.y};
                draggedMirror->move_mirror(mousePoint);
            }
            break;
            case SDL_WINDOWEVENT:
                //když změníte velikost okna tak tohle zaznamená ok kolik jste to změnili
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    int newWidth = event.window.data1;
                    window_width = newWidth;
                }     
                break; 
        }
    }
}

//constructor, který se zavolá při spuštění aplikace
App::App() : 
    source(new SourceObject()), 
    blocker(new BlockerObject()),
    addSourceButton(new AddSourceButton()), 
    addBlockerButton(new AddBlockerButton()),
    addMirrorButton(new AddMirrorButton()),
    deleteSourceButton(nullptr), 
    deleteBlockerButton(nullptr),
    deleteMirrorButton(nullptr),
    topBar(new TopBar()), 
    update(new Update()), 
    render(new Renderer1()), 
    running(false) 
{
    deleteSourceButton = new DeleteSourceButton(addSourceButton->sources);
    deleteBlockerButton = new DeleteBlockerButton(addBlockerButton->blockers);
    deleteMirrorButton = new DeleteMirrorButton(addMirrorButton->mirrors);
    
    source->generate_rays();
}

//destructor, který se zavolá při ukončení aplikace a smaže všechny objekty z paměti
App::~App()
{
    for (auto& src : addSourceButton->sources) 
    {
        delete src;
    }
    
    for (auto& blk : addBlockerButton->blockers) 
    {
        delete blk;
    }
    
    for (auto& mirror : addMirrorButton->mirrors)
    {
        delete mirror;
    }
    
    delete addSourceButton;
    delete addBlockerButton;
    delete addMirrorButton;
    delete deleteMirrorButton;
    delete deleteSourceButton;
    delete deleteBlockerButton;
    delete source;
    delete blocker;
    delete topBar;
    delete update;
    delete render;
}

//vytvoří okno, renderer a textury objektů
int App::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) != (IMG_INIT_PNG))
    {
        std::cerr << "IMG_Init error: " << IMG_GetError() << std::endl;
    }
    
    window = SDL_CreateWindow("RayTracing",
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        std::cerr << "Window could not be created: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) 
    {
        std::cerr << "Renderer could not be created: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    source->create_source_texture(renderer);
    blocker->create_blocker_texture(renderer);
    addSourceButton->create_add_source_button_texture(renderer);
    addBlockerButton->create_add_blocker_button_texture(renderer);
    addMirrorButton->create_add_mirror_button_texture(renderer);
    deleteMirrorButton->create_delete_mirror_button_texture(renderer);
    deleteSourceButton->create_delete_source_button_texture(renderer);
    deleteBlockerButton->create_delete_blocker_button_texture(renderer);
    
    addMirrorButton->set_renderer(renderer);
    addSourceButton->set_renderer(renderer);
    addBlockerButton->set_renderer(renderer);
    deleteMirrorButton->set_renderer(renderer);
    deleteSourceButton->set_renderer(renderer);
    deleteBlockerButton->set_renderer(renderer);
    
    running = true;
    return 0;
}

//hlavní loop aplikace
void App::run()
{
    if (init() != 0)
    {
        return;
    }

    while(running)
    {
        //limituje aplikaci tak aby běžela max na 60 fps
        int last_frame_time = 0;
        while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
        last_frame_time = SDL_GetTicks();

        //updatuje scénu na základě inputů
        update->process_input(running, addSourceButton, addBlockerButton, deleteSourceButton, deleteBlockerButton, addMirrorButton, deleteMirrorButton);
        //vykreslý to na obrazovku
        render->render(renderer, window, topBar, addSourceButton, addBlockerButton, addMirrorButton, deleteSourceButton, deleteBlockerButton, deleteMirrorButton);
    }
    
    //po ukončení aplikace se všechno vymaže, uvolní a zníčí
    SDL_DestroyTexture(source->sourceTexture);
    SDL_DestroyTexture(blocker->blockerTexture);
    SDL_DestroyTexture(addSourceButton->addSourceButtonTexture);
    SDL_DestroyTexture(addBlockerButton->addBlockerButtonTexture);
    SDL_DestroyTexture(addMirrorButton->addMirrorButtonTexture);
    SDL_DestroyTexture(deleteSourceButton->deleteSourceButtonTexture);
    SDL_DestroyTexture(deleteBlockerButton->deleteBlockerButtonTexture);
    SDL_DestroyTexture(deleteMirrorButton->deleteBlockerButtonTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}