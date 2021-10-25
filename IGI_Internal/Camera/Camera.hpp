#pragma once
#include "Common.hpp"
#include "CommonConst.hpp"
#include "GTLibc.hpp"

namespace IGI {
	class Camera
	{
	private:
		//Class to store angle of camera.
		struct Angle
		{
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
			double Pitch() { return pitch; }
			double Roll() { return roll; }
			double Yaw() { return yaw; }
			double Fov() { return fov; }
		};

		//Class to store Position of camera.
		struct Position
		{
			double x;
			double y;
			double z;

		public:
			double X() { return x; }
			double Y() { return y; }
			double Z() { return z; }
		};

	public:
		Camera() = default;
		~Camera() = default;

		Position ReadPosition();
		Angle ReadAngle();
	};
}