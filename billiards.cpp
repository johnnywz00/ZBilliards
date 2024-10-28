//
//  billiards.cpp
//  Billiards
//
//  Created by John Ziegler on 10/22/24.
//  Copyright © 2024 John Ziegler. All rights reserved.
//

#include "billiards.h"


float State::xLoc(int idx) {
    return firstBallLoc.x + (xOff * idx) + (2 * idx);
}
        // determining ball positions in the triangle
float State::yLoc(int ofs, bool yl /*=false*/) {
    float buffer = 0;
   if (ofs)
        buffer = (ofs / abs(ofs)) * 1;
    float ret = yLine + (ballRad * ofs + buffer);
   if (ofs == 2 || ofs == -2)
        ret = yLine + buffer * 2 + 3 * ballRad * (ofs / abs(ofs));
   if (yl && ofs)
        ret += ballRad * (ofs / abs(ofs));
    return ret;
}

void State::onCreate() {
    
        /* The game was created hardcoded to the 1440 x 900 laptop screen
         * I had at the time. This View keeps the old dimensions
         * centered on other screens, albeit with some stretch.
         */
    View view(FloatRect(0, 0, ScrW, ScrH));
    w->setView(view);
    
    if (!font[0].loadFromFile("media/HelveticaNeue.ttc")) {
         cerr << "Couldn't load font HelveticaNeue! \n";
     }
    if (!font[1].loadFromFile("media/BigBook-Heavy.otf")) {
         cerr << "Couldn't load font BigBook-Heavy! \n";
     }

    txt = Text("Player 2 wins!", font[1], 20);
    txt.setOrigin(txt.gLB().width / 2, 0);
    txt.setPosition(ScrCX, 25);
    txt.setOutlineColor(Color::Green);
    txt.setFillColor(Color::Green);
    txt.setString("");
    
    instrTxt = Text("", font[0], 12);
    instrTxt.setPosition(30, ScrH - 150);
    instrTxt.setOutlineColor(Color(70, 70, 70));
    instrTxt.setFillColor(Color(140, 140, 150));
    instrTxt.setString(instrMsg);

    playerTxt = Text("1", font[1], 40);
    playerTxt.setPosition(ScrW -80, 40);
    playerTxt.setOutlineColor(Color::White);
    playerTxt.setFillColor(Color::White);
    
        // called pocket graphics
    eightBallInd.setRadius(8);
    eightBallInd.setFillColor(Color(255, 130, 130));
    eightBallInd.sP(playerTxt.gP() + vecF(-56, 12));
    Texture tex;
    if (!tex.loadFromFile("media/arrowicon.png"))
        cerr << "Couldn't load texture for arrow! \n";
    txArrow = tex;
    calledPocketInd.setTexture(txArrow);
    centerOrigin(calledPocketInd);
    
        // the balls
    if (!tex.loadFromFile("media/ball.png"))
        cerr << "Couldn't load texture for ball! \n";
    txBall = tex;
    ballColors[0] = Color(38, 110, 225);
    ballColors[1] = Color(225, 198, 32);
    for(int i = 0; i < 15; ++i) {
        Ball b = Ball();
        b.s.setTexture(txBall);
        centerOrigin(b.s);
       if (i == 4) {
            b.s.setColor(Color(40, 40, 40));
            b.color = 0;
        }
        else if (i == 14)
            b.s.setColor(ballColors[0]);
        else if (i == 13)
            b.s.setColor(ballColors[1]);
        else if (i % 2)
            b.s.setColor(ballColors[0]);
        else b.s.setColor(ballColors[1]);
       if (b.s.getColor() == ballColors[1])
            b.color = 2;
        balls.push_back(b);
    }
    
        // cue ball
    if (!tex.loadFromFile("media/cueball.png"))
        cerr << "Couldn't load texture";
    txCueBall = tex;
    cueBall.s.setTexture(txCueBall);
    centerOrigin(cueBall.s);
    cueBall.color = 4;
    
        // pockets
    vector<vecF> cPocketLocs{
        {ScrCX - tableWidth / 2 + ofs, yLine + tableHeight / 2 - ofs},
        {ScrCX + tableWidth / 2 - ofs, yLine + tableHeight / 2 - ofs},
        {ScrCX + tableWidth / 2 - ofs, yLine - tableHeight / 2 + ofs},
        {ScrCX - tableWidth / 2 + ofs, yLine - tableHeight / 2 + ofs} };
    vector<string> fnames{
        "media/SW.png",
        "media/SE.png",
        "media/NE.png",
        "media/NW.png" };
    for(int i = 0; i < 4; ++i ) {
        Pocket p = Pocket();
        p.isCorner = true;
        Texture tex;
        if (!tex.loadFromFile(fnames[i]))
            cerr << "Couldn't load texture for corner pocket! \n";
       if (i == 0) {
            txSW = tex;
            p.s.setTexture(txSW);
        }
       if (i == 1) {
            txSE = tex;
            p.s.setTexture(txSE);
        }
       if (i == 2) {
            txNE = tex;
            p.s.setTexture(txNE);
        }
       if (i == 3) {
            txNW = tex;
            p.s.setTexture(txNW);
        }
        centerOrigin(p.s);
        p.s.sP(cPocketLocs[i]);
        pockets.push_back(p);
    }
    if (!tex.loadFromFile("media/pocket.png"))
        cerr << "Couldn't load texture for side pocket! \n";
    txPocket = tex;
    for(int i = 0; i < 2; ++i ) {
        Pocket p = Pocket();
        p.s.setTexture(txPocket);
        centerOrigin(p.s);
       if (i)
            p.s.setRotation(180);
       if (i)
            p.s.sP(ScrCX, 716);
        else p.s.sP(ScrCX, 184);
        pockets.push_back(p);
    }
    
        // table
    tableSurface.setSize(vecF(tableWidth, tableHeight));
    tableEdge.setSize(vecF(tableWidth + teThick, tableHeight + teThick));
    tableSurface.setFillColor(Color(60, 137, 47));
    tableEdge.setFillColor(Color(70, 70, 30));
    centerOrigin(tableSurface);
    centerOrigin(tableEdge);
    tableSurface.sP(ScrCX, ScrCY);
    tableEdge.sP(ScrCX, ScrCY);
    
        // power indicator
    powerBar[0].setSize(vecF(15, 350));
    powerBar[1].setSize(vecF(9, 344));
    powerBar[0].setOutlineThickness(1);
    powerBar[1].setOutlineThickness(2);
    powerBar[0].setOutlineColor(Color(150, 50, 50));
    powerBar[1].setOutlineColor(Color(255, 0, 0));
    powerBar[0].setFillColor(Color::Transparent);
    powerBar[1].setFillColor(Color::Red);
    powerBar[0].setOrigin(0, powerBar[0].gLB().top + powerBar[0].gLB().height);
    powerBar[1].setOrigin(0, powerBar[1].gLB().top + powerBar[1].gLB().height);
    powerBar[0].setPosition(ScrCX - tableWidth / 2 - 29, yLine + powerBar[0].gLB().height / 2);
    powerBar[1].setPosition(ScrCX - tableWidth / 2 - 26, yLine + powerBar[1].gLB().height  / 2);
    powerBar[1].setScale(1, 0);

        // kludge to round the table corners
    if (!tex.loadFromFile("media/tablecorner.png"))
        cerr << "Couldn't load texture for table corner! \n";
    txTableCorner = tex;
    for(int i = 0; i < 4; ++i ) {
        Sprite tc = Sprite();
        tc.setTexture(txTableCorner);
       if (i == 0)
            tc.sP(ScrCX - tableWidth / 2 - tcOffset, yLine - tableHeight / 2 - tcOffset);
        else if (i == 1) {
            tc.sP(ScrCX + tableWidth / 2 + tcOffset, yLine - tableHeight / 2 - tcOffset);
            tc.rotate(90);
        }
        else if (i == 2) {
            tc.sP(ScrCX + tableWidth / 2 + tcOffset, yLine + tableHeight / 2 + tcOffset);
            tc.rotate(180);
        }
        else if (i == 3) {
            tc.sP(ScrCX - tableWidth / 2 - tcOffset, yLine + tableHeight / 2 + tcOffset);
            tc.rotate(270);
        }
        tableCorners.push_back(tc);
    }
    
        // sounds
    buffers[0].loadFromFile("media/rockHit2.wav");  // ball collision
    sounds[0].setBuffer(buffers[0]);
    buffers[1].loadFromFile("media/cueshot.wav");
    sounds[1].setBuffer(buffers[1]);
    buffers[2].loadFromFile("media/hurt2.wav");     // pocket ball
    sounds[2].setBuffer(buffers[2]);
    buffers[3].loadFromFile("media/railbump.wav");     
    sounds[3].setBuffer(buffers[3]);
    
        // misc.
    players[0] = Player();
    players[1] = Player(2);
    
    reset();
} //end onCreate


void State::reset() {
    for(auto& p:players)
        p.reset();
    curPlayer = &players[0];
    otherPlayer = &players[1];
    gameOver = false;
    running = true;
    cueBallActive = false;
    pullingBack = false;
    balls1un = 0;
    balls2un = 0;
    balls1unTot = 0;
    balls2unTot = 0;
    arrowSkew = 0;
    arrowPlus = true;
        // Give break shot a little variation
    cueBall.s.sP(ScrCX - 400, yLine + (randRange(0, 400) - 200) / 100);
    cueBall.inPocket = false;
    cueBall.setVelocity(0, 0);
    Color c = cueBall.s.getColor();
    c.a = 255;
    cueBall.s.setColor(c);
    cue.s.sP(cueBall.s.gP().x - cue.centerOffset, yLine);
    cue.angle = 0 + randRange(0, 75) / 100;
    cue.s.setRotation(cue.angle);
    cue.cueEnd = cueBall.s.gP();
    for(int i = 0; i < 15; ++i) {
        balls[i].s.sP(locList[i]);
        balls[i].inPocket = false;
        balls[i].setVelocity(0, 0);
        Color c = balls[i].s.getColor();
        c.a = 255;
        balls[i].s.setColor(c);
    }
    for(auto& p:pockets)
        p.selected = false;
    powerBar[1].setScale(1, 0);
    txt.setString("");
    playerTxt.setString(tS(curPlayer->num));
    playerTxt.setFillColor(curPlayer->c);
    
    w->setMouseCursorVisible(false);
}

void State::onMouseDown(int x, int y) {
   if (!curPlayer->onEightBall || cueBallActive)
        return;
    for (auto& obj : pockets) {
        if (!(obj.s.gGB().contains(x, y)))
            continue;
        for(auto& p:pockets)
            p.selected = false;
        arrowSkew = 0;
        arrowPlus = true;
        arrowActive = true;
        obj.selected = true;
        int j = 54;
        switch(iO(pockets, obj)) {
            case 0:
                calledPocketInd.sP(obj.s.gP() + vecF(-j, j));
                calledPocketInd.sRot(315); break;
            case 1:
                calledPocketInd.sP(obj.s.gP() + vecF(j, j));
                calledPocketInd.sRot(225); break;
            case 2:
                calledPocketInd.sP(obj.s.gP() + vecF(j, -j));
                calledPocketInd.sRot(135); break;
            case 3:
                calledPocketInd.sP(obj.s.gP() + vecF(-j, -j));
                calledPocketInd.sRot(45); break;
            case 4:
                calledPocketInd.sP(obj.s.gP() + vecF(0, -j));
                calledPocketInd.sRot(90); break;
            case 5:
                calledPocketInd.sP(obj.s.gP() + vecF(0, j));
                calledPocketInd.sRot(270); break;
        }
    }
}

void State::onKeyPress(Keyboard::Key k) {
    switch(k) {
        case Keyboard::Y:
            reset(); break;
        case Keyboard::U:
            running = !running; break;
        case Keyboard::R:
            showInstr = !showInstr;
        default:
            break;
    }
}

void State::win(Player& p) {
    gameOver = true;
    txt.setString("Player " + tS(p.num) + " wins!\n(Y to restart)");
    txt.setFillColor(p.c);
}

void State::lose(Player& p) {
    gameOver = true;
    Player& winner = (players[0].num == p.num ? players[1] : players[0]);
    txt.setString("Player " + tS(winner.num) + " wins!\n(Y to restart)");
    txt.setFillColor(winner.c);
}

void State::setToCueBall() {
    cue.cueEnd = cueBall.s.gP();
    cue.s.sP(cue.cueEnd.x - toRect(cue.centerOffset, cue.angle).x,
             cue.cueEnd.y - toRect(cue.centerOffset, cue.angle).y);
    powerBar[1].setScale(1, 0);
}

void State::moveCueBall(float x, float y) {
    cueBall.s.move(x, y);
    cue.s.move(x, y);
    cue.cueEnd = cueBall.s.gP();
}

void State::move8Ball(float x, float y) {
    balls[4].s.move(x, y);
}

void State::updateGuide() {
    guideline.clear();
    vecF ogn = cueBall.s.gP();
    for(int i = 0; i < guideLength; i += 8) {
        Vertex v {vecF(ogn + toRect(i, cue.angle)), Color(200, 200, 200)};
        guideline.append(v);
        v = Vertex(vecF(ogn + toRect(i + 2, cue.angle)), Color(200, 200, 200));
        guideline.append(v);
    }
}

void State::animateArrow() {
    float inc = 1;
    calledPocketInd.rotate(arrowPlus ? inc : -inc);
    arrowSkew += (arrowPlus ? inc : -inc);
   if (arrowPlus) {
       if (arrowSkew >= 10)
            arrowPlus = false;
    }
    else if (!arrowPlus) {
       if (arrowSkew <= -10)
            arrowPlus = true;
    }
}

void State::launch() {
    cueBallActive = true;
    pullingBack = false;
    setToCueBall();
    cueBall.setVelocity(toRect(cue.power, cue.angle));
    cue.power = 0;
    sounds[1].play();
   if (curPlayer->onEightBall)
        curPlayer->onEightAtLaunch = true;
}

void State::pocketBall(Ball& b, Pocket& p) {
    b.inPocket = true;
    b.setVelocity(0, 0);
    vecF pocketedOfs;
    int inc = 4;
   if (iO(pockets, p) == 0)
        pocketedOfs = vecF(-inc, inc);
   if (iO(pockets, p) == 1)
        pocketedOfs = vecF(inc, inc);
   if (iO(pockets, p) == 2)
        pocketedOfs = vecF(inc, -inc);
   if (iO(pockets, p) == 3)
        pocketedOfs = vecF(-inc, -inc);
    b.s.sP(p.s.gP() + pocketedOfs);
    Color c = b.s.getColor();
    c.a = 100;
    b.s.setColor(c);
    sounds[2].play();
   if (b.color == 4) {  //cueBall
       if (curPlayer->onEightAtLaunch)
            curPlayer->pendingLoss = true;
        else return;
    }
   if (!b.color) {      //eight ball
       if (curPlayer->onEightBall && p.selected)
            curPlayer->pendingWin = true;
        else curPlayer->pendingLoss = true;
    }
    else if (!curPlayer->color) {
        curPlayer->pendingSetColor = true;
    }
   if (curPlayer->pendingSetColor) {
       if (b.color == 1)
            ++balls1un;
        else ++balls2un;
    }
    else if (b.color == curPlayer->color) {
        ++curPlayer->pendingBallInc;
        b.needRespot = true;
    }
    else {
       if (players[0].num  !=  curPlayer->num)
            ++players[0].ballsPocketed;
        else ++players[1].ballsPocketed;
    }
}

void State::spotCueBall() {
    Color c = cueBall.s.getColor();
    c.a = 255;
    cueBall.s.setColor(c);
    cueBall.inPocket = false;
    cueBall.s.sP(ScrCX - 400, yLine);
    bool checkAgain = true;
    while(checkAgain) {
        checkAgain = false;
        for (int bb = 0; bb < 15; ++bb) {
            Ball& cur2 = balls[bb];
           if (cur2.inPocket)
                continue;
            float radius = cur2.s.gGB().width / 2;
            vecF pos = cur2.s.gP();
            vecF cpos = cueBall.s.gP();
            float hyp1 = hyp(cpos, pos);
            if (hyp1 > radius + radius)
                continue;
            checkAgain = true;
            cueBall.s.move(0, 0);
        }
    }
}

void State::respot(Ball& b) {
    b.needRespot = false;
    Color c = b.s.getColor();
    c.a = 255;
    b.s.setColor(c);
    b.inPocket = false;
    b.s.sP(ScrCX + firstBallCXOffset, yLine);
    bool checkAgain = true;
    while(checkAgain) {
        checkAgain = false;
        for (int bb = 0; bb < 15; ++bb) {
            Ball& cur2 = balls[bb];
           if (&b == &cur2)
                continue;
           if (cur2.inPocket)
                continue;
            float radius = cur2.s.gGB().width / 2;
            vecF pos = cur2.s.gP();
            vecF cpos = b.s.gP();
            float hyp1 = hyp(cpos, pos);
            if (hyp1 > radius + radius)
                continue;
            checkAgain = true;
            b.s.move(2, 0);
           if (b.s.gP().x > ScrCX + tableWidth / 2 - ballRad)
                b.s.move(-2, 2);
        }
    }
}

void State::frictionAndFindHighSpeed(Ball& cur, float& highSpd) {
        vecF vpp = cur.vp;
        vpp.x -= fric;
       if (abs(vpp.x) < fricCl)
            vpp.x = 0;
        cur.setVelocity(toRect(vpp));
       if (vpp.x > highSpd)
            highSpd = vpp.x;
    }

void State::update(const Time& time) {
       
    //============== Testing aids =================
    adjustVal(P, collisionLoss, .02, 0, 2);
    ikp(Q) {
        sleep(seconds(.5));
        txt.setString(fS(collisionLoss, 2));
        //txt.setString(vecfStr(cueBall.s.gP()));
    }
    if (iKP(I)) { showGuide = !showGuide; PAUSE; }
    //if (iKP(O)) { curPlayer->onEightBall = true; }
    if (iKP(A)) moveCueBall(-speed, 0);
    if (iKP(D)) moveCueBall(speed, 0);
    if (iKP(W)) moveCueBall(0, -speed);
    if (iKP(S)) moveCueBall(0, speed);
    //if (iKP(B)) move8Ball(-speed, 0);
    //if (iKP(M)) move8Ball(speed, 0);
    //if (iKP(H)) move8Ball(0, -speed -1);
    //if (iKP(N)) move8Ball(0, speed);
    //===============================================*/
        
    if (gameOver || !running)
        return;

        // Key controls for aiming cue
    int rightPressed = iKP(Right);
    int leftPressed = iKP(Left);
    if (rightPressed ^ leftPressed) {   // Ignore when both keys are down
        float rate = angleRate;
        if (rightPressed) {
            if (iKP(Up))
                rate = fastAngleRate;
            else if (iKP(Down))
                rate = angleRate / 2;
        }
        else if (leftPressed) {
            if (iKP(Up))
                rate = -fastAngleRate;
            else rate = -angleRate / (iKP(Down) ? 2 : 1);
        }
        cue.s.rotate(rate);
        cue.angle = cue.s.getRotation();
        cue.s.sP(cue.cueEnd.x - toRect(cue.centerOffset, cue.angle).x,
                 cue.cueEnd.y - toRect(cue.centerOffset, cue.angle).y);
    }
    
        // Update the guideline for cue
    updateGuide();

        // Called pocket marker
   if (arrowActive)
        animateArrow();
    
        // Handle shooting
    if (!iKP(Space) && pullingBack)
        launch();
    else if (iKP(Space)) {
        pullingBack = true;
        cue.power += powerRate;
       if (cue.power > maxPower)
            cue.power = maxPower;
        powerBar[1].setScale(1, cue.power / maxPower);
       if (cue.power < maxPower)
            cue.s.move(toRect(cueDrawbackRate, cue.angle - 180));
    }

        // Balls are rolling
   if (cueBallActive) {
        
        float phi=0, hyp1 = 0;
        float ax1, ay1, bx1, by1;
        
            /* Find the fastest-moving ball to determine
             * increments for collision detection
             */
        float highSpd = 0;
        frictionAndFindHighSpeed(cueBall, highSpd);
        for (int b = 0; b < 15; ++b)
            frictionAndFindHighSpeed(balls[b], highSpd);
        highSpd = ceil(highSpd);
        
            /* Store original positions of balls; if no collisions
             * occur during the stepping off process, ball's new
             * position will be: saved position plus velocity
             */
        vector<vecF> savePosns(16);
        
            /* Fastest ball will be moved one pixel at a time
             * towards the location where it is supposed to be at
             * the end of this frame; all other balls will be moved
             * proportionately. Check for collisions after each
             * step.
             */
        for (int i = 0; i < highSpd; ++i) {
                // First move everything one step
            for (int b = 0; b < 16; ++b) {
                Ball& cur = (b == 0 ? cueBall : balls[b - 1]);
               if (cur.inPocket)
                    continue;
                savePosns[b] = cur.s.gP();
                cur.s.move(cur.velocity / highSpd);
            }
                /* Iterate through each ball; store its stats */
            for (int b = 0; b < 16; ++b) {
                Ball& cur = (b == 0 ? cueBall : balls[b - 1]);
               if (cur.inPocket)
                    continue;
                Ball& c = cur;
                Sprite& s = cur.s;
                ax1 = c.velocity.x;
                ay1 = c.velocity.y;
                vecF ap1 = toRPolar(c.velocity);
                float cradius = s.gGB().width / 2;
                vecF cpos = s.gP();
                
                    /* We only need to check the balls with higher
                     * numbers in the array, because this ball would've
                     * already been checked for a collision with the
                     * lower numbers.
                     */
                for (int bb = b + 1; bb < 16; ++bb) {
                    Ball& cur2 = balls[bb - 1];
                   if (cur2.inPocket)
                        continue;
                    float radius = cur2.s.gGB().width / 2;
                    vecF pos = cur2.s.gP();
                    hyp1 = hyp(cpos, pos);
                    if (hyp1 > cradius + radius)
                        continue;  // no collision
                    
                        /* A collision between these two balls:
                         * determine new angles and velocities
                         */
                    s.sP(savePosns[b]); cpos = s.gP();
                    if (hyp(cpos, pos) <= cradius + radius)
                        cur2.s.sP(savePosns[bb]);
                    
                    bx1 = cur2.velocity.x;
                    by1 = cur2.velocity.y;
                    vecF bp1 = toRPolar(cur2.velocity);
                    
                        /* Whichever ball is going faster, use its velocity
                         * to approximate the volume of the collision sound
                         */
                    sounds[0].setVolume(min(100, int(max(ap1.x, bp1.x) * 5)));
                    sounds[0].play();
                    phi = atan2(pos.y - cpos.y, pos.x - cpos.x);
                    float ag = cos(ap1.y - phi) * ap1.x;
                    float ah = sin(ap1.y - phi) * ap1.x;
                    float bg = cos(bp1.y - phi) * bp1.x;
                    float bh = sin(bp1.y - phi) * bp1.x;
                    
                    float ag2 = bg;
                    float bg2 = ag;
                    
                    float magA = hyp(ah, ag2);
                    float dirA = atan2(ah, ag2) + phi;
                    float magB = hyp(bh, bg2);
                    float dirB = atan2(bh, bg2) + phi;
                    
                    cur.setVelocity(vecF(toRRect(magA - collisionLoss * 2, dirA)));
                    cur2.setVelocity(vecF(toRRect(magB - collisionLoss * 2, dirB)));
                    
                } //end for bb
                
                    /* Now check if this ball is colliding with
                     * pockets or rails. Reverse the direction
                     * or pocket the ball as appropriate
                     */
                vecF pos = s.gP();
                int pofs = pocketOffset;
                float htw = tableWidth / 2;
                float hth = tableHeight / 2;
                float tew = ScrCX - htw; // tableEdgeWest, etc.
                float tee = ScrCX + htw;
                float ten = ScrCY - hth;
                float tes = ScrCY + hth;
                float hpw = 34; // half of pocket width

               if (     pos.x >= tew &&
                        pos.x <= tew + pofs &&
                        pos.y >= pos.x + tes - pofs - tew)
                    pocketBall(cur, pockets[0]); //SW
               if (     pos.x >= tee - pofs &&
                        pos.x <= tee &&
                        pos.y >= -pos.x + tee + tes - pofs)
                    pocketBall(cur, pockets[1]); //SE
               if (     pos.x >= tee - pofs &&
                        pos.x <= tee &&
                        pos.y <= pos.x + ten + pofs - tee)
                    pocketBall(cur, pockets[2]); //NE
               if (     pos.x >= tew &&
                        pos.x <= tew + pofs &&
                        pos.y <= -pos.x + tew + ten + pofs)
                    pocketBall(cur, pockets[3]); //NW
               if (     pos.x >= ScrCX - hpw &&
                        pos.x <= ScrCX + hpw &&
                        pos.y < ten + 2)
                    pocketBall(cur, pockets[4]);
               if (     pos.x >= ScrCX - hpw &&
                        pos.x <= ScrCX + hpw &&
                        pos.y > tes - 2)
                    pocketBall(cur, pockets[5]);
                
                vector<bool> hitRail = {false, false};
                int bumperCompression = 2;
                float x1 = tew + ballRad - bumperCompression;
                float x2 = tee - ballRad + bumperCompression;
                float y1 = ten + ballRad - bumperCompression;
                float y2 = tes - ballRad + bumperCompression;

               if (     pos.x > x2 &&
                        pos.y > ten + pofs &&
                        pos.y < tes - pofs) {
                    s.sP(x2, pos.y);
                    hitRail[0] = true;
                }
                else if (pos.x < x1 &&
                        pos.y > ten + pofs &&
                        pos.y < tes - pofs) {
                    s.sP(x1, pos.y);
                    hitRail[0] = true;
                }
               if (     pos.y > y2 &&
                        (pos.x < ScrCX - hpw || pos.x > ScrCX + hpw) &&
                        pos.x > tew + pofs &&
                        pos.x < tee - pofs) {
                    s.sP(pos.x, y2);
                    hitRail[1] = true;
                }
                else if (pos.y < y1 &&
                        (pos.x < ScrCX - hpw || pos.x > ScrCX + hpw) &&
                        pos.x > tew + pofs &&
                        pos.x < tee - pofs) {
                    s.sP(pos.x, y1);
                    hitRail[1] = true;
                }
                
                if (hitRail[0])
                    cur.setVelocity(cur.velocity.x * -1, cur.velocity.y);
                if (hitRail[1])
                    cur.setVelocity(cur.velocity.x, cur.velocity.y * -1);
                if (hitRail[0] || hitRail[1]) {
                    vecF vpp = cur.vp;
                    sounds[3].setVolume(min(100, int(vpp.x * 5)));
                    sounds[3].play();
                    vpp.x -= collisionLoss;
                    cur.setVelocity(toRect(vpp));
                }
            } //end for b
        } //end for i
        
            // Determine whether the balls are done rolling
        bool allStill = true;
        for(auto& b : balls) {
           if (b.vp.x) {
                allStill = false;
                break;
            }
        }
       if (cueBall.vp.x)
            allStill = false;
        
            /* Balls are all done rolling. Take appropriate actions
             * based on anything that has been pocketed, and
             * get ready for another shot
             */
       if (allStill) {
            cueBallActive = false;
           if (curPlayer->pendingLoss)
                lose(*curPlayer);
            bool changePlayer = true;
           if (cueBall.inPocket) {
                    // Scratched on the eight ball
               if (curPlayer->pendingWin || curPlayer->onEightBall) {
                    lose(*curPlayer);
                    return;
                }
                    // Scratched when about to establish stripes/solids
               if (curPlayer->pendingSetColor) {
                    curPlayer->pendingSetColor = false;
                    balls1unTot += balls1un;
                    balls2unTot += balls2un;
                    balls1un = 0;
                    balls2un = 0;
                }
                    // Scratched after shooting one of own balls in
               if (curPlayer->pendingBallInc)
                    for(auto& b:balls)
                       if (b.needRespot) respot(b);
                
                    // In all cases respot after scratch
                spotCueBall();
            }
            else {
                    // No scratch: nothing needs respotting
                for(auto& b:balls)
                    b.needRespot = false;
                    // Eight ball went in called pocket with no scratch
               if (curPlayer->pendingWin) {
                    win(*curPlayer);
                    return;
                }
                    // A player just sunk first majority of stripes/solids
               if (curPlayer->pendingSetColor) {
                    curPlayer->pendingSetColor = false;
                    balls1unTot += balls1un;
                    balls2unTot += balls2un;
                   if (balls1un  !=  balls2un) {
                       if (balls1un > balls2un) {
                            curPlayer->color = 1;
                            curPlayer->c = ballColors[0];
                        }
                        else {
                            curPlayer->color = 2;
                            curPlayer->c = ballColors[1];
                        }
                       if (curPlayer->color == 1)
                            otherPlayer->color = 2;
                        else otherPlayer->color = 1;
                       if (otherPlayer->color == 1)
                            otherPlayer->c = ballColors[0];
                        else otherPlayer->c = ballColors[1];
                        playerTxt.setFillColor(curPlayer->c);
                       if (curPlayer->color == 1) {
                            curPlayer->ballsPocketed += balls1unTot;
                            otherPlayer->ballsPocketed += balls2unTot;
                        }
                        else {
                            curPlayer->ballsPocketed += balls2unTot;
                            otherPlayer->ballsPocketed += balls1unTot;
                        }
                        changePlayer = false;
                    }
                    balls1un = 0;
                    balls2un = 0;
                } // end pendingSetColor
                
                    // Player sunk some of own balls without scratching
               if (curPlayer->pendingBallInc) {
                    curPlayer->ballsPocketed += curPlayer->pendingBallInc;
                    curPlayer->pendingBallInc = 0;
                    changePlayer = false;
                }
            } // end !cueBall.inPocket
            
                // Mark if a player is ready to shoot for eight ball
           if (players[0].ballsPocketed == 7) {
                players[0].onEightBall = true;
            }
           if (players[1].ballsPocketed == 7) {
                players[1].onEightBall = true;
            }
            
                // Just completed a shot so no pocket is currently called
            for(auto& p:pockets)
                p.selected = false;
            arrowActive = false;
            
                // Set the cue up for next shot, determine whose turn
            setToCueBall();
           if (changePlayer) {
               if (curPlayer == &players[0]) {
                    curPlayer = &players[1];
                    otherPlayer = &players[0];
                }
                else {
                    curPlayer = &players[0];
                    otherPlayer = &players[1];
                }
                playerTxt.setString(tS(curPlayer->num));
                playerTxt.setFillColor(curPlayer->c);
               if (curPlayer->onEightBall)
                        // Need to be able to click a pocket
                    w->setMouseCursorVisible(true);
                else
                    w->setMouseCursorVisible(false);
            }
        } //end if allStill
    } //end if cueBallActive
} //end update


void State::draw() {
    w->draw(tableEdge);
    w->draw(tableSurface);
    w->draw(powerBar[0]);
    w->draw(powerBar[1]);
    for(auto& c:tableCorners)
        w->draw(c);
    for(auto& p:pockets)
        w->draw(p.s);
    for(auto& b:balls)
        w->draw(b.s);
    w->draw(cueBall.s);
    w->draw(cue.s);
   if (showGuide && !cueBallActive && !gameOver)
        w->draw(guideline);
    w->draw(txt);
    w->draw(playerTxt);
    if(showInstr)
        w->draw(instrTxt);
   if (curPlayer->onEightBall && !gameOver)
        w->draw(eightBallInd);
   if (indexWhich(pockets, [&](auto& p) { return p.selected; }) > -1)
        w->draw(calledPocketInd);
}

