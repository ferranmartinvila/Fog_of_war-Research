#ifndef _J1PLAYER_H_
#define _J1PLAYER_H_

#include "j1Module.h"

class MyEntity;

//Animator Module -------------------------------
class j1Player : public j1Module
{
public:

	j1Player();
	~j1Player();

public:

	// Called each loop iteration
	bool Update(float dt);

private:

	MyEntity* player = nullptr;

public:

	void GeneratePlayerEntity();
	
};
#endif // !_J1PLAYER_H_
