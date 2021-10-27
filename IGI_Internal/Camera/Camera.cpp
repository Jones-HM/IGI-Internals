#include "Camera.hpp"
#include "CommonConst.hpp"
#include "NativeHelper.hpp"
#include <GTConsole.hpp>
using namespace IGI;

Camera::Position Camera::ReadPosition()
{
	Position position;
	std::memcpy((void*)&position, CAM_POS_ADDR, sizeof(Position));
	return position;
}

Camera::Angle Camera::ReadAngle()
{
	Angle angle;
	std::memcpy((void*)&angle, CAM_ANGLE_ADDR, sizeof(Angle));
	return angle;
}


void Camera::WritePosition(double x) {
	auto pos = ReadPosition();
	WritePosition(x, pos.Y(), pos.Z());
}

void Camera::WritePosition(double x, double y) {
	auto pos = ReadPosition();
	x = (x == NULLF) ? pos.X() : x;
	y = (y == NULLF) ? pos.Y() : y;
	WritePosition(x, y, pos.Z());
}

void Camera::WritePosition(double x, double y, double z) {
	auto pos = ReadPosition();
	x = (x == NULLF) ? pos.X() : x;
	y = (y == NULLF) ? pos.Y() : y;
	z = (z == NULLF) ? pos.Z() : z;

	Position position(x, y, z);
	WritePosition(position);
}

void Camera::WritePosition(Position& pos) {
	std::memcpy(CAM_POS_ADDR, (void*)&pos, sizeof(pos));
}

void Camera::WriteAngle(float pitch) {
	auto angle = ReadAngle();
	pitch = (pitch == NULLF) ? angle.Pitch() : pitch;
	WriteAngle(pitch, angle.Roll(), angle.Yaw(), angle.Fov());
}

void Camera::WriteAngle(float pitch, float roll) {
	auto angle = ReadAngle();
	pitch = (pitch == NULLF) ? angle.Pitch() : pitch;
	roll = (roll == NULLF) ? angle.Roll() : roll;
	WriteAngle(pitch, roll, angle.Yaw(), angle.Fov());
}

void Camera::WriteAngle(float pitch, float roll, float yaw) {
	auto angle = ReadAngle();
	pitch = (pitch == NULLF) ? angle.Pitch() : pitch;
	roll = (roll == NULLF) ? angle.Roll() : roll;
	yaw = (yaw == NULLF) ? angle.Yaw() : yaw;
	WriteAngle(pitch, roll, yaw, angle.Fov());
}

void Camera::WriteAngle(float pitch, float roll, float yaw, float fov) {
	auto read_angle = ReadAngle();
	pitch = (pitch == NULLF) ? read_angle.Pitch() : pitch;
	roll = (roll == NULLF) ? read_angle.Roll() : roll;
	yaw = (yaw == NULLF) ? read_angle.Yaw() : yaw;
	fov = (fov == NULLF) ? read_angle.Fov() : fov;
	Angle angle(pitch, roll, yaw, fov);
	WriteAngle(angle);
}

void Camera::WriteAngle(Angle& angle) {
	std::memcpy(CAM_ANGLE_ADDR, (void*)&angle, sizeof(angle));
}

void Camera::Attach() {
	GT_Write2Bytes(VIEWPORT_TASK_ADDR, 0xA5F3);
}

void Camera::Deattach() {
	GT_WriteNOP(VIEWPORT_TASK_ADDR, 2);
}

void Camera::CalibrateView() {
	address_t angle_h = READ_STATIC_PTR_OFF4(0x0056E210, 0x08, 0x7CC, 0x14, 0xF3C);
	address_t angle_v = READ_STATIC_PTR_OFF4(0x0056E210, 0x08, 0x7CC, 0x14, 0x50C);
	*(float*)angle_h = 0.0f;
	*(float*)angle_v = 3.095987082f;
}

void  Camera::FreeCam(Camera::Controls& ctrl) {

	if (sizeof(ctrl) == 0) return;

	//De-Attach ViewPort of camera and Set View.
	Deattach();
	HUMAN::CAM_VIEW_SET(3);

	//Main loop of FreeCam.
	while (!GT_IsKeyPressed(ctrl.QUIT())) {
		auto pos = ReadPosition();
		auto angle = ReadAngle();

		//This allows us to Move,Rotate,Zoom around FreeRoam camera.
		std::memcpy(CAM_ANGLE_ADDR, CAM_CONTROL_ADDR, (VIEWPORT_SIZE * FLOAT_SIZE));

		//Key event for X-Axis.
		if (GT_IsKeyPressed(ctrl.RIGHT())) pos.X(pos.X() - ctrl.AXIS_OFF());
		if (GT_IsKeyPressed(ctrl.LEFT())) pos.X(pos.X() + ctrl.AXIS_OFF());

		//Key event for Y-Axis.
		if (GT_IsKeyPressed(ctrl.FORWARD())) pos.Y(pos.Y() - ctrl.AXIS_OFF());
		if (GT_IsKeyPressed(ctrl.BACKWARD())) pos.Y(pos.Y() + ctrl.AXIS_OFF());

		//Key event for Z-Axis.
		if (GT_IsKeyPressed(ctrl.UP())) pos.Z(pos.Z() + ctrl.AXIS_OFF());
		if (GT_IsKeyPressed(ctrl.DOWN())) pos.Z(pos.Z() - ctrl.AXIS_OFF());

		//Key event for Calibrate view.
		if (GT_IsKeyPressed(ctrl.CALIBRATE())) CalibrateView();

		//Update camera position.
		WritePosition(pos);
		//UpdateInfo(pos,angle);
	}

	//Attach ViewPort of camera and Reset View.
	HUMAN::CAM_VIEW_SET(1);
	Attach();
}

//void UpdateInfo(Camera::Position pos,Camera::Angle angle) {
//
//	GT_SetConsoleCursorXY(5,0);
//	std::cout << "Position: " << std::endl;
//	GT_SetConsoleCursorXY(5, 1);
//	std::cout << "X: " << std::setprecision(17) << pos.X() << std::endl;
//	GT_SetConsoleCursorXY(5, 2);
//	std::cout << "Y: " << std::setprecision(17) << pos.Y() << std::endl;
//	GT_SetConsoleCursorXY(5, 3);
//	std::cout << "Z: " << std::setprecision(17) << pos.Z() << std::endl;
//
//	GT_SetConsoleCursorXY(5, 5);
//	std::cout << "Angle: " << std::endl;
//	GT_SetConsoleCursorXY(5, 6);
//	std::cout << "Pitch: " << std::setprecision(7) << angle.Pitch() << std::endl;
//	GT_SetConsoleCursorXY(5, 7);
//	std::cout << "Roll: " << std::setprecision(7) << angle.Roll() << std::endl;
//	GT_SetConsoleCursorXY(5, 8);
//	std::cout << "Yaw: " << std::setprecision(7) << angle.Yaw() << std::endl;
//	GT_SetConsoleCursorXY(5, 9);
//	std::cout << "Fov: " << std::setprecision(7) << angle.Fov() << std::endl;
//}
