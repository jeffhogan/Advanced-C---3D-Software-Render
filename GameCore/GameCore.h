//////////////////////////////
// GameCore.h
//////////////////////////////

#ifndef __GAME_CORE_H_
#define __GAME_CORE_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//additional includes here... .h files for each .cpp module
#include "CORE\CORE.Resources.h"
#include "CORE\CORE.Drawing.3D.h"
#include "CORE\CORE.Matrix.h"

namespace GameCore {

using namespace System;

	public ref class GameClass {
	public:

		void _OnInitialize(int nWidth, int nHeight);
		void _OnDraw(int nWidth, int nHeight, int nStride, IntPtr pSurface);
		void _OnRunAI();
	
	};
}

#endif
