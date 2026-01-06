//
//  objects.hpp
//  Billiards
//
//  Created by John Ziegler on 10/22/24.
//  Copyright © 2024 John Ziegler. All rights reserved.
//

#ifndef objects_hpp
#define objects_hpp

#include "jwzsfml.hpp"
#include "resourcemanager.hpp"


struct Player
{
    Player (int n = 1) : num(n) { }
    
    void reset ()
	{
        isMyTurn = (num == 1);
        onEightBall = false;
        onEightAtLaunch = false;
        pendingSetColor = false;
        pendingWin = false;
        pendingLoss = false;
		ballsPocketed = 0;
		pendingBallInc = 0;
		color = 0;
		c = Color::White;
    }

	bool    isMyTurn;
	bool    onEightBall;
	bool    onEightAtLaunch;
	bool    pendingWin;
	bool    pendingLoss;
	bool    pendingSetColor;
    int     num;
    int     pendingBallInc;
    int     ballsPocketed;
	int     color = 0;
	Color   c;
};



struct Cue
{
    Cue ()
	{        
		spr.setTexture(Resources::getTex("cue"));
        centerOrigin(spr);
        spr.sP(300, 300);
        centerOffset = spr.gLB().width / 2 + 30;
        cueEnd = vecF(spr.gP().x + centerOffset, spr.gP().y);
    }
    
    float       angle = 0;
    float       power = 0;
    float       centerOffset;
    vecF        cueEnd;
	Sprite      spr;
};



struct Ball
{
    void setVelocity (vecF vec)
	{
        velocity = vec;
        vp = toPolar(velocity);
    }

	void setVelocity (float x, float y)
	{
        velocity = vecF(x, y);
        vp = toPolar(velocity);
    }
    
	bool        inPocket = false;
	bool        needRespot = false;
	int         color = 1;
    vecF        velocity { 0, 0 };
    vecF        vp { 0, 0 };
	Sprite      spr;
};



struct Pocket {
    
    Pocket () { centerOrigin(spr); }

	static const int 	offsAbs = 5;
	bool        		selected = false;
	vecF				pocketedOffs;
	Sprite      		spr;
};


#endif /* objects_hpp */
