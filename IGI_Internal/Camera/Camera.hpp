#pragma once
#include "Common.hpp"
#include "GTLibc.hpp"

namespace IGI {
	class Camera
	{
	private:
		bool free_cam_run = false;
	public:
		//Struct to store angle of camera.
		struct Angle
		{
		private:
			float length_x;//X-Axis length of plane.
			float length_y;//Y-Axis length of plane.
			float pitch_length;//Pitch angle length.
			float pitch;//Pitch angle within plane.
			float roll;//Roll angle within plane.
			float length_z;//Z-Axis length of plane.
			float yaw;//Yaw angle within plane.
			float fov;//FOV (Field Of View) of ViewPort.
			float roll_length;//Roll angle length.

		public:
			//Ctors/Dtors
			Angle() = default;
			~Angle() = default;
			Angle(Angle& angle) { this->pitch = angle.pitch; this->roll = angle.roll; this->yaw = angle.yaw; this->fov = angle.fov; }
			Angle(float pitch, float roll, float yaw, float fov) { this->pitch = pitch; this->roll = roll; this->yaw = yaw; this->fov = fov; }

			//Getters.
			double Pitch() { return pitch; }
			double Roll() { return roll; }
			double Yaw() { return yaw; }
			double Fov() { return fov; }

			//Setters.
			void Pitch(float pitch) { this->pitch = pitch; }
			void Roll(float roll) { this->roll = roll; }
			void Yaw(float yaw) { this->yaw = yaw; }
			void Fov(float fov) { this->fov = fov; }
		};

		//Struct to store Position of camera.
		struct Position
		{
		private:
			double x;
			double y;
			double z;

		public:
			//Ctors/Dtors
			Position() = default;
			~Position() = default;
			Position(Position& pos) { this->x = pos.x; this->y = pos.y; this->z = pos.z; }
			Position(double x, double y, double z) { this->x = x; this->y = y; this->z = z; };

			//Getters.
			double X() { return x; }
			double Y() { return y; }
			double Z() { return z; }
			//Setters.
			void X(double x) { this->x = x; }
			void Y(double y) { this->y = y; }
			void Z(double z) { this->z = z; }
		};

		//Struct to store Controls of camera.
		struct Controls
		{
		private:
			key_t up;//Key event - Camera Up. (Z-Axis)
			key_t down;//Key event - Camera Down. (Z-Axis)
			key_t left;//Key event - Camera Left. (Y-Axis)
			key_t right;//Key event - Camera Right. (Y-Axis)
			key_t forward;//Key event - Camera Forward. (X-Axis)
			key_t backward;//Key event - Camera Backward. (X-Axis)
			key_t calibrate;//Key event - Camera Re-Calibrate. [Reset to X-Axis]
			key_t quit;//Key event - Quit Free cam.
			float axis_off; //Offset value to move Camera from Axis.

		public:
			//Ctor/Dtor
			Controls() = default;
			~Controls() = default;
			Controls(Controls& ctrl) { this->up = ctrl.up; this->down = ctrl.down; this->left = ctrl.left; this->right = ctrl.right; this->forward = ctrl.forward; this->backward = ctrl.backward; this->calibrate = ctrl.calibrate; this->quit = ctrl.quit; this->axis_off = ctrl.axis_off;}
			Controls(key_t up, key_t down, key_t left, key_t right, key_t forward, key_t backward, key_t calibrate, key_t quit, float axis_off) { this->up = up; this->down = down; this->left = left; this->right = right; this->forward = forward; this->backward = backward; this->calibrate = calibrate; this->quit = quit; this->axis_off = axis_off; };
		
			//Getters.
			key_t UP() { return up; }
			key_t DOWN() { return down; }
			key_t LEFT() { return left; }
			key_t RIGHT() { return right; }
			key_t FORWARD() { return forward; }
			key_t BACKWARD() { return backward; }
			key_t CALIBRATE() { return calibrate; }
			key_t QUIT() { return quit; }
			key_t AXIS_OFF() { return axis_off; }

			//Setters.
			void UP(key_t key) { this->up = key; }
			void DOWN(key_t key) { this->down = key; }
			void LEFT(key_t key) { this->left = key; }
			void RIGHT(key_t key) { this->right = key; }
			void FORWARD(key_t key) { this->forward = key; }
			void BACKWARD(key_t key) { this->backward = key; }
			void CALIBRATE(key_t key) { this->calibrate = key; }
			void QUIT(key_t key) { this->quit = key; }
			void AXIS_OFF(float axis_off) { this->axis_off = axis_off; }
		
		};

		//Camera Section.

		//Camera Default Ctors/Dtors.
		Camera() = default;
		~Camera() = default;

		//Camera Methods.
		Position ReadPosition();
		Angle ReadAngle();
		void WritePosition(Position& pos);
		void WritePosition(double x);
		void WritePosition(double x, double y);
		void WritePosition(double x, double y, double z);
		void WriteAngle(Camera::Angle angle);
		void WritePitch(float pitch);
		void WriteRoll(float roll);
		void WriteYaw(float yaw);
		void WriteFov(float fov);
		void CalibrateView();
		void AttachCam();
		void DeattachCam();
		void EnableFreeCam(Camera::Controls&);
	};
	inline Camera g_Camera;
}