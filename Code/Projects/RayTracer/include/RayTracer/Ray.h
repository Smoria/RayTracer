#pragma once
#include "Definitions.h"
#include "Color.h"

namespace RayTracer
{
    class Scene;

    namespace Geometry
    {
        class Geometry;
    }

    class Ray
    {
        public:
            Ray(Scene& scene,
                const Vector3f& origin = Vector3f(0,0,0),
                const Vector3f& previousHit = Vector3f(0, 0, 0),
                const Vector3f& direction = Vector3f(0, 0, 0),
                const Float& envRefractionCoefficient = 0.0);

            Color Trace(int depth);
            const Vector3f& Origin() const { return m_origin; }
            const Vector3f& Direction() const { return m_direction; }
            bool GetHitPoint(Geometry::Geometry*& hitGeometry, Float& hitDistance, Vector3f& hitPoint);
        
        private:
            Vector3f m_origin;
            Vector3f m_previousHit;
            Vector3f m_direction;
            Float m_EnvRefractionCoefficient;
            Scene& m_scene;
    };
}