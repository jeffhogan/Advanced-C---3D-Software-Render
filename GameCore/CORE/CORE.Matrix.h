// CORE.Matrix.h
/////////////////////////////////////////////////////////////////////////

#ifndef __COREMATRIX3D_H_
#define __COREMATRIX3D_H_

using namespace System;

namespace GameCore {

	class Point3D;

	public class Matrix3D {
	public:
		Matrix3D();
		Matrix3D(float m[16]);
		void _Initialize(float m[16]);
		void _SetIdentityMatrix();
		void _SetPerspectiveProjectionMatrix(float near, float far, float width, float height);

		Point3D _Multiply(Point3D v);
		Matrix3D _Multiply(Matrix3D m);

		void _SetEqualTo(Matrix3D m);

		void _Scale(float fAmt);
		void _ScalePerspective(float fAmt);

		void _TranslateAlongObjectUp(float fAmt);
		void _TranslateAlongObjectForward(float fAmt);
		void _TranslateAlongObjectRight(float fAmt);

		Matrix3D _CreateRotationAroundWorldUpVector(float fAmt);
		Matrix3D _CreateRotationAroundWorldForwardVector(float fAmt);
		Matrix3D _CreateRotationAroundWorldRightVector(float fAmt);

		Matrix3D _CreateTranslateAlongWorldUp(float fAmt);
		Matrix3D _CreateTranslateAlongWorldForward(float fAmt);
		Matrix3D _CreateTranslateAlongWorldRight(float fAmt);

		void _RotateAroundWorldUpVector(float fAmt);
		void _RotateAroundWorldForwardVector(float fAmt);
		void _RotateAroundWorldRightVector(float fAmt);

		void _TranslateAlongWorldUp(float fAmt);
		void _TranslateAlongWorldForward(float fAmt);
		void _TranslateAlongWorldRight(float fAmt);

	public:
		float r0c0, r0c1, r0c2, r0c3;
		float r1c0, r1c1, r1c2, r1c3;
		float r2c0, r2c1, r2c2, r2c3;
		float r3c0, r3c1, r3c2, r3c3;
	};

}

#endif