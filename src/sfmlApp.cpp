
#include "sfmlApp.hpp"
#include "resourcemanager.hpp"

SFGameWindow::SFGameWindow ()
{
	setup("ZBilliards", vecU(defaultWidth, defaultHeight));
}

SFGameWindow::SFGameWindow (const string& title, const vecU& size)
{
	setup(title, size);
}
	
void SFGameWindow::setup (const string& title, const vecU& size)
{
	windowTitle = title;
	windowSize = size;
	_isFullscreen = true;
	_isDone = false;
	_isFocused = true;
	create();
    if (!loadByMethod(icon, iconPath)) {
        cerr << "Couldn\'t load icon! \n";
    }
	else
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void SFGameWindow::create ()
{
    VideoMode mode = VideoMode::getDesktopMode();
	auto style = Style::Default;
	if (_isFullscreen) {
		auto fsmodes = VideoMode::getFullscreenModes();
		if (fsmodes.size()) {
			mode = fsmodes[0];
			style = Style::Fullscreen;
		}
		else _isFullscreen = false;
	}
	window.create(mode, windowTitle, style);
	window.setFramerateLimit(60);
}

void SFGameWindow::toggleFullscreen ()
{
	_isFullscreen = !_isFullscreen;
	destroy();
	create();
}


Game::Game ()
	: window()
{
    srand(unsigned(time(nullptr)));
    state.w = window.getRenderWindow();
	state.gw = &window;
	state.timedMgr = &timedMgr;
    state.onCreate();
    clock.restart();
}

void Game::update ()
{
	state.mxOld = state.mx;
	state.myOld = state.my;
    Event event;
    while (window.window.pollEvent(event)) {
		vecf adj = window.window.mapPixelToCoords(vecI(event.mouseButton.x,
													   event.mouseButton.y));
        switch(event.type) {
				
            case Event::KeyPressed:
                switch(event.key.code) {
                    
					case Keyboard::F5:
                        window.toggleFullscreen();
						break;
						
                    default:
                        state.onKeyPress(event.key.code);
						break;
                }
                break;
				
			case Event::KeyReleased:
				state.onKeyRelease(event.key.code);
                break;
				
			case Event::MouseMoved:
				state.mx = int(adj.x);
				state.my = int(adj.y);
				break;
				
            case Event::MouseButtonPressed:
                state.onMouseDown(int(adj.x), int(adj.y));
				break;
				
            case Event::MouseButtonReleased:
				state.onMouseUp(int(adj.x), int(adj.y));
				break;
				
            case Event::Closed:
                window.close();
				break;
				
            case Event::LostFocus:
                window._isFocused = false;
				break;
				
            case Event::GainedFocus:
                window._isFocused = true;
				break;
				
            default:
                break;
        }
    }
    state.update(elapsed);
}


int main (int argc, char* argv[])
{
        /* XCode folly: two instances of the program are launched if
		 * we customize the working directory. We can cause the extraneous
         * instance to silently quit immediately by giving it a relative
         * path that it can't find.
         */
    Image img;
	if (!img.loadFromFile(iconPath))
        return EXIT_FAILURE;
    
	Resources::initialize(argc, argv);
	
    Game game;
    while (!game.getWindow()->isDone()) {
        game.update();
        game.render();
        game.lateUpdate();
    }
}
