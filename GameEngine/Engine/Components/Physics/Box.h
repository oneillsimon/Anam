#include "../../Physics/Collider.h"

class Box : public ColliderBox
{
public:
	Box(PhysicsObject* obj)
	{
		m_body = new RigidBody(1);
		m_body->setParent(obj);
	}

	~Box()
	{
		delete m_body;
	}

	void setState(float z)
	{
		//m_body->setPosition(Vector3(0, 3, z));
		m_body->setOrientation(Quaternion(0, 0, 0, 1));
		m_body->setVelocity(Vector3(0, 0, 0));
		m_body->setRotation(Vector3(0, 0, 0));
		m_halfSize = Vector3(1, 1, 1);

		float mass = m_halfSize[0] * m_halfSize[1] * m_halfSize[2] * 8.0f;
		m_body->setMass(mass);

		Matrix3 tensor;
		tensor.setBlockInteriaTensor(m_halfSize, mass);
		m_body->setIntertiaTensor(tensor);

		m_body->setLinearDamping(0.95f);
		m_body->setAngularDamping(0.8f);
		m_body->clearAccumulators();
		//m_body->setAcceleration(Vector3(0, -10, 0));

		m_body->setCanSleep(true);
		m_body->setAwake();

		m_body->calculateDerivedData();
	}
};