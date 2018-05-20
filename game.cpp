#include<bits/stdc++.h>
#include<windows.h>
#include <SDL.h>
#include "game.h"
#include "draw.h"
using namespace std;
void Game:: build()
{
    memset(pieces,-1,sizeof(pieces));
    pieces[0].r = 1;
    pieces[0].c = 4;
    for(int i = 0;i<4;i++)
        pieces[0].arr[0][i] = 1;

    pieces[1].r = 2;
    pieces[1].c = 2;
    for(int i = 0;i<2;i++)
        for(int j = 0; j<2;j++)
        pieces[1].arr[i][j] = 1;

    pieces[2].r = 2;
    pieces[2].c = 3;
    for(int i = 0;i<2;i++)
        for(int j = 0; j<3;j++)
        pieces[2].arr[i][j] = 1;
    pieces[2].arr[0][1] = -1;
    pieces[2].arr[0][2] = -1;
    pieces[3].r = 2;
    pieces[3].c = 3;
    for(int i = 0;i<2;i++)
        for(int j = 0; j<3;j++)
        pieces[3].arr[i][j] = 1;
    pieces[3].arr[1][0] = -1;
    pieces[3].arr[0][2] = -1;
    pieces[4].r = 2;
    pieces[4].c = 3;
    for(int i = 0;i<2;i++)
        for(int j = 0; j<3;j++)
        pieces[4].arr[i][j] = 1;
    pieces[4].arr[0][0] = -1;
    pieces[4].arr[0][2] = -1;

    memset(board,-1,sizeof(board));

}
matrix Game::newPiece()
{
    int newType = rand()% numType;
    int newColor = rand()% numColor;
    matrix newMatrix;
    newMatrix.r = pieces[newType].r;
     newMatrix.c = pieces[newType].c;
    for(int i = 0; i<newMatrix.r;i++)
        for(int j = 0; j<newMatrix.c;j++)
            if (pieces[newType].arr[i][j] == 1) newMatrix.arr[i][j] = newColor;
            else newMatrix.arr[i][j] = -1;
    return newMatrix;
}
matrix rotation(matrix p)
{
    matrix newMatrix;
    newMatrix.r = p.c;
     newMatrix.c = p.r;
    for(int i = 0; i<p.c;i++)
    {
        int k = p.r-1;
        for(int j = 0; j<p.r;j++)
        {
            newMatrix.arr[i][j] = p.arr[k][i];
            k--;
        }
    }
    return newMatrix;
}
int Game::target()
{
    bool ok = true;
    int x = (posY-startY)/pieceLength;
    int y = (posX-startX)/pieceLength;
    int xx;
    for( xx = x; xx<numY; xx++)
    {
            for(int i = 0; i<currPiece.r; i++)
            for(int j = 0; j<currPiece.c; j++)
                if(currPiece.arr[i][j]!=-1 && board[xx+i][y+j] !=-1)
                ok = false;
            if (!ok) break;
                else x=xx;
    }
    if (ok) return numY-currPiece.r;
    return x;
}
void Game:: updatePiece()
{
    currPiece = nextPiece;
    nextPiece = newPiece();
    posY = startY;
    posX = startX + ((numX-(currPiece.c))/2)*pieceLength;
    currTarget = target();
    if (currTarget == 0) endGame = true;
}
void Game:: update()
{
    int x = (posY-startY)/pieceLength;
    int y = (posX-startX)/pieceLength;
    currTarget = target();
    if (x == currTarget)
    {
            for(int i = 0;i<currPiece.r;i++)
            for(int j = 0; j<currPiece.c; j++)
                    if(board[x+i][y+j] == -1) board[x+i][y+j] = currPiece.arr[i][j];
            updatePiece();
            checkClear  = false;
            memset(canClear, false, sizeof(canClear));
            for(int i = 0; i<numY; i++)
            {
                for(int j = 0; j<numX; j++)
                    canClear[i] = canClear[i] || (board[i][j]==-1);
                if (!canClear[i]) checkClear = true;
            }
            if (checkClear)
            {
                for(int i = 0; i<5; i++)
                {
                    checkClear = !checkClear;
                    drawGame();
                    Sleep(40);
                }
                int count = 0;
                for(int i = numY-1; i>=0; i--)
                    if(!canClear[i])
                    {
                        score +=10;
                        for(int ii = i+count; ii>0; ii--)
                            for(int j = 0; j<numX; j++)
                            board[ii][j] = board[ii-1][j];
                         for(int j = 0; j<numX; j++) board[0][j] = -1;
                         count ++;
                    }
            }
    }

}
bool Game::fail()
{
    int x = (posY-startY)/pieceLength;
    int y = (posX-startX)/pieceLength+((posX-startX)%pieceLength!=0);
    for(int i = 0;i<currPiece.r;i++)
    for(int j = 0; j<currPiece.c; j++)
        if(board[x+i][y+j] != -1 && currPiece.arr[i][j] !=1 )return true;
    return false;

}
int getLevel(int mouseX, int mouseY)
{
    if(mouseX>=251 && mouseX<=399 && mouseY>=111 && mouseY<=174) return 1;
    if(mouseX>=251 && mouseX<=399 && mouseY>=231 && mouseY<=292) return 2;
      if(mouseX>=251 && mouseX<=399 && mouseY>=351 && mouseY<=411) return 3;
       if(mouseX>=251 && mouseX<=399 && mouseY>=473 && mouseY<=530) return 4;
       return -1;
}
void Game::run()
{
    int mouseX, mouseY;
    SDL_Event e;
    init();
    startGame();
    srand(time(NULL));
    const int FPS = 30;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;
    bool quit = false;
    while( !quit )
    {
        frameStart = SDL_GetTicks();

		while( SDL_PollEvent( &e ) != 0 )
		{
		   // cout<<mouseX<<" "<<mouseY<<endl;
			if( e.type == SDL_QUIT ) quit = true;
            const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
            if (played || e.type == SDL_MOUSEBUTTONDOWN )
            {
                int d = getLevel(mouseX, mouseY);
                if(d!=-1)
                {

                if (!played)
                {
                    speed = d*2;
                    nextPiece = newPiece();
                    updatePiece();
                    played = true;

                }
                if( currentKeyStates[ SDL_SCANCODE_UP ] )
                {
                   currPiece = rotation(currPiece);
                   if(fail()) currPiece = rotation(currPiece);
                }
                else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
                {
                    posY += pieceLength;

                }
                else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
                {
                    posX -= pieceLength;
                }
                else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
                {
                    posX += pieceLength;
                }
                if (posX<startX) posX = startX;
                if (posX+currPiece.c*pieceLength>startX+(numX)*pieceLength) posX -=pieceLength ;
                if (posY+currPiece.r*pieceLength>startY+currTarget*pieceLength) posY = startX+currTarget*pieceLength;
            }
            }
            }
            frameTime = SDL_GetTicks() - frameStart;
            if (frameDelay > frameTime)
            {
                SDL_Delay(frameDelay - frameTime);
            }
            if(played)
            {
               if(!endGame)
               {
                   posY+=speed;
                    update();
                    drawGame();
               }
               else drawEnd();
            }

            if( e.type == SDL_MOUSEMOTION){
                mouseX = e.button.x;
                mouseY = e.button.y;
			}


    }

	close();

}

