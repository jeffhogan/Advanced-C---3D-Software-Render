//////////////////////////////
// CORE.Drawing.3D.cpp
//////////////////////////////

#include "..\GameCore.h"

namespace GameCore {

	void Color3D::SetColor(double a, double r, double g, double b) {

		this->a = a;
		this->r = r;
		this->g = g;
		this->b = b;

	}

	void Point3D::SetPoint(double x, double y, double z, double w) {
	
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;

	}

	void Point3D::_Homogenize() {

		this->x = this->x/w;
		this->y = this->y/w;
		this->z = this->z/w;
		this->w = 1;

	}

	void Point3D::_Translate(float x, float y, float z, float w) {

		this->x += x;
		this->y += y;
		this->z += z;
		this->w = 1;

	}

	void Triangle3D::_SetUV(const TextureUV& uv0, const TextureUV& uv1, const TextureUV& uv2) {

		this->uv0 = uv0;
		this->uv1 = uv1;
		this->uv2 = uv2;

	}

	#define _PUTPIXEL(video, x, y, width, height, c) ((video)[(y)*(width)+(x)] = (c))
	#define _COLOR32_ARGB(a, r, g, b) (((int)((a))<<24) + (((int)(r))<<16) + (((int)(g))<<8) + ((int)(b)))

	//barycentric calculations
	#define _f12(xx,yy) ((v1.y-v2.y)*(xx) + (v2.x-v1.x)*(yy) + v1.x*v2.y - v2.x*v1.y)
	#define _f20(xx,yy) ((v2.y-v0.y)*(xx) + (v0.x-v2.x)*(yy) + v2.x*v0.y - v0.x*v2.y)
	#define _f01(xx,yy) ((v0.y-v1.y)*(xx) + (v1.x-v0.x)*(yy) + v0.x*v1.y - v1.x*v0.y)
	#define __min3f(a, b, c) ((a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c)))
	#define __max3f(a, b, c) ((a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c)))

	void _DrawTriangle(Mesh3D *mesh, int *video, double *pZBuffer, int width, int height, const Triangle3D &triangle) {

		const Point3D &v0 = triangle.a;
		const Point3D &v1 = triangle.b;
		const Point3D &v2 = triangle.c;

		const TextureUV &uv0 = triangle.uv0;
		const TextureUV &uv1 = triangle.uv1;
		const TextureUV &uv2 = triangle.uv2;

		int xmin = (int)__min3f(v0.x, v1.x, v2.x);
		int ymin = (int)__min3f(v0.y, v1.y, v2.y);
		int xmax = (int)__max3f(v0.x, v1.x, v2.x);
		int ymax = (int)__max3f(v0.y, v1.y, v2.y);

		if(xmin < 0) xmin = 0;
		if(ymin < 0) ymin = 0;
		if(xmax > width-1) xmax = width-1;
		if(ymax > height-1) ymax = height-1;

		const Color3D &c0 = v0.color;
		const Color3D &c1 = v1.color;
		const Color3D &c2 = v2.color;

		////////////////////////////////////////////////////////////

		float alpha, beta, gamma;
		float r, g, b, a;

		for(int y = ymin; y <= ymax; y++) {

			for(int x = xmin; x <= xmax; x++) {

				alpha = _f12(x,y)/_f12(v0.x,v0.y);
				beta = _f20(x,y)/_f20(v1.x,v1.y);
				gamma = _f01(x,y)/_f01(v2.x,v2.y);

				if(alpha>=0 && beta>=0 && gamma>=0) {

					double z = alpha*v0.z + beta*v1.z + gamma*v2.z;

					float tu = alpha*uv0.tu + beta*uv1.tu + gamma*uv2.tu;
					float tv = alpha*uv0.tv + beta*uv1.tv + gamma*uv2.tv;

					int tx = tu * mesh->texture_width;
					int ty = tv * mesh->texture_height;

					int pixel = mesh->pTexture[ty * mesh->texture_width + tx];

					double zb = pZBuffer[y*width+x];
					if(z < zb) continue;
					pZBuffer[y*width+x] = z;

					// Calculate interpolated color //////////////////

					r = alpha*c0.r + beta*c1.r + gamma*c2.r;
					g = alpha*c0.g + beta*c1.g + gamma*c2.g;
					b = alpha*c0.b + beta*c1.b + gamma*c2.b;
					a = 1;


					_PUTPIXEL(video, x, y, width, height, pixel);
					//_PUTPIXEL(video, x, y, width, height, _COLOR32_ARGB(a*255,r*255,g*255,b*255));
				}
			}
		}
	}

	void _DrawTriangle(Mesh3D *mesh, Matrix3D *m, int *video, double *pZBuffer, int width, int height, const Triangle3D& triangle) {
	
		if(m == NULL) return;

		//make a copy of the triangle
		Triangle3D t = triangle;

		//apply the transformation matrix to the triangle
		t.a = m->_Multiply(t.a);
		t.b = m->_Multiply(t.b);
		t.c = m->_Multiply(t.c);

		//clip any triangle that are behind the viewer
		if(t.a.z > 0.0 && t.b.z > 0.0 && t.c.z > 0.0) return;

		//perspective step
		t.a._Homogenize();
		t.b._Homogenize();
		t.c._Homogenize();

		//push camera origin to center of viewport
		t.a._Translate(width/2, height/2, 0.0, 0.0);
		t.b._Translate(width/2, height/2, 0.0, 0.0);
		t.c._Translate(width/2, height/2, 0.0, 0.0);

		_DrawTriangle(mesh, video, pZBuffer, width, height, t);

	}

	/*void _DrawTriangle(Matrix3D *m, int *video, int width, int height, int nNumTriangles, Triangle3D triangles[]) {

		_DrawTriangle(m, video, width, height, triangles[n]);


	}*/

	void _DrawTriangleList(Mesh3D *mesh, Matrix3D *m, int *video, double *pZBuffer, int width, int height) {
	
		for(int n = 0; n < mesh->num_t; n++){

			_DrawTriangle(mesh, m, video, pZBuffer, width, height, mesh->pTriangles[n]);

		}
	
	}

	Triangle3D* createCube() {

		double xmin = -100.0, xmax = 100.0;
		double ymin = -100.0, ymax = 100.0;
		double zmin = -100.0, zmax = 100.0;

		Point3D v[] = {
			{ xmin, ymax, zmin, 1.0 }, // A 0
			{ xmin, ymin, zmin, 1.0 }, // B 1
			{ xmax, ymin, zmin, 1.0 }, // C 2
			{ xmax, ymax, zmin, 1.0 }, // D 3
			{ xmin, ymax, zmax, 1.0 }, // E 4
			{ xmin, ymin, zmax, 1.0 }, // F 5
			{ xmax, ymin, zmax, 1.0 }, // G 6
			{ xmax, ymax, zmax, 1.0 }, // H 7
		};

		Color3D c0 = { 1.0, 1.0, 0.0, 0.0 };
		Color3D c1 = { 1.0, 0.0, 1.0, 0.0 };
		Color3D c2 = { 1.0, 0.0, 0.0, 1.0 };

		for(int n = 0; n < 8; n++){
			
			if(n % 3 == 0) {
			
				v[n].color = c0;

			}
			else if (n % 3 == 1) {

				v[n].color = c1;

			}
			else if (n % 3 == 2) {

				v[n].color = c2;

			}
		
		}
			
		Triangle3D t[] = {
			{ v[0], v[2], v[1] }, //back
			{ v[0], v[3], v[2] },
			{ v[1], v[2], v[5] }, //front
			{ v[2], v[6], v[5] },
			{ v[0], v[1], v[5] }, //left
			{ v[0], v[5], v[4] },
			{ v[4], v[5], v[7] }, //right
			{ v[7], v[5], v[6] },
			{ v[3], v[7], v[2] }, //top
			{ v[2], v[7], v[6] },
			{ v[0], v[4], v[7] }, //bottom
			{ v[0], v[7], v[3] },
		};

		TextureUV cube_uv[] = {
			{ 0.0, 0.0 },
			{ 0.0, 1.0 },
			{ 1.0, 0.0 },
			{ 1.0, 1.0 }
		};

		//setting the texture
		t[0]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[1]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[2]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[3]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[4]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[5]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[6]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[7]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[8]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[9]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[10]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);
		t[11]._SetUV(cube_uv[0], cube_uv[1], cube_uv[2]);

		Triangle3D* cube = new Triangle3D[12];
		for(int n = 0; n < 12; n++) {
			cube[n] = t[n];
		}
		return cube;
	
	}


	//void _DrawTriangle(int *video, int width, int height, int nNumTriangles, Triangle3D triangles[]) {
	//		
	//	for(int n = 0; n < nNumTriangles; n++){

	//		_DrawTriangle(video, width, height, triangles[n]);

	//	}

	//}

	//#define _PUTPIXEL(video, x, y, width, height, c) ((video)[(y)*(width)+(x)] = (c))
	//#define _COLOR32_ARGB(a, r, g, b) (((int)((a))<<24) + (((int)(r))<<16) + (((int)(g))<<8) + ((int)(b)))

	////barycentric calculations
	//#define _f12(xx,yy) ((v1.y-v2.y)*(xx) + (v2.x-v1.x)*(yy) + v1.x*v2.y - v2.x*v1.y)
	//#define _f20(xx,yy) ((v2.y-v0.y)*(xx) + (v0.x-v2.x)*(yy) + v2.x*v0.y - v0.x*v2.y)
	//#define _f01(xx,yy) ((v0.y-v1.y)*(xx) + (v1.x-v0.x)*(yy) + v0.x*v1.y - v1.x*v0.y)
	//#define __min3f(a, b, c) ((a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c)))
	//#define __max3f(a, b, c) ((a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c)))

	//void _DrawTriangle(int *video, int width, int height, const Triangle3D &triangle) {

	//	const Point3D &v0 = triangle.a;
	//	const Point3D &v1 = triangle.b;
	//	const Point3D &v2 = triangle.c;

	//	int xmin = (int)__min3f(v0.x, v1.x, v2.x);
	//	int ymin = (int)__min3f(v0.y, v1.y, v2.y);
	//	int xmax = (int)__max3f(v0.x, v1.x, v2.x);
	//	int ymax = (int)__max3f(v0.y, v1.y, v2.y);

	//	const Color3D &c0 = v0.color;
	//	const Color3D &c1 = v1.color;
	//	const Color3D &c2 = v2.color;

	//	////////////////////////////////////////////////////////////

	//	float alpha, beta, gamma;
	//	float r, g, b, a;

	//	for(int y = ymin; y <= ymax; y++) {

	//		for(int x = xmin; x <= xmax; x++) {

	//			alpha = _f12(x,y)/_f12(v0.x,v0.y);
	//			beta = _f20(x,y)/_f20(v1.x,v1.y);
	//			gamma = _f01(x,y)/_f01(v2.x,v2.y);

	//			if(alpha>=0 && beta>=0 && gamma>=0) {

	//				// Calculate interpolated color //////////////////

	//				r = alpha*c0.r + beta*c1.r + gamma*c2.r;
	//				g = alpha*c0.g + beta*c1.g + gamma*c2.g;
	//				b = alpha*c0.b + beta*c1.b + gamma*c2.b;
	//				a = 1;


	//				_PUTPIXEL(video, x, y, width, height, 
	//					_COLOR32_ARGB(a*255,r*255,g*255,b*255)
	//					);
	//			}
	//		}
	//	}
	//}
	
}

