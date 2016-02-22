#pragma once

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
        const size_t paddingSize = ((4 - pixelBytes) * imageWidth) % 4;

        bfh.bfType = 0x4D42;
        bfh.bfOffBits = sizeof(bfh) + sizeof(bih);
        bfh.bfSize = bfh.bfOffBits +
            sizeof(pixelBytes) * imageWidth * imageHeight +
            imageHeight * paddingSize;
        memset(&bih, 0, sizeof(bih));
        bih.biSize = sizeof(bih);
        bih.biBitCount = pixelBits;
        bih.biClrUsed = 0;
        bih.biCompression = BI_RGB;
        bih.biHeight = imageHeight;
        bih.biWidth = imageWidth;
        bih.biPlanes = 1;

        HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
        if(hFile == INVALID_HANDLE_VALUE)
            return;

        DWORD RW;
        WriteFile(hFile, &bfh, sizeof(bfh), &RW, NULL);
        WriteFile(hFile, &bih, sizeof(bih), &RW, NULL);

        const uint32_t null = 0;
        const size_t bufferSize = imageWidth * pixelBytes;

        for(size_t i = 0; i < imageHeight; i++)
        {
            WriteFile(hFile, &data[i], bufferSize, &RW, NULL);

            if(paddingSize > 0)
            {
                WriteFile(hFile, &null, paddingSize, &RW, NULL);
            }
        }

        CloseHandle(hFile);
    }
}