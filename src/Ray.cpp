#include "Ray.hpp"
#include "SourceObject.hpp"
#include "BlockerObject.hpp"
#include "definitions.hpp"
#include <cmath>
#include <iostream>

//constructor, který se zavolá při vytvoření paprsku
Ray::Ray(SourceObject* src, float ang) : angle(ang), source(src)
{
    update_position();
    originalEndX = endX;
    originalEndY = endY;
}

void Ray::update_position()
{
    //když se hýbne se zdrojem tak se updatne i pozice paprsků
    //startX, startY = záčátek paprsku
    //paprskek bude začínat na pozici zdroje + polovinu jeho velikosti (protože obejkty jsou corner alligned a ne center aligned = x a y pozice jsou v rohu toho objektu a ne v jeho středu)
    startX = source->sourceRect.x + source->sourceRect.w / 2;
    startY = source->sourceRect.y + source->sourceRect.h / 2;

    //vypočítá kde bude paprsek končit (originalEndX a EndY se nemění)
    originalEndX = startX + 1000 * cos(angle);
    originalEndY = startY + 1000 * sin(angle);
    
    //konec paprsku
    endX = originalEndX;
    endY = originalEndY;

}

//zkontroluje jestli nastala kolize paprsků s něčím
void Ray::check_for_ray_collision(const std::vector<BlockerObject*>& blockers, const std::vector<MirrorObject*>& mirrors) {
    hasCollision = false;
    hasMirrorCollision = false;
    float nearestCollisionDistance = FLT_MAX;
    
    // směr a délka paprsku
    float rayDirX = originalEndX - startX;
    float rayDirY = originalEndY - startY;
    float rayLength = std::hypot(rayDirX, rayDirY);
    
    // normalizace směru
    if (rayLength > 0) { rayDirX /= rayLength; rayDirY /= rayLength; }

    // kontrola kolize s blockery
    for (const auto& blocker : blockers) 
    {
        float circleCenterX = blocker->blockerRect.x + blocker->blockerRect.w / 2.0f;
        float circleCenterY = blocker->blockerRect.y + blocker->blockerRect.h / 2.0f;
        float circleRadius = 225.0f / 2.0f;
        
        float toCircleX = circleCenterX - startX;
        float toCircleY = circleCenterY - startY;
        float projection = toCircleX * rayDirX + toCircleY * rayDirY;
        
        if (projection <= 0) continue; // přeskočit, pokud je kruh za paprskem
        
        float closestPointX = startX + projection * rayDirX;
        float closestPointY = startY + projection * rayDirY;
        float distanceToCenter = std::hypot(closestPointX - circleCenterX, closestPointY - circleCenterY);
        
        if (distanceToCenter <= circleRadius) 
        {
            float offset = std::sqrt(circleRadius * circleRadius - distanceToCenter * distanceToCenter);
            float intersectionDistance = projection - offset;
            
            if (intersectionDistance > 0 && intersectionDistance < nearestCollisionDistance)
            {
                nearestCollisionDistance = intersectionDistance;
                collisionX = startX + intersectionDistance * rayDirX;
                collisionY = startY + intersectionDistance * rayDirY;
                hasCollision = true;
                hasMirrorCollision = false;
            }
        }
    }

    // kontrola kolize se zrcadly
    for (auto& mirror : mirrors) {
        SDL_Rect mirrorRect = mirror->mirrorRect;
        int rayStartX = startX, rayStartY = startY;
        int rayEndX = originalEndX, rayEndY = originalEndY;

        if (SDL_IntersectRectAndLine(&mirrorRect, &rayStartX, &rayStartY, &rayEndX, &rayEndY))
        {
            float mirrorCollisionDistance = std::hypot(rayStartX - startX, rayStartY - startY);
            
            if (mirrorCollisionDistance < nearestCollisionDistance)
            {
                nearestCollisionDistance = mirrorCollisionDistance;
                collisionX = rayStartX;
                collisionY = rayStartY;
                hasCollision = true;
                hasMirrorCollision = true;
                
                endX = collisionX;
                endY = collisionY;
                
                // přidat paprsek do vektoru odražených
                if (std::find(mirror->hittingRays.begin(), mirror->hittingRays.end(), this) == mirror->hittingRays.end())
                {
                    mirror->hittingRays.push_back(this);
                }
            }
        }
    }

    // aktualizuje koncový bod paprsku
    endX = hasCollision ? collisionX : originalEndX;
    endY = hasCollision ? collisionY : originalEndY;
    collided = hasCollision;
}

//vykreslí paprsky
void Ray::draw_ray(SDL_Renderer* renderer)
{ 
    //nastaví aby renderer vykresloval žlutou barvu  
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //vykreslí čárů od startX, startY do endX, endY
    SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
}