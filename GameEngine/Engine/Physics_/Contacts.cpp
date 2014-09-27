//#include "Contacts.h"
//
//void Contact::calculateContactBasis()
//{
//	Vector3 contactTangent[2];
//
//	// Check if Z axis is nearer to X or Y axis
//	if (fabs(m_contactNormal.getX()) > fabs(m_contactNormal.getY()))
//	{
//		const float s = 1.0f / sqrtf(m_contactNormal.getZ() * m_contactNormal.getZ() + m_contactNormal.getX() * m_contactNormal.getX());
//
//		contactTangent[0].setX(m_contactNormal.getZ() * s);
//		contactTangent[0].setY(0);
//		contactTangent[0].setZ(-m_contactNormal.getX() * s);
//
//		contactTangent[1].setX(m_contactNormal.getY() * contactTangent[0].getX());
//		contactTangent[1].setY(m_contactNormal.getZ() * contactTangent[0].getX() -
//			m_contactNormal.getX() * contactTangent[0].getZ());
//		contactTangent[1].setZ(-m_contactNormal.getY() * contactTangent[0].getX());
//	}
//	else
//	{
//		const float s = 1.0f / sqrtf(m_contactNormal.getZ() * m_contactNormal.getZ() + m_contactNormal.getY() * m_contactNormal.getY());
//
//		contactTangent[0].setX(0);
//		contactTangent[0].setY(-m_contactNormal.getZ() * s);
//		contactTangent[0].setZ(m_contactNormal.getY() * s);
//
//		contactTangent[1].setX(m_contactNormal.getY() * contactTangent[0].getZ() -
//			m_contactNormal.getZ() * contactTangent[0].getY());
//		contactTangent[1].setY(-m_contactNormal.getX() * contactTangent[0].getZ());
//		contactTangent[1].setZ(m_contactNormal.getX() * contactTangent[0].getY());
//	}
//
//	contactToWorld.setComponents(m_contactNormal, contactTangent[0], contactTangent[1]);
//}