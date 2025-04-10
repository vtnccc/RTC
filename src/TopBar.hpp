#ifndef TOP_BAR_H
#define TOP_BAR_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "definitions.hpp"
#include "BlockerObject.hpp"
#include "SourceObject.hpp"
#include "Mirror.hpp"
#include <filesystem>
#include <fstream>
#include <unordered_map>

namespace fs = std::filesystem;

class Button
{
protected:
    std::unordered_map <std::string, fs::path> fileMap;
public:
    SDL_Rect addSourceButtonRect; 
    SDL_Rect addBlockerButtonRect;
    SDL_Rect addMirrorButtonRect;
    SDL_Rect deleteSourceButtonRect;
    SDL_Rect deleteBlockerButtonRect;
    SDL_Rect deleteMirrorButtonRect;

    SDL_Surface* addSourceButtonSurface;
    SDL_Surface* addBlockerButtonSurface;
    SDL_Surface* addMirrorButtonSurface;
    SDL_Surface* deleteSourceButtonSurface;
    SDL_Surface* deleteBlockerButtonSurface;
    SDL_Surface* deleteMirrorButtonSurface;

    SDL_Texture* addSourceButtonTexture;
    SDL_Texture* addBlockerButtonTexture;
    SDL_Texture* addMirrorButtonTexture;
    SDL_Texture* deleteSourceButtonTexture;
    SDL_Texture* deleteBlockerButtonTexture;
    SDL_Texture* deleteMirrorButtonTexture;

    Button();
    virtual ~Button();
    void scan_directory();
};

class TopBar
{
protected:
    SDL_Rect topBar;

public:
    TopBar();
    void render_top_bar(SDL_Renderer* renderer);
};

class AddSourceButton : public Button
{
private:
    SDL_Renderer* renderer;
public:
    std::vector<SourceObject*> sources;
    
    void set_renderer(SDL_Renderer* r);
    void create_add_source_button_texture(SDL_Renderer* renderer);
    void render_add_source_button(SDL_Renderer* renderer);
    void add_source();
};

class DeleteSourceButton : public Button
{
private:
    SDL_Renderer* renderer;
public:
    std::vector<SourceObject*>& sources;
    
    DeleteSourceButton(std::vector<SourceObject*>& srcs);
    void set_renderer(SDL_Renderer* r);
    void create_delete_source_button_texture(SDL_Renderer* renderer);
    void render_delete_source_button(SDL_Renderer* renderer);
    void delete_source();
};

class AddBlockerButton : public Button
{
private:
    SDL_Renderer* renderer;
public:
    std::vector<BlockerObject*> blockers;
    void set_renderer(SDL_Renderer* r);
    void create_add_blocker_button_texture(SDL_Renderer* renderer);
    void render_add_blocker_button(SDL_Renderer* renderer);
    void add_blocker();
};

class DeleteBlockerButton : public Button
{
private:
    SDL_Renderer* renderer;
public:
    std::vector<BlockerObject*>& blockers;
    DeleteBlockerButton(std::vector<BlockerObject*>& blks);
    void set_renderer(SDL_Renderer* r);
    void create_delete_blocker_button_texture(SDL_Renderer* renderer);
    void render_delete_blocker_button(SDL_Renderer* renderer);
    void delete_blocker();
};

class AddMirrorButton : public Button
{
private:
    SDL_Renderer* renderer;
public:
    std::vector<MirrorObject*> mirrors; 
    AddMirrorButton(); 
    void set_renderer(SDL_Renderer* r);
    void create_add_mirror_button_texture(SDL_Renderer* renderer);
    void render_add_mirror_button(SDL_Renderer* renderer);
    void add_mirror();
};

class DeleteMirrorButton : public Button
{
private:
    SDL_Renderer* renderer;
public:
    std::vector<MirrorObject*>& mirrors;
    DeleteMirrorButton(std::vector<MirrorObject*>& mirrs);
    void set_renderer(SDL_Renderer* r);
    void create_delete_mirror_button_texture(SDL_Renderer* renderer);
    void render_delete_mirror_button(SDL_Renderer* renderer);
    void delete_mirror();
};

#endif