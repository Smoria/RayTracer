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
        std::mutex lock;
        std::set<std::thread*> threads;

        int next_pixel = 0;
        const int queue_size = 5;
        const int totalPixelsCount = screenWidth * screenHeight;
        for (size_t threadNumber = 0; threadNumber < threadsCount; ++threadNumber)
        {
            threads.insert(new std::thread([&scene, this, &lock, &next_pixel,
                screenWidth, totalPixelsCount, screenHeight, queue_size]()
            {
                while (true)
                {
                    lock.lock();
                    int current_pixel = next_pixel;
                    const int end_pixel = next_pixel += queue_size;
                    lock.unlock();

                    bool mustBreak = false;
                    for (; current_pixel < end_pixel; current_pixel += 1)
                    {
                        if (current_pixel >= totalPixelsCount)
                        {
                            mustBreak = true;
                            break;
                        }

                        const int x = current_pixel % screenWidth;
                        const int y = current_pixel / screenWidth;
                        Ray ray = CreateMainRay(scene, x, y, screenWidth, screenHeight);
                        Color col = ray.Trace(0);
                        OnHitScreen(col, Vector2(x, y));
                    }

                    if (mustBreak)
                    {
                        break;
                    }
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