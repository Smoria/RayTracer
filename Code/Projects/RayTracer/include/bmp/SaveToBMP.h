#ifndef SAVETOBMP_H_INCLUDED
#define SAVETOBMP_H_INCLUDED

#define NOMINMAX

#include <Windows.h>
#include "Collections\CConstSize2DArray.h"

namespace bmp
{
    template<size_t imageWidth, size_t imageHeight, class T>
    void SaveToBMP(const std::string& fileName, const Collections::CConstSize2DArray<imageWidth, imageHeight, T>& data)
    {
        BITMAPFILEHEADER bfh;
        BITMAPINFOHEADER bih;

        memset(&bfh, 0, sizeof(bfh));
        memset(&bih, 0, sizeof(bih));

        const size_t pixelBytes = sizeof(T);
        const size_t pixelBits = pixelBytes * 8;

        bfh.bfType = 0x4D42;
        bfh.bfOffBits = sizeof(bfh) + sizeof(bih);
        bfh.bfSize = bfh.bfOffBits +
            sizeof(pixelBytes) * imageWidth * imageHeight +
            imageHeight * (((4 - pixelBytes) * imageWidth) % 4);
        memset(&bih, 0, sizeof(bih));
        bih.biSize = sizeof(bih);
        bih.biBitCount = pixelBits;
        bih.biClrUsed = 0;
        bih.biCompression = BI_RGB;
        bih.biHeight = imageHeight;
        bih.biWidth = imageWidth;
        bih.biPlanes = 1;

        HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            return;

        DWORD RW;
        WriteFile(hFile, &bfh, sizeof(bfh), &RW, NULL);
        WriteFile(hFile, &bih, sizeof(bih), &RW, NULL);

        const uint32_t null = 0;

        for (size_t i = 0; i < imageHeight; i++)
        {
            for (size_t j = 0; j < imageWidth; j++)
            {
                const T& color = data[i][j];
                WriteFile(hFile, &color, pixelBytes, &RW, NULL);
            }

            WriteFile(hFile, &null, ((4 - pixelBytes) * imageWidth) % 4, &RW, NULL);
        }
        CloseHandle(hFile);
    }
}

#endif /*SAVETOBMP_H_INCLUDED*/