#pragma once

#include "Definitions.h"

namespace RayTracer
{
    Vector3f Reflect(const Vector3f& target, const Vector3f& normal);

    Vector3f Refract(const Vector3f& target,
        Vector3f normal, const Float& n1,
        Float n2, Float& quitN);
}