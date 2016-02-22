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
            Scene(const Vector2& cameraPane,
                const Vector3& eyePosition,
                const Type& refractionCoeff,
                const std::vector<RaySource>& lights = std::vector<RaySource>(),
                const std::vector<Geometry::Geometry*> objects = std::vector<Geometry::Geometry*>());

            void SetMinDistance(const Type& value)
            {
                m_minDistance = value;
                m_minDistance2 = value / 2;
            }
            const Type& GetMinDistance() const { return m_minDistance; }
            const Type& GetMinDistance2() const { return m_minDistance2; }
            const std::vector<RaySource>& Lights() const { return m_lights; }
            std::vector<Geometry::Geometry*>& Objects() { return m_objects; }
            const std::vector<Geometry::Geometry*>& Objects() const { return m_objects; }
            const Vector2& CameraPane() const { return m_cameraPane; }
            const Ray& Eye() const { return m_eye; }

            const Type& RefractionCoeff() const { return m_refractionCoeff; }

        private:
            Ray m_eye;
            Type m_minDistance;
            Type m_minDistance2;
            Type m_refractionCoeff;
            Vector2 m_cameraPane;
            std::vector<RaySource> m_lights;
            std::vector<Geometry::Geometry*> m_objects;
    };
}