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
                Sphere(const Vector3f& position,
                    const Float& radius,
                    const Color& diffuse = defaultDiffuse,
                    const Float& refractionCoeff = defaultRefractionCoeff,
                    const Float& refraction = defaultRefraction,
                    const Float& reflection = defaultReflection,
                    const Float& shininessCoeff = defaultShininessCoeff,
                    const Float& specularCoeff = defaultSpecularCoeff,
                    const Float& diffuseCoeff = defaultDiffuseCoeff,
                    const Bitmap& texture = emptyTexture,
                    const Bitmap& normalTexture = emptyTexture);

                Vector2f GetUV(const Vector3f& point) const override;
                Vector3f GetNormalAtPoint(const Vector3f& point) const override;
                double Intersects(const Ray& ray) const override;

            protected:
                Float m_radius;
        };
    }
}