//////////////////////////////
// GameCore.cpp
//////////////////////////////

#include "GameCore.h"

namespace GameCore {

	// <add global variables here>
	float g_fDegrees = 0.0;
	float g_fRadians = 0.0;
	Triangle3D cube[12];
	double *g_pZBuffer = NULL;

	void GameClass::_OnInitialize(int nWidth, int nHeight) {

		Triangle3D* cubeTemp = createCube();
		for(int i = 0; i < 12; i++) {

			cube[i] = cubeTemp[i];

		}

	}

	void GameClass::_OnRunAI() {



	}

	void _ClearBackground(int *video, int nActualWidth, int nHeight, int color) {
		for(int n = 0; n < nActualWidth*nHeight; n++) {
			video[n] = color; 
		}
	}

	void _ClearZBuffer(double *pZBuffer, int nActualWidth, int nHeight) {

		if(pZBuffer == NULL) return;
		int n = nActualWidth*nHeight;
		do {

			*pZBuffer = -99999.0;
			pZBuffer++;

		} while(--n > 0);

	}

	void GameClass::_OnDraw(int nWidth, int nHeight, int nStride, IntPtr pSurface) {

		int* video = (int*)pSurface.ToPointer();
		int nActualWidth = nStride / 4;
		if(g_pZBuffer == NULL) {
			g_pZBuffer = new double[nActualWidth * nHeight];
		}
		_ClearBackground(video, nActualWidth, nHeight, 0xFF000000);

		//Drawing code goes here..

		//lab3

		//clear z-buffer
		_ClearZBuffer(g_pZBuffer, nActualWidth, nHeight);

		g_fDegrees += 5.0;
		if(g_fDegrees > 360.0) g_fDegrees = 0.0;
		g_fRadians = g_fDegrees * (3.141592653 / 180.0);

		Matrix3D m;
		m._SetIdentityMatrix();
		m._RotateAroundWorldUpVector(g_fRadians);
		m._RotateAroundWorldRightVector(3.141592653/4.0);
		m._TranslateAlongWorldForward(5000.0);

		Mesh3D mesh;

		mesh.num_t = 12;
		mesh.pTriangles = cube;

		mesh.texture_width = 256;
		mesh.texture_height = 256;

		mesh.pTexture = GameCore::Resources::m_cube_texture;

		//_DrawTriangle(&m, video, nActualWidth, nHeight, triangle);
		_DrawTriangleList(&mesh, &m, video, g_pZBuffer, nActualWidth, nHeight);
		
		
	}

}

