#include <cstdlib>
#include "basics.h"
#include <iostream>
//
#include "phongReflectionModel.h"
#include "mpi.h"
#include <stdexcept>
#include <map>
#include "CImg.hpp"
#define IS 1000
using namespace std;
using namespace cimg_library;

bool ray_sphere_intersect2(Ray r, Sphere s, Vector& v);
void tracerRecursive(Camera ca, Scene s);
void tracerRecursive(CImg<float> img,World& world,int y,double *answer);
//Color computeColor(World world,Camera ca, Scene s, Ray origin, int index,int nb);
Color computeColor(Camera ca, Scene s, Ray origin, int index, int nb);
bool intersecti(Ray r, Sphere s, Vector& pos);
//void createWorld(){}
void computeImage(CImg<float>& img,World& world);
Color mix(Color a, Color b, double r);
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int myrank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    World world;
    world.createWorld();
    CImg<float> img(world.getCamera().getH(),world.getCamera().getW(),1,3); //create image
    computeImage(img,world);
    //tracerRecursive(ca,sc);
    //Affichage de l'image
    if(myrank==0){
    CImgDisplay main_disp(img,"Click a point");
    img.save_png("test.png");
    img.save_jpeg("test.jpeg");
    img.save_bmp("test.bmp");
    while (!main_disp.is_closed()){};
  }
    MPI_Finalize();
    return 0;
}
Color mix(Color a, Color b, double r){
    Color c;
    for(int i = 0; i < 3;i++){
        c.setValue(a.getValue(i)*r+b.getValue(i)*(1-r),i);
    }

    return c;
}

void computeImage(CImg<float>& img,World& world){
  //Initialisation des variables MPI
  int myrank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;
  //Etendue de la surface à calculer
  int yRange=world.getCamera().getH();
  int xRange=world.getCamera().getW();
  //Code du maitre, chargé de distribuer le calcul aux esclaves
  if (myrank == 0) {
    //Création d'un tableau pour sauvegarder l'image.
    std::cout<<"Nb de tâches : "<<yRange<<std::endl;
    double **tempImg=new double*[yRange];
    double garbage[3*xRange];
    for(int y=0;y<yRange;++y) *(tempImg+y)=new double[3*xRange];
    int answers[yRange];
    int flag=0;
    int index=0;
    MPI_Request requests[size-1];
    bool working[size-1];
    for(int i=0;i<size-1;++i){
        requests[i]=MPI_REQUEST_NULL;
        working[i]=false;
    }
    for(int i=0;i<yRange;++i) answers[i]=0;
     MPI_Request std;
     int count=0;
     int received=0;
     while(received<yRange){
         for(int i=0;i<size-1;++i){
              MPI_Test(requests+i,&flag,&status);
              if(flag){
                    std::cout<<"Tâches terminées : "<<received<<std::endl;
                    if(working[i]) ++received;
              if(count<yRange){
                   working[i]=true;
                   *(requests+i)=MPI_REQUEST_NULL;
                   MPI_Isend(&count,1, MPI_INT,i+1,0, MPI_COMM_WORLD,&std);
                   MPI_Irecv(tempImg[count],3*xRange, MPI_DOUBLE,i+1,0, MPI_COMM_WORLD,requests+i);
                   ++count;
              }
              }
         }
     }
     std::cout<<"----------------------------------FIN DU TRAITEMENT---------------------------"<<std::endl;
     //On signale aux esclaves qu'ils peuvent arrêter leur tâche. Le received n'a ici aucun role.
     count=-1;
     for(int i=0;i<size-1;++i){
        std::cout<<"sending "<<i+1<<std::endl;
       MPI_Send(&count,1, MPI_INT,i+1,0, MPI_COMM_WORLD);
       std::cout<<"wait for "<<i+1<<std::endl;
       MPI_Recv(garbage,3*xRange, MPI_DOUBLE,i+1,0, MPI_COMM_WORLD,0);
      std::cout<<"ok "<<i+1<<std::endl;
     }
     std::cout<<"copy"<<std::endl;
     for(int y=0;y<yRange;++y){
       for(int x=0;x<xRange;++x){
         for(int i=0;i<3;++i) img(x,y,i)=tempImg[y][3*x+i];
       }
     }
     std::cout<<"end"<<std::endl;
   }
//Code des esclaves
else {
  int y=0;
  MPI_Request std;
  double answer[3*xRange];
  while(y!=-1){
    MPI_Recv(&y,1,MPI_INT,0,0,MPI_COMM_WORLD,0);
    if(y!=-1){
    std::cout<<myrank<<"still alive. Next y: "<<y<<std::endl;
    tracerRecursive(img,world,y,answer);
    std::cout<<"Proc "<<myrank<<"  a traite ligne "<<y<<std::endl;
  }
    MPI_Isend(answer,3*xRange, MPI_DOUBLE,0,0, MPI_COMM_WORLD,&std);
  }
  }
}

/*Dans cette version, si on est dans une boule elle est invisible (Pas d'intersection détectée)
* Le problème des intersections avec des boules derrière le rayon est réglé.
*/
bool ray_sphere_intersect2(Ray r, Sphere s,Vector& pos){//le rayon part du point de vision
  Vector m=s.getCenter()-r.getPoint();
    Vector a=abs((m*r.getVector().normalize()))*r.getVector().normalize();
    Vector b=m-a;
    if(b.norm()-s.getRadius()>0)return false;
    double d=sqrt(pow(s.getRadius(),2)-pow(b.norm(),2));
    pos=(a.norm()-d)*r.getVector().normalize()+r.getPoint();
    return (a*m>0 && a.norm()>d); //On détecte une intersection seulement si la sphère n'est pas derrière le pt de départ du rayon(a*m>0)
                                  //et que le point de départ du rayon n'est pas dans la sphère (a.norm()>d)
}

/*Color computeColor(World world,Camera ca, Scene s, Ray origin, int index, int nb){//calcul la couleur du point vu "depuis" le vecteur origin

    double dist = -1;//distance au point intersecté
    int j;
    Vector pos, res;//vecteur à intersecter
/*
    std::vector<Light> lights;
    lights.push_back(Light(Vector(0,100,200), Color(1.,1.,1.),Color(0.,0.,0.)));
    lights.push_back(Light(Vector(0,-100,-200), Color(1.,1.,1.),Color(1.,1.,1.)));*/
    //recherc he une intersection
    /*for(int i = 0; i < s.size(); i++){//pour chaque sphere
        if(ray_sphere_intersect2(origin,s.getSphere(i),pos) && i != index){//si intersection, pas sur la même sphère
            if(dist == -1 || dist > (pos-origin.getVector()).norm()){//si intersection plus près
                res = pos;//on change l'intersection
                dist = (pos-origin.getVector()).norm();
                j = i;
            }
        }
    }
    if(dist == -1 || nb == 0){ //pas d'intersection' ou plus de reflexion
        return  phongColor(ca, s,world.getLights(), s.getSphere(index), origin.getPoint());
    }
    else{//intersection donc calcul recursif
        Vector rf = origin.getVector().normalize();
        Vector n = s.getSphere(j).getNormal(res).normalize();
        rf = rf - 2 * (rf*n)*n;

        double r = s.getSphere(index).getR();
        //return (phongColor(ca, s, lights, s.getSphere(index), origin.getPoint()).multiply(1));//.add(computeColor(ca,s,Ray(rf ,res),j, nb--).multiply(1-r));
        return mix(phongColor(ca, s, world.getLights(), s.getSphere(index), origin.getPoint()), computeColor(world,ca,s,Ray(rf ,res),j, nb--), r);
    }
}*/

/*void tracerRecursive(Camera ca, Scene s)*/
/*void tracerRecursive(CImg<float> img,World& world,int y,double *answer){
    //pour tous les spheres
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    Camera  ca=world.getCamera();
    Scene  scene=world.getScene();
    Vector  eye = ca.getEye();
    Vector  target = ca.getTarget();
    //Etendue de la surface à calculer
    int yRange=world.getCamera().getH();
    int xRange=world.getCamera().getW();
    double fov = 50, aspectratio = ca.getW() / double(ca.getH());
    double angle = tan(M_PI * 0.5 * fov / 180.);
    std::cout<<" Init--------------- "<<myrank<<" y "<<y<<std::endl;
    ////////////////////////////////////////////
    //Calcul et remplissage du tableau.

      for(int x=0;x<xRange;++x){
        double yp = (2 * ((x + 0.5) * 1/xRange) - 1) * angle * aspectratio;
        double zp = (1 - 2 * ((y + 0.5) * 1/yRange)) * angle;
        //Ray r = ca.Rayf(x*0.8,y*0.8);//rayon qui va de l'oeil au point (x.y)
        Ray r = Ray(Vector(1,yp,zp).normalize(), Vector(0,0,0));
        bool intersect = false;

        double dist = -1;//distance au point intersecté
        int j = -1; //numero de la sphere finale retenue
        Vector pos, res;//vecteur à intersecter

        for(int i = 0; i < scene.size(); i++){//pour chaque sphere
            if(ray_sphere_intersect2(r,scene.getSphere(i),pos)){//si intersection
                intersect = true;
                if(dist == -1 || dist > (pos-r.getPoint()).norm()){//si intersection plus près
                    res = pos;//on change l'intersection
                    dist = (pos-r.getPoint()).norm();
                    j = i;
                }
            }
        }
        if(intersect){//si intersection
            Color c = computeColor(world,ca, scene, Ray(r.getVector(), res), j,2);
            for(int i = 0; i <3; i++) answer[3*x+i]=c.getValue(i);
        }
          else{//sinon couleur de fond noire
            for(int i = 0; i < 3; i++) answer[3*x+i]=0;
          }
        }
          std::cout<<" End--------------- "<<myrank<<std::endl;
}*/
///////////////////////////////////////////////////
//////////////////////////////////////////////////
/////////////////////////////////////////////////
void tracerRecursive(CImg<float> img,World& world,int y,double *answer){

    double fov = 50, aspectratio = world.getCamera().getW() / double(world.getCamera().getH());
    double angle = tan(M_PI * 0.5 * fov / 180.);

    //code for Cimg found at
    //https://stackoverflow.com/questions/14914709/
    for(int x=0;x<world.getCamera().getW();++x) {  //for each pixel de la ligne
        double yp = (2 * ((x + 0.5) * 1/world.getCamera().getW()) - 1) * angle * aspectratio;
        double zp = (1 - 2 * ((y + 0.5) * 1/world.getCamera().getH())) * angle;
        //Ray r = ca.Rayf(x*0.8,y*0.8);//rayon qui va de l'oeil au point (x.y)
        Ray r = Ray(Vector(1,yp,zp).normalize(), Vector(0,0,0));
        bool intersect = false;

        double dist = -1;//distance au point intersecté
        int j = -1; //numero de la sphere finale retenue
        Vector pos, res;//vecteur à intersecter

        for(int i = 0; i < world.getScene().size(); i++){//pour chaque sphere
            if(ray_sphere_intersect2(r,world.getScene().getSphere(i),pos)){//si intersection
                intersect = true;
                if(dist == -1 || dist > (pos-r.getPoint()).norm()){//si intersection plus près
                    res = pos;//on change l'intersection
                    dist = (pos-r.getPoint()).norm();
                    j = i;
                }
            }
        }

        if(intersect){//si intersection
            Vector rf = r.getVector().normalize();
            Vector n = world.getScene().getSphere(j).getNormal(res).normalize();
            rf = rf - 2 * (rf*n)*n;
            Color c = computeColor(world.getCamera(), world.getScene(), Ray(rf ,res), j, 2);
            for(int i = 0; i <3; i++){
                answer[3*x+i] = c.getValue(i);
            }
        }

        else{//sinon couleur de fond noire
            for(int i = 0; i < 3; i++){
                answer[3*x+i] = 0;
            }
        }
        //img(x,y,c) = pixel_value_at(x,y,c);
    }
}

Color computeColor(Camera ca, Scene s, Ray origin, int index, int nb){//calcul la couleur du point vu "depuis" le vecteur origin

    double dist = -1;//distance au point intersecté
    int j;
    Vector pos, res;//vecteur à intersecter

    std::vector<Light> lights;
    lights.push_back(Light(Vector(0,100,200), Color(1.,1.,1.),Color(0.,0.,0.)));
    lights.push_back(Light(Vector(0,-100,-200), Color(1.,1.,1.),Color(1.,1.,1.)));
    //recherc he une intersection
    for(int i = 0; i < s.size(); i++){//pour chaque sphere
        if(ray_sphere_intersect2(origin,s.getSphere(i),pos) && i != index){//si intersection, pas sur la même sphère
            if(dist == -1 || dist > (pos-origin.getVector()).norm()){//si intersection plus près
                res = pos;//on change l'intersection
                dist = (pos-origin.getVector()).norm();
                j = i;
            }
        }
    }
    if(dist == -1 || nb == 0){ //pas d'intersection' ou plus de reflexion
        return  phongColor(ca, s, lights, s.getSphere(index), origin.getPoint());
    }
    else{//intersection donc calcul recursif
        Vector rf = origin.getVector().normalize();
        Vector n = s.getSphere(j).getNormal(res).normalize();
        rf = rf - 2 * (rf*n)*n;

        double r = s.getSphere(index).getR();
        //return (phongColor(ca, s, lights, s.getSphere(index), origin.getPoint()).multiply(1));//.add(computeColor(ca,s,Ray(rf ,res),j, nb--).multiply(1-r));
        return mix(phongColor(ca, s, lights, s.getSphere(index), origin.getPoint()), computeColor(ca,s,Ray(rf ,res),j, nb--), r);
    }



}
