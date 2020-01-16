#include "raylib.h"

// const variables

const int screenWidth = 750;
const int screenHeight = 750;
const int blockSize = 50;
const int NumberOfBlocks = 92;
const int playerSize = 30;
const int speed = 5;
Vector2 blockPositions[92];

// other variables

Vector2 player1Position = {60,60};
Vector2 player2Position = {660,660};

Vector2 pair(int a,int b)
{
    Vector2 c={a,b};
    return c;
}

void initBlocks()
{
    int pom=0;
    for(int x=0;x<15;x++)
        for(int y=0;y<15;y++)
           if(x==0 || x==14 || y==0 || y==14 || (x%2==0 && y%2==0))
           {
               blockPositions[pom]=pair(x*50,y*50);
               pom++;
           }
}

bool inBlock(Vector2 point)
{
    int wsp1=point.x,wsp2=point.y;
    if(wsp1%blockSize==0 || wsp2%blockSize==0)
        return false;
    wsp1/=blockSize;
    wsp2/=blockSize;
    if(wsp1==0 || wsp1==14 || wsp2==0 || wsp2==14 || (wsp1%2==0 && wsp2%2==0))
        return true;
    return false;
}
bool checkCollision(Vector2 rect)
{
    for(int x=0;x<NumberOfBlocks;x++)
    {
        if(inBlock(pair(rect.x + playerSize,rect.y + playerSize)))
            return true;
        if(inBlock(pair(rect.x + playerSize,rect.y)))
            return true;
        if(inBlock(pair(rect.x ,rect.y + playerSize)))
            return true;
        if(inBlock(pair(rect.x ,rect.y)))
            return true;
    }
    return false;

}

void movePlayer1()
{
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_W) && !checkCollision( pair( player1Position.x , player1Position.y-pom_speed) ) )
        {
            player1Position.y-=pom_speed;
            break;
        }            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_S) && !checkCollision( pair( player1Position.x , player1Position.y+pom_speed) ) ) 
        {
            player1Position.y+=pom_speed;
            break;
        }
            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_A) && !checkCollision( pair( player1Position.x-pom_speed , player1Position.y) ) )
        {
            player1Position.x-=pom_speed;
            break;
        }            
            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_D) && !checkCollision( pair( player1Position.x+pom_speed , player1Position.y) ) ) 
        {
            player1Position.x+=pom_speed;
            break;
        }
            
}

void movePlayer2()
{
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_UP) && !checkCollision( pair( player2Position.x , player2Position.y-pom_speed) ) )
        {
            player2Position.y-=pom_speed;
            break;
        }            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_DOWN) && !checkCollision( pair( player2Position.x , player2Position.y+pom_speed) ) ) 
        {
            player2Position.y+=pom_speed;
            break;
        }
            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_LEFT) && !checkCollision( pair( player2Position.x-pom_speed , player2Position.y) ) )
        {
            player2Position.x-=pom_speed;
            break;
        }            
            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_RIGHT) && !checkCollision( pair( player2Position.x+pom_speed , player2Position.y) ) ) 
        {
            player2Position.x+=pom_speed;
            break;
        }
}

void Rysuj()
{
    BeginDrawing();

    ClearBackground(GRAY);

    for(int x=0;x<NumberOfBlocks;x++)
        DrawRectangleV(blockPositions[x],pair(blockSize,blockSize),DARKGRAY);
    
    DrawRectangleV(player1Position,pair(playerSize,playerSize),BLUE);
    
    DrawRectangleV(player2Position,pair(playerSize,playerSize),RED);

    EndDrawing();
}
int main(void)
{
    initBlocks();

    InitWindow(screenWidth, screenHeight, "Bomberman");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        
        movePlayer1();
        
        movePlayer2();
        
        Rysuj();
        
    }
    CloseWindow();
    return 0;
}