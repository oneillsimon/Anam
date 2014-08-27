#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

#include "Input.h"
#include "Transform.h"

class Camera;
class CoreEngine;
class GameComponent;
class RenderingEngine;
class Shader;

class GameObject
{
private:
	Transform m_transform;
	CoreEngine* m_coreEngine;
	std::vector<GameObject*> m_children;
	std::vector<GameComponent*> m_components;

protected:
	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;

public:
	GameObject(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(0, 0, 0, 1), float scale = 1.0f);
	~GameObject();

	GameObject* addChild(GameObject* child);
	GameObject* addComponent(GameComponent* component);

	void inputAll(const Input& input, float delta);
	void updateAll(float delta);
	void renderAll(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;

	std::vector<GameObject*> getAllAttached();

	Transform* getTransform();

	void setEngine(CoreEngine* engine);
};

#endif