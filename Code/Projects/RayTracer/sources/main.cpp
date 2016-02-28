#include "RayTracer/Scene.h"
#include "RayTracer/Geometry/Plane.h"
#include "RayTracer/Geometry/Sphere.h"
#include "RayTracer/RayTracer.h"
#include "Collections/CConstSize2DArray.h"
#include "bmp\SaveToBMP.h"
#include <memory>
#include <iostream>
#include "Stopwatch.h"
#include "RayTracer/Downsampling.h"
#include "RayTracer/Refraction.h"
#include "BGR.h"

template<size_t w, size_t h, bool is_static>
using BitMap = Collections::CConstSize2DArray<w, h, BGR, is_static>;

void main()
{
#pragma region Constants
    const bool bitmapDataInStack = false;
    const size_t upsampling = 1;
    const size_t threadsCount = 3;
    const size_t pictureWidth = 3200 * 4;
    const size_t pictureHeight = (pictureWidth / 16) * 9;
    const size_t picturePixelsCount = pictureWidth * pictureHeight;
    const size_t upsampledPictureWidth = pictureWidth * upsampling;
    const size_t upsampledPictureHeight = pictureHeight * upsampling;
    const std::string filename("1.bmp");

    if (upsampling > 1)
    {
        std::cout << "Upsampling rate: " << upsampling << std::endl;
        std::cout << "Upsampled picture width: " << upsampledPictureWidth << std::endl;
        std::cout << "Upsampled picture height: " << upsampledPictureHeight << std::endl;
    }
    std::cout << "Threads count: " << threadsCount << std::endl;
    std::cout << "Output picture width: " << pictureWidth << std::endl;
    std::cout << "Output picture height: " << pictureHeight << std::endl;
    std::cout << "Output filename: " << filename << std::endl;
#pragma endregion
#pragma region Usings
    using TBitMap = BitMap<pictureWidth, pictureHeight, bitmapDataInStack>;
    using TBitMapPtr = std::unique_ptr<TBitMap>;

    using TUpsampledBitMap = BitMap<upsampledPictureWidth, upsampledPictureHeight, bitmapDataInStack>;
    using TUpsampledBitMapPtr = std::unique_ptr<TUpsampledBitMap>;

    using Geom = RayTracer::Geometry::Geometry;
    using Tracer = RayTracer::RayTracer;
    using namespace RayTracer;
    using namespace Geometry;
#pragma endregion
#pragma region Scene Preparing
    std::cout << "Preparing scene" << std::endl;
    Plane* back = new Plane(Vector3f(0, 0, 40), Vector3f(0, 0, -1), Color(255, 255, 255));

    Plane* floor = new Plane(Vector3f(0, 0, 0), Vector3f(0, 1, 0), Color(0, 0, 255),
        Plane::defaultRefractionCoeff, Plane::defaultRefraction, 0.75);

    Plane* left = new Plane(Vector3f(-30, 0, 0), Vector3f(1, 0, 0), Color(255, 255, 0),
        Plane::defaultRefractionCoeff, Plane::defaultRefraction, 0.2);

    Plane* right = new Plane(Vector3f(30, 0, 0), Vector3f(-1, 0, 0), Color(0, 255, 0));

    Sphere* ball0 = new Sphere(Vector3f(-10, 10, 10), 5, Color(255, 165, 0),
        Plane::defaultRefractionCoeff, Plane::defaultRefraction, 0.75);

    Sphere* ball1 = new Sphere(Vector3f(10, 0, 10), 5, Color(64, 64, 64),
        Plane::defaultRefractionCoeff, Plane::defaultRefraction, 0.25);

    Sphere* ball2 = new Sphere(Vector3f(5, 3, 1), 2.5, Color(255, 0, 0),
        Plane::defaultRefractionCoeff, Plane::defaultRefraction, 0.75);

    Sphere* ball3 = new Sphere(Vector3f(-5, 3, 1), 2.5,
        Color(153, 255, 255), Refraction::helium, 1);

    RaySource raySource(Color(255, 255, 255), Vector3f(25, 20, 10));

    const Vector2f cameraPane = Vector2f(1.6, 0.9);
    const Vector3f cameraPosition = Vector3f(0, 10, -50);
    Scene scene(cameraPane, cameraPosition, Refraction::air,
    { raySource }, { floor, back, left, right, ball0, ball1, ball2, ball3 });

#pragma endregion
#pragma region Ray tracing
    Tracer rayTracer;
    TUpsampledBitMapPtr upsampledBitmap = std::make_unique<TUpsampledBitMap>();

    rayTracer.AddOnHitScreen(
        [&](const Color& color, const Vector2f& position) -> void
    {
        auto& pixel = (*upsampledBitmap)
            [static_cast<size_t>(position.y())]
            [static_cast<size_t>(position.x())];
        pixel.rgbRed = static_cast<BYTE>(255 * color.GetRed());
        pixel.rgbGreen = static_cast<BYTE>(255 * color.GetGreen());
        pixel.rgbBlue = static_cast<BYTE>(255 * color.GetBlue());
        pixel.rgbReserved = 0;
    });

    rayTracer.AddOnUpdateProgress([](int nextPixel, int maxPixels)
    {
        printf_s("\r%2.f%%", 100.0 * (double)nextPixel / (double)maxPixels);
    });

    std::cout << "Ray tracing" << std::endl;
    Stopwatch<std::chrono::high_resolution_clock> stopwatch;
    rayTracer.Run(scene, upsampledPictureWidth, upsampledPictureHeight, threadsCount);
    std::cout << "\rRay tracing duration: " <<
        stopwatch.GetElapsedTime<std::chrono::seconds>() << " seconds." << std::endl;
#pragma endregion
#pragma region Downsampling
    TBitMapPtr downsampledBitmap = nullptr;
    if (upsampling > 1)
    {
        std::cout << "Downsampling (x" << upsampling << ')' << std::endl;
        stopwatch.Start();
        TBitMapPtr downsampledBitmap = std::make_unique<TBitMap>();
        MakeDownsampling(*upsampledBitmap, *downsampledBitmap);
        std::cout << "Downsampling duration: " << stopwatch.GetElapsedTime<std::chrono::seconds>()
            << " seconds." << std::endl;

        //Release upsampled
        upsampledBitmap = nullptr;
    }
#pragma endregion
#pragma region Saving
    std::cout << "Saving to bmp" << std::endl;
    stopwatch.Start();
    bmp::SaveToBMP(filename, upsampling > 1 ? *downsampledBitmap : *upsampledBitmap);
    std::cout << "Save duration: " << stopwatch.GetElapsedTime<std::chrono::seconds>()
        << " seconds." << std::endl;
#pragma endregion

    system(filename.c_str());

    std::cout << "Press any key to continue..." << std::endl;
    std::cin.get();
}