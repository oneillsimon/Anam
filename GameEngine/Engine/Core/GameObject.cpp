#include "CoreEngine.h"
#include "GameComponent.h"
#include "GameObject.h"

GameObject::GameObject(const Vector3& position, const Quaternion& rotation, float scale) :
	m_transform(position, rotation, scale),
	m_coreEngine(0)
{
}


GameObject::~GameObject()
{
	for(unsigned int i = 0; i < m_components.size(); i++)
	{
		if(m_components[i])
		{
			delete m_components[i];
		}
	}

	for(unsigned int i = 0; i < m_children.size(); i++)
	{
		if(m_children[i])
		{
			delete m_children[i];
		}
	}
}

GameObject* GameObject::addChild(GameObject* child)
{
	m_children.push_back(child);
	child->getTransform()->setParent(&m_transform);
	child->setEngine(m_coreEngine);
	return this;
}

GameObject* GameObject::addComponent(GameComponent* component)
{
	m_components.push_back(component);
	component->setParent(this);
	return this;
}

void GameObject::inputAll(const Input& input, float delta)
{
	this->input(input, delta);

	for(unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->inputAll(input, delta);
	}
}

void GameObject::updateAll(float delta)
{
	update(delta);

	for(unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->updateAll(delta);
	}
}

void GameObject::renderAll(const Shader& shader, const RenderingEngine& renderingEgine, const Camera& camera) const
{
	render(shader, renderingEgine, camera);

	for(unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->renderAll(shader, renderingEgine, camera);
	}
}

void GameObject::input(const Input& input, float delta)
{
	m_transform.update();

	for(unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->input(input, delta);
	}
}

void GameObject::update(float delta)
{
	for(unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->update(delta);
	}
}

void GameObject::render(const Shader& shader, const RenderingEngine& renderingEngine, const Camera& camera) const
{
	for(unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->render(shader, renderingEngine, camera);
	}
}

std::vector<GameObject*> GameObject::getChildren() const
{
	return m_children;
}

std::vector<GameObject*> GameObject::getAllAttached()
{
	std::vector<GameObject*> result;

	for(unsigned int i = 0; i < m_children.size(); i++)
	{
		std::vector<GameObject*> childObjects = m_children[i]->getAllAttached();
		result.insert(result.end(), childObjects.begin(), childObjects.end());
	}

	result.push_back(this);
	return result;
}

Transform* GameObject::getTransform()
{
	return &m_transform;
}

CoreEngine* GameObject::getEngine()
{
	return m_coreEngine;
}

void GameObject::setEngine(CoreEngine* engine)
{
	if(m_coreEngine != engine)
	{
		m_coreEngine = engine;

		for(unsigned int i = 0; i < m_components.size(); i++)
		{
			m_components[i]->addToCoreEngine(engine);
		}

		for(unsigned int i = 0; i < m_children.size(); i++)
		{
			m_children[i]->setEngine(engine);
		}
	}
}

void GameObject::enableScripting()
{
	scriptManager.generateScriptName(this);
	scriptManager.pushGlobal(m_transform, "transform");
}

lua_State* GameObject::getL()
{
	return scriptManager.getL();
}