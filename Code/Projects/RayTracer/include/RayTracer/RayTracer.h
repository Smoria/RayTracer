#pragma once

#include <set>
#include "Ray.h"
#include "Definitions.h"

namespace RayTracer
{
    class Scene;

    class RayTracer
    {
        public:
            using HitScreenCallback = void(*)(const Color&, const Vector2&);

            bool AddOnHitScreen(HitScreenCallback callback)
            {
                return m_hitScreenCallbacks.insert(callback).second;
            }

            void Run(Scene& scene, int screenWidth, int screenHeight, size_t threadsCount);
            void Run(Scene& scene, int screenWidth, int screenHeight, int sX, int sY, int eX, int eY);
            void RunSingleRay(Scene scene, int screenWidth, int screenHeight, int x, int y);
            Ray CreateMainRay(Scene& scene, int screenX, int screenY, int screenWidth, int screenHeight);

        private:
            void OnHitScreen(const Color& color, const Vector2& position)
            {
                for (auto& callback : m_hitScreenCallbacks)
                {
                    callback(color, position);
                }
            }

            std::set<HitScreenCallback> m_hitScreenCallbacks;
    };
}