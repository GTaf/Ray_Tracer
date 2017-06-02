#include <cmath>
#include "phongReflectionModel.h"
using namespace std;

bool ray_sphere_intersect2(Ray r, Sphere s,Vector& pos);

Color phongColor(const Camera& cam,const Scene& scene,const std::vector<Light>& lights, const Sphere& sphere,const Vector& pos){
  Color c;

  for(int i=0;i<3;i++){
    Material m=sphere.getMaterial();
    double ambiantLighting = m.getAmbient()*sphere.getColor().getValue(i)*scene.getAmbiantLighting();
    double diffuseLighting=0.;
    double specularLighting=0.;
    for(std::vector<Light>::const_iterator it=lights.begin(); it!=lights.end(); ++it){
      //On vérifie qu'un objet n'obstrue pas la lumière de la lampe
      bool shaded=false;
      for(int i=0;i<scene.size();++i){
        Ray r(it->getPoint()-pos,pos);
        Vector temp;
        if(ray_sphere_intersect2(r,scene.getSphere(i),temp)){
            shaded=true;
            break;
        }
      }
      if(!shaded){
          Vector L=(it->getPoint()-pos).normalize();
          Vector N=sphere.getNormal(pos);
          Vector R=2*(L*N)*N-L;
          Vector V=(cam.getEye()-pos).normalize();
          if(L*N>0)
            diffuseLighting+=m.getDiffuse()*sphere.getColor().getValue(i)*(L*N)*it->getDiffuse(i);
          double mirror=0.;
          if(R*V>0) mirror+=pow(R*V,m.getAlpha());
          specularLighting+=m.getSpecular()*sphere.getColor().getValue(i)*mirror*it->getSpecular(i);
        }
    }
    c.setValue(c.getValue(i)+ambiantLighting+diffuseLighting+specularLighting,i);
  }
  return c;
}
