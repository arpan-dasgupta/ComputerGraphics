#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"


class Maze
{
public:

    std::vector<GameObject> Walls;
    glm::vec2   Position;
    glm::vec2   topLeft, bottomRight;
    glm::vec2   mazeSize;
    glm::vec2   exitPos, powerUp1Pos, powerUp2Pos;

    std::vector<std::pair<float,float>> availableRooms;
    std::vector<std::vector<int>> adjList;
    std::vector<std::vector<std::pair<int,float>>> adjListWeighted;
    std::vector<std::pair<float,float>> vertexPositions;
    std::vector<std::pair<int,int>> edges;
    glm::vec2   playerInitPos;
    std::vector<glm::vec2> roomCoords, pathCoords;

    // constructor
    Maze() { }
    Maze(glm::vec2 pos) { Position = pos; }
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
    void init();
    int checkInside(glm::vec2 point);
private:
    glm::vec4 get_corners(glm::vec2 position, float rotate, glm::vec2 size, int room);
    // initialize level from tile data
    // void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

class PowerUp
{
public:

    std::vector<GameObject> Walls;
    glm::vec2   Position;
    glm::vec2   topLeft, bottomRight;
    glm::vec2   mazeSize;
    glm::vec2   exitPos, powerUp1Pos, powerUp2Pos;
    int         ObjectType; // 0 exit, 1 , 2
    bool        active;

    std::vector<std::vector<int>> adjList;
    std::vector<std::vector<std::pair<int,float>>> adjListWeighted;
    std::vector<std::pair<float,float>> vertexPositions;
    std::vector<std::pair<int,int>> edges;
    glm::vec2   playerInitPos;
    std::vector<glm::vec2> roomCoords, pathCoords;

    // constructor
    PowerUp() { }
    PowerUp(glm::vec2 pos, int type) { Position = pos; ObjectType = type;}
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
    void init();
    int checkInside(glm::vec2 point);
private:
    glm::vec4 get_corners(glm::vec2 position, float rotate, glm::vec2 size, int room);
    // initialize level from tile data
    // void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

class GameCharacter
{
public:

    std::vector<GameObject> Walls;
    glm::vec2   Position;
    glm::vec2   topLeft, bottomRight;
    glm::vec2   mazeSize;
    glm::vec2   exitPos, powerUp1Pos, powerUp2Pos;
    int         ObjectType; // 0 exit, 1 , 2
    bool        active;

    std::vector<std::vector<int>> adjList;
    std::vector<std::vector<std::pair<int,float>>> adjListWeighted;
    std::vector<std::pair<float,float>> vertexPositions;
    std::vector<std::pair<int,int>> edges;
    glm::vec2   playerInitPos;
    std::vector<glm::vec2> roomCoords, pathCoords;

    // constructor
    GameCharacter() { }
    GameCharacter(glm::vec2 pos, int type) { Position = pos; ObjectType = type;}
    // loads level from file
    void Load(const char *file, unsigned int levelWidth, unsigned int levelHeight);
    // render level
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
    void init();
    int checkInside(glm::vec2 point);
private:
    glm::vec4 get_corners(glm::vec2 position, float rotate, glm::vec2 size, int room);
    // initialize level from tile data
    // void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif