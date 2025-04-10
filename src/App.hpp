#ifndef APP_H
#define APP_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "definitions.hpp"
#include "TopBar.hpp"
#include "SourceObject.hpp" 
#include "Mirror.hpp"

extern int window_width;

//obsashuje deklarace proměných, funkcý a class
class Renderer1
{
public:
    void render(SDL_Renderer* renderer,
                SDL_Window* window,
                TopBar* topBar, 
                AddSourceButton* addSourceButton,
                AddBlockerButton* addBlockerButton,
                AddMirrorButton* addMirrorButton,
                DeleteSourceButton* deleteSourceButton,
                DeleteBlockerButton* deleteBlockerButton,
                DeleteMirrorButton* deleteMirrorButton);
};

class Update
{
    SDL_Event event;
    bool isDraggingBlocker = false;
    bool isDraggingSource = false;
    bool isDraggingMirror = false;
    MirrorObject* draggedMirror = nullptr;
    SourceObject* draggedSource = nullptr;
    BlockerObject* draggedBlocker = nullptr;

public:
    void process_input(bool &running, AddSourceButton* addSourceButton, AddBlockerButton* addBlockerButton,
        DeleteSourceButton* deleteSourceButton, DeleteBlockerButton* deleteBlockerButton, AddMirrorButton* addMirrorButton, DeleteMirrorButton* deleteMirrorButton);
};

class App
{
protected: 
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SourceObject* source;
    BlockerObject* blocker;
    AddSourceButton* addSourceButton;
    AddBlockerButton* addBlockerButton;
    AddMirrorButton* addMirrorButton; 
    DeleteSourceButton* deleteSourceButton;
    DeleteBlockerButton* deleteBlockerButton;
    DeleteMirrorButton* deleteMirrorButton; 
    TopBar* topBar;
    Update* update;
    Renderer1* render;
    
    bool running;

public:
    App();
    ~App();
    int init();
    void run();
};

#endif