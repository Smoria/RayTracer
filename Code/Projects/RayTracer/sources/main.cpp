#include "RayTracer/Scene.h"
#include "RayTracer/Geometry/Plane.h"
#include "RayTracer/Geometry/Sphere.h"
#include "RayTracer/RayTracer.h"
#include "Collections/CConstSize2DArray.h"
#include "bmp\SaveToBMP.h"
#include <memory>
#include <iostream>

static const double refraction_air = 1.000293;
static const double refraction_helium = 1.000036;
const size_t upsampling = 1;
const size_t threadsCount = 4;
const size_t pictureWidth = 3200;
const size_t pictureHeight = (pictureWidth / 16) * 9;
const size_t upsampledPictureWidth = pictureWidth * upsampling;
const size_t upsampledPictureHeight = pictureHeight * upsampling;

template<size_t w, size_t h>
using BitMap = Collections::CConstSize2DArray<w, h, RGBQUAD>;

using TBitMap = BitMap<pictureWidth, pictureHeight>;
using TBitMapPtr = std::unique_ptr<TBitMap>;

using TUpsampledBitMap = BitMap<upsampledPictureWidth, upsampledPictureHeight>;
using TUpsampledBitMapPtr = std::unique_ptr<TUpsampledBitMap>;

TUpsampledBitMapPtr g_upsampledBitmap = std::make_unique<TUpsampledBitMap>();

void main()
{
    using Geom = RayTracer::Geometry::Geometry;
    using Tracer = RayTracer::RayTracer;
    using namespace RayTracer;
    using namespace Geometry;

    auto back = new Plane(Vector3(0, 0, 40), Vector3(0, 0, -1));
    back->Diffuse() = Color(255, 255, 255);
    back->DiffuseCoeff() = 1;

    auto floor = new Plane(Vector3(0, 0, 0), Vector3(0, 1, 0));
    floor->Diffuse() = Color(0, 0, 255);
    floor->DiffuseCoeff() = 1;
    floor->Reflection() = 0.75;

    auto left = new Plane(Vector3(-30, 0, 0), Vector3(1, 0, 0));
    left->Diffuse() = Color(255, 255, 0);
    left->DiffuseCoeff() = 1;
    left->Reflection() = 0.2;

    auto right = new Plane(Vector3(30, 0, 0), Vector3(-1, 0, 0));
    right->Diffuse() = Color(0, 255, 0);
    right->DiffuseCoeff() = 1;
    right->Reflection() = 0;

    auto ball0 = new Sphere(Vector3(-10, 10, 10), 5);
    ball0->Diffuse() = Color(255, 165, 0);
    ball0->DiffuseCoeff() = 1;
    ball0->Reflection() = 0.75;

    auto ball1 = new Sphere(Vector3(10, 0, 10), 5);
    ball1->Diffuse() = Color(64, 64, 64);
    ball1->DiffuseCoeff() = 1;
    ball1->Reflection() = 0.25;

    auto ball2 = new Sphere(Vector3(5, 3, 1), 2.5);
    ball2->Diffuse() = Color(255, 0, 0);
    ball2->DiffuseCoeff() = 1;
    ball2->Reflection() = 0.75;

    auto ball3 = new Sphere(Vector3(-5, 3, 1), 2.5);
    ball3->Diffuse() = Color(153, 255, 255);
    ball3->DiffuseCoeff() = 1;
    ball3->Reflection() = 0;
    ball3->Refraction() = 1;
    ball3->RefractionCoeff() = refraction_helium;

    RaySource raySource(Color(255, 255, 255), Vector3(25, 20, 10));

    const Vector2 cameraPane = Vector2(1.6, 0.9);
    const Vector3 cameraPosition = Vector3(0, 10, -50);
    Scene scene(cameraPane, cameraPosition, refraction_air,
    { raySource },
    { floor, back, left, right,
        ball0, ball1, ball2, ball3 });


    const std::string fileName1("1.bmp");

    Tracer rayTracer;
    rayTracer.AddOnHitScreen(
        [](const Color& color, const Vector2& position) -> void
    {
        auto& pixel = (*g_upsampledBitmap)
            [static_cast<size_t>(position.y())]
            [static_cast<size_t>(position.x())];
        pixel.rgbRed = static_cast<BYTE>(255 * color.GetRed());
        pixel.rgbGreen = static_cast<BYTE>(255 * color.GetGreen());
        pixel.rgbBlue = static_cast<BYTE>(255 * color.GetBlue());
        pixel.rgbReserved = 0;
    });

    rayTracer.Run(scene, upsampledPictureWidth, upsampledPictureHeight, threadsCount);

    const size_t squaredUpsampling = upsampling * upsampling;
    TBitMapPtr downsampled = std::make_unique<TBitMap>();
    for(size_t _ux = 0; _ux < upsampledPictureWidth; _ux += upsampling)
    {
        for(size_t _uy = 0; _uy < upsampledPictureHeight; _uy += upsampling)
        {
            size_t summR = 0;
            size_t summG = 0;
            size_t summB = 0;

            for(size_t i = 0; i < upsampling; ++i)
            {
                for(size_t j = 0; j < upsampling; ++j)
                {
                    auto& upsampledPixel = (*g_upsampledBitmap)[_uy][_ux];
                    summR += upsampledPixel.rgbRed;
                    summG += upsampledPixel.rgbGreen;
                    summB += upsampledPixel.rgbBlue;
                }
            }

            auto& downsampledPixel = (*downsampled)[_uy / upsampling][_ux / upsampling];
            downsampledPixel.rgbRed = summR / squaredUpsampling;
            downsampledPixel.rgbGreen = summG / squaredUpsampling;
            downsampledPixel.rgbBlue = summB / squaredUpsampling;
        }
    }

    bmp::SaveToBMP(fileName1, *downsampled);
    system(fileName1.c_str());
}