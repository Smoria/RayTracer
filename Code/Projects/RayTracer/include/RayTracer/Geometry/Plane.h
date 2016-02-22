#pragma once

#include "Geometry.h"

namespace RayTracer
{
    namespace Geometry
    {
        class Plane
            : public Geometry
        {
            public:
                Plane(const Vector3& position, const Vector3& normal, const Type& UVScale = 30);

                Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);

                Vector3 GetNormalAtPoint(const Vector3& point) const;

                Vector2 GetUV(const Vector3& point) const override;

                Type Intersects(const Ray& ray) const override;

            protected:
                Type m_UVScale;
                Vector3 m_normal;
        };
    }
}