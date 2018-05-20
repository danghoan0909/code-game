
#include<bits/stdc++.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "game.h"
#include "draw.h"
using namespace std;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* surface[12];
SDL_Texture*  texture[12];
TTF_Font *font = NULL;
SDL_Color textColor = {255, 255, 255};

Mix_Music *music = NULL;

void init()
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) >= 0 )
	{
		window = SDL_CreateWindow( "TETRIS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window != NULL )
		{
			renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
			if( renderer != NULL )
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		}
		Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 );
	}
	for(int i = 0; i<numColor; i++)
    {
        surface[i] = IMG_Load( color[i].c_str());
        texture[i] = SDL_CreateTextureFromSurface( renderer, surface[i] );
    }
    surface[6] = IMG_Load("background.png");
    texture[6] = SDL_CreateTextureFromSurface( renderer, surface[6] );
    surface[7] = IMG_Load("start1.png");
    texture[7] = SDL_CreateTextureFromSurface( renderer, surface[7] );
    surface[8] = IMG_Load("end.png");
    texture[8] = SDL_CreateTextureFromSurface( renderer, surface[8] );
    TTF_Init();
    font = TTF_OpenFont("superstar.ttf",50);
     music = Mix_LoadMUS( "music.mp3" );

}

void loadMedia(int ob, int x,int y)
{
    SDL_Rect dest = {x, y,surface[ob]->w,surface[ob]->h};
    SDL_RenderCopy(renderer, texture[ob], NULL, &dest);
}
void startGame()
{
    loadMedia(7,0,0);

    SDL_RenderPresent( renderer );
}
void drawEnd()
{
    loadMedia(8,startX-1,150);

    SDL_RenderPresent( renderer );
}
void printText(string text,int x, int y)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(),textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_Rect dest;
    dest.x = x+(25*(10-text.size())/2);
    dest.y = y;
    dest.w = 25*text.size();
    dest.h = 50;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_DestroyTexture(texture);
 	SDL_FreeSurface(surface);
}
void Game::printScore()
{
    char buffer[10];
    itoa(score,buffer,10);
    string text = string(buffer);
    printText(text, 375 , 235);
}

void Game::drawPiece(matrix p,int pX, int pY)
{
    for(int i = 0;i<p.c; i++)
        for(int j = 0; j<p.r; j++)
        if(p.arr[j][i] != -1)
        {
            loadMedia(p.arr[j][i],pX+i*pieceLength, pY+j*pieceLength);
        }
}
void Game::drawBoard()
{
    for(int i = 0;i<numX; i++)
        for(int j = 0; j<numY; j++)
        if(board[j][i] != -1 && (!checkClear || canClear[j]))
        {
            loadMedia(board[j][i],startX+i*pieceLength, startY+j*pieceLength);
        }
}
void Game::drawGame()
{
     if( Mix_PlayingMusic() == 0 )
    {
        Mix_PlayMusic( music, -1 );
    }
    bool check = false;
    loadMedia(6,0,0);
    printScore();
    drawPiece(currPiece,posX,posY);
    drawPiece(nextPiece,500,50);
    drawBoard();
    SDL_RenderPresent( renderer );
}

void close()
{
	for(int i = 0; i<12;i++)
        SDL_DestroyTexture(texture[i]);

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;
	 Mix_FreeMusic( music );
    TTF_CloseFont( font );
    Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();
}




