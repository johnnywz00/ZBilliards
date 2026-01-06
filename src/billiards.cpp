//
//  billiards.cpp
//  ZBilliards
//
//  Created by John Ziegler on 10/22/24.
//  Copyright © 2024 John Ziegler. All rights reserved.
//

#include "billiards.hpp"
#include "sfmlApp.hpp"

void State::onCreate ()
{
	app->setRedrawColor(Color::Black);

		/* Splash screen */
	Color splashGreen = decreaseSaturation(Color::Green, 50);
	splashTitle = Text("ZBilliards", Resources::getFont("splashTitle"), 200);
	splashTitle.setOutlineThickness(4);
	splashTitle.setOutlineColor(decreaseBrightness(MUTEDGRASS, 40));
	splashTitle.setFillColor(splashGreen);
	centerOrigin(splashTitle);
	splashTitle.setPosition(scrcx, 200);
	
	splashByline = Text("John W. Ziegler, 2020-2024", Resources::getFont("text"), 20);
	splashByline.setFillColor(splashGreen);
	centerOrigin(splashByline);
	splashByline.setPosition(scrcx, scrh - 70);

		/* Init texts */
    txt = Text("Player 2 wins!", Resources::getFont("player"), 20);
    txt.setOrigin(txt.gLB().width / 2, 0);
    txt.setPosition(scrcx, 40);
    txt.setOutlineColor(Color::Green);
    txt.setFillColor(Color::Green);
    txt.setString("");
    
    instrTxt = Text("", Resources::getFont("text"), 12);
    instrTxt.setPosition(30, scrh - 170);
    instrTxt.setOutlineColor(Color(70, 70, 70));
    instrTxt.setFillColor(Color(140, 140, 150));
    instrTxt.setString(instrMsg);

    playerTxt = Text("1", Resources::getFont("player"), 40);
    playerTxt.setPosition(scrw - 80, 40);
    playerTxt.setOutlineColor(Color::White);
    playerTxt.setFillColor(Color::White);
    
		/* Table */
	yLine = scrcy;
	assembleTable();

		/* Pockets */
	float halfTabWid = tableWidth / 2;
	float halfTabHt = tableHeight / 2;
	vector<vecF> cPocketLocs {
		{scrcx - halfTabWid + ofs, yLine - halfTabHt + ofs}
		, {scrcx + halfTabWid - ofs, yLine - halfTabHt + ofs}
		, {scrcx + halfTabWid - ofs, yLine + halfTabHt - ofs}
		, {scrcx - halfTabWid + ofs, yLine + halfTabHt - ofs}
	};
	vector<vecF> pocketedOffsets {
		{-Pocket::offsAbs, -Pocket::offsAbs}
		, {Pocket::offsAbs, -Pocket::offsAbs}
		, {Pocket::offsAbs, Pocket::offsAbs}
		, {-Pocket::offsAbs, Pocket::offsAbs}
		, {0, Pocket::offsAbs}
		, {0, -Pocket::offsAbs}
	};
	forNum(6) {
		Pocket p;
		p.spr.setTexture(Resources::getTex("pocket"));
		centerOrigin(p.spr);
		p.pocketedOffs = pocketedOffsets[i];
		if (i < 4) {
			p.spr.setRotation(i * 90);
			p.spr.sP(cPocketLocs[i]);
		}
		else if (i == 4) {
			p.spr.setRotation(225);
			p.spr.sP(scrcx, yLine + tableHeight / 2 + 12);
		}
		else {
			p.spr.setRotation(45);
			p.spr.sP(scrcx, yLine - tableHeight / 2 - 12);
		}
		pockets.push_back(p);
	}

        /* The balls */
    ballColors[0] = Color(38, 110, 225);
    ballColors[1] = Color(225, 198, 32);
	firstBallLoc = {scrcx + firstBallCXOffset, yLine };
	locList = {
				firstBallLoc
				, {xLoc(1), yLoc(1)}
				, {xLoc(1), yLoc(-1)}
				, {xLoc(2), yLoc(1, true)}
				, {xLoc(2), yLoc(0, true)}
				, {xLoc(2), yLoc(-1, true)}
				, {xLoc(3), yLoc(2)}
				, {xLoc(3), yLoc(1)}
				, {xLoc(3), yLoc(-1)}
				, {xLoc(3), yLoc(-2)}
				, {xLoc(4), yLoc(2, true)}
				, {xLoc(4), yLoc(1, true)}
				, {xLoc(4), yLoc(0, true)}
				, {xLoc(4), yLoc(-1, true)}
				, {xLoc(4), yLoc(-2, true)}
	};
    forNum(15) {
        Ball b;
		b.spr.setTexture(Resources::getTex("ball"));
        centerOrigin(b.spr);
        if (i == 4) {	// Eight ball
            b.spr.setColor(Color(40, 40, 40));
            b.color = 0;
        }
        else if (i == 10 || i % 2 && i != 3)
            b.spr.setColor(ballColors[0]);
		else {
			b.spr.setColor(ballColors[1]);
			b.color = 2;
		}
        balls.push_back(b);
    }
    
        /* Cue ball */
	cueBall.spr.setTexture(Resources::getTex("cueBall"));
    centerOrigin(cueBall.spr);
    cueBall.color = 4;
    
		/* Called-pocket graphics */
	eightBallInd.setRadius(8);
	eightBallInd.setFillColor(Color(255, 130, 130));
	eightBallInd.sP(playerTxt.gP() + vecF(-56, 12));
	calledPocketInd.setTexture(Resources::getTex("arrow"));
	centerOrigin(calledPocketInd);

        /* Power indicator */
    powerBar[0].setSize(vecF(15, 350));
    powerBar[1].setSize(vecF(9, 344));
    powerBar[0].setOutlineThickness(1);
    powerBar[1].setOutlineThickness(2);
	powerBar[0].setOutlineColor(AZURE50);
	powerBar[1].setOutlineColor(AZURE);
    powerBar[0].setFillColor(Color::Transparent);
	powerBar[1].setFillColor(DKAZURE);
    powerBar[0].setOrigin(0, powerBar[0].gLB().top + powerBar[0].gLB().height);
    powerBar[1].setOrigin(0, powerBar[1].gLB().top + powerBar[1].gLB().height);
    powerBar[0].setPosition(scrcx - tableWidth / 2 - 59, yLine + powerBar[0].gLB().height / 2);
    powerBar[1].setPosition(scrcx - tableWidth / 2 - 56, yLine + powerBar[1].gLB().height  / 2);
    powerBar[1].setScale(1, 0);

        /* Misc. */
    players[0] = Player(1);
    players[1] = Player(2);
    
    reset();
} //end onCreate

void State::onMouseDown (int x, int y)
{
	if (showSplash) {
		showSplash = false;
		return;
	}
	
		/* The only other thing we're using clicking for is
		 * selecting pocket for eight ball
		 */
	if (!curPlayer->onEightBall || cueBallActive)
		return;
	
	for (auto& obj : pockets) {
		obj.selected = false;
		if (!(obj.spr.gGB().contains(x, y)))
			continue;
		obj.selected = true;
		arrowSkew = 0;
		arrowPlus = true;
		arrowActive = true;
		int indOffs = 54;
		vecF pos = obj.spr.getPosition();
		switch(indexOfRef(pockets, obj)) {
			case 0:
				calledPocketInd.sP(pos + vecF(-indOffs, -indOffs));
				calledPocketInd.sRot(45);
				break;
			case 1:
				calledPocketInd.sP(pos + vecF(indOffs, -indOffs));
				calledPocketInd.sRot(135);
				break;
			case 2:
				calledPocketInd.sP(pos + vecF(indOffs, indOffs));
				calledPocketInd.sRot(225);
				break;
			case 3:
				calledPocketInd.sP(pos + vecF(-indOffs, indOffs));
				calledPocketInd.sRot(315);
				break;
			case 4:
				calledPocketInd.sP(pos + vecF(0, indOffs));
				calledPocketInd.sRot(270);
				break;
			case 5:
				calledPocketInd.sP(pos + vecF(0, -indOffs));
				calledPocketInd.sRot(90);
				break;
			default:
				break;
		}
	}
}

void State::onKeyPress (Keyboard::Key k)
{
	if (showSplash) {
		showSplash = false;
		return;
	}
	
	switch(k) {
			
		case Keyboard::Escape:
			app->close();
			break;
		
		case Keyboard::Y:
			reset();
			break;
		
		case Keyboard::I:
			showGuide = !showGuide;
			break;
		
		case Keyboard::U:
			running = !running;
			break;
		
		case Keyboard::R:
			showInstr = !showInstr;
			break;
		
		default:
			break;
	}
}

void State::update (const Time& time)
{
	//============== Testing aids =================
//	adjustVal(P, collisionLoss, .02, 0, 2);
//	adjustVal(LBracket, bumperLoss, .02, 0, 2);
//	adjustVal(RBracket, friction, .0005, 0, 1);
//	if (iKP(O)) { curPlayer->onEightBall = true; }
	//===============================================*/
		
		/* If no click or keystroke, transition from splash */
	if (time.asSeconds() > 6)
		showSplash = false;
	
	if (gameOver || !running)
		return;

		/* Make cue ball movable after a scratch */
	if (placingCueBall)	{
		if (iKP(A))
			moveCueBall(-spotSpeed, 0);
		if (iKP(D))
			moveCueBall(spotSpeed, 0);
		if (iKP(W))
			moveCueBall(0, -spotSpeed);
		if (iKP(S))
			moveCueBall(0, spotSpeed);
	}
	
		/* Aiming cue */
	int rightPressed = iKP(Right);
	int leftPressed = iKP(Left);
	if (rightPressed ^ leftPressed) {   // Ignore when both keys are down
		float rate = angleRate;
		if (rightPressed) {
			if (iKP(Up))
				rate = fastAngleRate;
			else if (iKP(Down))
				rate /= 3;
		}
		else if (leftPressed) {
			if (iKP(Up))
				rate = -fastAngleRate;
			else rate = -angleRate / (iKP(Down) ? 3 : 1);
		}
		cue.spr.rotate(rate);
		cue.angle = cue.spr.getRotation();
		
			/* Cue may be mid-drawback, so get correct position */
		float curDist = toPolar(cue.spr.gP() - cue.cueEnd).x;
		cue.spr.sP(cue.cueEnd - pVec(curDist, cue.angle));
	
			/* Update the aiming guideline for cue */
		updateGuide();
	}
	
		/* Animate called-pocket marker if applicable */
	if (arrowActive)
		animateArrow();
	
		/* Handle shooting */
	if (!cueBallActive) {
		
			/* Shoot when space bar is just released */
		if (!iKP(Space) && pullingBack)
			launch();
		
			/* Spacebar is being held down: drawback in progress */
		else if (iKP(Space)) {
			pullingBack = true;
			float maxPow = maxPower;
			if (isBreakShot)
				maxPow += 10;
			cue.power = min(cue.power + powerRate, maxPow); // Increment with cap
			powerBar[1].setScale(1, cue.power / maxPow);
			if (cue.power < maxPow)
				cue.spr.move(toRect(cueDrawbackRate, cue.angle - 180));
		}
	}

	else {
			/* Balls are rolling */
		float 	phi = 0
				, hyp1 = 0
				, ax1, ay1, bx1, by1
		;
		
			/* Find the fastest-moving ball to determine
			 * increments for collision detection
			 */
		float highSpd = 0;
		frictionAndFindHighSpeed(cueBall, highSpd);
		forNum(15)
			frictionAndFindHighSpeed(balls[i], highSpd);
		highSpd = ceil(highSpd);
		
			/* Store original positions of balls; if no collisions
			 * occur during the stepping off process, ball's new
			 * position will be saved position + velocity
			 */
		vector<vecF> savePosns(16);
		
			/* Fastest ball will be moved one pixel at a time
			 * towards the location where it is supposed to be at
			 * the end of this frame; all other balls will be moved
			 * proportionately. Check for collisions after each
			 * step.
			 */
		forNum(highSpd) {
			
				/* First move everything one step */
			for (int b = 0; b < 16; ++b) {
				Ball& cur = (b == 0 ? cueBall : balls[b - 1]);
				if (cur.inPocket)
					continue;
				savePosns[b] = cur.spr.gP();
				cur.spr.move(cur.velocity / highSpd);
			}
			
				/* Iterate through each ball; store its stats */
			for (int b = 0; b < 16; ++b) {
				Ball& cur = (b == 0 ? cueBall : balls[b - 1]);
				if (cur.inPocket)
					continue;
				Sprite& s = cur.spr;
				ax1 = cur.velocity.x;
				ay1 = cur.velocity.y;
				vecF ap1 = toRPolar(cur.velocity);
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
					float radius = cur2.spr.gLB().width / 2;
					vecF pos = cur2.spr.gP();
					hyp1 = hyp(cpos, pos);
					if (hyp1 > cradius + radius)
						continue;  // No collision
					
						/* A collision between these two balls:
						 * determine new angles and velocities
						 */
					s.sP(savePosns[b]);
					cpos = s.gP();
					if (hyp(cpos, pos) <= cradius + radius) {
						cur2.spr.sP(savePosns[bb]);
						pos = savePosns[bb];
					}
					
					bx1 = cur2.velocity.x;
					by1 = cur2.velocity.y;
					vecF bp1 = toRPolar(cur2.velocity);
					
						/* Whichever ball is going faster, use its velocity
						 * to approximate the volume of the collision sound
						 */
					Resources::getSound("ballsHit").setVolume(min(100, int(max(cur.vp.x, cur2.vp.x) * 5)));
					Resources::getSound("ballsHit").play();
					
						/* Sphere collision physics */
					phi = atan2(pos.y - cpos.y, pos.x - cpos.x);
					float ag = cos(ap1.y - phi) * ap1.x;
					float ah = sin(ap1.y - phi) * ap1.x;
					float bg = cos(bp1.y - phi) * bp1.x;
					float bh = sin(bp1.y - phi) * bp1.x;
					
						/* Magnitudes are simplified because balls are
						 * all of same mass
						 */
					float magA = hyp(ah, bg);
					float dirA = atan2(ah, bg) + phi;
					float magB = hyp(bh, ag);
					float dirB = atan2(bh, ag) + phi;
					
					
					//======== Debug checking for wrong physics/velocities ========
//					float dif;
//					if (cur.vp.x > cur2.vp.x) {
//						dif = (magB - collisionLoss) - cur.vp.x;
//					}
//					else {
//						dif = (magA - collisionLoss) - cur2.vp.x;
//					}
//					if (dif >= 0)
//						txt.setString(tS(dif));
					//=======================================================
					
						/* A little kludge to help the break shot disperse
						 * the balls better. Aside from break shot, apply
						 * velocity loss when balls collide with each other
						 */
					float loss = isBreakShot ? 0 : collisionLoss;
					cur.setVelocity(toRRect(max(0.f, magA - loss), dirA));
					cur2.setVelocity(toRRect(max(0.f, magB - loss), dirB));
				} //end for bb
				
					/* Now check if this ball is colliding with
					 * pockets or rails. Reverse the direction
					 * or pocket the ball as appropriate
					 */
				vecF pos = s.gP();
				int pofs = pocketOffset;
				float htw = tableWidth / 2;
				float hth = tableHeight / 2;
				float tew = scrcx - htw; // tableEdgeWest, etc.
				float tee = scrcx + htw;
				float ten = scrcy - hth;
				float tes = scrcy + hth;
				float hpw = 34; // half of pocket width

					/* Ball is at extreme left of table */
				if (isOrBetween(pos.x, tew, tew + pofs)) {
					if (pos.y >= pos.x + tes - pofs - tew)
						pocketBall(cur, pockets[3]); //SW
					else if (pos.y <= -pos.x + tew + ten + pofs)
						pocketBall(cur, pockets[0]); //NW
				}
				
					/* Ball is at extreme right of table */
				else if (isOrBetween(pos.x, tee - pofs, tee)) {
					if (pos.y >= -pos.x + tee + tes - pofs)
						pocketBall(cur, pockets[2]); //SE
					else if (pos.y <= pos.x + ten + pofs - tee)
						pocketBall(cur, pockets[1]); //NE
				}
				
					/* Ball is near left-right centerline of table */
				else if (isOrBetween(pos.x, scrcx - hpw, scrcx + hpw)) {
					if (pos.y < ten + 2)
						pocketBall(cur, pockets[5]);
					else if (pos.y > tes - 2)
						pocketBall(cur, pockets[4]);
				}
				
					/* If it didn't get pocketed, check for hitting a rail */
				if (!cur.inPocket) {
					vector<bool> hitRail = {false, false};
					int bumperCompression = 2;
					float x1 = tew + ballRad - bumperCompression;
					float x2 = tee - ballRad + bumperCompression;
					float y1 = ten + ballRad - bumperCompression;
					float y2 = tes - ballRad + bumperCompression;

					if (between(pos.y, ten + pofs, tes - pofs)) {
						if (pos.x > x2) {
							s.sP(x2, pos.y);
							hitRail[0] = true;
						}
						else if (pos.x < x1) {
							s.sP(x1, pos.y);
							hitRail[0] = true;
						}
					}
					else if ((pos.x < scrcx - hpw
								|| pos.x > scrcx + hpw)
							 && pos.x > tew + pofs
							 && pos.x < tee - pofs) {
						if (pos.y > y2) {
							s.sP(pos.x, y2);
							hitRail[1] = true;
						}
						else if (pos.y < y1) {
							s.sP(pos.x, y1);
							hitRail[1] = true;
						}
					}
					
					if (hitRail[0])
						cur.setVelocity(cur.velocity.x * -1, cur.velocity.y);
					if (hitRail[1])
						cur.setVelocity(cur.velocity.x, cur.velocity.y * -1);
					if (hitRail[0] || hitRail[1]) {
						vecF vpp = cur.vp;
						Resources::getSound("railHit").setVolume(min(100, int(vpp.x * 5)));
						Resources::getSound("railHit").play();
						vpp.x = max(0.f, vpp.x - bumperLoss);
						cur.setVelocity(toRect(vpp));
						isBreakShot = false;
					}
				}
			} //end for b
		} //end for i
		
			/* Determine whether the balls are done rolling */
		bool allStill = true;
		forNum(16) {
			Ball& b = (i == 15 ? cueBall : balls[i]);
			if (b.vp.x >= speedClamp) {
				allStill = false;
				break;
			}
		}
		
			/* Balls are all done rolling. Take appropriate actions
			 * based on anything that has been pocketed, and
			 * get ready for another shot
			 */
		if (allStill) {
			cueBallActive = false;
			
				/* Someone hit eight ball in early, or into uncalled pocket */
			if (curPlayer->pendingLoss)
				lose(*curPlayer);
			
			bool changePlayer = true;
			if (cueBall.inPocket) {
				
					/* Scratched on the eight ball */
				if (curPlayer->pendingWin || curPlayer->onEightBall) {
					lose(*curPlayer);
					return;
				}
				
					/* Scratched when about to establish stripes/solids */
				if (curPlayer->pendingSetColor) {
					curPlayer->pendingSetColor = false;
					balls1unTot += balls1un;
					balls2unTot += balls2un;
					balls1un = 0;
					balls2un = 0;
				}
				
					/* Scratched after shooting one of own balls in */
				if (curPlayer->pendingBallInc) {
					curPlayer->pendingBallInc = 0;
					for (auto& b:balls)
					   if (b.needRespot)
						   respot(b);
				}

					/* In all cases respot after scratch */
				spotCueBall();
			}
			
				/* No scratch */
			else {
					/* Nothing needs respotting */
				for (auto& b:balls)
					b.needRespot = false;
				
					/* Eight ball went in called pocket with no scratch */
				if (curPlayer->pendingWin) {
					winGame(*curPlayer);
					return;
				}
				
					/* A player just sunk first majority of stripes/solids */
				if (curPlayer->pendingSetColor) {
					curPlayer->pendingSetColor = false;
					balls1unTot += balls1un;
					balls2unTot += balls2un;
					if (balls1un != balls2un) {
						bool curIsFirst = balls1un > balls2un;
						if (curIsFirst) {
							curPlayer->color = 1;
							curPlayer->c = ballColors[0];
							otherPlayer->color = 2;
							otherPlayer->c = ballColors[1];
						}
						else {
							curPlayer->color = 2;
							curPlayer->c = ballColors[1];
							otherPlayer->color = 1;
							otherPlayer->c = ballColors[0];
						}
						curPlayer->ballsPocketed += (curIsFirst ? balls1unTot : balls2unTot);
						otherPlayer->ballsPocketed += (curIsFirst ? balls2unTot : balls1unTot);
						playerTxt.setFillColor(curPlayer->c);
						changePlayer = false;
					}
					balls1un = 0;
					balls2un = 0;
				} // end pendingSetColor
				
					/* Player sunk some of own balls without scratching */
				if (curPlayer->pendingBallInc) {
					curPlayer->ballsPocketed += curPlayer->pendingBallInc;
					curPlayer->pendingBallInc = 0;
					changePlayer = false;
				}
			} // end !cueBall.inPocket
			
				/* Mark if a player is ready to shoot for eight ball */
			if (players[0].ballsPocketed == 7) {
				players[0].onEightBall = true;
			}
			if (players[1].ballsPocketed == 7) {
				players[1].onEightBall = true;
			}
			
				/* Just completed a shot so no pocket is currently called */
			for (auto& p : pockets)
				p.selected = false;
			arrowActive = false;
			
				/* Set the cue up for next shot; determine whose turn */
			setToCueBall();
			if (changePlayer) {
				swap(curPlayer, otherPlayer);
				playerTxt.setString(tS(curPlayer->num));
				playerTxt.setFillColor(curPlayer->c);
				if (curPlayer->onEightBall)
						/* Need to be able to click a pocket */
					win->setMouseCursorVisible(true);
				else
					win->setMouseCursorVisible(false);
			}
		} //end if allStill
	} //end if cueBallActive
} //end update

void State::draw ()
{
		/* Splash screen */
	if (showSplash) {
		win->draw(splashTitle);
		win->draw(splashByline);
		return;
	}
	
	win->draw(tabSpr);
	win->draw(powerBar[0]);
	win->draw(powerBar[1]);
	for (auto& p:pockets)
		win->draw(p.spr);
	for (auto& b:balls)
		win->draw(b.spr);
	win->draw(cueBall.spr);
	win->draw(cue.spr);
	if (showGuide && !cueBallActive && !gameOver)
		win->draw(guideline);
	win->draw(txt);
	win->draw(playerTxt);
	if(showInstr)
		win->draw(instrTxt);
	if (curPlayer->onEightBall && !gameOver)
		win->draw(eightBallInd);
	if (indexWhich(pockets, [&](auto& p) { return p.selected; }) > -1)
		win->draw(calledPocketInd);
}

void State::assembleTable ()
{
		/* Set up RenderTexture used to draw all table elements */
	Texture tex;
	rt.create(tableWidth + teThick * 2, tableHeight + teThick * 2);
	rt.clear(Color::Transparent);
	auto rtsz = toVecF(rt.getTexture().getSize());
	float halfEdge = teThick / 2;

		/* We need to draw a solid rectangle first; the flood-fill
		 * (with random color deviation) algorithm will follow the
		 * area of that arbitrary color to make table felt
		 */
	RectangleShape r;
	r.setSize({tableWidth + 2, tableHeight + 2});
	centerOrigin(r);
	r.setFillColor(Color::Blue);
	r.setPosition(rtsz / 2.f);
	rt.draw(r);
	
		/* Fill the rectangle with pixels that deviate slightly from
		 * the chosen green color
		 */
	ZImage zim {rt.getTexture().copyToImage()};
	zim.fillInWithColor({100, 100}, Color(60, 137, 47), 5);
	tex.loadFromImage(zim);
	Sprite surfaceSpr(tex);
	rt.draw(surfaceSpr);

		/* The .png used for table edge wasn't the desired color,
		 * so use ZImage to modify it before drawing with it
		 */
	int darkenVal = 50;
	tex.loadFromFile((Resources::executingDir() / "resources" / "images" / "rail.png").string());
	ZImage zim2 {tex.copyToImage()};
	zim2.prportDarken(darkenVal);
	tex.loadFromImage(zim2);
	tex.setRepeated(true);
	Sprite railSpr(tex);
	
		/* Draw table rails by sizing the texture rectangles of repeating texture */
	railSpr.setTextureRect(IntRect(0, 0, tableWidth, teThick));
	centerOrigin(railSpr);
	railSpr.setPosition(rtsz.x / 2, halfEdge);
	rt.draw(railSpr);
	railSpr.move(0, tableHeight + teThick);
	railSpr.setScale(1, -1);
	rt.draw(railSpr);
	railSpr.rotate(90);
	railSpr.setTextureRect(IntRect(0, 0, tableHeight, teThick));
	centerOrigin(railSpr);
	railSpr.setPosition(halfEdge, rtsz.y / 2);
	rt.draw(railSpr);
	railSpr.setScale(1, 1);
	railSpr.move(tableWidth + teThick, 0);
	rt.draw(railSpr);

		/* Table corners need same color adjustment as rail sprite */
	tex.loadFromFile((Resources::executingDir() / "resources" / "images" / "tabcorn.png").string());
	ZImage zim3 {tex.copyToImage()};
	zim3.prportDarken(darkenVal);
	tex.loadFromImage(zim3);
	Sprite tabCornSpr(tex);
	
		/* Draw table corners */
	centerOrigin(tabCornSpr);
	tabCornSpr.setPosition(halfEdge, halfEdge);
	rt.draw(tabCornSpr);
	tabCornSpr.rotate(90);
	tabCornSpr.move(tableWidth + teThick, 0);
	rt.draw(tabCornSpr);
	tabCornSpr.rotate(90);
	tabCornSpr.move(0, tableHeight + teThick);
	rt.draw(tabCornSpr);
	tabCornSpr.rotate(90);
	tabCornSpr.move(-(tableWidth + teThick), 0);
	rt.draw(tabCornSpr);
	
		/* Create diamond-shaped sights with a VertexArray. Make a larger one
		 * first and then a smaller diamond of different color to give it
		 * an outline color
		 */
	RenderTexture sightsRt;
	sightsRt.create(7, 15);
	sightsRt.clear(Color::Transparent);
	VertexArray va {TriangleFan};
	Color c1 = decreaseBrightness(Color::Yellow, 20);
	Color c2 {215, 215, 205}; // Grayish
	va.appendCoordsC(3, 14, c1);
	va.appendCoordsC(0, 7, c1);
	va.appendCoordsC(3, 0, c1);
	va.appendCoordsC(6, 7, c1);
	sightsRt.draw(va);
	va.clear();
	va.appendCoordsC(3, 13, c2);
	va.appendCoordsC(1, 7, c2);
	va.appendCoordsC(3, 1, c2);
	va.appendCoordsC(5, 7, c2);
	sightsRt.draw(va);
	sightsRt.display();
	Sprite sightSpr(sightsRt.getTexture());
	centerOrigin(sightSpr);
	
		/* Position and draw the sights to the render texture */
	float hrtx = rtsz.x / 2;
	float hrty = rtsz.y / 2 ;
	float hth = tableHeight / 2;
	float htw = tableWidth / 2;
	float frac = .25;
	for (int i = 0; i < 3; ++i, frac += .25) {
		sightSpr.setRotation(0);
		sightSpr.setPosition(hrtx - htw * frac, hrty - hth - halfEdge - 10);
		rt.draw(sightSpr);
		sightSpr.setPosition(hrtx - htw * frac, hrty + hth + halfEdge + 10);
		rt.draw(sightSpr);
		sightSpr.setPosition(hrtx + htw * frac, hrty - hth - halfEdge - 10);
		rt.draw(sightSpr);
		sightSpr.setPosition(hrtx + htw * frac, hrty + hth + halfEdge + 10);
		rt.draw(sightSpr);
		sightSpr.rotate(90);
		sightSpr.setPosition(halfEdge - 10, hrty - hth + tableHeight * frac);
		rt.draw(sightSpr);
		sightSpr.move(tableWidth + teThick + 20, 0);
		rt.draw(sightSpr);
	}

		/* Finalize the render texture and set the Sprite that will
		 * be used every frame by draw()
		 */
	rt.display();
	tabSpr.setTexture(rt.getTexture());
	centerOrigin(tabSpr);
	tabSpr.setPosition(scrcx, scrcy);
}

void State::reset ()
{
    for (auto& p : players)
        p.reset();
    curPlayer = &players[0];
    otherPlayer = &players[1];
    gameOver = false;
    running = true;
    cueBallActive = false;
    pullingBack = false;
	placingCueBall = false;
	arrowPlus = true;
	isBreakShot = true;
	arrowSkew = 0;
    balls1un = 0;
    balls2un = 0;
    balls1unTot = 0;
    balls2unTot = 0;
	
        /* Give break shot a little variation */
	cue.angle = czdg((randRange(0, 400) - 200) / 100.f);
	cue.spr.setRotation(cue.angle);
    cueBall.spr.sP(scrcx - 400, yLine + (randRange(0, 400) - 200) / 100.f);
	
    cueBall.inPocket = false;
    cueBall.setVelocity(0, 0);
    cueBall.spr.setColor(Color::White);
	setToCueBall();
    forNum(15) {
        balls[i].spr.sP(locList[i]);
        balls[i].inPocket = false;
        balls[i].setVelocity(0, 0);
        Color c = balls[i].spr.getColor();
        c.a = 255;
        balls[i].spr.setColor(c);
    }
    for (auto& p : pockets)
        p.selected = false;
    powerBar[1].setScale(1, 0);
    txt.setString("");
    playerTxt.setString(tS(curPlayer->num));
    playerTxt.setFillColor(curPlayer->c);
    
    win->setMouseCursorVisible(false);
}

void State::setToCueBall ()
{
    cue.cueEnd = cueBall.spr.gP();
    cue.spr.sP(cue.cueEnd.x - toRect(cue.centerOffset, cue.angle).x,
             cue.cueEnd.y - toRect(cue.centerOffset, cue.angle).y);
    powerBar[1].setScale(1, 0);
}

void State::updateGuide () {
	 
	guideline.clear();
	vecF ogn = cueBall.spr.gP();
	Color c = Color(200, 200, 200);
	for (int i = 0; i < guideLength; i += 8) {
		guideline.appendPtC(ogn + pVec(i, cue.angle), c);
		guideline.appendPtC(ogn + pVec(i + 2, cue.angle), c);
	}
}

void State::launch ()
{
	cueBallActive = true;
	pullingBack = false;
	placingCueBall = false;
	setToCueBall();
	cueBall.spr.setColor(Color::White);
	cueBall.setVelocity(toRect(cue.power, cue.angle));
	cue.power = 0;
	Resources::getSound("shoot").play();
	if (curPlayer->onEightBall)
		curPlayer->onEightAtLaunch = true;
}

void State::frictionAndFindHighSpeed (Ball& cur, float& highSpd)
{
	vecF vpp = cur.vp;
	vpp.x -= friction;
	if (vpp.x < speedClamp) {
		vpp.x = 0;
	}
	cur.setVelocity(toRect(vpp));
	if (vpp.x > highSpd)
		highSpd = vpp.x;
}

void State::moveCueBall (float x, float y)
{
		/* Keep cue ball within kitchen and table bounds */
	auto oldPos = cueBall.spr.getPosition();
	float htw = tableWidth / 2;
	float hth = tableHeight / 2;
	float tew = ballRad + scrcx - htw; // tableEdgeWest, etc.
	float kitchen = ballRad + scrcx - (htw * .5);
	float ten = ballRad + scrcy - hth;
	float tes = -ballRad + scrcy + hth;
	
	vecf newPos {clamp(oldPos.x + x, tew, kitchen),
				clamp(oldPos.y + y, ten, tes)};
    cueBall.spr.setPosition(newPos);
    cue.spr.move(newPos - oldPos);
    cue.cueEnd = cueBall.spr.gP();
}

void State::pocketBall (Ball& b, Pocket& p)
{
    b.inPocket = true;
    b.setVelocity(0, 0);
    b.spr.sP(p.spr.gP() + p.pocketedOffs);
    Color c = b.spr.getColor();
    c.a = 100;
    b.spr.setColor(c);
    Resources::getSound("pocketBall").play();
	
    if (b.color == 4) {  //cueBall
       if (curPlayer->onEightAtLaunch)
            curPlayer->pendingLoss = true;
        else return;
    }
    else if (!b.color) {      //eight ball
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
       if (players[0].num != curPlayer->num)
            ++players[0].ballsPocketed;
        else ++players[1].ballsPocketed;
    }
}

void State::spotCueBall ()
{
    cueBall.spr.setColor(ORANGE);
    cueBall.inPocket = false;
    cueBall.spr.sP(scrcx - 400, yLine);
	
		/* Make sure we don't place the cue ball on top
		 * of an existing ball
		 */
    bool checkAgain;
	do {
		checkAgain = false;
        for (int bb = 0; bb < 15; ++bb) {
            Ball& cur2 = balls[bb];
           if (cur2.inPocket)
                continue;
            float radius = cur2.spr.gGB().width / 2;
            vecF pos = cur2.spr.gP();
            vecF cpos = cueBall.spr.gP();
            float hyp1 = hyp(cpos, pos);
            if (hyp1 > radius + radius)
                continue;
            checkAgain = true;
            cueBall.spr.move(0, -5);
        }
    }
	while (checkAgain);
	placingCueBall = true;
}

void State::respot (Ball& b)
{
    b.needRespot = false;
    Color c = b.spr.getColor();
    c.a = 255;
    b.spr.setColor(c);
    b.inPocket = false;
    b.spr.sP(scrcx + firstBallCXOffset, yLine);
    bool checkAgain;
	do {
		checkAgain = false;
		for (int bb = 0; bb < 15; ++bb) {
			Ball& cur2 = balls[bb];
			if (&b == &cur2)
				continue;
			if (cur2.inPocket)
				continue;
            float radius = cur2.spr.gGB().width / 2;
            vecF pos = cur2.spr.gP();
            vecF cpos = b.spr.gP();
            float hyp1 = hyp(cpos, pos);
            if (hyp1 > radius + radius)
				continue;
            checkAgain = true;
            b.spr.move(2, 0);
			if (b.spr.gP().x > scrcx + tableWidth / 2 - ballRad)
				b.spr.move(-2, 2);
        }
    }
	while (checkAgain);
}

void State::animateArrow ()
{
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

void State::winGame (Player& p)
{
	gameOver = true;
	txt.setString("Player " + tS(p.num) + " wins!\n(Y to restart)");
	txt.setFillColor(p.c);
}

void State::lose (Player& p)
{
	gameOver = true;
	Player& winner = (players[0].num == p.num ? players[1] : players[0]);
	txt.setString("Player " + tS(winner.num) + " wins!\n(Y to restart)");
	txt.setFillColor(winner.c);
}

float State::xLoc (int idx)
{
	return firstBallLoc.x + (xOfs * idx) + (2 * idx);
}

float State::yLoc (int ofs, bool yl /*=false*/)
{
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

