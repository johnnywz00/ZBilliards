
#ifndef BILLIARDS_H
#define BILLIARDS_H


#include "objects.hpp"

class FuseManager;
class SFGameWindow;

class State {
    
public:
    
    State () { }
    
    ~State () { }
    
    float xLoc (int idx);
    
    float yLoc (int ofs, bool yl=false);
    
    void onCreate ();
    
    void reset ();
    
    void onMouseDown (int x, int y);
    
    void onMouseUp (int x, int y) { }
    
    void onKeyPress (Keyboard::Key) ;

    void onKeyRelease (Keyboard::Key) { }

    void win (Player& p);
    
    void lose (Player& p);
    
    void setToCueBall ();
    
    void moveCueBall (float x, float y);
    
    void move8Ball (float x, float y);
    
    void updateGuide ();
    
    void animateArrow ();
    
    void launch ();
    
    void pocketBall (Ball& b, Pocket& p);
    
    void spotCueBall ();
    
    void respot (Ball&);
    
    void frictionAndFindHighSpeed (Ball& cur, float& highSpd);
    
    void update (const Time&);
    
    void draw ();

    RenderWindow*       w;
    SFGameWindow*       gw;
    FuseManager*        fuseMgr;
    Font                font[2];
    Texture             txNW, txNE, txSE, txSW,
                        txPocket,
                        txTableCorner,
                        txCueBall,
                        txBall,
                        txArrow;
    Text                txt,
                        instrTxt,
                        playerTxt;
    VertexArray         guideline { Lines };
    RectangleShape      tableSurface,
                        tableEdge;
    RectangleShape      powerBar[2];
    CircleShape         eightBallInd;
    Sprite              calledPocketInd;
    vector<Sprite>      tableCorners;
    Color               ballColors[2];
    
    Cue                 cue;
    Ball                cueBall;
    vector<Ball>        balls;
    vector<Pocket>      pockets;
    Player*             curPlayer;
    Player*             otherPlayer;
    Player              players[2];

    SoundBuffer         buffers[4];
    Sound               sounds[4];
    
    static constexpr int        ballRad = 15;
    static constexpr float      yOff = ballRad * 2;
    const float                 xOff = sin(toRad(60)) * yOff;
    int     mx = 0,
            my = 0,
            mxOld = 0,
            myOld = 0,
            ofs = 14,
            balls1un = 0,
            balls2un = 0,
            balls1unTot = 0,
            balls2unTot = 0,
            tableWidth = 1000,
            tableHeight = 500,
            teThick = 80,
            tcOffset = 40,
            firstBallCXOffset = 275,
            pocketOffset = 42;
    float   scale = 1,
            arrowSkew = 0,
            yLine = ScrCY,
            speed = 3,
            angleRate = .5,
            fastAngleRate = 3.5,
            powerRate = .5,
            cueDrawbackRate = 2.7,
            guideLength = 350,
            maxPower = 31,
            fricCl = .0001,
            fric = .04,
            collisionLoss = .25,
            maxVol = 5;
    
    vecF    firstBallLoc { ScrCX + firstBallCXOffset, yLine };
    vector<vecF>    locList = {
                {firstBallLoc.x + 1, firstBallLoc.y},
                {xLoc(1), yLoc(1)},
                {xLoc(1), yLoc(-1)},
                {xLoc(2), yLoc(1, true)},
                {xLoc(2), yLoc(0, true)},
                {xLoc(2), yLoc(-1, true)},
                {xLoc(3), yLoc(2)},
                {xLoc(3), yLoc(1)},
                {xLoc(3), yLoc(-1)},
                {xLoc(3), yLoc(-2)},
                {xLoc(4), yLoc(2, true)},
                {xLoc(4), yLoc(1, true)},
                {xLoc(4), yLoc(0, true)},
                {xLoc(4), yLoc(-1, true)},
                {xLoc(4), yLoc(-2, true)}
    };
    
    bool    arrowActive = false,
            arrowPlus = true,
            gameOver = false,
            running  = true,
            cueBallActive = false,
            pullingBack = false,
            showGuide = false,
            showInstr = true
    ;
    

    string instrMsg =   
        "Cue clockwise/ccw -    RIGHT/LEFT ARROW \n"
        "(Simultaneously hold UP/DOWN ARROW to speed up or slow down) \n"
        "Shoot -                hold SPACE \n"
        "Click to select 8-ball pocket \n"
        "New game -             Y \n"
        "Pause -                    U \n"
        "Move cue ball -        A, D, W, S \n"
        "Show/hide aiming guideline -   I \n"
        "Show/hide instructions -           R\n"
        "Stretch graphics to screen -       F6"
        "                                   "
        "                                   "
        "John W. Ziegler, 2020-2024  johnnywz00@yahoo.com"
    ;
}; //end class State
#endif
