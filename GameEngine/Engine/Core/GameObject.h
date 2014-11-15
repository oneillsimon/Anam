#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

#include "Input.h"
#include "Lua.h"
#include "ScriptManager.h"
#include "Transform.h"

class Camera;
class CoreEngine;
class GameComponent;
class RenderingEngine;
class Shader;

class GameObject
{
private:
	CoreEngine* m_coreEngine;
	std::vector<GameObject*> m_children;
	std::vector<GameComponent*> m_components;

	ScriptManager scriptManager;

protected:
	Transform m_transform;

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

	void enableScripting();

	std::vector<GameObject*> getChildren() const;
	std::vector<GameObject*> getAllAttached();

	Transform* getTransform();
	CoreEngine* getEngine();
	
	void setTransform(const Transform& t);
	void setEngine(CoreEngine* engine);

	lua_State* getL();
	ScriptManager& getScriptManager();
};

#endif