
#include "sfmlApp.hpp"


SFGameWindow::SFGameWindow() {
    setup("Billiards", vecU(defaultWidth, defaultHeight));
}

SFGameWindow::SFGameWindow(const string& title, const vecU& size) {
	setup(title, size);
}
	
void SFGameWindow::setup(const string& title, const vecU& size) {
	windowTitle = title;
	windowSize = size;
	_isFullscreen = false;
	_isDone = false;
	_isFocused = true;
	create();
	}

void SFGameWindow::create() {
    VideoMode vm = VideoMode::getDesktopMode();
    auto fsmodes = VideoMode::getFullscreenModes();
    VideoMode mode = _isFullscreen ? fsmodes.at(fsmodes.size()-1) : vm;  /////
    auto style = _isFullscreen ? Style::Fullscreen : Style::Default;
    window.create( mode, windowTitle, style);
    if (!icon.loadFromFile("media/icon.png")) {
        cerr << "Couldn\'t load icon! \n";
    }
    else
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}
	
void SFGameWindow::toggleFullscreen() {
	_isFullscreen = !_isFullscreen;
	destroy();
	create();
}



Game::Game() : window("Billiards", vecU( VideoMode::getDesktopMode().width,
                                        VideoMode::getDesktopMode().height )) {
    srand(unsigned(time(nullptr)));
    auto rw = window.getRenderWindow();
    rw->setFramerateLimit(60);
    state.w = rw;
    state.onCreate();
    clock.restart();
}

void Game::update() {
    Event event;
    while (window.window.pollEvent(event)) {
        switch(event.type) {
            case Event::MouseMoved:
                state.mx = event.mouseMove.x;
                state.my = event.mouseMove.y;
                break;
            case Event::KeyPressed:
                switch(event.key.code) {
                    case Keyboard::Escape:
                        window.close();
                    case Keyboard::F5:
                        window.toggleFullscreen();
                    default:
                        state.onKeyPress(event.key.code);
                }
            case Event::MouseButtonPressed:
                state.onMouseDown(event.mouseButton.x, event.mouseButton.y);  break;
            case Event::MouseButtonReleased:
                state.onMouseUp(event.mouseButton.x, event.mouseButton.y);  break;
            case Event::Closed:
                window.close();  break;
            case Event::LostFocus:
                window._isFocused = false;  break;
            case Event::GainedFocus:
                window._isFocused = true;  break;
            default:
                break;
        }
    }
    state.update(elapsed);
}



int main() {
        /* XCode folly: two instances of the program are launched if * we customize the working directory. We can cause the extraneous
         * instance to silenty quit immediately by giving it a relative
         * path that it can't find.
         */
    Image img;
   if (!img.loadFromFile("media/icon.png"))
        return EXIT_FAILURE;
    
        /* Apparently SFML 2.6.1 fullscreen is not properly functional on
         * ARM Macs; this is for troubleshooting.
         */
//    auto ms = VideoMode::getFullscreenModes();
//    for(size_t i = 0; i < ms.size(); ++i) {
//        cout << i << ": " << ms[i].width << "-" << ms[i].height << "-" << ms[i].bitsPerPixel << endl;
//    }
    

    Game game;
    while (!game.getWindow()->isDone()) {
        game.update();
        game.render();
        game.lateUpdate();
    }
}
