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
                Plane(const Vector3f& position,
                    const Vector3f& normal,
                    const Color& diffuse = defaultDiffuse,
                    const Float& refractionCoeff = defaultRefractionCoeff,
                    const Float& refraction = defaultRefraction,
                    const Float& reflection = defaultReflection,
                    const Float& shininessCoeff = defaultShininessCoeff,
                    const Float& specularCoeff = defaultSpecularCoeff,
                    const Float& diffuseCoeff = defaultDiffuseCoeff,
                    const Bitmap& texture = emptyTexture,
                    const Bitmap& normalTexture = emptyTexture,
                    const Float& uvScale = defaultUVScale);

                Plane(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3);

                Vector3f GetNormalAtPoint(const Vector3f& point) const;

                Vector2f GetUV(const Vector3f& point) const override;

                Float Intersects(const Ray& ray) const override;

            protected:
                Float m_UVScale;
                Vector3f m_normal;

            public:
                static const Float defaultUVScale;
        };
    }
}