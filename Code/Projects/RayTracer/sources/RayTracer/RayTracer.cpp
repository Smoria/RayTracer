#include "RayTracer/RayTracer.h"
#include "RayTracer/Scene.h"
#include <thread>
#include <mutex>
#include <iostream>

namespace RayTracer
{
    void RayTracer::RunSingleRay(Scene scene,
        int screenWidth, int screenHeight, int x, int y)
    {
        Ray ray = CreateMainRay(scene, x, y, screenWidth, screenHeight);
        Color color = ray.Trace(0);
        OnHitScreen(color, Vector2(x, y));
    }

    void RayTracer::Run(Scene& scene, int screenWidth, int screenHeight, size_t threadsCount)
    {
        const size_t rowsPerThread = screenHeight / threadsCount;

        std::mutex lock;
        std::set<std::thread*> threads;

        int current_pixel = 0;
        for (size_t threadNumber = 0; threadNumber < threadsCount; ++threadNumber)
        {
            threads.insert(new std::thread([&scene, this, &lock, &current_pixel,
                screenWidth, screenHeight, threadNumber, rowsPerThread]()
            {
                while (true)
                {
                    lock.lock();
                    const int target_pixel = current_pixel++;
                    lock.unlock();

                    if (target_pixel >= screenWidth * screenHeight)
                    {
                        break;
                    }

                    const int x = target_pixel % screenWidth;
                    const int y = target_pixel / screenWidth;
                    Ray ray = CreateMainRay(scene, x, y, screenWidth, screenHeight);
                    Color col = ray.Trace(0);
                    OnHitScreen(col, Vector2(x, y));
                }
            }));
        }

        for (auto& thread : threads)
        {
            thread->join();
            delete thread;
        }
    }

    void RayTracer::Run(Scene& scene, int screenWidth, int screenHeight, int sX, int sY, int eX, int eY)
    {
        if (m_hitScreenCallbacks.empty())
        {
            return;
        }

        for (int x = sX; x < eX; ++x)
        {
            for (int y = sY; y < eY; ++y)
            {
                Ray ray = CreateMainRay(scene, x, y, screenWidth, screenHeight);
                Color col = ray.Trace(0);
                OnHitScreen(col, Vector2(x, y));
            }
        }
    }

    Ray RayTracer::CreateMainRay(
        Scene& scene, int screenX, int screenY,
        int screenWidth, int screenHeight)
    {
        const Type focalLength = scene.CameraPane().x() / scene.CameraPane().y();
        const Type x = (-(screenWidth / 2) + screenX) * (scene.CameraPane().x() / screenWidth);
        const Type y = (-(screenHeight / 2) + screenY) * (scene.CameraPane().y() / screenHeight);
        const Type z = focalLength;
        const Vector3 dir = Vector3(x, y, z);
        const Vector3 rayPos = scene.Eye().Origin() + dir;

        return Ray(scene, rayPos, Vector3(),
            (rayPos - scene.Eye().Origin()).normalized(),
            scene.RefractionCoeff());
    }
}