#pragma once
#include "camera.h"
#include <graphics.h>

class Player
{
public:
	Player() = default;
	~Player() = default;

	virtual void on_update(int delta) {

	}

	virtual void on_draw(const Camera& camera){

	}

	virtual void on_input(const ExMessage& msg) {

	}
private:

};

