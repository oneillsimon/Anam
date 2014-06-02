#include "../Core/3DMath.h"

class Particle
{
private:
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	
	float damping;
	float inverseMass;
	Vector3 forceAccumulated;

public:
	void update(float delta);
};