// Simple snake game
// Move with arrow keys
// (WIP)

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <array>
#include <list>

using std::cin, std::cout, std::endl;
using std::string;
using std::array;
using std::list;

bool init();
bool loop();
void game();
void render();
void kill();

const float TARGET_FRAMERATE = 10;
const int WINDOW_WIDTH = 480;
const int WINDOW_HEIGHT = 480;
const int GRID_SIZE = 12;
const array<int, 3> COLOR_SNAKE_RGB = {255, 255, 255};
const array<int, 3> COLOR_BACKGROUND_RGB = {0, 0, 0};
const int SNAKE_NONE = 0;
const int SNAKE_UP = 1;
const int SNAKE_RIGHT = 2;
const int SNAKE_DOWN = 3;
const int SNAKE_LEFT = 4;
const int START_X = WINDOW_WIDTH/2/GRID_SIZE;
const int START_Y = WINDOW_HEIGHT/2/GRID_SIZE;

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* gameSurface;

SDL_Event event;

// Rect used for blitting everything to the screen, respecting the grid
SDL_Rect rendererRect = {0, 0, GRID_SIZE, GRID_SIZE};

Uint64 ticksLast = 0;
float deltaTime = 0; // In seconds

// For saving keyboard inputs
array<bool, SDL_NUM_SCANCODES> keyStates = {false};

// For saving keyboard inputs for one frame
array<bool, SDL_NUM_SCANCODES> keyStatesTap = {false};

// Represents a piece of the snake (head or body)
class SnakeSegment {
	private:
		int x;
		int y;
	public:
		SnakeSegment(int x, int y) {
			this->x = x;
			this->y = y;
		}

		int getX() {
			return this->x;
		}
		int getY() {
			return this->y;
		}

		void setX(int x) {
			this->x = x;
		}
		void setY(int y) {
			this->y = y;
		}
};

// The movable snake
class {
	private:
		int direction = SNAKE_NONE;
		list<SnakeSegment> segments = {
			SnakeSegment(START_X, START_Y),
			SnakeSegment(START_X, START_Y),
			SnakeSegment(START_X, START_Y),
			SnakeSegment(START_X, START_Y),
			SnakeSegment(START_X, START_Y),
			SnakeSegment(START_X, START_Y)
		};

		// The current segment at the head of the snake
		list<SnakeSegment>::iterator head = this->segments.begin();
	public:
		int 				getDirection() const { return this->direction; }
		list<SnakeSegment>&	getSegments()		 { return this->segments; }

		void turn(int direction) {
			// Disable 180 degree turns
			if (
				   this->direction == SNAKE_LEFT && direction == SNAKE_RIGHT
				|| this->direction == SNAKE_RIGHT && direction == SNAKE_LEFT
				|| this->direction == SNAKE_UP && direction == SNAKE_DOWN
				|| this->direction == SNAKE_DOWN && direction == SNAKE_UP
			) {
				return;
			}

			this->direction = direction;
		}
		void move() {
			if (this->direction == SNAKE_NONE) return;

			// Save coordinates of the current head
			int headX = (*this->head).getX();
			int headY = (*this->head).getY();

			// Set the head to be the next segment in the list, which will
			// always be the segment at the tip of the tail
			this->head++;

			// Loop around and pick the first element if it's trying to choose
			// an element beyond the end boundary
			if (this->head == this->segments.end()) {
				this->head = this->segments.begin();
			}

			switch (this->direction) {
				case SNAKE_UP:
					this->head->setX(headX);
					this->head->setY(headY - 1);
					break;
				case SNAKE_RIGHT:
					this->head->setX(headX + 1);
					this->head->setY(headY);
					break;
				case SNAKE_DOWN:
					this->head->setX(headX);
					this->head->setY(headY + 1);
					break;
				case SNAKE_LEFT:
					this->head->setX(headX - 1);
					this->head->setY(headY);
					break;
			}
		}
} snake;

int main(int argc, char** argv) {
	if (!init()) return 1;

	while (loop()) {}

	kill();
	return 0;
}

// Event loop
bool loop() {
	deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;

	if (deltaTime < 1/TARGET_FRAMERATE) return true; // Framerate cap

	ticksLast = SDL_GetTicks64();

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					// Quit by pressing Escape
					case SDL_SCANCODE_ESCAPE:
						return false;
				}

				if (!keyStates[event.key.keysym.scancode]) {
					keyStates[event.key.keysym.scancode] = true;
					keyStatesTap[event.key.keysym.scancode] = true;
				}
				break;
			case SDL_KEYUP:
				keyStates[event.key.keysym.scancode] = false;
				break;
		}
	}

	game();
	render();

	SDL_BlitSurface(gameSurface, NULL, winSurface, NULL);
	SDL_UpdateWindowSurface(window);
	return true;
}

// Game logic
void game() {
	  if 		(keyStatesTap[SDL_SCANCODE_UP])		{ snake.turn(SNAKE_UP);
	} else if	(keyStatesTap[SDL_SCANCODE_RIGHT])	{ snake.turn(SNAKE_RIGHT);
	} else if	(keyStatesTap[SDL_SCANCODE_DOWN])	{ snake.turn(SNAKE_DOWN);
	} else if	(keyStatesTap[SDL_SCANCODE_LEFT])	{ snake.turn(SNAKE_LEFT);
	}

	snake.move();
	keyStatesTap = {false};
}

// Clears screen and draws everything
void render() {
	Uint32 backgroundColor = SDL_MapRGB(
		gameSurface->format,
		COLOR_BACKGROUND_RGB[0],
		COLOR_BACKGROUND_RGB[1],
		COLOR_BACKGROUND_RGB[2]
	);

	Uint32 snakeColor = SDL_MapRGB(
		gameSurface->format,
		COLOR_SNAKE_RGB[0],
		COLOR_SNAKE_RGB[1],
		COLOR_SNAKE_RGB[2]
	);

	// Clear screen before drawing
	SDL_FillRect(gameSurface, NULL, backgroundColor);

	// Draw all segments
	for (auto &segment : snake.getSegments()) {
		rendererRect.x = segment.getX()*GRID_SIZE;
		rendererRect.y = segment.getY()*GRID_SIZE;

		SDL_FillRect(gameSurface, &rendererRect, snakeColor);
	}
}

// Initialize SDL
bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow("Template", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	winSurface = SDL_GetWindowSurface(window);
	if (!winSurface) {
		cout << "Error getting surface: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	// Temporary surface, will be formatted into the game surface
	SDL_Surface* temp = SDL_CreateRGBSurfaceWithFormat(
		winSurface->flags,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		winSurface->format->BitsPerPixel,
		winSurface->format->format
	);
	if (!temp) {
		cout << "Error creating game surface: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	// Convert game surface to window surface for faster blitting
	gameSurface = SDL_ConvertSurface(temp, winSurface->format, 0);
	if (!gameSurface) {
		cout << "Error formatting game surface: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	return true;
}

// Quit SDL2
void kill() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}