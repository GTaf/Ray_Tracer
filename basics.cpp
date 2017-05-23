#include <cmath>
#include "phongReflectionModel.h"

Color phongColor(const Camera& cam,const Scene& scene,const std::vector<Light>& lights, const Sphere& sphere,const Vector& pos){
  Color c;
  for(int i=0;i<2;i++){
    Material m=sphere.getMaterial();
    double ambiantLighting = m.getAmbient()*scene.getAmbiantLighting();
    double diffuseLighting=0.;
    double specularLighting=0.;
    for(std::vector<Light>::const_iterator it=lights.begin(); it!=lights.end(); ++it){
      Vector L=(pos-it->getPoint()).normalize();
      Vector N=sphere.getNormal(pos);
      Vector R=2*(L*N)*N-L;
      Vector V=(cam.getEye()-pos).normalize();
      diffuseLighting+=m.getDiffuse()*(L*N)*it->getDiffuse();
      specularLighting+=m.getSpecular()*pow(R*V,m.getAlpha())*it->getSpecular();
    }
    c.setValue(c.getValue(i)+ambiantLighting+diffuseLighting+specularLighting,i);
  }
  return c;
}
