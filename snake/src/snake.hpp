#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <array>
#include <list>

using std::array;
using std::list;

extern const int START_X;
extern const int START_Y;

const int SNAKE_DIR_NONE = 0;
const int SNAKE_DIR_UP = 1;
const int SNAKE_DIR_DOWN = 2;
const int SNAKE_DIR_LEFT = 3;
const int SNAKE_DIR_RIGHT = 4;

// Represents a piece of the snake (head, body or tail)
class SnakeSegment {
	private:
		int x;
		int y;
	public:
		SnakeSegment(int x, int y);

		int getX() const;
		int getY() const;

		void setX(int x);
		void setY(int y);
};

// The snake the player controls
class Snake {
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
		int							getDirection() const;
		const list<SnakeSegment>&	getSegments() const;
		const SnakeSegment&			getHead() const;

		// Buffer a turn
		bool turn(int direction);

		// Attempt to move the snake forward, or sideways if a turn is buffered
		// Returns false when bumping into solid things
		bool move();

		// Grow an extra segment at the tail
		void grow();

		// Return the snake to its starting state
		void reset();
};

#endif