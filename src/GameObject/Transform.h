#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../Component/Component.h"

struct Vector2 {
	Vector2() : mX(0), mY(0) {}
	Vector2(unsigned x, unsigned y) : mX(x), mY(y) {}

	// X and Y positions start at top left corner of the screen/terminal
	unsigned mX; // the column
	unsigned mY; // the row
};


// Stores positions (generally for GameObjects)
class Transform : public Component {
public:
	Transform(unsigned x, unsigned y) : mPosition(x, y) {}
	Transform(Vector2 pos) : mPosition(pos) {}
	Transform() : mPosition(0, 0) {}

	unsigned GetX() { return mPosition.mX; }
	unsigned GetY() { return mPosition.mY; }
	Vector2 GetPosition() { return mPosition; }

	void SetX(unsigned x) { mPosition.mX = x; }
	void SetY(unsigned y) { mPosition.mY = y; }
	void SetPosition(Vector2 pos) { mPosition = pos; }

protected:
	Vector2 mPosition;
};

typedef std::unique_ptr<Transform> TransformPtr;

#endif
