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


struct Player {
    
    Player (int n = 1) : num(n) { }
    
    void reset () {
        
        isMyTurn = (num == 1);
        onEightBall = false;
        onEightAtLaunch = false;
        ballsPocketed = 0;
        color = 0;
        c = Color(Color::White);
        pendingBallInc = 0;
        pendingSetColor = false;
        pendingWin = false;
        pendingLoss = false;
    }

    int     num;
    int     color = 0;
    Color   c;
    bool    isMyTurn;
    bool    onEightBall;
    bool    onEightAtLaunch;
    bool    pendingWin;
    bool    pendingLoss;
    bool    pendingSetColor;
    int     pendingBallInc;
    int     ballsPocketed;
};



struct Cue  {
    
    Cue () {
        
        Texture tex;
        if (!tex.loadFromFile("media/cue.png"))
            cerr << "Couldn't load texture for cue! \n";
        txCue = tex;
        s.setTexture(txCue);
        centerOrigin(s);
        s.sP(300, 300);
        centerOffset = s.gLB().width / 2 + 30;
        cueEnd = vecF(s.gP().x + centerOffset, s.gP().y);
    }
    
    Sprite      s;
    Texture     txCue;
    float       angle = 0;
    float       power = 0;
    float       centerOffset;
    vecF        cueEnd;
};



struct Ball {
    
    Ball () { }
    
    void setVelocity (vecF vec) {
        
        velocity = vec;
        vp = toPolar(velocity);
    }
    void setVelocity (float x, float y) {
        
        velocity = vecF(x, y);
        vp = toPolar(velocity);
    }
    
    Sprite      s;
    vecF        velocity { 0, 0 };
    vecF        vp { 0, 0 };
    bool        inPocket = false;
    bool        needRespot = false;
    int         color = 1;
};



struct Pocket {
    
    Pocket () { centerOrigin(s); }

    Sprite      s;
    bool        isCorner = true;
    bool        selected = false;
};


#endif /* objects_hpp */
