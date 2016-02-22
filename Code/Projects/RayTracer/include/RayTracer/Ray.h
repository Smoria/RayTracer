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
                const Vector3& origin = Vector3(0,0,0),
                const Vector3& previousHit = Vector3(0, 0, 0),
                const Vector3& direction = Vector3(0, 0, 0),
                const Type& envRefractionCoefficient = 0.0);

            Color Trace(int depth);
            const Vector3& Origin() const { return m_origin; }
            const Vector3& Direction() const { return m_direction; }
            bool GetHitPoint(Geometry::Geometry*& hitGeometry, Type& hitDistance, Vector3& hitPoint);
        
        private:
            Vector3 m_origin;
            Vector3 m_previousHit;
            Vector3 m_direction;
            Type m_EnvRefractionCoefficient;
            Scene& m_scene;
    };
}