#include <climits>
#include <iostream>
#include "RayTracer\Ray.h"
#include "RayTracer\Scene.h"
#include "RayTracer\Geometry\Geometry.h"
#include "RayTracer\VectorOperations.h"

namespace RayTracer
{
    Ray::Ray(Scene&scene,
        const Vector3f& origin,
        const Vector3f& previousHit,
        const Vector3f& direction,
        const Float& envRefractionCoefficient) :
            m_origin(origin),
            m_previousHit(previousHit),
            m_direction(direction),
            m_EnvRefractionCoefficient(envRefractionCoefficient),
            m_scene(scene)
    {}

    Color Ray::Trace(int depth)
    {
        Color color(0,0,0);

        Geometry::Geometry* hitGeometry = nullptr;
        Float hitDistance;
        Vector3f hit;
        if (!GetHitPoint(hitGeometry, hitDistance, hit))
        {
            return color;
        }

        Vector3f normal = hitGeometry->GetNormalAtPoint(hit);

        auto& sceneLights = m_scene.Lights();
        for (int i = 0; i < sceneLights.size(); i++)
        {
            auto& sceneLight = sceneLights[i];
            Float hitShadowDistance;
            Vector3f dir = sceneLight.Position() - hit;
            dir.normalize();
            Ray lightRay(m_scene,
                hit + dir * m_scene.GetMinDistance2(),
                Vector3f(), dir,
                m_EnvRefractionCoefficient);;


            const Float distanceToLight = (lightRay.Origin() - sceneLight.Position()).norm();

            Vector3f shadowHit;
            Geometry::Geometry* hitShadowGeometry;
            if (!lightRay.GetHitPoint(hitShadowGeometry, hitShadowDistance, shadowHit)
                || hitShadowDistance >= distanceToLight)
            {
                color = hitGeometry->Shade(lightRay, hit, color, &normal, sceneLight.GetColor(), sceneLight.Intensity());
            }
        }

        if (depth > 1000)
        {
            return color;
        }

        if (hitGeometry->Reflection() > 0)
        {
            Vector3f dir = Reflect(m_direction, normal);
            Ray reflectionRay = Ray(m_scene,
                hit + dir * m_scene.GetMinDistance2(),
                Vector3f(0,0,0), dir,
                m_EnvRefractionCoefficient);

            color += reflectionRay.Trace(depth + 1) * hitGeometry->Reflection();
        }

        if (hitGeometry->Refraction() > 0)
        {
            Float quitCoeff = m_scene.RefractionCoeff();

            const Vector3f dir = Refract(m_direction, normal, m_EnvRefractionCoefficient,
                hitGeometry->RefractionCoeff(), quitCoeff);

            Ray refractionRay = Ray(m_scene,
                hit + dir * m_scene.GetMinDistance2(),
                Vector3f(0,0,0), dir, quitCoeff);

            color += refractionRay.Trace(depth + 1) * hitGeometry->Refraction();
        }

        return color;
    }
    bool Ray::GetHitPoint(Geometry::Geometry*& hitGeometry, Float& hitDistance, Vector3f& hitPoint)
    {
        Float minDist = std::numeric_limits<Float>::max();
        hitDistance = std::numeric_limits<Float>::max();
        hitGeometry = nullptr;

        bool valueFound = false;
        for(auto& geom : m_scene.Objects())
        {
            const Float dist = geom->Intersects(*this);
            if (dist > m_scene.GetMinDistance())
            {
                if (dist < minDist)
                {
                    hitGeometry = geom;
                    minDist = dist;
                    hitDistance = dist;
                    valueFound = true;
                }
            }
        }

        if (valueFound)
        {
            hitPoint = Geometry::Geometry::GetIntersectPoint(*this, minDist);
        }

        return valueFound;
    }
}