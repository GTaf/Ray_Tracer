#include <cmath>
#include "phongReflectionModel.h"
using namespace std;
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
      double mirror=0.;
      if(R*V>0) mirror+=pow(R*V,m.getAlpha()); /************************à rediscuter !!!************************************/
      specularLighting+=m.getSpecular()*mirror*it->getSpecular();
    }
    c.setValue(c.getValue(i)+ambiantLighting+diffuseLighting+specularLighting,i);
  }
  cout<<"Computed color"<<endl<<c;
  return c;
}
