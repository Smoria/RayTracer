#pragma once

#include <vector>
#include "RaySource.h"
#include "RayTracer/Ray.h"

namespace RayTracer
{
    namespace Geometry
    {
        class Geometry;
    }

    class Scene
    {
        public:
            Scene(const Vector2f& cameraPane,
                const Vector3f& eyePosition,
                const Float& refractionCoeff,
                const std::vector<RaySource>& lights = std::vector<RaySource>(),
                const std::vector<Geometry::Geometry*> objects = std::vector<Geometry::Geometry*>());

            void SetMinDistance(const Float& value)
            {
                m_minDistance = value;
                m_minDistance2 = value / 2;
            }
            const Float& GetMinDistance() const { return m_minDistance; }
            const Float& GetMinDistance2() const { return m_minDistance2; }
            const std::vector<RaySource>& Lights() const { return m_lights; }
            std::vector<Geometry::Geometry*>& Objects() { return m_objects; }
            const std::vector<Geometry::Geometry*>& Objects() const { return m_objects; }
            const Vector2f& CameraPane() const { return m_cameraPane; }
            const Ray& Eye() const { return m_eye; }

            const Float& RefractionCoeff() const { return m_refractionCoeff; }

        private:
            Ray m_eye;
            Float m_minDistance;
            Float m_minDistance2;
            Float m_refractionCoeff;
            Vector2f m_cameraPane;
            std::vector<RaySource> m_lights;
            std::vector<Geometry::Geometry*> m_objects;
    };
}