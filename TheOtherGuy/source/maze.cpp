/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "maze.h"

#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

// void Maze::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
// {
//     // clear old data
//     this->Walls.clear();
//     // load from file
//     unsigned int tileCode;
//     Maze level;
//     std::string line;
//     std::ifstream fstream(file);
//     std::vector<std::vector<unsigned int>> tileData;
//     if (fstream)
//     {
//         while (std::getline(fstream, line)) // read each line from level file
//         {
//             std::istringstream sstream(line);
//             std::vector<unsigned int> row;
//             while (sstream >> tileCode) // read each word separated by spaces
//                 row.push_back(tileCode);
//             tileData.push_back(row);
//         }
//         if (tileData.size() > 0)
//             this->init(tileData, levelWidth, levelHeight);
//     }
// }

void Maze::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Walls)
    {
        // if (!tile.Destroyed)
        tile.Offset = this->Position;
        tile.Draw(renderer);
    }
}

// bool Maze::IsCompleted()
// {
//     for (GameObject &tile : this->Walls)
//         if (!tile.IsSolid && !tile.Destroyed)
//             return false;
//     return true;
// }

// function SameSide(p1,p2, a,b)
//     cp1 = CrossProduct(b-a, p1-a)
//     cp2 = CrossProduct(b-a, p2-a)
//     if DotProduct(cp1, cp2) >= 0 then return true
//     else return false

// function PointInTriangle(p, a,b,c)
//     if SameSide(p,a, b,c) and SameSide(p,b, a,c)
//         and SameSide(p,c, a,b) then return true
//     else return false

bool pointIsInQuad(glm::vec2 point, glm::vec2 quad[4])
{
    bool sides[4];
    for (int i = 0; i < 4; i++) {
        sides[i] = ((point.x - quad[i].x)*(quad[(i + 1)%4].y - quad[i].y) - (point.y - quad[i].y)*(quad[(i + 1)%4].x - quad[i].x)) > 0.0f;
    }
    return ((sides[0] == sides[1]) && (sides[0] == sides[2]) && (sides[0] == sides[3]));
}

int Maze::checkInside(glm::vec2 point)
{
    // std::cout<<point.x<<" "<<point.y<<'\n';
    // for(int i=0;i<roomCoords.size();i+=4)
    // {
    //     if(point.x>=roomCoords[i].x && point.y<=roomCoords[i].y && point.x<=roomCoords[i+1].x && point.y<=roomCoords[i+1].y &&
    //         point.x<=roomCoords[i+2].x && point.y>=roomCoords[i+2].y && point.x>=roomCoords[i+3].x && point.y>=roomCoords[i+3].y)
    //         return 1;
    // }
    for(int i=0;i<roomCoords.size();i+=4)
    {
        glm::vec2 quad[4] = {roomCoords[i],roomCoords[i+1],roomCoords[i+2],roomCoords[i+3]};
        if(pointIsInQuad(point,quad))
            return 2;
    }
    for(int i=0;i<pathCoords.size();i+=4)
    {
        glm::vec2 quad[4] = {pathCoords[i],pathCoords[i+1],pathCoords[i+2],pathCoords[i+3]};
        if(pointIsInQuad(point,quad))
            return 2;
    }
    // std::cout<<"No\n";
    return 0;
}

glm::vec4 Maze::get_corners(glm::vec2 position, float rotate, glm::vec2 size, int room)
{
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800.0), static_cast<float>(800.0), 0.0f, -1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    glm::vec2 vertex1 = glm::vec2(0.0,0.0);
    glm::vec4 vert1 = model * glm::vec4(vertex1.x, vertex1.y, 0.0, 1.0);
    glm::vec2 vertex2 = glm::vec2(1.0,0.0);
    glm::vec4 vert2 = model * glm::vec4(vertex2.x, vertex2.y, 0.0, 1.0);
    glm::vec2 vertex3 = glm::vec2(1.0,1.0);
    glm::vec4 vert3 = model * glm::vec4(vertex3.x, vertex3.y, 0.0, 1.0);
    glm::vec2 vertex4 = glm::vec2(0.0,1.0);
    glm::vec4 vert4 = model * glm::vec4(vertex4.x, vertex4.y, 0.0, 1.0);

    if (room==1)
    {
        roomCoords.push_back(glm::vec2(vert1.x,vert1.y));
        roomCoords.push_back(glm::vec2(vert2.x,vert2.y));
        roomCoords.push_back(glm::vec2(vert3.x,vert3.y));
        roomCoords.push_back(glm::vec2(vert4.x,vert4.y));
    }
    else
    {
        pathCoords.push_back(glm::vec2(vert1.x,vert1.y));
        pathCoords.push_back(glm::vec2(vert2.x,vert2.y));
        pathCoords.push_back(glm::vec2(vert3.x,vert3.y));
        pathCoords.push_back(glm::vec2(vert4.x,vert4.y));
    }

    return vert1;
}

void Maze::init()
{
    // srand(time(0)); // Truly random
    int num_hor = (rand()%1 + 3), num_vert = (rand()%1 + 3);
    float roomsize = 200;
    float roomdist = 100;
    glm::vec2 room_shape = glm::vec2(roomsize,roomsize);
    glm::vec2 room_offset = glm::vec2(-roomsize/2,-roomsize/2);
    mazeSize = glm::vec2(roomdist*(num_hor-1) + roomsize*num_hor + 200, roomdist*(num_vert-1) + roomsize*num_vert + 200);
    int dir = rand()%2;
    if(dir==0)
    {
        int curr = 0;
        adjList.resize(4*num_hor);
        adjListWeighted.resize(4*num_hor);
        std::pair<int,int> prep1, prep2;
        int pre_count=0;
        for(int i=0;i<num_hor;i++)
        {
            int num = (rand()%2)+1;
            std::pair<int,int> pp1, pp2;
            if(num==1)
            {
                curr++;
                pp1 = {i*(roomsize+roomdist),(rand()%num_vert)*(roomsize+roomdist)};
                vertexPositions.push_back(pp1);
                if(i!=0)
                {
                    if(pre_count==1)
                    {
                        adjList[curr-1].push_back(curr-1-1);
                        adjList[curr-1-1].push_back(curr-1);
                        edges.push_back({curr-1,curr-1-1});
                    }
                    else
                    {
                        adjList[curr-1].push_back(curr-1-1);
                        adjList[curr-1-1].push_back(curr-1);
                        edges.push_back({curr-1,curr-1-1});
                        adjList[curr-1].push_back(curr-1-2);
                        adjList[curr-1-2].push_back(curr-1);
                        edges.push_back({curr-1,curr-1-2});
                    }
                }
            }
            else
            {
                curr +=2;
                pp1 = {i*(roomsize+roomdist),(rand()%(num_vert/2))*(roomsize+roomdist)};
                pp2 = {i*(roomsize+roomdist),(rand()%(num_vert/2)+(num_vert/2))*(roomsize+roomdist)};
                vertexPositions.push_back(pp1);
                vertexPositions.push_back(pp2);
                adjList[curr-1].push_back(curr-2);
                adjList[curr-2].push_back(curr-1);
                edges.push_back({curr-2,curr-1});
                if(i!=0)
                {
                    if(pre_count==1)
                    {
                        adjList[curr-1].push_back(curr-3);
                        adjList[curr-3].push_back(curr-1);
                        edges.push_back({curr-1,curr-3});
                        adjList[curr-1-1].push_back(curr-1-2);
                        adjList[curr-1-2].push_back(curr-1-1);
                        edges.push_back({curr-1-1,curr-1-2});
                    }
                    else
                    {
                        adjList[curr-1].push_back(curr-1-2);
                        adjList[curr-1-2].push_back(curr-1);
                        edges.push_back({curr-1,curr-1-2});
                        adjList[curr-1-1].push_back(curr-1-3);
                        adjList[curr-1-3].push_back(curr-1-1);
                        edges.push_back({curr-1-1,curr-1-3});
                    }
                }
            }
            pre_count = num;
            prep1 = pp1, prep2 = pp2;
        }
        // std::cout<<"Hi\n";
    }
    else if(dir==1)
    {
        int curr = 0;
        adjList.resize(4*num_vert);
        adjListWeighted.resize(4*num_vert);
        std::pair<int,int> prep1, prep2;
        int pre_count=0;
        for(int i=0;i<num_vert;i++)
        {
            int num = (rand()%2)+1;
            std::pair<int,int> pp1, pp2;
            if(num==1)
            {
                curr++;
                pp1 = {(rand()%num_hor)*(roomsize+roomdist),i*(roomsize+roomdist)};
                vertexPositions.push_back(pp1);
                if(i!=0)
                {
                    if(pre_count==1)
                    {
                        adjList[curr-1].push_back(curr-1-1);
                        adjList[curr-1-1].push_back(curr-1);
                        edges.push_back({curr-1,curr-1-1});
                    }
                    else
                    {
                        adjList[curr-1].push_back(curr-1-1);
                        adjList[curr-1-1].push_back(curr-1);
                        edges.push_back({curr-1,curr-1-1});
                        adjList[curr-1].push_back(curr-1-2);
                        adjList[curr-1-2].push_back(curr-1);
                        edges.push_back({curr-1,curr-1-2});
                    }
                }
            }
            else
            {
                curr +=2;
                pp1 = {(rand()%(num_hor/2))*(roomsize+roomdist),i*(roomsize+roomdist)};
                pp2 = {(rand()%(num_hor/2)+(num_hor/2))*(roomsize+roomdist),i*(roomsize+roomdist)};
                vertexPositions.push_back(pp1);
                vertexPositions.push_back(pp2);
                adjList[curr-1].push_back(curr-2);
                adjList[curr-2].push_back(curr-1);
                edges.push_back({curr-2,curr-1});
                if(i!=0)
                {
                    if(pre_count==1)
                    {
                        adjList[curr-1].push_back(curr-3);
                        adjList[curr-3].push_back(curr-1);
                        edges.push_back({curr-1,curr-3});
                        adjList[curr-1-1].push_back(curr-1-2);
                        adjList[curr-1-2].push_back(curr-1-1);
                        edges.push_back({curr-1-1,curr-1-2});
                    }
                    else
                    {
                        adjList[curr-1].push_back(curr-1-2);
                        adjList[curr-1-2].push_back(curr-1);
                        edges.push_back({curr-1,curr-1-2});
                        adjList[curr-1-1].push_back(curr-1-3);
                        adjList[curr-1-3].push_back(curr-1-1);
                        edges.push_back({curr-1-1,curr-1-3});
                    }
                }
            }
            pre_count = num;
            prep1 = pp1, prep2 = pp2;
        }
    }
    for(int i=0;i<vertexPositions.size();i++)
    {
        // std::cout<<vertexPositions[i].first<<" "<<vertexPositions[i].second<<'\n';
        glm::vec2 room_pos = glm::vec2(vertexPositions[i].first,vertexPositions[i].second);
        this->Walls.push_back(GameObject(room_pos+room_offset, room_shape, ResourceManager::GetTexture("grey")));     

        glm::vec4 res = get_corners(room_pos+room_offset,0.0,room_shape,1);
        // glm::vec2 gree = glm::vec2(res.x,res.y);
        // this->Walls.push_back(GameObject(gree, room_shape, ResourceManager::GetTexture("block")));     
    }
    for(int i=0;i<edges.size();i++)
    {
        float rot = 0;
        glm::vec2 room_pos_1 = glm::vec2(vertexPositions[edges[i].first].first,vertexPositions[edges[i].first].second);
        glm::vec2 room_pos_2 = glm::vec2(vertexPositions[edges[i].second].first,vertexPositions[edges[i].second].second);
        rot = atan2((room_pos_2.y-room_pos_1.y),(room_pos_2.x-room_pos_1.x));
        float dist = glm::length(room_pos_1-room_pos_2);
        glm::vec2 path_shape = glm::vec2(dist,90.0);
        glm::vec2 mid = glm::vec2((room_pos_1.x+room_pos_2.x)/2,(room_pos_1.y+room_pos_2.y)/2);
        glm::vec2 hallaf = glm::vec2(dist/2.0,75.0/2.0);
        GameObject obj = GameObject(mid-hallaf, path_shape, ResourceManager::GetTexture("grey"));
        obj.Rotation = rot*180.0/PI;
        this->Walls.push_back(obj);
        glm::vec4 res = get_corners(mid-hallaf,rot*180.0/PI, path_shape, 0);
        // glm::vec2 gree = glm::vec2(res.x,res.y);
        // this->Walls.push_back(GameObject(gree, room_shape, ResourceManager::GetTexture("block")));     

        adjListWeighted[edges[i].first].push_back({edges[i].second,dist});
        adjListWeighted[edges[i].second].push_back({edges[i].first,dist});
    }

    // check if corners are correct
    for(auto a: roomCoords)
    {
        this->Walls.push_back(GameObject(a, glm::vec2(5.0,5.0), ResourceManager::GetTexture("block")));
    }

    int playerInitVertex = rand()%vertexPositions.size();
    playerInitPos = glm::vec2(vertexPositions[playerInitVertex].first,vertexPositions[playerInitVertex].second);
    // this->Walls.push_back(GameObject(playerInitPos, glm::vec2(5.0,5.0), ResourceManager::GetTexture("block")));
    this->Position = -(playerInitPos-glm::vec2(400.0,400.0));
    // exit(0);
    // this->Walls.push_back(GameObject(pos2, size2, ResourceManager::GetTexture("block")));
}