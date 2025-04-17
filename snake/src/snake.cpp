#include "snake.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <list>
#include <iterator>

using std::string;
using std::list;
using std::next;

extern const int SNAKE_DIR_NONE;
extern const int SNAKE_DIR_UP;
extern const int SNAKE_DIR_DOWN;
extern const int SNAKE_DIR_LEFT;
extern const int SNAKE_DIR_RIGHT;

extern string checkCellContent(int checkX, int checkY);

// ------------------
// SnakeSegment class
// ------------------

SnakeSegment::SnakeSegment(int x, int y) {
	this->x = x;
	this->y = y;
};

int SnakeSegment::getX() const { return this->x; };
int SnakeSegment::getY() const { return this->y; };

void SnakeSegment::setX(int x) {
	this->x = x;
};
void SnakeSegment::setY(int y) {
	this->y = y;
};

// -----------
// Snake class
// -----------

// There is no dedicated head segment. Every movement, the segment at the tip
// of the tail is moved to the front of the snake and becomes the new head

int 					  Snake::getDirection() const { return this->direction; };
const list<SnakeSegment>& Snake::getSegments() const  { return this->segments; }
const SnakeSegment&		  Snake::getHead() const	  { return *this->head; }

// Doesn't immediately turn the snake, but rather buffers a turn
// Returns false if it's a repeat input (e.g. pressing right when
// you're already going right)
bool Snake::turn(int direction) {
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

// Try to move the snake to whichever direction is first in the buffer queue,
// or forward if no direction is buffered
bool Snake::move() {
	// Move the second buffered input to the front, if the first one is empty
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
		// Set direction to the first buffered input in queue
		this->direction = this->bufferedDirection[0];
	}

	// Move the second buffered input to the front for use in the next movement
	this->bufferedDirection[0] = this->bufferedDirection[1];
	this->bufferedDirection[1] = SNAKE_DIR_NONE;

	// Which cell to move the head of the snake to
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
		// The tail would move out of the way next frame anyway, so this gives
		// the player some more wiggle room
		if (targetX != tail->getX() || targetY != tail->getY()) {
			return false;
		}
	}

	// Set the head to be the next segment in the list, which will always be
	// the segment at the tip of the tail
	this->head++;
	if (this->head == this->segments.end()) {
		this->head = this->segments.begin();
	}

	// Save the new head segment's position (currently still positioned at the
	// tail)
	this->lastTailPos.x = this->head->getX();
	this->lastTailPos.y = this->head->getY();

	// Teleport the new head to the desired cell
	this->head->setX(targetX);
	this->head->setY(targetY);

	return true;
}

void Snake::grow() {
	// Determine where to insert the newly grown segment
	// Should always be on the tail, that is, the element right after the head
	// on the segment list
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

void Snake::reset() {
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