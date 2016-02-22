#include <climits>
#include <iostream>
#include "RayTracer\Ray.h"
#include "RayTracer\Scene.h"
#include "RayTracer\Geometry\Geometry.h"
#include "RayTracer\VectorOperations.h"

namespace RayTracer
{
    Ray::Ray(Scene&scene,
        const Vector3& origin,
        const Vector3& previousHit,
        const Vector3& direction,
        const Type& envRefractionCoefficient) :
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
        Type hitDistance;
        Vector3 hit;
        if (!GetHitPoint(hitGeometry, hitDistance, hit))
        {
            return color;
        }

        Vector3 normal = hitGeometry->GetNormalAtPoint(hit);

        auto& sceneLights = m_scene.Lights();
        for (int i = 0; i < sceneLights.size(); i++)
        {
            auto& sceneLight = sceneLights[i];
            Type hitShadowDistance;
            Vector3 dir = sceneLight.Position() - hit;
            dir.normalize();
            Ray lightRay(m_scene,
                hit + dir * m_scene.GetMinDistance2(),
                Vector3(), dir,
                m_EnvRefractionCoefficient);;


            const Type distanceToLight = (lightRay.Origin() - sceneLight.Position()).norm();

            Vector3 shadowHit;
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
            Vector3 dir = Reflect(m_direction, normal);
            Ray reflectionRay = Ray(m_scene,
                hit + dir * m_scene.GetMinDistance2(),
                Vector3(0,0,0), dir,
                m_EnvRefractionCoefficient);

            color += reflectionRay.Trace(depth + 1) * hitGeometry->Reflection();
        }

        if (hitGeometry->Refraction() > 0)
        {
            Type quitCoeff = m_scene.RefractionCoeff();

            const Vector3 dir = Refract(m_direction, normal, m_EnvRefractionCoefficient,
                hitGeometry->RefractionCoeff(), quitCoeff);

            Ray refractionRay = Ray(m_scene,
                hit + dir * m_scene.GetMinDistance2(),
                Vector3(0,0,0), dir, quitCoeff);

            color += refractionRay.Trace(depth + 1) * hitGeometry->Refraction();
        }

        return color;
    }
    bool Ray::GetHitPoint(Geometry::Geometry*& hitGeometry, Type& hitDistance, Vector3& hitPoint)
    {
        Type minDist = std::numeric_limits<Type>::max();
        hitDistance = std::numeric_limits<Type>::max();
        hitGeometry = nullptr;

        bool valueFound = false;
        for(auto& geom : m_scene.Objects())
        {
            const Type dist = geom->Intersects(*this);
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