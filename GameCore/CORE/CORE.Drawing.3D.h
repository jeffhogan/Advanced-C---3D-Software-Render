//////////////////////////////
// CORE.Drawing.3D.h
//////////////////////////////

#ifndef __GAME_CORE_DRAWING_3D_H_
#define __GAME_CORE_DRAWING_3D_H_
#include "CORE.Matrix.h"

using namespace System;

namespace GameCore {

	class Color3D {
	public:
		void SetColor(double a, double r, double g, double b);
		double a, r, g, b;
	};

	class Point3D {
	public:
		void SetPoint(double x, double y, double z, double w);
		void _Homogenize();
		void _Translate(float x, float y, float z, float w);
		double x, y, z, w;
		Color3D color;
	};

	class TextureUV {
	public:
		double tu, tv;

	};

	class Triangle3D {
	public:
		Point3D a, b, c;
		TextureUV uv0, uv1, uv2;
		void _SetUV(
			const TextureUV& uv0,
			const TextureUV& uv1,
			const TextureUV& uv2
			);
	};

	class Mesh3D {
	public:
		int num_t;
		Triangle3D *pTriangles;

		int texture_width, texture_height;
		int *pTexture;

	};

	Triangle3D* createCube();
	void _DrawTriangle(Mesh3D *mesh, Matrix3D *m, int *video, double *pZBuffer, int width, int height, const Triangle3D& triangle);
	void _DrawTriangle(Mesh3D *mesh, int *video, double *pZBuffer, int width, int height, int nNumTriangles, Triangle3D triangles[]);
	void _DrawTriangle(Mesh3D *mesh, int *video, double *pZBuffer, int width, int height, const Triangle3D &triangle);
	void _DrawTriangleList(Mesh3D *mesh, Matrix3D *m, int *video, double *pZBuffer, int width, int height);

}

#endif
