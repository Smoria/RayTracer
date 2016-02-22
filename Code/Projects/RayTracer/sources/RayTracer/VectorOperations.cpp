#include "RayTracer/VectorOperations.h"

namespace RayTracer
{
    Vector3 Reflect(const Vector3& target, const Vector3& normal)
    {
        const Vector3 normalized = normal.normalized();
        Vector3 res = target - 2 * normalized * (target.dot(normalized));
        res.normalize();
        return res;
    }

    Vector3 Refract(const Vector3& target,
        Vector3 normal, const Type& n1,
        Type n2, Type& quitN)
    {
        normal.normalize();
        if (normal.dot(target) > 0)
        {
            normal -= normal;
            n2 = quitN;
        }
        quitN = n2;
        const Type r = n1 / n2;
        const Type cosi = -(normal.dot(target));

        const Type sini = std::min<Type>(1, r * r * (1 - cosi * cosi));
        Vector3 res = r * target + (r * cosi - std::sqrt(1 - sini)) * normal;
        res.normalize();

        return res;
    }
}