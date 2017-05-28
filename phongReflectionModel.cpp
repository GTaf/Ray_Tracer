#include <cmath>
#include "phongReflectionModel.h"
using namespace std;
Color phongColor(const Camera& cam,const Scene& scene,const std::vector<Light>& lights, const Sphere& sphere,const Vector& pos){
  Color c;
  //cout<<"sphere "<<sphere.getColor()<<endl;
  for(int i=0;i<3;i++){
    Material m=sphere.getMaterial();
    double ambiantLighting = m.getAmbient()*sphere.getColor().getValue(i)*scene.getAmbiantLighting();
    double diffuseLighting=0.;
    double specularLighting=0.;
    for(std::vector<Light>::const_iterator it=lights.begin(); it!=lights.end(); ++it){
      Vector L=(it->getPoint()-pos).normalize();
      Vector N=sphere.getNormal(pos);
      Vector R=2*(L*N)*N-L;
      Vector V=(cam.getEye()-pos).normalize();
      if(L*N>0)          //Pourquoi ? cette situation ne devrait JAMAIS arriver !!!
      diffuseLighting+=m.getDiffuse()*sphere.getColor().getValue(i)*(L*N)*it->getDiffuse(i);
      double mirror=0.;
      if(R*V>0) mirror+=pow(R*V,m.getAlpha()); /************************à rediscuter !!!************************************/
      specularLighting+=m.getSpecular()*sphere.getColor().getValue(i)*mirror*it->getSpecular(i);
      //cout<<"test"<<endl;
      //cout<<"tttttttttttttttttttttttt  "<<it->getSpecular(i)<<"  "<<it->getDiffuse(i)<<" i "<<i<<endl;
      //cout<<diffuseLighting<<" diffuse "<<L*N<<endl;
    }
    //cout<<"i "<<i<<" v "<<c.getValue(i)+ambiantLighting+diffuseLighting+specularLighting<<endl;
    c.setValue(c.getValue(i)+ambiantLighting+diffuseLighting+specularLighting,i);
  }
  //cout<<"Computed color"<<endl<<c<<endl;
  //cout<<"dist : "<<(pos-sphere.getCenter()).norm()<<endl;
  return c;
}
