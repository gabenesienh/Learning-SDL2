//TODO: remember brush size for every tool
//TODO: show brush size in pixels
//TODO: fix gaps when drawing lines
//TODO: better screen clear effect
//TODO: change background color
//TODO: color picker (brush)
//TODO: color picker (background)

//TODO-OPT: custom color selector
//TODO-OPT: brush size slider
//TODO-OPT: custom background image
//TODO-OPT: layers

#include <SDL2/SDL.h>
#include <iostream>
#include <array>

using std::cin, std::cout;
using std::array;

bool init();
bool loop();
void kill();

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

// Default background color (white)
struct {
	int r = 255;
	int g = 255;
	int b = 255;
} bgColor;

Uint32 bgColorValue;

const float CLEAR_CANVAS_DELAY = 0.5; // In seconds

// Saves the value of SDL_GetPerformanceCounter() from previous frame
// Used for calculating delta time
Uint64 ticksLast = 0;
float deltaTime = 0; // In milliseconds

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* canvas;

SDL_Event event;

array<bool, SDL_NUM_SCANCODES> keyStates = {false};

// Copy values from mouse events
// These don't represent the real mouse input, since they are forcibly disabled
// a few times in the code
bool mouseLeftPressed = false;
bool mouseRightPressed = false; // Currently unused

enum eToolTypes {
	TOOL_BRUSH,	// Default
	TOOL_ERASER,

	TOOL_COUNT // Not a tool! Use only for getting the size of this enum
};

//array<SDL_Rect*, TOOL_COUNT> brushes;
SDL_Rect* brush;
int brushColor = 9;
int brushMode = TOOL_BRUSH;

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
	{0, 0, 0}			// Black (default)
}};

// Counts up when holding the "clear canvas" key
float clearCanvasTimer = 0;

int main(int argc, char** argv) {
	if (!init()) return 1;

	bgColorValue = SDL_MapRGB(canvas->format, bgColor.r, bgColor.g, bgColor.b);
	brush = new SDL_Rect {0, 0, 5, 5};

	SDL_ShowCursor(SDL_DISABLE);
	SDL_FillRect(canvas, NULL, bgColorValue);

	while (loop()) {
		//SDL_Delay(17);
	}

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
				brush->x = event.motion.x - (brush->w/2);
				brush->y = event.motion.y - (brush->h/2);

				if (mouseLeftPressed && mouseRightPressed) {
					// Cancel out M1 + M2
					mouseLeftPressed = false;
					mouseRightPressed = false;
				}

				if (mouseLeftPressed) {
					switch (brushMode) {
						case TOOL_BRUSH:
							{
								// Paint the canvas
								int r = colors[brushColor][0];
								int g = colors[brushColor][1];
								int b = colors[brushColor][2];
								SDL_FillRect(canvas, brush, SDL_MapRGB(canvas->format, r, g, b));
								break;
							}
						case TOOL_ERASER:
							// Erase the canvas (by painting the background color)
							SDL_FillRect(canvas, brush, bgColorValue);
							break;
					} 
				}

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
					brush->x = event.wheel.mouseX - (brush->w/2);
					brush->y = event.wheel.mouseY - (brush->h/2);
					if (brush->w - event.wheel.y > 1 && brush->w - event.wheel.y <= 64) {
						brush->w += -event.wheel.y;
						brush->h += -event.wheel.y;
					}
				}
				break;
			case SDL_QUIT:
				return false;
		}
	}

	SDL_BlitSurface(canvas, NULL, winSurface, NULL);

	// Draw cursor by drawing each bar individually and with different offsets
	SDL_FillRect(winSurface, new SDL_Rect {brush->x, brush->y - 2, brush->w, 2}, SDL_MapRGB(winSurface->format, 0, 0, 0));
	SDL_FillRect(winSurface, new SDL_Rect {brush->x + brush->w, brush->y, 2, brush->h}, SDL_MapRGB(winSurface->format, 0, 0, 0));
	SDL_FillRect(winSurface, new SDL_Rect {brush->x, brush->y + brush->h, brush->w, 2}, SDL_MapRGB(winSurface->format, 0, 0, 0));
	SDL_FillRect(winSurface, new SDL_Rect {brush->x - 2, brush->y, 2, brush->h}, SDL_MapRGB(winSurface->format, 0, 0, 0));

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
			SDL_FillRect(canvas, NULL, bgColorValue);
			clearCanvasTimer = -1; // Prevent repeated clearing
		}
	}

	SDL_UpdateWindowSurface(window);
	return true;
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
	SDL_Quit();
}