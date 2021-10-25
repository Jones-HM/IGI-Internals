#include "Camera.hpp"
#include "CommonConst.hpp"
#include "NativeHelper.hpp"

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

void Camera::WritePosition(Camera::Position& pos) {
	std::memcpy(CAM_POS_ADDR, (void*)&pos, sizeof(pos));
}

void Camera::WritePosition(double x) {
	auto pos = ReadPosition();
	WritePosition(x, pos.Y(), pos.Z());
}

void Camera::WritePosition(double x, double y) {
	auto pos = ReadPosition();
	WritePosition(x, y, pos.Z());
}

void Camera::WritePosition(double x, double y, double z) {
	Position position(x, y, z);
	std::memcpy(CAM_POS_ADDR, (void*)&position, sizeof(position));
}

void Camera::WriteAngle(Camera::Angle angle) {
	std::memcpy(CAM_ANGLE_ADDR, (void*)&angle, sizeof(angle));
}

void Camera::WritePitch(float pitch) {
	*(float*)(((address_t)CAM_ANGLE_ADDR) + 0xC) = pitch;
}

void Camera::WriteRoll(float roll) {
	*(float*)(((address_t)CAM_ANGLE_ADDR) + 0x10) = roll;
}

void Camera::WriteYaw(float yaw) {
	*(float*)(((address_t)CAM_ANGLE_ADDR) + 0x18) = yaw;
}

void Camera::WriteFov(float fov) {
	*(float*)(((address_t)CAM_ANGLE_ADDR) + 0x1C) = fov;
}

void Camera::AttachCam() {
	GT_Write2Bytes(VIEWPORT_TASK_ADDR, 0xA5F3);
}

void Camera::DeattachCam() {
	GT_WriteNOP(VIEWPORT_TASK_ADDR, 2);
}

void Camera::CalibrateView() {
	address_t angle_h = READ_STATIC_PTR_OFF4(0x0056E210, 0x08, 0x7CC, 0x14, 0xF3C);
	address_t angle_v = READ_STATIC_PTR_OFF4(0x0056E210, 0x08, 0x7CC, 0x14, 0x50C);
	*(float*)angle_h = 0.0f;
	*(float*)angle_v = 3.095987082f;
}

void  Camera::EnableFreeCam(Camera::Controls& ctrl) {
	free_cam_run = true;

	if (sizeof(ctrl) == 0) return;

	//De-Attach ViewPort of camera and Set View.
	DeattachCam();
	HUMAN::CAM_VIEW_SET(3);

	while (!GT_IsKeyPressed(ctrl.QUIT())) {
		auto pos = ReadPosition();

		//This loop allows us to Move,Rotate,Zoom around FreeRoam camera.
		for (int index = 0; index < (VIEWPORT_SIZE * FLOAT_SIZE); index += FLOAT_SIZE) {
			float camera_prop = GT_ReadFloat((void*)(CAM_CTRL_ADDR + index));
			GT_WriteFloat((void*)(VIEWPORT_BASE_ADDR + index), camera_prop);
		}

		//Key event for X-Axis.
		if (GT_IsKeyPressed(ctrl.RIGHT()))
			pos.X(pos.X() - VIEWPORT_OFF);

		if (GT_IsKeyPressed(ctrl.LEFT()))
			pos.X(pos.X() + VIEWPORT_OFF);

		//Key event for Y-Axis.
		if (GT_IsKeyPressed(ctrl.FORWARD()))
			pos.Y(pos.Y() - VIEWPORT_OFF);

		if (GT_IsKeyPressed(ctrl.BACKWARD()))
			pos.Y(pos.Y() + VIEWPORT_OFF);

		//Key event for Z-Axis.
		if (GT_IsKeyPressed(ctrl.UP()))
			pos.Z(pos.Z() + VIEWPORT_OFF);

		if (GT_IsKeyPressed(ctrl.DOWN()))
			pos.Z(pos.Z() - VIEWPORT_OFF);

		//Key event for Calibrate view.
		if (GT_IsKeyPressed(ctrl.CALIBRATE()))
			CalibrateView();

		//Update camera position.
		WritePosition(pos.X(), pos.Y(), pos.Z());
	}

	//Attach ViewPort of camera and Reset View.
	HUMAN::CAM_VIEW_SET(1);
	AttachCam();
}
