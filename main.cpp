/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: gtaf
 *
 * Created on 21 mai 2017, 14:05
 */

#include <cstdlib>
#include "basics.h"
#include <iostream>
#include "CImg.hpp"
#include "phongReflectionModel.h"

#define IMAGE_SIZE_F 10;
using namespace std;
using namespace cimg_library;



/*
 *
 */
bool ray_sphere_intersect(Ray r, Sphere s, double *dist);
bool ray_sphere_intersect2(Ray r, Sphere s, Vector& v);
void tracerRecursive(Camera ca, Scene s);
Color computeColor(Camera ca, Scene s, Ray origin, int index);
void tracer(Camera ca, Scene s);

int main(int argc, char** argv) {
    Sphere s = Sphere(250, Vector(400,400,300), Color(255,50,0), Material(0.5,0.5,0.5,20.));
    Sphere t = Sphere(250, Vector(400,0,0), Color(255,50,0), Material(0.5,0.5,0.5,20.));
    Ray r = Ray(Vector(1,0,0), Vector(0,5,0));
    double dist;
    //bool b = ray_sphere_intersect(r,s,&dist);
    //cout << b << endl;  Camera ca = Camera(Vector(0,0,0), Vector(100,0,0), Vector(0,1,0), 1000, 1000);
    Camera ca = Camera(Vector(0,0,0), Vector(100,0,0), Vector(0,1,0), 1000, 1000);
    Scene sc = Scene(0);
    sc.setAmbiantLighting(0.3);
    sc.addSphere(s);
    sc.addSphere(t);

    tracerRecursive(ca,sc);
    /*Sphere s(2,Vector(4,0,0),Color(255,50,0),Material(0.5,0.5,0.5,0.1));
    Ray r(Vector(4,1,0),Vector(0,0,0));
    double dist;
    Vector pos;
    if(ray_sphere_intersect(r,s,&dist)){//compute color
        Vector intersect = Vector(1,0,0)+r.getVector().normalize().multiply(dist);
        cout << intersect<<std::endl<<"first"<<std::endl;
    }
    if(ray_sphere_intersect2(r,s,pos)){
      std::cout<<pos<<std::endl;
    }*/
    return 0;
}

bool ray_sphere_intersect(Ray r, Sphere s, double *dist){//le rayon part du point de vision
    double d = 0;
    Vector o = r.getPoint(), c = s.getCenter(), l = r.getVector();

    Vector m = o-c;
    //cout << l << endl;
    d = l.normalize()*m;
    d = d*d;
    d -= m*m;
    d += s.getRadius()*s.getRadius();
    //cout << d << endl;
    //according to
    //https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    //where o = r.getPoint() c = s l = r.getVector
    if(d>=0){
        *dist = -(r.getVector()*(r.getVector()-s.getCenter()))-sqrt(d);
    }
    return d >= 0;
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

void tracer(Camera ca, Scene s){
    //pour tous les spheres

    Vector eye = ca.getEye();
    Vector target = ca.getTarget();

    //code for Cimg found at
    //https://stackoverflow.com/questions/14914709/
    CImg<float> img(ca.getH(),ca.getW(),1,3); //create image

    cimg_forXY(img,x,y) {  //for each pixel of the image
        Ray r = ca.Rayf(x,y);//rayon qui va de l'oeil au point (x.y)
        //cout << r.getPoint() << r.getVector() << s.getSphere(0).getCenter() << s.getSphere(0).getRadius() << endl<<endl;
        for(int i = 0; i < s.size(); i++){
            double dist = 0;
            //if(ray_sphere_intersect(r,s.getSphere(i),&dist)){//compute color
              //  Vector intersect = ca.getEye()+r.getVector().normalize().multiply(dist);
                //cout << intersect;
                Vector pos;
              if(ray_sphere_intersect2(r,s.getSphere(i),pos)){
                std::vector<Light> lights;
                //lights.push_back(Light(Vector(0,100,200), Color(1.,1.,1.),Color(0.,0.,0.)));
                lights.push_back(Light(Vector(0,-100,-200), Color(1.,1.,1.),Color(1.,1.,1.)));
                Color c = phongColor(ca, s, lights, s.getSphere(i), pos);
                for(int i = 0; i <3; i++){
                    img(x,y,i) = c.getValue(i);
                }
            }
            else{
                for(int i = 0; i <3; i++){
                    img(x,y,i) = 0;
                }
            }
        }
        //img(x,y,c) = pixel_value_at(x,y,c);
    }
    CImgDisplay main_disp(img,"Click a point");
    img.save_png("test.png");
    img.save_jpeg("test.jpeg");
    img.save_bmp("test.bmp");
    while (!main_disp.is_closed()){}
}


Color computeColor(Camera ca, Scene s, Ray origin, int index){//calcul la couleur du point vu "depuis" le vecteur origin

    double dist = -1;//distance au point intersecté
    int j;
    Vector pos, res;//vecteur à intersecter

    std::vector<Light> lights;
    //lights.push_back(Light(Vector(0,100,200), Color(1.,1.,1.),Color(0.,0.,0.)));
    lights.push_back(Light(Vector(0,-100,-200), Color(1.,1.,1.),Color(1.,1.,1.)));
    //recherc he une intersection
    /*for(int i = 0; i < s.size(); i++){//pour chaque sphere
        if(ray_sphere_intersect2(origin,s.getSphere(i),pos) && i != index){//si intersection, pas sur la même sphère
            if(dist == -1 || dist > (pos-origin.getVector()).norm()){//si intersection plus près
                res = pos;//on change l'intersection
                dist = (pos-origin.getVector()).norm();
                j = i;
            }
        }
    }*/
    if(dist == -1){ //pas d'intersection'
        return  phongColor(ca, s, lights, s.getSphere(index), origin.getPoint());
    }
    /*else{//intersection donc calcul recursif
        cout << "problem" << endl;
        double r = s.getSphere(index).getR();
        return phongColor(ca, s, lights, s.getSphere(j), res).multiply(r).add(computeColor(ca,s,origin,j).multiply(1-r));
    }*/


}

void tracerRecursive(Camera ca, Scene s){
    //pour tous les spheres

    Vector eye = ca.getEye();
    Vector target = ca.getTarget();

    //code for Cimg found at
    //https://stackoverflow.com/questions/14914709/
    CImg<float> img(ca.getH(),ca.getW(),1,3); //create image
    cimg_forXY(img,x,y) {  //for each pixel of the image
        Ray r = ca.Rayf(x,y);//rayon qui va de l'oeil au point (x.y)
        bool intersect = false;

        double dist = -1;//distance au point intersecté
        int j = -1; //numero de la sphere finale retenue
        Vector pos, res;//vecteur à intersecter

        for(int i = 0; i < s.size(); i++){//pour chaque sphere
            if(ray_sphere_intersect2(r,s.getSphere(i),pos)){//si intersection
                intersect = true;
                if(dist == -1 || dist > (pos-r.getPoint()).norm()){//si intersection plus près
                    res = pos;//on change l'intersection
                    dist = (pos-r.getPoint()).norm();
                    j = i;
                }
            }
        }

        if(intersect){//si intersection
            Color c = computeColor(ca, s, Ray(r.getVector(), res), j);
            for(int i = 0; i <3; i++){
                img(x,y,i) = c.getValue(i);
            }
        }

        else{//sinon couleur de fond noire
            cout << "coucou" << endl;
            for(int i = 0; i < 3; i++){
                img(x,y,i) = 0;
            }
        }
        //img(x,y,c) = pixel_value_at(x,y,c);
    }
    CImgDisplay main_disp(img,"Click a point");
    img.save_png("test.png");
    img.save_jpeg("test.jpeg");
    img.save_bmp("test.bmp");
    while (!main_disp.is_closed()){}
}
