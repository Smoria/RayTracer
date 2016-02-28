#pragma once

#include "Bitmap.h"
#include "RayTracer/Definitions.h"
#include "RayTracer/Color.h"

namespace RayTracer
{
    class Ray;

    namespace Geometry
    {
        class Geometry
        {
            public:
                Geometry(const Vector3f& position,
                        const Color& diffuse = defaultDiffuse,
                        const Float& refractionCoeff = defaultRefractionCoeff,
                        const Float& refraction = defaultRefraction,
                        const Float& reflection = defaultReflection,
                        const Float& shininessCoeff = defaultShininessCoeff,
                        const Float& specularCoeff = defaultSpecularCoeff,
                        const Float& diffuseCoeff = defaultDiffuseCoeff,
                        const Bitmap& texture = emptyTexture,
                        const Bitmap& normalTexture = emptyTexture
                    );

                virtual Float Intersects(const Ray& ray) const = 0;
                virtual Vector3f GetNormalAtPoint(const Vector3f& point) const = 0;
                virtual Vector2f GetUV(const Vector3f& point) const = 0;

                Vector3f GetNormalAtPointFromMap(const Vector3f& point,
                    const Vector3f& normal) const;

                Vector3f GetNormalAtPointFromMap(const Vector3f& point,
                    const Vector3f& normal, const Vector2f& uv) const;

                static Vector3f GetIntersectPoint(const Ray& ray, Float length);

                Color Shade(const Ray& ray, const Vector3f& hitPoint, Color& color,
                    const Vector3f* normal, Color lightColor, Float lightIntensity);

                Float& Reflection() { return m_reflection; }

                Float& Refraction() { return m_refraction; }
                const Float& Refraction() const { return m_refraction; }

                Float& RefractionCoeff() { return m_refractionCoeff; }
                const Float& RefractionCoeff() const { return m_refractionCoeff; }

                Float& DiffuseCoeff() { return m_diffuseCoeff; }
                const Float& DiffuseCoeff() const { return m_diffuseCoeff; }

                Color& Diffuse() { return m_diffuse; }
                const Color& Diffuse() const { return m_diffuse; }

            protected:
                static Vector2f PrepareUV(double u, double v);

                Color m_diffuse;
                Float m_diffuseCoeff;
                Float m_specularCoeff;
                Float m_shininessCoeff;
                Float m_reflection;
                Float m_refraction;
                Float m_refractionCoeff;
                Vector3f m_position;
                Bitmap m_texture;
                Bitmap m_normalTexture;

            public:
                static const Vector3f defaultDiffuse;
                static const Float defaultRefractionCoeff;
                static const Float defaultRefraction;
                static const Float defaultReflection;
                static const Float defaultShininessCoeff;
                static const Float defaultSpecularCoeff;
                static const Float defaultDiffuseCoeff;
                static const Bitmap emptyTexture;
        };
    }
}