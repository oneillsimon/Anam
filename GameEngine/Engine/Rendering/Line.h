#include "../Core/3DMath.h"
#include "../Rendering/Mesh.h"

class Line : public Mesh
{
private:
	Vector3 point0;
	Vector3 point1;
public:
	Line(const Vector3& point0, const Vector3& point1);
	~Line();

	virtual void draw();
};