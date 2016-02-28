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
        OnHitScreen(color, Vector2f(x, y));
    }

    void RayTracer::Run(Scene& scene, int screenWidth, int screenHeight, size_t threadsCount)
    {
        std::mutex lock;
        std::set<std::thread*> threads;

        //Threads will use this number as index of pixel, which they must render
        int next_pixel = 0;

        //Threads will render series of 'queue_size' pixel before updating 'next_pixel'
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
                        OnHitScreen(col, Vector2f(x, y));
                    }

                    if (mustBreak)
                    {
                        break;
                    }
                }
            }));
        }

        while (true)
        {
            lock.lock();
            const int next_pixel_val = next_pixel;
            lock.unlock();

            if (next_pixel_val >= totalPixelsCount)
            {
                break;
            }

            OnUpdateProgress(next_pixel_val, totalPixelsCount);

            using namespace std::literals;
            std::this_thread::sleep_for(500ms);
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
                OnHitScreen(col, Vector2f(x, y));
            }
        }
    }

    Ray RayTracer::CreateMainRay(
        Scene& scene, int screenX, int screenY,
        int screenWidth, int screenHeight)
    {
        const Float focalLength = scene.CameraPane().x() / scene.CameraPane().y();
        const Float x = (-(screenWidth / 2) + screenX) * (scene.CameraPane().x() / screenWidth);
        const Float y = (-(screenHeight / 2) + screenY) * (scene.CameraPane().y() / screenHeight);
        const Float z = focalLength;
        const Vector3f dir = Vector3f(x, y, z);
        const Vector3f rayPos = scene.Eye().Origin() + dir;

        return Ray(scene, rayPos, Vector3f(),
            (rayPos - scene.Eye().Origin()).normalized(),
            scene.RefractionCoeff());
    }
}