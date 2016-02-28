#pragma once

#include "RayTracer/Geometry/Geometry.h"

namespace RayTracer
{
    namespace Geometry
    {
        class Sphere :
            public Geometry
        {
            public:
                Sphere(const Vector3& position,
                    const Type& radius,
                    const Color& diffuse = defaultDiffuse,
                    const Type& refractionCoeff = defaultRefractionCoeff,
                    const Type& refraction = defaultRefraction,
                    const Type& reflection = defaultReflection,
                    const Type& shininessCoeff = defaultShininessCoeff,
                    const Type& specularCoeff = defaultSpecularCoeff,
                    const Type& diffuseCoeff = defaultDiffuseCoeff,
                    const Bitmap& texture = emptyTexture,
                    const Bitmap& normalTexture = emptyTexture);

                Vector2 GetUV(const Vector3& point) const override;
                Vector3 GetNormalAtPoint(const Vector3& point) const override;
                double Intersects(const Ray& ray) const override;

            protected:
                Type m_radius;
        };
    }
}