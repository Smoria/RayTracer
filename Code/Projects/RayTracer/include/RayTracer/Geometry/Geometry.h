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
                Geometry(const Vector3& position,
                        const Color& diffuse = Color(0,0,0),
                        const Type& refractionCoeff = 1,
                        const Type& refraction = 0,
                        const Type& reflection = 0,
                        const Type& shininessCoeff = 5,
                        const Type& specularCoeff = 0.5,
                        const Type& diffuseCoeff = 1,
                        const Bitmap& texture = Bitmap(),
                        const Bitmap& normalTexture = Bitmap()
                    );

                virtual Type Intersects(const Ray& ray) const = 0;
                virtual Vector3 GetNormalAtPoint(const Vector3& point) const = 0;
                virtual Vector2 GetUV(const Vector3& point) const = 0;

                Vector3 GetNormalAtPointFromMap(const Vector3& point,
                    const Vector3& normal) const;

                Vector3 GetNormalAtPointFromMap(const Vector3& point,
                    const Vector3& normal, const Vector2& uv) const;

                static Vector3 GetIntersectPoint(const Ray& ray, Type length);

                Color Shade(const Ray& ray, const Vector3& hitPoint, Color& color,
                    const Vector3* normal, Color lightColor, Type lightIntensity);

                Type& Reflection() { return m_reflection; }

                Type& Refraction() { return m_refraction; }
                const Type& Refraction() const { return m_refraction; }

                Type& RefractionCoeff() { return m_refractionCoeff; }
                const Type& RefractionCoeff() const { return m_refractionCoeff; }

                Type& DiffuseCoeff() { return m_diffuseCoeff; }
                const Type& DiffuseCoeff() const { return m_diffuseCoeff; }

                Color& Diffuse() { return m_diffuse; }
                const Color& Diffuse() const { return m_diffuse; }

         protected:
                static Vector2 PrepareUV(double u, double v);

                Color m_diffuse;
                Type m_diffuseCoeff;
                Type m_specularCoeff;
                Type m_shininessCoeff;
                Type m_reflection;
                Type m_refraction;
                Type m_refractionCoeff;
                Vector3 m_position;
                Bitmap m_texture;
                Bitmap m_normalTexture;
        };
    }
}