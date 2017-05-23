#pragma once

#include <stdlib>
#include "basics.h"

Color phongColor(const& Camera cam,const& Scene scene, const& std::vector<Light> lights, const& Sphere sphere,const& Vector pos);
