// #include "power_up.h"
#include "maze.h"

#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

void PowerUp::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Walls)
    {
        // if (!tile.Destroyed)
        tile.Offset = this->Position;
        tile.Draw(renderer);
        // std::cout<<this->Position.x<<" "<<this->Position.y<<"\n";
    }
}

// int PowerUp::checkInside(glm::vec2 point)
// {
//     for(int i=0;i<roomCoords.size();i+=4)
//     {
//         glm::vec2 quad[4] = {roomCoords[i],roomCoords[i+1],roomCoords[i+2],roomCoords[i+3]};
//         if(pointIsInQuad(point,quad))
//             return 2;
//     }
//     for(int i=0;i<pathCoords.size();i+=4)
//     {
//         glm::vec2 quad[4] = {pathCoords[i],pathCoords[i+1],pathCoords[i+2],pathCoords[i+3]};
//         if(pointIsInQuad(point,quad))
//             return 2;
//     }
//     // std::cout<<"No\n";
//     return 0;
// }

// glm::vec4 PowerUp::get_corners(glm::vec2 position, float rotate, glm::vec2 size, int room)
// {
//     glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800.0), static_cast<float>(800.0), 0.0f, -1.0f, 1.0f);
//     glm::mat4 model = glm::mat4(1.0f);
//     model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

//     model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
//     model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
//     model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

//     model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

//     glm::vec2 vertex1 = glm::vec2(0.0,0.0);
//     glm::vec4 vert1 = model * glm::vec4(vertex1.x, vertex1.y, 0.0, 1.0);
//     glm::vec2 vertex2 = glm::vec2(1.0,0.0);
//     glm::vec4 vert2 = model * glm::vec4(vertex2.x, vertex2.y, 0.0, 1.0);
//     glm::vec2 vertex3 = glm::vec2(1.0,1.0);
//     glm::vec4 vert3 = model * glm::vec4(vertex3.x, vertex3.y, 0.0, 1.0);
//     glm::vec2 vertex4 = glm::vec2(0.0,1.0);
//     glm::vec4 vert4 = model * glm::vec4(vertex4.x, vertex4.y, 0.0, 1.0);

//     if (room==1)
//     {
//         roomCoords.push_back(glm::vec2(vert1.x,vert1.y));
//         roomCoords.push_back(glm::vec2(vert2.x,vert2.y));
//         roomCoords.push_back(glm::vec2(vert3.x,vert3.y));
//         roomCoords.push_back(glm::vec2(vert4.x,vert4.y));
//     }
//     else
//     {
//         pathCoords.push_back(glm::vec2(vert1.x,vert1.y));
//         pathCoords.push_back(glm::vec2(vert2.x,vert2.y));
//         pathCoords.push_back(glm::vec2(vert3.x,vert3.y));
//         pathCoords.push_back(glm::vec2(vert4.x,vert4.y));
//     }

//     return vert1;
// }

void PowerUp::init()
{
    this->active = true;
    if(ObjectType==0)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("door")));
    }
    else if(ObjectType==1)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("star")));
    }
    else if(ObjectType==2)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("button")));
    }
    else if(ObjectType==3)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("coin")));
    }
    else if(ObjectType==4)
    {
        this->Walls.push_back(GameObject(glm::vec2(-25.0,-25.0), glm::vec2(50.0,50.0), ResourceManager::GetTexture("danger")));
    }
    for (GameObject &tile : this->Walls)
    {
        // if (!tile.Destroyed)
        tile.Offset = this->Position;
        // tile.Draw(renderer);
        // std::cout<<this->Position.x<<" "<<this->Position.y<<"\n";
    }
}