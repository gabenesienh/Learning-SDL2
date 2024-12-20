//TODO: make brush and bgColor into classes with methods
//TODO: remember brush size for every tool
//TODO: show brush size in pixels
//TODO: better screen clear effect
//TODO: change background color
//TODO: color picker (brush)
//TODO: color picker (background)
//TODO: refactor code into smaller functions
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

void handleTools();

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;
const float CLEAR_CANVAS_DELAY = 0.5; // In seconds
const int DEFAULT_BRUSH_SIZE = 5;

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
	private:
		array<int, 3> rgbValue = {255, 255, 255};
	public:
		Color(int r, int g, int b) {
			this->rgbValue = {r, g, b};
		}
		Color(array<int, 3> rgb) {
			this->rgbValue = rgb;
		}

		array<int, 3> getValue() const {
			return this->rgbValue;
		}

		void setValue(int r, int g, int b) {
			this->rgbValue = {r, g, b};
		}

		Uint32 getFormattedValue(SDL_PixelFormat* format) const {
			return SDL_MapRGB(format, this->rgbValue[0], this->rgbValue[1], this->rgbValue[2]);
		}
};

array<array<int, 3>, 10> colors = {{
	{235, 64, 52},		// Red
	{245, 124, 59},		// Orange
	{245, 242, 73},		// Yellow
	{50, 168, 58},		// Green
	{66, 135, 245},		// Blue
	{52, 57, 201},		// Indigo
	{145, 75, 242},		// Purple
	{252, 96, 185}, 	// Pink
	{127, 127, 127},	// Gray
	{0, 0, 0}			// Black (default for brush)
}};

Color bgColor = Color(255, 255, 255);

enum {
	TOOL_BRUSH,	// Default
	TOOL_ERASER,

	TOOL_COUNT // Not a tool! Use only for getting the size of this enum
} eToolTypes;

//array<SDL_Rect*, TOOL_COUNT> brushes;
SDL_Rect* brush;
int brushColor = 9;
int brushMode = TOOL_BRUSH;

// Counts up when holding the "clear canvas" key
float clearCanvasTimer = 0;

// Stores the mouse's position on the current and previous frames
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

	brush = new SDL_Rect {0, 0, DEFAULT_BRUSH_SIZE, DEFAULT_BRUSH_SIZE};

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
					case SDL_SCANCODE_B: brushMode = TOOL_BRUSH;  break;
					case SDL_SCANCODE_E: brushMode = TOOL_ERASER; break;

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
						brushColor = event.key.keysym.scancode - SDL_SCANCODE_1;
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
				brush->x = mousePos.x - (brush->w/2);
				brush->y = mousePos.y - (brush->h/2);

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
					if (brush->w - event.wheel.y > 1 && brush->w - event.wheel.y <= 64) {
						brush->w += -event.wheel.y;
						brush->h += -event.wheel.y;
					}

					// Update brush position
					brush->x = event.wheel.mouseX - (brush->w/2);
					brush->y = event.wheel.mouseY - (brush->h/2);
				}
				break;
			case SDL_QUIT:
				return false;
		}
	}

	// Update canvas
	SDL_BlitSurface(canvas, NULL, winSurface, NULL);

	// Draw cursor by drawing each bar individually and with different offsets
	*cursorSegments[0] = {brush->x, brush->y - 2, brush->w, 2};
	*cursorSegments[1] = {brush->x + brush->w, brush->y, 2, brush->h};
	*cursorSegments[2] = {brush->x, brush->y + brush->h, brush->w, 2};
	*cursorSegments[3] = {brush->x - 2, brush->y, 2, brush->h};

	for (int i = 0; i < cursorSegments.size(); i++) {
		SDL_FillRect(winSurface, cursorSegments[i], SDL_MapRGB(winSurface->format, 0, 0, 0));
	}

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

	SDL_UpdateWindowSurface(window);
	return true;
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
		switch (brushMode) {
			case TOOL_BRUSH:
				// Paint the canvas
				{
					int r = colors[brushColor][0];
					int g = colors[brushColor][1];
					int b = colors[brushColor][2];

					drawLine(canvas, brush, SDL_MapRGB(canvas->format, r, g, b),
								mousePosLast.x, mousePosLast.y,
								mousePos.x, mousePos.y);
					break;
				}
			case TOOL_ERASER:
				// Erase by painting the background color
				drawLine(canvas, brush, bgColor.getFormattedValue(canvas->format),
							mousePosLast.x, mousePosLast.y,
							mousePos.x, mousePos.y);
				break;
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
	delete brush;
	delete cursorSegments[0];
	delete cursorSegments[1];
	delete cursorSegments[2];
	delete cursorSegments[3];
	SDL_Quit();
}