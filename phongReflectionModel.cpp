#include <cmath>
#include "phongReflectionModel.h"
using namespace std;

bool ray_sphere_intersect2(Ray r, Sphere s,Vector& pos);/*{//le rayon part du point de vision
  Vector m=s.getCenter()-r.getPoint();
    Vector a=abs((m*r.getVector().normalize()))*r.getVector().normalize();
    Vector b=m-a;
    if(b.norm()-s.getRadius()>0)return false;
    double d=sqrt(pow(s.getRadius(),2)-pow(b.norm(),2));
    pos=(a.norm()-d)*r.getVector().normalize()+r.getPoint();
    return (a*m>0 && a.norm()>d); //On détecte une intersection seulement si la sphère n'est pas derrière le pt de départ du rayon(a*m>0)
                                  //et que le point de départ du rayon n'est pas dans la sphère (a.norm()>d)
}*/

Color phongColor(const Camera& cam,const Scene& scene,const std::vector<Light>& lights, const Sphere& sphere,const Vector& pos){
  Color c;
  //cout<<"sphere "<<sphere.getColor()<<endl;
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
          if(R*V>0) mirror+=pow(R*V,m.getAlpha()); /************************à rediscuter !!!************************************/
          specularLighting+=m.getSpecular()*sphere.getColor().getValue(i)*mirror*it->getSpecular(i);
        }
    }
    c.setValue(c.getValue(i)+ambiantLighting+diffuseLighting+specularLighting,i);
  }
  return c;
}
