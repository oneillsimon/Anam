#pragma once

#include <vector>
#include "Transform.h"

class CoreEngine;
class GameComponent;
class Shader;
class RenderingEngine;

class GameObject
{
private:
	Transform m_transform;
	CoreEngine* m_coreEngine;
	std::vector<GameObject*> m_children;
	std::vector<GameComponent*> m_components;

protected:
	virtual void input(float delta);
	virtual void update(float delta);
	virtual void render(Shader* shader, RenderingEngine* renderingEngine);

public:
	GameObject();
	~GameObject();

	GameObject* addChild(GameObject* child);
	GameObject* addComponent(GameComponent* component);

	void inputAll(float delta);
	void updateAll(float delta);
	void renderAll(Shader* shader, RenderingEngine* renderingEngine);

	std::vector<GameObject*> getAllAttached();

	Transform& getTransform();

	void setEngine(CoreEngine* engine);
};

