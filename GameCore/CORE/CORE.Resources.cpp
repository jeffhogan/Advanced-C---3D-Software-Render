//////////////////////////////
// CORE.Resources.cpp
//////////////////////////////

#include "..\GameCore.h"

namespace GameCore {

	//////////////////////////////
	// Resource Infomration
	//////////////////////////////

	int *Resources::m_cube_texture = NULL;
	// add resource variables here

	void ResourceLoader::_LoadResources() {

		//function slightly unhappy...possibly because no exampleresource exists yet
		_LoadImage(&Resources::m_cube_texture, GameResources::Properties::Resources::cube_texture);
		//load more resources here

	}

	//////////////////////////////
	// Utility Functions
	//////////////////////////////

	BitmapData^ _GetImageData(Bitmap^ bmp) {

		Rectangle^ rect = gcnew Rectangle(0, 0, bmp->Width, bmp->Height);
		BitmapData^ bmpData = bmp->LockBits(*rect, ImageLockMode::ReadWrite, PixelFormat::Format32bppArgb);
		return bmpData;

	}

	void _LoadImage(int** pDest, IntPtr pSurface, int length) {

		int *pSrc = (int*)pSurface.ToPointer();
		*pDest = new int[length];
		memcpy(*pDest, pSrc, length);

	}

	void _LoadImage(int** pDest, Bitmap^ bmp) {

		BitmapData^ bmpData = _GetImageData(bmp);
		_LoadImage(pDest, bmpData->Scan0, bmpData->Stride * bmpData->Height);
		bmp->UnlockBits(bmpData);

	}

}