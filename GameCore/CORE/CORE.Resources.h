//////////////////////////////
// CORE.Resources.h
//////////////////////////////

#ifndef __CORE_RESOURCES_H_
#define __CORE_RESOURCES_H_

#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Windows::Forms;

namespace GameCore {

	//////////////////////////////
	// Resource Information
	//////////////////////////////

	public class Resources {

	public:
		
		static int *m_cube_texture;
		// add resource variables here

	};

	public ref class ResourceLoader {

	public:
		static void _LoadResources();

	};

	//////////////////////////////
	// Utility Functions
	//////////////////////////////

	BitmapData^ _GetImageData(Bitmap^ bmp);
	void _LoadImage(int** pDest, IntPtr pSurface, int length);
	void _LoadImage(int** pDest, Bitmap^ bmp);

}

#endif