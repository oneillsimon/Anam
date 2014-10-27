#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>

#include "Input.h"
#include "Lua.h"
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

protected:
	Transform m_transform;

	virtual void input(const Input& input, float delta);
	virtual void update(float delta);
	virtual void render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;

public:
	static void registerMembers(const std::string& namespace_, lua_State* luaState)
	{
		luabridge::getGlobalNamespace(luaState)
			.beginNamespace(namespace_.c_str())
			.beginClass<GameObject>("GameObject")
			.addConstructor<void(*)(const Vector3&, const Quaternion&, const float&)>()
			.addFunction("transform", &GameObject::getTransform)
			.addFunction("parentTransform", &GameObject::parent)
			.addFunction("getX_", &GameObject::getX_)
			.endClass()
			.endNamespace();
	}

	RefCountedPtr<Transform> parent()
	{
		static RefCountedPtr<Transform> a(getTransform());
		return a;
	}

	GameObject(const Vector3& position = Vector3(), const Quaternion& rotation = Quaternion(0, 0, 0, 1), float scale = 1.0f);
	~GameObject();

	GameObject* addChild(GameObject* child);
	GameObject* addComponent(GameComponent* component);

	void inputAll(const Input& input, float delta);
	void updateAll(float delta);
	void renderAll(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const;

	std::vector<GameObject*> getChildren() const;
	std::vector<GameObject*> getAllAttached();

	Transform* getTransform();

	CoreEngine* getEngine();

	float getX_()
	{
		return m_transform.getPosition().getX();
	}

	void setEngine(CoreEngine* engine);
};

#endif