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
            template<class T>
            using TCallbacks = std::set<T, FunctionsComparator<T>>;

            using HitScreenCallback = std::function<void(const Color&, const Vector2&)>;
            using HitScreenCallbacks = TCallbacks<HitScreenCallback>;

            using UpdateProgressCallback = std::function<void(int nextPixel, int maxPixels)>;
            using UpdateProgressCallbacks = TCallbacks<UpdateProgressCallback>;

            bool AddOnHitScreen(const HitScreenCallback& callback)
            {
                return m_hitScreenCallbacks.insert(callback).second;
            }

            bool AddOnUpdateProgress(const UpdateProgressCallback& callback)
            {
                return m_updateProgressCallbacks.insert(callback).second;
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

            void OnUpdateProgress(int nextPixel, int maxPixels)
            {
                for (auto& callback : m_updateProgressCallbacks)
                {
                    callback(nextPixel, maxPixels);
                }
            }

            HitScreenCallbacks m_hitScreenCallbacks;
            UpdateProgressCallbacks m_updateProgressCallbacks;
    };
}