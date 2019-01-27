#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>

#define WINDOWS_LEAN_AND_MEAN

namespace alib
{
	class ImageCreator
	{
	public:
		int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
		{
			UINT  num = 0;		  // number of image encoders
			UINT  size = 0;		 // size of the image encoder array in bytes
			Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
			Gdiplus::GetImageEncodersSize(&num, &size);
			if (size == 0)
				return -1;  // Failure
			pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
			if (pImageCodecInfo == NULL)
				return -1;  // Failure
			Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
			for (UINT j = 0; j < num; ++j)
			{
				if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
				{
					*pClsid = pImageCodecInfo[j].Clsid;
					free(pImageCodecInfo);
					return j;  // Success
				}
			}
			free(pImageCodecInfo);
			return -1;
		}

		void createBMP(unsigned char *data, int w, int h, int comp, std::string out_file)
		{
			FILE *f;
			int filesize = 54 + comp * w * h;
			unsigned char bmpfileheader[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
			unsigned char bmpinfoheader[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
			unsigned char bmppad[3] = { 0, 0, 0 };

			unsigned char *output_data = new unsigned char[w*h * comp];
			for (int i = 0; i < w*h*comp; i += comp)
			{
				output_data[i] = data[i + 2];
				output_data[i + 1] = data[i + 1];
				output_data[i + 2] = data[i + 0];
			}

			bmpfileheader[2] = (unsigned char)(filesize);
			bmpfileheader[3] = (unsigned char)(filesize >> 8);
			bmpfileheader[4] = (unsigned char)(filesize >> 16);
			bmpfileheader[5] = (unsigned char)(filesize >> 24);

			bmpinfoheader[4] = (unsigned char)(w);
			bmpinfoheader[5] = (unsigned char)(w >> 8);
			bmpinfoheader[6] = (unsigned char)(w >> 16);
			bmpinfoheader[7] = (unsigned char)(w >> 24);
			bmpinfoheader[8] = (unsigned char)(h);
			bmpinfoheader[9] = (unsigned char)(h >> 8);
			bmpinfoheader[10] = (unsigned char)(h >> 16);
			bmpinfoheader[11] = (unsigned char)(h >> 24);

			f = fopen("temp.bmp", "wb");
			fwrite(bmpfileheader, 1, 14, f);
			fwrite(bmpinfoheader, 1, 40, f);
			for (int i = 0; i<h; i++)
			{
				fwrite(output_data + (w*(h - i - 1) * 3), 3, w, f);
				fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
			}
			fclose(f);

			Gdiplus::GdiplusStartupInput startupInput;
			ULONG_PTR token;
			Gdiplus::GdiplusStartup(&token, &startupInput, NULL);
			Gdiplus::Image testImg(L"temp.bmp", false);
			CLSID clsid;
			int ret = -1;
			if (-1 != GetEncoderClsid(L"image/jpeg", &clsid))
			{
				ret = testImg.Save(L"output.jpg", &clsid);
			}
			int result;
			char oldname[] = "output.jpg";
			result = rename(oldname, out_file.c_str());
		}

	};
}

