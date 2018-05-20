#ifndef game_h_
#define game_h_
typedef  struct
{
    int r,c;
    int arr[4][4];
} matrix;
const int numType = 5;
const int numX = 10;
const int numY = 20;


class Game {
    matrix currPiece, nextPiece, pieces[numType];
    int board[numY][numX];
    bool canClear[numY];
    bool checkClear  = false;
    int direct;
    int posX = 0;
    int posY = 0;
    int score = 0;
    int currTarget;
    int speed;
    bool played = false;
    bool restart;
    bool endGame;


public:
    void build();
    void initGame();
    void run();
private:
    void drawGame();
    matrix newPiece();
    void drawPiece(matrix p,int Px, int pY);
    int target();
    void updatePiece();
    void update();
    void drawBoard();
    bool fail();
    void printScore();
};
#endif  game_h_
