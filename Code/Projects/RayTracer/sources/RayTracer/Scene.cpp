#include "RayTracer/Scene.h"
#include "RayTracer/Ray.h"
#include "RayTracer/Geometry/Geometry.h"

namespace RayTracer
{
    Scene::Scene(
        const Vector2f& cameraPane,
        const Vector3f& eyePosition,
        const Float& refractionCoeff,
        const std::vector<RaySource>& lights,
        const std::vector<Geometry::Geometry*> objects) :
            m_eye(*this, eyePosition, Vector3f(0,0,0), Vector3f(0,0,0), refractionCoeff),
            m_minDistance(),
            m_refractionCoeff(refractionCoeff),
            m_cameraPane(cameraPane),
            m_lights(lights),
            m_objects(objects)
    {
        SetMinDistance(0.00000001);
    }
}