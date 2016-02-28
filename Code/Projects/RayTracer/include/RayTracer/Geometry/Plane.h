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
                Plane(const Vector3& position,
                    const Vector3& normal,
                    const Color& diffuse = defaultDiffuse,
                    const Type& refractionCoeff = defaultRefractionCoeff,
                    const Type& refraction = defaultRefraction,
                    const Type& reflection = defaultReflection,
                    const Type& shininessCoeff = defaultShininessCoeff,
                    const Type& specularCoeff = defaultSpecularCoeff,
                    const Type& diffuseCoeff = defaultDiffuseCoeff,
                    const Bitmap& texture = emptyTexture,
                    const Bitmap& normalTexture = emptyTexture,
                    const Type& uvScale = defaultUVScale);

                Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3);

                Vector3 GetNormalAtPoint(const Vector3& point) const;

                Vector2 GetUV(const Vector3& point) const override;

                Type Intersects(const Ray& ray) const override;

            protected:
                Type m_UVScale;
                Vector3 m_normal;

            public:
                static const Type defaultUVScale;
        };
    }
}