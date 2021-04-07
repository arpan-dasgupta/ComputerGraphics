
#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "maze.h"
#include "power_up.h"
// #include "power_up.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOSS
};

// Represents the four possible (collision) directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

// Initial size of the player
const glm::vec2 PLAYER_SIZE(50.0f, 60.0f);
// Initial velocity of the maze
const float MAZE_VELOCITY(300.0f);
const float ENEMY_VELOCITY(2.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const glm::vec2 CENTER(400.0f, 400.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;	
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    unsigned int            Width, Height;
    std::vector<PowerUp*>   powerups;
    int                     tasks;
    glm::vec2               MazeInitPos;
    // Maze                    maze;
    // std::vector<PowerUp>    PowerUps;
    unsigned int            Level;
    unsigned int            Lives;
    int            Score, Health, Time;
    bool                    Lights;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    bool CheckIfConnected(glm::vec2 a, glm::vec2 b);
    // void DoCollisions();
    // // reset
    // void ResetLevel();
    // void ResetPlayer();
    // // powerups
    // void SpawnPowerUps(GameObject &block);
    // void UpdatePowerUps(float dt);
};

#endif