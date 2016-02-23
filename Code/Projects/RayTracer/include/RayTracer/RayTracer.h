#pragma once

#include <set>
#include <functional>

#include "Ray.h"
#include "Definitions.h"

template <class Function>
struct FunctionsComparator
{
    bool operator()(const Function& f1, const Function& f2) const
    {
        auto ptr1 = f1.target<Function>();
        auto ptr2 = f2.target<Function>();
        return ptr1 < ptr2;
    }
};

namespace RayTracer
{
    class Scene;

    class RayTracer
    {
        public:
            using HitScreenCallback = std::function<void(const Color&, const Vector2&)>;
            using HitScreenCallbacks = std::set<HitScreenCallback,
                FunctionsComparator<HitScreenCallback>>;

            bool AddOnHitScreen(const HitScreenCallback& callback)
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

            HitScreenCallbacks m_hitScreenCallbacks;
    };
}