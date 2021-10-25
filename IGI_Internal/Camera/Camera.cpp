#include "Camera.hpp"
using namespace IGI;

Camera::Position Camera::ReadPosition()
{
	Position position;
	void* pos_val = GT_ReadAddress((void*)pos_base, sizeof(Position));
	std::memcpy((void*)&position, pos_val, sizeof(Position));
	return position;
}

Camera::Angle Camera::ReadAngle()
{
	Angle angle;
	void* angle_val = GT_ReadAddress((void*)angle_base, sizeof(Angle));
	std::memcpy((void*)&angle, angle_val, sizeof(Angle));
	return angle;
}