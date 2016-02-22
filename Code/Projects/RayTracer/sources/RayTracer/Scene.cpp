#include "RayTracer/Scene.h"
#include "RayTracer/Ray.h"
#include "RayTracer/Geometry/Geometry.h"

namespace RayTracer
{
    Scene::Scene(
        const Vector2& cameraPane,
        const Vector3& eyePosition,
        const Type& refractionCoeff,
        const std::vector<RaySource>& lights,
        const std::vector<Geometry::Geometry*> objects) :
            m_eye(*this, eyePosition, Vector3(0,0,0), Vector3(0,0,0), refractionCoeff),
            m_minDistance(),
            m_refractionCoeff(refractionCoeff),
            m_cameraPane(cameraPane),
            m_lights(lights),
            m_objects(objects)
    {
        SetMinDistance(0.00000001);
    }
}