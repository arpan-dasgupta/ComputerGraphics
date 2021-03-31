#include <algorithm>
#include <sstream>
#include <iostream>

#include "game.h"
#include "maze.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"
#include "power_up.h"
#include "text_renderer.h"


// Game-related State data
SpriteRenderer    *Renderer;
// GameObject        *Player;
GameCharacter         *Player, *Enemy;
Maze              *maze;
// PowerUp                 *powerups;
// BallObject        *Ball;
TextRenderer      *Text;
int               disbled_back_attract;
GameObject        *Background;


Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), KeysProcessed(), Width(width), Height(height), Level(0), Lives(3)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete maze;
    delete Enemy;
    delete Text;
    // SoundEngine->drop();
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("../source/sprite.vs", "../source/sprite.fs", nullptr, "sprite");

    // // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    ResourceManager::LoadTexture("../assets/textures/block.png", false, "block");
    ResourceManager::LoadTexture("../assets/textures/grey.jpg", false, "grey");
    ResourceManager::LoadTexture("../assets/textures/among_us_0.png", true, "player_1");
    ResourceManager::LoadTexture("../assets/textures/among_us_1.png", true, "player_2");
    ResourceManager::LoadTexture("../assets/textures/among_us_2.png", true, "player_3");
    ResourceManager::LoadTexture("../assets/textures/among_us_3.png", true, "player_4");
    ResourceManager::LoadTexture("../assets/textures/among_us_4.png", true, "player_5");
    ResourceManager::LoadTexture("../assets/textures/ghost.png", true, "enemy");
    ResourceManager::LoadTexture("../assets/textures/coin3.png", true, "coin");
    ResourceManager::LoadTexture("../assets/textures/star2.png", true, "star");
    ResourceManager::LoadTexture("../assets/textures/door.png", true, "door");
    ResourceManager::LoadTexture("../assets/textures/button4.png", true, "button");
    ResourceManager::LoadTexture("../assets/textures/danger2.png", true, "danger");
    ResourceManager::LoadTexture("../assets/textures/space.jpg", true, "space");

    // // set render-specific controls
    Shader sh = Shader(ResourceManager::GetShader("sprite"));
    Renderer = new SpriteRenderer(sh);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../assets/fonts/OCRAEXT.TTF", 24);

    Background = new GameObject(glm::vec2(0.0,0.0), glm::vec2(1200,1200), ResourceManager::GetTexture("space"));

    // // load maze
    glm::vec2 mazePos = glm::vec2(CENTER.x, CENTER.y);
    maze =  new Maze(mazePos);
    maze->init();
    MazeInitPos = maze->Position;
    maze->floydWarshall();

    // configure game objects
    glm::vec2 playerPos = glm::vec2(CENTER.x, CENTER.y);
    // Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player_1"));
    Player = new GameCharacter(playerPos, 0);
    Player->init();
    // exit(0);
    // std::cout<<powerups.size()<<'\n';

    glm::vec2 exitPos = maze->exitPos, but1 = maze->powerUp1Pos, but2 = maze->powerUp2Pos;
    PowerUp *p1 = new PowerUp(exitPos, 0);
    PowerUp *p2 = new PowerUp(but1, 1);
    PowerUp *p3 = new PowerUp(but2, 2);
    p1->init();
    p2->init();
    p3->init();
    powerups.push_back(p1);
    powerups.push_back(p2);
    powerups.push_back(p3);
    // std::cout<<p1->Walls[0].Position.x<<" "<<p1->Walls[0].Position.y<<" "<<p1->Walls[0].Offset.x<<" "<<p1->Walls[0].Offset.y<<" "<<p1->Walls[0].Size.x<<" "<<p1->Walls[0].Size.y<<'\n';
    // std::cout<<exitPos.x<<" "<<exitPos.y<<'\n';

    Enemy = new GameCharacter(exitPos, 1);
    Enemy->init();
    
    tasks = 0;
    this->Health = 100;
    this->Score = 0;
    this->Lights = 1;
    this->Time = 3000; 
}


bool CheckCollision(GameObject one, GameObject two) // AABB - AABB collision
{
    // std::cout<<one.Position.x<<" "<<one.Position.y<<" "<<one.Offset.x<<" "<<one.Offset.y<<" "<<one.Size.x<<" "<<one.Size.y<<'\n';
    // std::cout<<two.Position.x<<" "<<two.Position.y<<" "<<two.Offset.x<<" "<<two.Offset.y<<" "<<two.Size.x<<" "<<two.Size.y<<'\n';
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x + one.Offset.x >= two.Position.x + two.Offset.x &&
        two.Position.x + two.Size.x + two.Offset.x >= one.Position.x + one.Offset.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y + one.Offset.y >= two.Position.y + two.Offset.y &&
        two.Position.y + two.Size.y + two.Offset.y >= one.Position.y + one.Offset.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

bool Game::CheckIfConnected(glm::vec2 a, glm::vec2 b)
{
    for(float lambda = 0;lambda<=1.0;lambda+=0.002)
    {
        glm::vec2 new_point = glm::vec2(a.x*lambda + b.x*(1-lambda),a.y*lambda + b.y*(1-lambda));
        if(maze->checkInside(new_point-maze->Position)==0)
            return false;
        // maze->Walls.push_back(GameObject(new_point-this->MazeInitPos, glm::vec2(5.0,5.0), ResourceManager::GetTexture("block")));
    }
    return true;
}

void Game::Update(float dt)
{
    if(this->State != GAME_ACTIVE)
        return;
    int flagg = 0;
    std::vector<int> todel;
    int cnt=0;
    for(auto pp : powerups)
    {
        // if(CheckCollision(*Player,pp->Walls[0]))
        // std::cout<<pp->Walls[0].Position.x<<" "<<pp->Walls[0].Position.y<<" "<<pp->Walls[0].Offset.x<<" "<<pp->Walls[0].Offset.y<<" "<<pp->Walls[0].Size.x<<" "<<pp->Walls[0].Size.y<<'\n';
        if(CheckCollision(Player->Walls[0],pp->Walls[0]))
        {
        // exit(0);
        // std::cout<<"OK\n";
            if(tasks==2 && pp->ObjectType==0)
            {
                this->State = GAME_WIN;
                return;
            }
            if(pp->ObjectType==1)
            {
                Enemy->Position = maze->Position + maze->powerUp2Pos - this->MazeInitPos;
                // std::cout<<Enemy->Position.x<<" "<<Enemy->Position.y<<'\n';
                // std::cout<<maze->Position.x<<" "<<maze->Position.y<<'\n';
                // std::cout<<maze->powerUp1Pos.x<<" "<<maze->powerUp1Pos.y<<'\n';
                // std::cout<<CENTER.x<<" "<<CENTER.y<<'\n';
                // std::cout<<powerups[2]->Position.x<<" "<<powerups[2]->Position.y<<'\n';
                tasks++;
            }
            if(pp->ObjectType==2)
            {
                flagg=1;
                tasks++;
            }
            if(pp->ObjectType>2)
            {
                this->Score += (pp->ObjectType==3)?50:-50;
                if(pp->ObjectType==4)  
                    this->Health -= 50;
            }
            if(pp->ObjectType!=0)
                todel.push_back(cnt);
        }
        cnt++;
    }
    // std::cout<<"OKK\n";
    for(int i=0;i<todel.size();i++)
    {
        // std::cout<<todel[i]<<'\n';
        int pp = todel[i];
        delete(powerups[pp]);
        powerups.erase(powerups.begin()+pp);
    }
    // std::cout<<"OKKK\n";
    if(flagg==1)
    {
        glm::vec2 pos1 = glm::vec2(maze->availableRooms[0].first + maze->Position.x,maze->availableRooms[0].second + maze->Position.y);
        glm::vec2 pos2 = glm::vec2(maze->availableRooms[1].first + maze->Position.x,maze->availableRooms[1].second + maze->Position.y);
        glm::vec2 pos3 = glm::vec2(maze->availableRooms[2].first + maze->Position.x,maze->availableRooms[2].second + maze->Position.y);
        glm::vec2 pos4 = glm::vec2(maze->availableRooms[3].first + maze->Position.x,maze->availableRooms[3].second + maze->Position.y);
        PowerUp *p1 = new PowerUp(pos1, 3);
        PowerUp *p2 = new PowerUp(pos2, 3);
        PowerUp *p3 = new PowerUp(pos3, 4);
        PowerUp *p4 = new PowerUp(pos4, 4);
        p1->init();
        p2->init();
        p3->init();
        p4->init();
        powerups.push_back(p1);
        powerups.push_back(p2);
        powerups.push_back(p3);
        powerups.push_back(p4);
    }
    this->Time--;
    if(this->Health<=0)
    {
        this->State = GAME_LOSS;
        return;
    }
    if(this->Time<=0)
    {
        this->State = GAME_LOSS;
        return;
    }
    if(CheckCollision(Player->Walls[0], Enemy->Walls[0]))
    {
        this->State = GAME_LOSS;
        return;
    }
    Player->moved = std::max(Player->moved-1,0);
    if(Player->moved==0)
        Player->active = false;

    // std::cout<<Enemy->Position.x<<" "<<Enemy->Position.y<<'\n';
    // std::cout<<Player->Position.x<<" "<<Player->Position.y<<'\n';
    // std::cout<<'\n';

    disbled_back_attract--;
    if(this->CheckIfConnected(Player->Position,Enemy->Position))
    {
        // std::cout<<"Okay";
        float lambda = ENEMY_VELOCITY;
        float dist = glm::length(Player->Position- Enemy->Position);
        glm::vec2 new_point = glm::vec2((Player->Position.x*lambda + Enemy->Position.x*(dist-lambda))/dist,(Player->Position.y*lambda + Enemy->Position.y*(dist-lambda))/dist);
        Enemy->Position = new_point;
    }
    else
    {
        std::vector<int> dest_closest;
        for(int i=0;i<maze->allCoords.size();i++)
        {
            if(CheckIfConnected(maze->allCoords[i]+maze->Position,Player->Position))
            {
                dest_closest.push_back(i);
                // std::cout<<maze->allCoords[i].x+maze->Position.x<<" "<<maze->allCoords[i].y+maze->Position.y<<'\n';
                // std::cout<<Player->Position.x<<" "<<Player->Position.y<<'\n';
                // maze->Walls.push_back(GameObject(maze->allCoords[i], glm::vec2(5.0,5.0), ResourceManager::GetTexture("block")));
            }
            // std::cout<<"PPPPPPPPPPPPPPPPPPPPP\n";
        }
        // std::cout<<dest_closest.size()<<'\n';
        int frag=0;
        for(int i=0;i<maze->allCoords.size();i++)
        {
            if(glm::length(Enemy->Position-maze->allCoords[i]-maze->Position)<=3.0 && disbled_back_attract<=0)
            {
                int next_min=100000, next_dest=-1, tt=0;
                for(auto a: dest_closest)
                {
                    if(glm::length(Player->Position-maze->allCoords[a]-maze->Position)+maze->fw_distances[i][a].first<next_min)
                    {
                        next_min = glm::length(Player->Position-maze->allCoords[a]-maze->Position)+maze->fw_distances[i][a].first;
                        next_dest = maze->fw_distances[i][a].second;
                        tt=a;
                    }
                }
                float lambda = ENEMY_VELOCITY;
                float dist = glm::length(maze->allCoords[next_dest]+maze->Position-Enemy->Position);
                float a = (maze->allCoords[next_dest].x + maze->Position.x)*lambda + Enemy->Position.x*(dist-lambda);
                float b = (maze->allCoords[next_dest].y + maze->Position.y)*lambda + Enemy->Position.y*(dist-lambda);
                glm::vec2 new_point = glm::vec2(a/dist,b/dist);
                // exit(0);
                // std::cout<<maze->allCoords[tt].x+maze->Position.x<<" "<<maze->allCoords[tt].y+maze->Position.y<<"\n";
                // std::cout<<new_point.x<<" "<<new_point.y<<"\n";
                // std::cout<<next_dest<<'\n';
                // std::cout<<maze->allCoords[next_dest].x+maze->Position.x<<" "<<maze->allCoords[next_dest].y+maze->Position.y<<"\n";
                // std::cout<<Enemy->Position.x<<" "<<Enemy->Position.y<<"\n";
                // std::cout<<"++++++++++++++++++++++++++++++++++\n";
                Enemy->Position = new_point;
                frag=1;
                disbled_back_attract=10;
            }
        }
        if(frag==0)
        {
            int next_min=10000000, next_dest=0, tt=0;
            for(int i=0;i<maze->allCoords.size();i++)
            {
                if(CheckIfConnected(maze->allCoords[i]+maze->Position,Enemy->Position))
                {
                    // std::cout<<maze->allCoords[i].x+maze->Position.x<<" "<<maze->allCoords[i].y+maze->Position.y<<" oooo \n";
                    // maze->Walls.push_back(GameObject(maze->allCoords[i], glm::vec2(5.0,5.0), ResourceManager::GetTexture("block")));
                    for(auto a: dest_closest)
                    {
                        if(glm::length(Player->Position-maze->allCoords[a]-maze->Position)+maze->fw_distances[i][a].first+glm::length(Enemy->Position-maze->allCoords[i]-maze->Position)<next_min)
                        {
                            next_min = glm::length(Player->Position-maze->allCoords[a]-maze->Position)+maze->fw_distances[i][a].first+glm::length(Enemy->Position-maze->allCoords[i]-maze->Position);
                            next_dest = i;
                            tt=a;
                        }
                    }
                }
            }
            float lambda = ENEMY_VELOCITY;
            float dist = glm::length(maze->allCoords[next_dest]+maze->Position-Enemy->Position);
            float a = (maze->allCoords[next_dest].x + maze->Position.x)*lambda + Enemy->Position.x*(dist-lambda);
            float b = (maze->allCoords[next_dest].y + maze->Position.y)*lambda + Enemy->Position.y*(dist-lambda);
            glm::vec2 new_point = glm::vec2(a/dist,b/dist);
            // std::cout<<maze->allCoords[tt].x+maze->Position.x<<" "<<maze->allCoords[tt].y+maze->Position.y<<"\n";
            // std::cout<<next_dest<<'\n';
            // std::cout<<maze->allCoords[next_dest].x+maze->Position.x<<" "<<maze->allCoords[next_dest].y+maze->Position.y<<"\n";
            // std::cout<<Enemy->Position.x<<" "<<Enemy->Position.y<<"\n";
            // std::cout<<maze->checkInside(maze->allCoords[next_dest])<<'\n';
            // std::cout<<CheckIfConnected(maze->allCoords[next_dest]+maze->Position,Enemy->Position)<<'\n';
            // std::cout<<maze->checkInside(new_point-maze->Position)<<'\n';
            // std::cout<<lambda/dist<<'\n';
            // std::cout<<"==========================\n";
            Enemy->Position = new_point;
        }
    }
}


void Game::ProcessInput(float dt)
{
    // if (this->State == GAME_MENU)
    // {
    //     if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
    //     {
    //         this->State = GAME_ACTIVE;
    //         this->KeysProcessed[GLFW_KEY_ENTER] = true;
    //     }
    //     if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
    //     {
    //         this->Level = (this->Level + 1) % 4;
    //         this->KeysProcessed[GLFW_KEY_W] = true;
    //     }
    //     if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
    //     {
    //         if (this->Level > 0)
    //             --this->Level;
    //         else
    //             this->Level = 3;
    //         //this->Level = (this->Level - 1) % 4;
    //         this->KeysProcessed[GLFW_KEY_S] = true;
    //     }
    // }
    if (this->State == GAME_WIN || this->State == GAME_LOSS)
    {
        if (this->Keys[GLFW_KEY_ENTER])
        {
            exit(0);
        }
    }
    if (this->State == GAME_ACTIVE)
    {
        // std::cout<<"OK " << this->State<<" " <<GAME_ACTIVE<<" ";
        glm::vec2 curmaze = maze->Position;
        float velocity = MAZE_VELOCITY * dt;
        bool didMove=0;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            didMove = 1;
            if (maze->Position.x - CENTER.x<= maze->mazeSize.x && maze->checkInside(Player->Position+glm::vec2(-velocity,0.0) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(-velocity,PLAYER_SIZE.y) - maze->Position))
            {
                maze->Position.x += velocity;
            }
            Player->flipped = true;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            didMove = 1;
            if (maze->Position.x - CENTER.x>= -maze->mazeSize.x && maze->checkInside(Player->Position+glm::vec2(velocity + PLAYER_SIZE.x,PLAYER_SIZE.y) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(velocity + PLAYER_SIZE.x,0.0) - maze->Position))
            {
                maze->Position.x -= velocity;
            }
            Player->flipped = false;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            didMove = 1;
            if (maze->Position.y - CENTER.y >= -maze->mazeSize.y && maze->checkInside(Player->Position+glm::vec2(PLAYER_SIZE.x,velocity+PLAYER_SIZE.y) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(0.0,PLAYER_SIZE.y + velocity) - maze->Position))
            {
                maze->Position.y -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_W])
        {
            didMove = 1;
            if (maze->Position.y - CENTER.y <= maze->mazeSize.y && maze->checkInside(Player->Position+glm::vec2(0.0, -velocity) - maze->Position) && maze->checkInside(Player->Position+glm::vec2(PLAYER_SIZE.x,-velocity) - maze->Position))
            {
                maze->Position.y += velocity;
            }
        }
        if(didMove)
        {
            Player->moved = 15;
            Player->active = true;
        }
        // if (this->Keys[GLFW_KEY_SPACE])
        //     Ball->Stuck = false;
        glm::vec2 new_maze = maze->Position;
        for(auto pp: powerups)
        {
            pp->Position += new_maze - curmaze;
        }
        Enemy->Position += new_maze - curmaze;
    }
}

void Game::Render()
{
    Background->Draw(*Renderer);
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU)
    {
        maze->Draw(*Renderer);   

        for(auto aa: powerups)
        {
            aa->Draw(*Renderer);
        }

        Player->Draw(*Renderer);
        Enemy->Draw(*Renderer);

        std::stringstream ss1, ss2, ss3, ss4;
        ss1 << this->Health;
        Text->RenderText("Health: " + ss1.str(), 20.0f, 20.0f, 1.0f);
        ss2 << this->Score;
        Text->RenderText("Score: " + ss2.str(), 20.0f, 50.0f, 1.0f);
        ss3 << (this->Lights)?"On":"Off";
        Text->RenderText((this->Lights)?"Lights: On":"Lights: Off", 20.0f, 80.0f, 1.0f);
        ss4 << (this->Time)/100;
        Text->RenderText("Time: " + ss4.str(), 20.0f, 110.0f, 1.0f);
    }
    // if (this->State == GAME_MENU)
    // {
    //     Text->RenderText("Press ENTER to start", 250.0f, this->Height / 2.0f, 1.0f);
    //     Text->RenderText("Press W or S to select level", 245.0f, this->Height / 2.0f + 20.0f, 0.75f);
    // }
    if (this->State == GAME_WIN)
    {
        std::stringstream ss;
        ss << this->Score;
        Text->RenderText("You WON!!!", 320.0f, this->Height / 2.0f - 30.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Score - " + ss.str(), 300.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
        Text->RenderText("Press ESC to quit", 250.0f, this->Height / 2.0f + 30.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
    if (this->State == GAME_LOSS)
    {
        Text->RenderText("You LOST!!!", 320.0f, this->Height / 2.0f - 30.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Press ESC to quit", 250.0f, this->Height / 2.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    }
}