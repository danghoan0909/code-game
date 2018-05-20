#ifndef draw_h_
#define draw_h_
const int SCREEN_WIDTH  = 670;
const int SCREEN_HEIGHT = 670;
const int startX = 34;
const int startY = 34;
const int pieceLength = 30;
const int numColor = 6;
const std::string color[numColor] = {"color1.png","color2.png","color3.png","color4.png","color5.png","color6.png"};
void init();
void loadMedia(int ob, int x,int y);
void render();
void startGame();
void drawEnd();
void close();
#endif draw_h_h;

