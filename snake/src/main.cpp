// Simple snake game
// Move with arrow keys and collect apples

//TODO: score tracking
//TODO: options menu (apple count, speed)

//TODO-OPT: high score tracking
//TODO-OPT: silly modes (wrap around, bombs)

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <random>
#include <iterator>

using std::cin, std::cout, std::endl;
using std::string;
using std::array;
using std::vector;
using std::list;
using std::random_device, std::mt19937, std::uniform_int_distribution;
using std::next;

bool init();
bool doEvents();
void doGame();
void doRender();
void kill();

const int CELLS_SIZE = 15;
const int CELLS_HORIZONTAL = 16;
const int CELLS_VERTICAL = 16;
const int START_X = CELLS_HORIZONTAL/2;
const int START_Y = CELLS_VERTICAL/2;
const float SNAKE_MOVE_DELAY = 150; // In milliseconds
const float FLICKER_LOOP_DELAY = 1; // In seconds
const SDL_Color COLOR_BACKGROUND_RGB = {31, 31, 31, 255};
const SDL_Color COLOR_SNAKE_RGB = {255, 255, 255, 255};
const SDL_Color COLOR_APPLE_RGB = {255, 63, 63, 255};

const int SNAKE_DIR_NONE = 0;
const int SNAKE_DIR_UP = 1;
const int SNAKE_DIR_DOWN = 2;
const int SNAKE_DIR_LEFT = 3;
const int SNAKE_DIR_RIGHT = 4;
const int GS_LAUNCHED = 0;
const int GS_PLAYING = 1;
const int GS_GAMEOVER = 2;

string checkCellContent(int x, int y);

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* gameSurface;

SDL_Event event;

Uint64 ticksLast = 0;
float deltaTime = 0; // In seconds

// For blitting everything to the screen, respecting the grid
SDL_Rect gameRendererRect = {0, 0, CELLS_SIZE, CELLS_SIZE};

// For saving keyboard inputs
array<bool, SDL_NUM_SCANCODES> keyStates = {false};

// For saving keyboard inputs for one frame
array<bool, SDL_NUM_SCANCODES> keyStatesTap = {false};

// Global game state, defines what part of the game logic should run
int gameState = GS_LAUNCHED;

// Counts down to zero before allowing the snake to move
float snakeMoveTimer = SNAKE_MOVE_DELAY;

// Global object flickering timer, affected objects are invisible whenever the
// timer is on its higher half (i.e. higher than FLICKER_LOOP_DELAY/2)
float flickerTimer = 0;

// Random number generator
mt19937 rng(random_device{}());

// For generating random coordinates within the grid
uniform_int_distribution<int> distGridX(0, CELLS_HORIZONTAL - 1);
uniform_int_distribution<int> distGridY(0, CELLS_VERTICAL - 1);

// Represents a piece of the snake (head, body or tail)
class SnakeSegment {
	private:
		int x;
		int y;
	public:
		SnakeSegment(int x, int y) {
			this->x = x;
			this->y = y;
		}

		int getX() const {
			return this->x;
		}
		int getY() const {
			return this->y;
		}

		void setX(int x) {
			this->x = x;
		}
		void setY(int y) {
			this->y = y;
		}
};

// The snake the player controls
class {
	private:
		// Direction the snake is facing
		int direction = SNAKE_DIR_NONE;

		// Stores buffered inputs for turning the snake
		// Inputs are registered in FIFO order
		array<int, 2> bufferedDirection = {SNAKE_DIR_NONE};

		// The segments that make up the snake
		list<SnakeSegment> segments = {
			SnakeSegment(START_X, START_Y),
			SnakeSegment(START_X, START_Y),
			SnakeSegment(START_X, START_Y),
			SnakeSegment(START_X, START_Y)
		};

		// The segment currently at the head of the snake
		list<SnakeSegment>::iterator head = this->segments.begin();

		// Stores the position of the tail segment from the previous movement
		struct {
			int x;
			int y;
		} lastTailPos;
	public:
		int							getDirection() const { return this->direction; }
		const list<SnakeSegment>&	getSegments() const	 { return this->segments; }
		const SnakeSegment&			getHead() const		 { return *this->head; }

		// Doesn't immediately turn the snake, but rather buffers a turn
		// Returns false if it's a repeat input (e.g. pressing right when
		// you're already going right)
		bool turn(int direction) {
			if (this->bufferedDirection[0] == SNAKE_DIR_NONE) {
				if (this->direction == direction) {
					return false;
				}

				this->bufferedDirection[0] = direction;
			} else {
				if (this->bufferedDirection[0] == direction) {
					return false;
				}

				this->bufferedDirection[1] = direction;
			}

			return true;
		}
		// Returns false when bumping into solid things
		bool move() {
			// Move the second buffered input to the front, if the first one is
			// empty
			if (this->bufferedDirection[0] == SNAKE_DIR_NONE) {
				this->bufferedDirection[0] = this->bufferedDirection[1];
				this->bufferedDirection[1] = SNAKE_DIR_NONE;
			}

			// Stay still if you have no direction
			if (this->direction == SNAKE_DIR_NONE
			&&  this->bufferedDirection[0] == SNAKE_DIR_NONE) {
				return true;
			}

			// Prevent illegal 180 degree turning
			if ((this->bufferedDirection[0] != SNAKE_DIR_NONE)
			&&  !((this->direction == SNAKE_DIR_UP && this->bufferedDirection[0] == SNAKE_DIR_DOWN)
			||    (this->direction == SNAKE_DIR_DOWN && this->bufferedDirection[0] == SNAKE_DIR_UP)
			||    (this->direction == SNAKE_DIR_LEFT && this->bufferedDirection[0] == SNAKE_DIR_RIGHT)
			||    (this->direction == SNAKE_DIR_RIGHT && this->bufferedDirection[0] == SNAKE_DIR_LEFT))) {
				// Set direction to the first buffered input in line
				this->direction = this->bufferedDirection[0];
			}

			// Move the second buffered input to the front for use in the next
			// movement
			this->bufferedDirection[0] = this->bufferedDirection[1];
			this->bufferedDirection[1] = SNAKE_DIR_NONE;

			// Where to move the head of the snake
			int targetX = (*this->head).getX();
			int targetY = (*this->head).getY();

			// Adjust for movement direction
			switch (this->direction) {
				case SNAKE_DIR_UP:
					targetY -= 1;
					break;
				case SNAKE_DIR_DOWN:
					targetY += 1;
					break;
				case SNAKE_DIR_LEFT:
					targetX -= 1;
					break;
				case SNAKE_DIR_RIGHT:
					targetX += 1;
					break;
			}

			if (checkCellContent(targetX, targetY) == "wall") {
				return false;
			} else if (checkCellContent(targetX, targetY) == "snake") {
				auto tail = next(this->head, 1);

				if (tail == this->segments.end()) {
					tail = this->segments.begin();
				}

				// If blocked by a body piece, only stop if it's not the tail
				// The tail would move out of the way next frame anyway, so
				// this gives the player some more wiggle room
				if (targetX != tail->getX() || targetY != tail->getY()) {
					return false;
				}
			}

			// Set the head to be the next segment in the list, which will
			// always be the segment at the tip of the tail
			this->head++;
			if (this->head == this->segments.end()) {
				this->head = this->segments.begin();
			}

			// Save the new head segment's position (still positioned at the
			// tail)
			this->lastTailPos.x = this->head->getX();
			this->lastTailPos.y = this->head->getY();

			// Teleport the new head
			this->head->setX(targetX);
			this->head->setY(targetY);

			return true;
		}
		void grow() {
			// Determine where to insert the newly grown segment
			// Should always be on the tail, that is, the element right after
			// the head on the segment list
			auto insertIndex = next(this->head, 1);
			if (insertIndex == this->segments.end()) {
				insertIndex = this->segments.begin();
			}

			// Spawn the new segment
			this->segments.insert(
				insertIndex,
				SnakeSegment(this->lastTailPos.x, this->lastTailPos.y)
			);
		}
		// Return the snake to its starting state
		void reset() {
			this->direction = SNAKE_DIR_NONE;
			this->bufferedDirection = {SNAKE_DIR_NONE};
			this->segments.erase(
				next(this->segments.begin(), 4),
				this->segments.end()
			);
			this->head = this->segments.begin();

			for (auto& segment : this->segments) {
				segment.setX(START_X);
				segment.setY(START_Y);
			}
		}
} snake;

// Apples that increase your length when eaten
class Apple {
	private:
		int x;
		int y;
	public:
		Apple(int x, int y) {
			this->x = x;
			this->y = y;
		}

		int getX() const { return this->x; }
		int getY() const { return this->y; }

		void setX(int x) { this->x = x; }
		void setY(int y) { this->y = y; }
};

// For storing all apples currently in game
vector<Apple> apples = {};

int main(int argc, char** argv) {
	if (!init()) return 1;

	while (true) {
		deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;	
		ticksLast = SDL_GetTicks64();

		if (!doEvents()) break;
		doGame();
		doRender();
	}

	kill();
	return 0;
}

// Event loop
bool doEvents() {
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

	return true;
}

// Game logic
void doGame() {
	switch (gameState) {
		case GS_LAUNCHED:
			flickerTimer += 1*deltaTime;

			if (flickerTimer > FLICKER_LOOP_DELAY) {
				flickerTimer -= FLICKER_LOOP_DELAY;
			}

			if (keyStatesTap[SDL_SCANCODE_SPACE]
			||  keyStatesTap[SDL_SCANCODE_RETURN]) {
				gameState = GS_PLAYING;
				flickerTimer = 0;

				// Spawn apple
				apples.push_back(Apple(distGridX(rng), distGridY(rng)));
			}

			break;
		case GS_PLAYING:
		{
			if		(keyStatesTap[SDL_SCANCODE_UP])		{ snake.turn(SNAKE_DIR_UP); }
			else if	(keyStatesTap[SDL_SCANCODE_DOWN])	{ snake.turn(SNAKE_DIR_DOWN); }
			else if	(keyStatesTap[SDL_SCANCODE_LEFT])	{ snake.turn(SNAKE_DIR_LEFT); }
			else if	(keyStatesTap[SDL_SCANCODE_RIGHT])	{ snake.turn(SNAKE_DIR_RIGHT); }
		
			snakeMoveTimer -= 1000*deltaTime;

			if (snakeMoveTimer < 0) {
				// Attempt to move the snake
				if (!snake.move()) {
					gameState = GS_GAMEOVER;
					flickerTimer = FLICKER_LOOP_DELAY/2;
					break;
				}

				snakeMoveTimer += SNAKE_MOVE_DELAY;
			}

			// Detect if you're eating an apple after moving
			auto snakeHead = snake.getHead();

			for (auto& apple : apples) {
				if (snakeHead.getX() == apple.getX()
				&&  snakeHead.getY() == apple.getY()) {
					snake.grow();

					while (true) {
						// Pick a cell to teleport the apple to
						int checkX = distGridX(rng);
						int checkY = distGridY(rng);

						if (checkCellContent(checkX, checkY) == "empty") {
							// Teleport apple
							apple.setX(checkX);
							apple.setY(checkY);
							break;
						}
					}
				}
			}

			break;
		}
		case GS_GAMEOVER:
			flickerTimer += 1*deltaTime;

			if (flickerTimer > FLICKER_LOOP_DELAY) {
				flickerTimer -= FLICKER_LOOP_DELAY;
			}

			if (keyStatesTap[SDL_SCANCODE_SPACE]
			||  keyStatesTap[SDL_SCANCODE_RETURN]) {
				gameState = GS_LAUNCHED;
				flickerTimer = 0;

				snake.reset();
				apples.clear();
			}

			break;
	}

	// Reset tap inputs for the next frame
	keyStatesTap = {false};
}

// Clears screen and draws everything
void doRender() {
	Uint32 backgroundColor = SDL_MapRGB(
		gameSurface->format,
		COLOR_BACKGROUND_RGB.r,
		COLOR_BACKGROUND_RGB.g,
		COLOR_BACKGROUND_RGB.b
	);

	Uint32 snakeColor = SDL_MapRGB(
		gameSurface->format,
		COLOR_SNAKE_RGB.r,
		COLOR_SNAKE_RGB.g,
		COLOR_SNAKE_RGB.b
	);

	Uint32 appleColor = SDL_MapRGB(
		gameSurface->format,
		COLOR_APPLE_RGB.r,
		COLOR_APPLE_RGB.g,
		COLOR_APPLE_RGB.b
	);

	// Clear screen before drawing
	SDL_FillRect(gameSurface, NULL, backgroundColor);

	// Draw apple(s)
	for (auto& apple : apples) {
		gameRendererRect.x = apple.getX()*CELLS_SIZE;
		gameRendererRect.y = apple.getY()*CELLS_SIZE;
		
		SDL_FillRect(gameSurface, &gameRendererRect, appleColor);
	}

	// Draw snake parts whenever the flicker timer isn't supposed to hide them
	if (flickerTimer <= FLICKER_LOOP_DELAY/2) {
		for (auto& segment : snake.getSegments()) {
			gameRendererRect.x = segment.getX()*CELLS_SIZE;
			gameRendererRect.y = segment.getY()*CELLS_SIZE;

			SDL_FillRect(gameSurface, &gameRendererRect, snakeColor);
		}
	}

	SDL_BlitSurface(gameSurface, NULL, winSurface, NULL);
	SDL_UpdateWindowSurface(window);
}

// Returns what is currently occupying a given cell, or if it's empty
string checkCellContent(int checkX, int checkY) {
	// Check if the cell is a wall
	if (checkX < 0
	||  checkX > CELLS_HORIZONTAL - 1
	||  checkY < 0
	||  checkY > CELLS_VERTICAL - 1) {
		return "wall";
	}

	// Check for a snake segment in the cell
	for (auto& segment : snake.getSegments()) {
		if (checkX == segment.getX()
		&&  checkY == segment.getY()) {
			return "snake";
		}
	}

	// Check for an apple in the cell
	for (auto& apple : apples) {
		if (checkX == apple.getX()
		&&  checkY == apple.getY()) {
			return "apple";
		}
	}

	return "empty";
}

// Initialize SDL
bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow(
		"Snake",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		CELLS_HORIZONTAL*CELLS_SIZE,
		CELLS_VERTICAL*CELLS_SIZE,
		0
	);
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
		CELLS_HORIZONTAL*CELLS_SIZE,
		CELLS_HORIZONTAL*CELLS_SIZE,
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