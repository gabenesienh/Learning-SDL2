//TODO: remember brush size for every tool
//TODO: show brush size in pixels
//TODO: better screen clear effect
//TODO: change background color
//TODO: color picker (brush)
//TODO: color picker (background)
//TODO: avoid duplicated linealg files

//TODO-OPT: custom color selector
//TODO-OPT: brush size slider
//TODO-OPT: custom background image
//TODO-OPT: layers

#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include "linealg.h"

using std::cin, std::cout;
using std::array;

bool init();
bool loop();
void kill();

void drawCursor();
void handleTools();
void handleClear();

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;
const float CLEAR_CANVAS_DELAY = 0.5; // In seconds
const int DEFAULT_BRUSH_SIZE = 5;
const int MIN_BRUSH_SIZE = 1;
const int MAX_BRUSH_SIZE = 64;

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* canvas;

SDL_Event event;

// Saves the value of SDL_GetPerformanceCounter() from previous frame
// Used for calculating delta time
Uint64 ticksLast = 0;
float deltaTime = 0; // In milliseconds

// Copy values from keyboard events
array<bool, SDL_NUM_SCANCODES> keyStates = {false};

// Copy values from mouse events
// These don't represent raw mouse input, since they are forcibly disabled a
// few times in the code
bool mouseLeftPressed = false;
bool mouseRightPressed = false; // Currently unused

// General color class, used for brush and background
class Color {
	protected:
		array<int, 3> rgbValue = {255, 255, 255};
	public:
		Color(array<int, 3> rgb) {
			this->rgbValue = rgb;
		}

		array<int, 3> getValue() const {
			return this->rgbValue;
		}

		void setValue(array<int, 3> rgb) {
			this->rgbValue = rgb;
		}

		Uint32 getFormattedValue(SDL_PixelFormat* format) const {
			return SDL_MapRGB(format, this->rgbValue[0], this->rgbValue[1], this->rgbValue[2]);
		}
};

array<Color, 10> colors = {{
	Color({235, 64, 52}),		// Red
	Color({245, 124, 59}),		// Orange
	Color({245, 242, 73}),		// Yellow
	Color({50, 168, 58}),		// Green
	Color({66, 135, 245}),		// Blue
	Color({52, 57, 201}),		// Indigo
	Color({145, 75, 242}),		// Purple
	Color({252, 96, 185}), 	// Pink
	Color({127, 127, 127}),	// Gray
	Color({0, 0, 0})			// Black (default for brush)
}};

Color bgColor = Color({255, 255, 255});

enum {
	TOOL_BRUSH,	// Default
	TOOL_ERASER,

	TOOL_COUNT // Not a tool! Use only for getting the size of this enum
} eTools;

// Brush you move with the cursor
class {
	private:
		int mode = TOOL_BRUSH;
		SDL_Rect rect = {0, 0, DEFAULT_BRUSH_SIZE, DEFAULT_BRUSH_SIZE};
		Color color = Color(colors[9]);
	public:
		int           getMode() const  		{ return this->mode; }
		int 		  getX() const     		{ return this->rect.x; }
		int 		  getY() const     		{ return this->rect.y; }
		int 		  getSize() const  		{ return this->rect.w; }
		SDL_Rect* 	  getRect()        		{ return &this->rect; } //get rekt
		Color 		  getColor() const      { return this->color; }
		array<int, 3> getColorValue() const { return this->color.getValue(); }
		Uint32 		  getFormattedColorValue(SDL_PixelFormat* format) const {
			return this->color.getFormattedValue(format);
		}

		void setMode(int mode) {
			if (mode >= 0 && mode < TOOL_COUNT) {
				this->mode = mode;
			}
		}
		void setPosition(int x, int y) {
			this->rect.x = x;
			this->rect.y = y;
		}
		void setSize(int newSize) {
			if (newSize > MIN_BRUSH_SIZE && newSize <= MAX_BRUSH_SIZE) {
				this->rect.w = newSize;
				this->rect.h = newSize;
			}
		}
		void setColor(Color newColor) {
			this->color = newColor;
		}
		void setColorValue(array<int, 3> rgb) {
			this->color.setValue(rgb);
		}
} brush;

// Counts up when holding the "clear canvas" key
float clearCanvasTimer = 0;

// Stores the mouse's position on the current frame and previous frame
// Used for drawing lines
struct {
	Sint32 x;
	Sint32 y;
} mousePos;
struct {
	Sint32 x;
	Sint32 y;
} mousePosLast;

// The individual lines that make up the box-shaped cursor
array<SDL_Rect*, 4> cursorSegments = {
	new SDL_Rect{0, 0, 1, 1},
	new SDL_Rect{0, 0, 1, 1},
	new SDL_Rect{0, 0, 1, 1},
	new SDL_Rect{0, 0, 1, 1},
};

int main(int argc, char** argv) {
	if (!init()) return 1;

	SDL_ShowCursor(SDL_DISABLE);
	SDL_FillRect(canvas, NULL, bgColor.getFormattedValue(canvas->format));

	while (loop()) {}

	kill();
	return 0;
}

// Event loop
bool loop() {
	deltaTime = static_cast<float>(SDL_GetTicks64() - ticksLast)/1000;
	ticksLast = SDL_GetTicks64();

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_B: brush.setMode(TOOL_BRUSH);  break;
					case SDL_SCANCODE_E: brush.setMode(TOOL_ERASER); break;

					case SDL_SCANCODE_1:
					case SDL_SCANCODE_2:
					case SDL_SCANCODE_3:
					case SDL_SCANCODE_4:
					case SDL_SCANCODE_5:
					case SDL_SCANCODE_6:
					case SDL_SCANCODE_7:
					case SDL_SCANCODE_8:
					case SDL_SCANCODE_9:
					case SDL_SCANCODE_0:
						brush.setColor(colors[event.key.keysym.scancode - SDL_SCANCODE_1]);
						break;
				}

				keyStates[event.key.keysym.scancode] = true;
				break;
			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
					clearCanvasTimer = 0;
				}

				keyStates[event.key.keysym.scancode] = false;
				break;
			case SDL_MOUSEMOTION:
				mousePosLast = {mousePos.x, mousePos.y};
				mousePos = {event.motion.x, event.motion.y};

				// Center brush on cursor
				brush.setPosition(mousePos.x - (brush.getSize()/2),
								  mousePos.y - (brush.getSize()/2));

				handleTools();

				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						mouseLeftPressed = true;
						break;
					case SDL_BUTTON_RIGHT:
						mouseRightPressed = true;
						break;
				}

				// For the first frame of drawing, reset the last mouse
				// position
				mousePosLast = {event.button.x, event.button.y};
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						mouseLeftPressed = false;
						break;
					case SDL_BUTTON_RIGHT:
						mouseRightPressed = false;
						break;
				}
				break;
			case SDL_MOUSEWHEEL:
				if (keyStates[SDL_SCANCODE_LCTRL]) {
					// Resize brush
					brush.setSize(brush.getSize() - event.wheel.y);

					// Update brush position
					brush.setPosition(event.wheel.mouseX - (brush.getSize()/2),
							   		  event.wheel.mouseY - (brush.getSize()/2));
				}
				break;
			case SDL_QUIT:
				return false;
		}
	}

	// Update canvas
	SDL_BlitSurface(canvas, NULL, winSurface, NULL);

	drawCursor();
	handleClear();

	SDL_UpdateWindowSurface(window);
	return true;
}

// Draw the box outline around the brush
void drawCursor() {
	// Draw cursor by drawing each bar individually and with different offsets
	*cursorSegments[0] = {brush.getX(), brush.getY() - 2, brush.getSize(), 2};
	*cursorSegments[1] = {brush.getX() + brush.getSize(), brush.getY(), 2, brush.getSize()};
	*cursorSegments[2] = {brush.getX(), brush.getY() + brush.getSize(), brush.getSize(), 2};
	*cursorSegments[3] = {brush.getX() - 2, brush.getY(), 2, brush.getSize()};

	// Black cursor when drawing, brush color when not
	Color cursorColor = (mouseLeftPressed ? colors[9] : brush.getColor());

	for (int i = 0; i < cursorSegments.size(); i++) {
		SDL_FillRect(winSurface, cursorSegments[i], cursorColor.getFormattedValue(canvas->format));
	}
}

// Tool logic
void handleTools() {
	if (mouseLeftPressed && mouseRightPressed) {
		// Cancel out M1 + M2
		mouseLeftPressed = false;
		mouseRightPressed = false;
	}

	// Check if the brush is currently being used
	if (mouseLeftPressed) {
		switch (brush.getMode()) {
			case TOOL_BRUSH:
				// Paint the canvas
				{
					drawLine(canvas, brush.getRect(), brush.getFormattedColorValue(canvas->format),
							 mousePosLast.x, mousePosLast.y,
							 mousePos.x, mousePos.y);
					break;
				}
			case TOOL_ERASER:
				// Erase by painting the background color
				drawLine(canvas, brush.getRect(), bgColor.getFormattedValue(canvas->format),
						 mousePosLast.x, mousePosLast.y,
						 mousePos.x, mousePos.y);
				break;
		}
	}
}

// Canvas clearing logic
void handleClear() {
	if (clearCanvasTimer != -1) {
		if (keyStates[SDL_SCANCODE_SPACE]) {
			if (clearCanvasTimer <= CLEAR_CANVAS_DELAY) {
				clearCanvasTimer += 1*deltaTime;
			}
		} else {
			clearCanvasTimer = 0;
		}
	}

	if (clearCanvasTimer > 0) {
		if (clearCanvasTimer < CLEAR_CANVAS_DELAY) {
			// Implement gradient here
			// SDL_FillRect(winSurface, NULL, SDL_MapRGB(canvas->format, 255, 255, 255));
		} else {
			SDL_FillRect(canvas, NULL, bgColor.getFormattedValue(canvas->format));
			clearCanvasTimer = -1; // Prevent repeated clearing
		}
	}
}

// Initialize SDL2
bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	window = SDL_CreateWindow("Worseprite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	winSurface = SDL_GetWindowSurface(window);
	if (!winSurface) {
		cout << "Error getting surface: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	// Temporary canvas, will be formatted into the real canvas
	SDL_Surface* temp = SDL_CreateRGBSurfaceWithFormat(
		winSurface->flags,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		winSurface->format->BitsPerPixel,
		winSurface->format->format
	);
	if (!temp) {
		cout << "Error creating canvas surface: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	canvas = SDL_ConvertSurface(temp, winSurface->format, 0);
	if (!canvas) {
		cout << "Error formatting canvas surface: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	SDL_FreeSurface(temp);

	return true;
}

// Quit SDL2
void kill() {
	SDL_DestroyWindow(window);
	delete brush.getRect();
	delete cursorSegments[0];
	delete cursorSegments[1];
	delete cursorSegments[2];
	delete cursorSegments[3];
	SDL_Quit();
}