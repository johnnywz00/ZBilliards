
#ifndef BILLIARDS_H
#define BILLIARDS_H


/*
 bkgd "carpet"
 -why collision loss doesn't seem right:
	got a positive reading once on speed dif after collis. physics must be wrong, magA was computed higher than the top speed of either ball involved
		(potentially only when multiple balls or rails collide in same frame?)
		does collision loss need to be proport. to speed: higher speeds seem like they ought to transfer less than they do
 -blue ball got against wall and started to "creep" after all still: rail bounce physics are crude
 -can't shoot balls in kitchen
 */




#include "objects.hpp"
#include "timedeventmanager.hpp"

class FullscreenOnlyApp;

class State
{
public:
    void onCreate ();
    
    void onMouseDown (int x, int y);
    
    void onMouseUp (int x, int y) { }
    
    void onKeyPress (Keyboard::Key);

    void onKeyRelease (Keyboard::Key) { }

	void update (const Time&);
	
	void draw ();
	
private:
	void assembleTable ();

	void reset ();
	
	void setToCueBall ();
	
	void updateGuide ();
	
	void launch ();
	
	void frictionAndFindHighSpeed (Ball& cur, float& highSpd);
	
    void moveCueBall (float x, float y);
    
    void pocketBall (Ball& b, Pocket& p);
    
    void spotCueBall ();
    
    void respot (Ball&);
    
	void animateArrow ();
	
	void winGame (Player& p);
	
	void lose (Player& p);
	
	/* Determining ball positions in the rack */
	float xLoc (int idx);
	
	float yLoc (int ofs, bool yl =false);
	
	float SCRW () { return win->getDefaultView().getSize().x; }
	float SCRH () { return win->getDefaultView().getSize().y; }
	float SCRCX () { return win->getDefaultView().getSize().x / 2; }
	float SCRCY () { return win->getDefaultView().getSize().y / 2; }
	
	
	
public:
    RenderWindow*       win;
	FullscreenOnlyApp*  app;
    TimedEventManager*  timedMgr;
	vecI				mouseVec
						, oldMouse
	;
	
private:
	RenderTexture		rt;

	Sprite				tabSpr;
	vector<Pocket>      pockets;
    Cue                 cue;
    Ball                cueBall;
    vector<Ball>        balls;
	RectangleShape      powerBar[2];
	VertexArray         guideline { Lines };
	CircleShape         eightBallInd;
	Sprite              calledPocketInd;
 
	Player              players[2];
    Player*             curPlayer;
    Player*             otherPlayer;
	Color               ballColors[2];

	Text                txt
						, instrTxt
						, playerTxt
						, splashTitle
						, splashByline
	;

	bool    showSplash = true
			, arrowPlus = true
			, showInstr = true
			, running  = true
			, arrowActive = false
			, gameOver = false
			, cueBallActive = false
			, pullingBack = false
			, showGuide = false
			, placingCueBall = false
			, isBreakShot = true
	;

    static constexpr int        ballRad = 15;
    static constexpr float      yOfs = ballRad * 2 - 1;
    const float                 xOfs = sind(60) * yOfs - 1;
	int		ofs = 6
            , balls1un = 0
            , balls2un = 0
            , balls1unTot = 0
            , balls2unTot = 0
	;
	float	tableWidth = 1173
			, tableHeight = 587
            , teThick = 80
            , firstBallCXOffset = 275
            , pocketOffset = 42
	;
    float   scale = 1
            , arrowSkew = 0
            , yLine
            , spotSpeed = 3
            , angleRate = .5
            , fastAngleRate = 3.5
            , powerRate = .5
            , cueDrawbackRate = 2.7
            , guideLength = 350
            , maxPower = 31
            , speedClamp = .007
            , friction = .028
            , collisionLoss = .9
			, bumperLoss = 1
	;
    
    vecF    		firstBallLoc;
    vector<vecF>    locList;

    string instrMsg =   
        "Cue clockwise/ccw -    RIGHT/LEFT ARROW \n"
        "(Simultaneously hold UP/DOWN ARROW to speed up or slow down) \n"
        "Shoot -                hold SPACE \n"
        "Click to select 8-ball pocket \n"
        "New game -             Y \n"
        "Pause -                    U \n"
        "Move cue ball -        A, D, W, S \n"
        "Show/hide aiming guideline -   I \n"
		"Show/hide cursor -                   G / Shift + G\n"
        "Show/hide instructions -           R\n"
        "                                   "
        "                                   "
        "John W. Ziegler, 2020-2024  johnnywz00@yahoo.com"
    ;
}; //end class State
#endif
