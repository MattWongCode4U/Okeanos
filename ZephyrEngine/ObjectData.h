#pragma once
//#include "GameObject.h"
#include <vector>
#include "Msg.h"

class GameObject;
class ObjectData
{
public:
	std::vector<GameObject*> toDestroyVector;
	//std::vector<GameObject*> toMessageVector;
	//std::map<std::string, GameObject*> gameObjects;

	ObjectData();
	~ObjectData();
};
