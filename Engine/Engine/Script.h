#pragma once

class GameObject;

class Script
{
public:

	GameObject *object;

	Script();

	void Start();
	void Update();

	~Script();
};
