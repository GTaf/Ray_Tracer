#pragma once

#include <cstdlib>
#include <iostream>
#include "basics.h"

/**
* Cette fonction calcule la couleur perçue par la caméra d'un point donné sur une sphère.
*   @param cam    la Camera
*   @param scene  la scene
*   @param lights l'ensemble des lumières de la scène
*   @param sphere la sphere sur laquelle se trouve le point considéré
*   @param pos    la position du point
*   @return       un objet de classe Color représentant la couleur perçue
*/
Color phongColor(const Camera& cam,const Scene& scene,const std::vector<Light>& lights, const Sphere& sphere,const Vector& pos);
