//
//  sfmlApp.hpp
//  Billiards
//
//  Created by John Ziegler on 10/22/24.
//  Copyright © 2024 John Ziegler. All rights reserved.
//

#ifndef sfmlApp_hpp
#define sfmlApp_hpp


#include "billiards.h"


class SFGameWindow {
friend class Game;
public:
    
    SFGameWindow();
    SFGameWindow(const string& title, const vecU& size);
    ~SFGameWindow() { destroy(); }
    void draw(Drawable& d) { window.draw(d); }
    void beginDraw() { window.clear(redrawColor); }
    void endDraw() { window.display(); }
    bool isDone() { return _isDone; }
    bool isFullscreen() { return _isFullscreen; }
    bool isFocused() { return _isFocused; }
    vecU getWindowSize() { return windowSize; };
    RenderWindow* getRenderWindow() { return &window; };
    void close() { _isDone = true; };
    void toggleFullscreen();
        
private:
    
    void destroy() { window.close(); };
    void setup(const string& title, const vecU& size);
    void create();
    
    Image                   icon;
    RenderWindow            window;
    string                  windowTitle;
    vecU                    windowSize;
    bool                    _isDone;
    bool                    _isFullscreen;
    bool                    _isFocused;
    Color                   redrawColor { Color::Black };
    static const int        defaultWidth { 1280 };
    static const int        defaultHeight { 720 };
};




class Game {
public:
    
    Game();
    ~Game() { };
    void update();
    void render() {
        window.beginDraw();
        state.draw();
        window.endDraw();
        }
    void lateUpdate() { restartClock(); }
    SFGameWindow* getWindow() { return &window; };
    Time getElapsed() { return elapsed; };
    void restartClock() { elapsed += clock.restart(); };

private:

    SFGameWindow     window;
    State            state;
    Clock            clock;
    Time            elapsed;
};




#endif /* sfmlApp_hpp */
