#pragma once

#include "Definitions.h"

namespace RayTracer
{
    Vector3 Reflect(const Vector3& target, const Vector3& normal);

    Vector3 Refract(const Vector3& target,
        Vector3 normal, const Type& n1,
        Type n2, Type& quitN);
}