#ifndef RAY_H
#define RAY_H

#include <SDL.h>
#include <vector>
#include "Mirror.hpp"

class SourceObject;
class BlockerObject;
class MirrorObject;

class Ray
{
public:
    float angle;
    float collisionX;
    float collisionY;
    bool hasCollision;
    bool hasMirrorCollision;
    float nearestCollisionDistance;
    int startX, startY;
private:
    int endX, endY;
    float originalEndX, originalEndY;
    SourceObject* source;
    bool collided = false;

public:
    Ray(SourceObject* src, float ang);
    void update_position();
    void draw_ray(SDL_Renderer* renderer);
    void check_for_ray_collision(const std::vector<BlockerObject*>& blockers, const std::vector<MirrorObject*>& mirrors);
    bool has_collided() const { return collided; }
    void reset_collision() { collided = false; endX = originalEndX; endY = originalEndY; }
};

#endif