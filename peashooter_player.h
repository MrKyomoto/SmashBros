#pragma once
#include "player.h"

#include <iostream>

class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer() = default;
	~PeashooterPlayer() = default;

	void on_update(int delta) {
		std::cout << "Peashooter is updating..." << std::endl;
	}

private:

};

