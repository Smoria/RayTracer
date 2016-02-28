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

                LimitedPlane(const Vector3& position,
                    const std::vector<Vector3>& limitedPoints,
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

                LimitedPlane(const Vector3& position,
                    const std::vector<Vector3>& limitedPoints,
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

                Type Intersects(const Ray& ray) const override;

                bool Contains(const Vector3& point) const;

            protected:
                std::vector<Vector3> m_limitPoints;
        };
    }
}