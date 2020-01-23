#include "raylib.h"
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <stdbool.h>

//structures

typedef struct strukt1 Punkt;
struct strukt1
{
    int x;
    int y;
};

// const variables

const int screenWidth = 750;
const int screenHeight = 750;
const int blockSize = 50;
const int playerSize = 30;
const int speed = 3;
const int bombTime = 150;
const int explosionTime = 60;

// other variables

Punkt player1Position = {60,60};
Punkt player2Position = {660,660};
int blockType[16][16]; // 0 - puste | 1 - stały blok | 2 - blok do rozwalenia | 3 - bomba | 4 - wybuch | 5 - 1/2 wybuchu XD
int bombTimer[16][16];
int whoseBomb[16][16];
int explosionTimer[16][16];
int player1Bombs=5;
int player2Bombs=5;
int rayRange1=2;
int rayRange2=2;
bool EndOfGame = false;

Vector2 pair_double(Punkt p)
{
    Vector2 c={p.x,p.y};
    return c;
}

Punkt pair(int a,int b)
{
    Punkt c={a,b};
    return c;
}

void initBlocks()
{
    for(int x=0;x<15;x++)
        for(int y=0;y<15;y++)
        {
           if(x==0 || x==14 || y==0 || y==14 || (x%2==0 && y%2==0))
               blockType[x][y]=1;
           else if((x+y>4 && x+y<24) && GetRandomValue(0,1)==1)
               blockType[x][y]=2;
        }
           
     
}

Punkt findNearest(Punkt p)
{
    int mini=1e9,x1=0,y1=0;
    for(int x=0;x<15;x++)
    {
        if(abs(p.x-blockSize/2-x*blockSize)<mini)
        {
            x1=x;
            mini=abs(p.x-blockSize/2-x*blockSize);
        }
    }
    mini=1e9;
    for(int y=0;y<15;y++)
    {
        if(abs(p.y-blockSize/2-y*blockSize)<mini)
        {
            y1=y;
            mini=abs(p.y-blockSize/2-y*blockSize);
        }
    }
    return pair(x1,y1);
}

bool inBlock(Punkt point)
{
    Punkt position=findNearest(point);
    if(point.x%50==0 || point.y%50==0)
        return false;
    if(blockType[position.x][position.y]==0 || blockType[position.x][position.y]==3 || blockType[position.x][position.y]==4)
        return false;
    return true;
}

bool inBomb(Punkt point)
{
    Punkt position=findNearest(point);
    if(point.x%50==0 || point.y%50==0)
        return false;
    if(blockType[position.x][position.y]==3)
        return true;
    return false;
}

bool inExplosion(Punkt point)
{
    Punkt position=findNearest(point);
    if(point.x%50==0 || point.y%50==0)
        return false;
    if(blockType[position.x][position.y]==4)
        return true;
    return false;
}
bool checkCollision(Punkt rect, Punkt change)
{
    Punkt rect2=pair(rect.x+change.x,rect.y+change.y);
    if(inBlock(pair(rect2.x + playerSize,rect2.y + playerSize)))
        return true;
    if(inBlock(pair(rect2.x + playerSize,rect2.y)))
        return true;
    if(inBlock(pair(rect2.x ,rect2.y + playerSize)))
        return true;
    if(inBlock(pair(rect2.x ,rect2.y)))
        return true;
    
    bool pom=false;
    
    if(inBomb(pair(rect.x + playerSize,rect.y + playerSize)))
        pom=true;
    if(inBomb(pair(rect.x + playerSize,rect.y)))
        pom=true;
    if(inBomb(pair(rect.x ,rect.y + playerSize)))
        pom=true;
    if(inBomb(pair(rect.x ,rect.y)))
        pom=true;
    
    if(pom==true)
        return false;
    
    if(inBomb(pair(rect2.x + playerSize,rect2.y + playerSize)))
        return true;
    if(inBomb(pair(rect2.x + playerSize,rect2.y)))
        return true;
    if(inBomb(pair(rect2.x ,rect2.y + playerSize)))
        return true;
    if(inBomb(pair(rect2.x ,rect2.y)))
        return true;
    return false;
   
}
void triggerExplosion(int x,int y,int ch_x,int ch_y,int cnt,int who)
{
    if(cnt==0 || blockType[x][y]==1)
        return ;
    if(blockType[x][y]==0 || blockType[x][y]==4)
    {
        blockType[x][y]=4;
        explosionTimer[x][y]=explosionTime;
        triggerExplosion(x+ch_x,y+ch_y,ch_x,ch_y,cnt-1,who);
        return;
    }
    if(blockType[x][y]==2 || blockType[x][y]==5)
    {
        blockType[x][y]=5;
        explosionTimer[x][y]=explosionTime;
        return;
    }
    if(blockType[x][y]==3)
        startExplosion(x,y,who);
}

void startExplosion(int x,int y,int who)
{
    if(who==1)
        player1Bombs++;
    else
        player2Bombs++;
    blockType[x][y]=4;
    explosionTimer[x][y]=explosionTime;
    if(who==1)
    {
        triggerExplosion(x-1,y,-1,0,rayRange1,who);
        triggerExplosion(x+1,y,1,0,rayRange1,who);
        triggerExplosion(x,y-1,0,-1,rayRange1,who);
        triggerExplosion(x,y+1,0,1,rayRange1,who);
    }
    else
    {
        triggerExplosion(x-1,y,-1,0,rayRange2,who);
        triggerExplosion(x+1,y,1,0,rayRange2,who);
        triggerExplosion(x,y-1,0,-1,rayRange2,who);
        triggerExplosion(x,y+1,0,1,rayRange2,who);
    }
}
void movePlayer1()
{
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_W) && !checkCollision( pair( player1Position.x , player1Position.y) , pair(0,-pom_speed) ) )
        {
            player1Position.y-=pom_speed;
            break;
        }            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_S) && !checkCollision( pair( player1Position.x , player1Position.y) , pair(0,pom_speed) ) ) 
        {
            player1Position.y+=pom_speed;
            break;
        }
            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_A) && !checkCollision( pair( player1Position.x , player1Position.y) , pair(-pom_speed,0) ) )
        {
            player1Position.x-=pom_speed;
            break;
        }            
            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_D) && !checkCollision( pair( player1Position.x , player1Position.y) , pair(pom_speed,0) ) ) 
        {
            player1Position.x+=pom_speed;
            break;
        }
    if(IsKeyPressed(KEY_SPACE) && player1Bombs > 0 )
    {
        Punkt point=findNearest(pair(player1Position.x+playerSize/2,player1Position.y+playerSize/2));
        if(blockType[point.x][point.y]==0)
        {
            blockType[point.x][point.y]=3;
            bombTimer[point.x][point.y]=bombTime;
            whoseBomb[point.x][point.y]=1;
            player1Bombs--;
        }
    }
}

void movePlayer2()
{
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_UP) && !checkCollision( pair( player2Position.x , player2Position.y) , pair(0,-pom_speed) ) )
        {
            player2Position.y-=pom_speed;
            break;
        }            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_DOWN) && !checkCollision( pair( player2Position.x , player2Position.y) , pair(0,pom_speed) ) ) 
        {
            player2Position.y+=pom_speed;
            break;
        }
            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_LEFT) && !checkCollision( pair( player2Position.x , player2Position.y) , pair(-pom_speed,0) ) )
        {
            player2Position.x-=pom_speed;
            break;
        }            
            
    for(int pom_speed=speed;pom_speed>0;pom_speed--)
        if ( IsKeyDown(KEY_RIGHT) && !checkCollision( pair( player2Position.x , player2Position.y) , pair(pom_speed,0) ) ) 
        {
            player2Position.x+=pom_speed;
            break;
        }
    if(IsKeyPressed(KEY_P) && player2Bombs > 0 )
    {
        Punkt point=findNearest(pair(player2Position.x+playerSize/2,player2Position.y+playerSize/2));
        if(blockType[point.x][point.y]==0)
        {
            blockType[point.x][point.y]=3;
            bombTimer[point.x][point.y]=bombTime;
            whoseBomb[point.x][point.y]=2;
            player2Bombs--;
        }
    }
}

void Rysuj()
{
    BeginDrawing();

    ClearBackground(GRAY);
    for(int x=0;x<15;x++)
        for(int y=0;y<15;y++)
        {
            if(blockType[x][y]==1)
                DrawRectangle(x*blockSize,y*blockSize,blockSize,blockSize,DARKGRAY);
            if(blockType[x][y]==2)
                DrawRectangle(x*blockSize,y*blockSize,blockSize,blockSize,LIGHTGRAY);
            if(blockType[x][y]==3)
                DrawCircle(x*blockSize+blockSize/2,y*blockSize+blockSize/2,blockSize/2-5,BLACK);
            if(blockType[x][y]==4)
                DrawCircle(x*blockSize+blockSize/2,y*blockSize+blockSize/2,blockSize/2-5,RED);
        }
    DrawRectangle(player1Position.x,player1Position.y,playerSize,playerSize,BLUE);
    
    DrawRectangle(player2Position.x,player2Position.y,playerSize,playerSize,RED);

    EndDrawing();
}

void change5to4()
{
    for(int x=0;x<15;x++)
        for(int y=0;y<15;y++)
            if(blockType[x][y]==5)
                blockType[x][y]=4;
}
void AfterMoveSimulation()
{
    for(int x=0;x<15;x++)
        for(int y=0;y<15;y++)
        {
            if(blockType[x][y]==3)
            {
                bombTimer[x][y]--;
                if(bombTimer[x][y]==0)
                    startExplosion(x,y,whoseBomb[x][y]);
            }
            if(blockType[x][y]==4)
            {
                explosionTimer[x][y]--;
                if(explosionTimer[x][y]==0)
                    blockType[x][y]=0;
            }
        }
    change5to4();
}

void DetectLose()
{
   Punkt pos1=findNearest(pair(player1Position.x+playerSize/2,player1Position.y+playerSize/2)),pos2=findNearest(pair(player2Position.x+playerSize/2,player2Position.y+playerSize/2));
   bool if1Lost=(blockType[pos1.x][pos1.y]==4),if2Lost=(blockType[pos2.x][pos2.y]==4);
   if(if1Lost || if2Lost)
       EndOfGame = true;
}

int main(void)
{
    srand(time(0));
    initBlocks();

    InitWindow(screenWidth, screenHeight, "Bomberman");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        if(!EndOfGame)
        {
            movePlayer1();
        
            movePlayer2();
        
            AfterMoveSimulation();
        }
        Rysuj();
        
        DetectLose();
        
    }
    CloseWindow();
    return 0;
}