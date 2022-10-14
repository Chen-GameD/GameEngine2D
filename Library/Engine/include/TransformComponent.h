#pragma once
#include <math.h>
#include "Vector2D.h"
#include "Matrix4x4.h"

namespace Engine {

	class TransformComponent
	{
	public:
		Vector2D position;
		Vector2D rotation;
		Vector2D scale;


		TransformComponent(Vector2D vec1, Vector2D vec2, Vector2D vec3) {
			position = vec1;
			rotation = vec2;
			scale = vec3;
		}

		Matrix4x4 GetModelMatrix() {
			Matrix4x4 modelMatrix = GetTransportMatrix() * GetRotateMatrix() * GetScaleMatrix();
			return modelMatrix;
		}
	private:
		double pi = 2 * acos(0.0);
		Matrix4x4 GetScaleMatrix() {
			Vector4D vec0(scale.GetX(), 0, 0, 0);
			Vector4D vec1(0, scale.GetY(), 0, 0);
			Vector4D vec2(0, 0, 1, 0);
			Vector4D vec3(0, 0, 0, 1);
			Matrix4x4 scaleMatrix(vec0, vec1, vec2, vec3);
			return scaleMatrix;
		}

		Matrix4x4 GetRotateMatrix() {
			Vector4D rot2_4(rotation.GetX(), rotation.GetY(), 1, 1);
			Vector4D rot = rot2_4 * pi / 180;
			Vector4D vec0(1, 0, 0, 0);
			Vector4D vec1(0, cos(rot.GetX()), -sin(rot.GetX()), 0);
			Vector4D vec2(0, sin(rot.GetX()), cos(rot.GetX()), 0);
			Vector4D vec3(0, 0, 0, 1);
			Matrix4x4 XRotMatrix(vec0, vec1, vec2, vec3);


			Vector4D vec0_0(cos(rot.GetY()), 0, sin(rot.GetY()), 0);
			Vector4D vec1_0(0, 1, 0, 0);
			Vector4D vec2_0(-sin(rot.GetY()), 0, cos(rot.GetY()), 0);
			Vector4D vec3_0(0, 0, 0, 1);
			Matrix4x4 YRotMatrix(vec0_0, vec1_0, vec2_0, vec3_0);


			Vector4D vec0_1(cos(rot.GetZ()), -sin(rot.GetZ()), 0, 0);
			Vector4D vec1_1(sin(rot.GetZ()), cos(rot.GetZ()), 0, 0);
			Vector4D vec2_1(0, 0, 1, 0);
			Vector4D vec3_1(0, 0, 0, 1);
			Matrix4x4 ZRotMatrix(vec0_1, vec1_1, vec2_1, vec3_1);

			Matrix4x4 rotMatrix = ZRotMatrix * XRotMatrix * YRotMatrix;
			return rotMatrix;
		}

		Matrix4x4 GetTransportMatrix() {
			Vector4D vec0(1, 0, 0, 0);
			Vector4D vec1(0, 1, 0, 0);
			Vector4D vec2(0, 0, 1, 0);
			Vector4D vec3(position.GetX(), position.GetY(), 1, 1);
			Matrix4x4 scaleMatrix(vec0, vec1, vec2, vec3);
			return scaleMatrix;
		}


	};
}