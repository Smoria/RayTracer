#pragma once

#include <vector>
#include "Plane.h"

namespace RayTracer
{
    namespace Geometry
    {
        class LimitedPlane :
            public Plane
        {
            public:
                using Base = Plane;

                LimitedPlane(const Vector3f& position,
                    const std::vector<Vector3f>& limitedPoints,
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

                LimitedPlane(const Vector3f& position,
                    const std::vector<Vector3f>& limitedPoints,
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

                Float Intersects(const Ray& ray) const override;

                bool Contains(const Vector3f& point) const;

            protected:
                std::vector<Vector3f> m_limitPoints;
        };
    }
}