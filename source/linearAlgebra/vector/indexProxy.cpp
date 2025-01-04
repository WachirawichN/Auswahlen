#include "vector.h"

namespace vector
{
    indexProxy::indexProxy(float& input)
        : ref(input)
    {
    }

    indexProxy& indexProxy::operator+=(float value)
    {
        ref += value;
        return *this;
    }
    indexProxy& indexProxy::operator-=(float value)
    {
        ref -= value;
        return *this;
    }
    indexProxy& indexProxy::operator*=(float value)
    {
        ref *= value;
        return *this;
    }
    indexProxy& indexProxy::operator/=(float value)
    {
        ref /= value;
        return *this;
    }

    indexProxy::operator float() const
    {
        return ref;
    }
}