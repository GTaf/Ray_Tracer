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

#define IS 1000
using namespace std;
using namespace cimg_library;



/*
 *
 */
bool ray_sphere_intersect2(Ray r, Sphere s, Vector& v);
void tracerRecursive(Camera ca, Scene s);
Color computeColor(Camera ca, Scene s, Ray origin, int index, int nb);
//void tracer(Camera ca, Scene s);
bool intersecti(Ray r, Sphere s, Vector& pos);
Color mix(Color a, Color b, double r);

int main(int argc, char** argv) {
    Sphere s = Sphere(1.5, Vector(10,2,2), Color(255,0,0), Material(0.5,0.5,0.5,20.));
    s.setR(0.5);
    Sphere t = Sphere(1, Vector(10,0,0), Color(0,255,0), Material(1,0.5,0.5,50));
    t.setR(1);
    Camera ca = Camera(Vector(0,0,0), Vector(110,0,0), Vector(0,1,0), IS, IS);
    Scene sc = Scene(0);
    sc.setAmbiantLighting(0.3);
    sc.addSphere(s);
    sc.addSphere(t);

    Color c = Color(1.,1.,1.);
    cout << c << "            " << mix(c,c,0.5) << endl;
    tracerRecursive(ca,sc);
    return 0;
}

Color mix(Color a, Color b, double r){
    Color c;
    for(int i = 0; i < 3;i++){
        c.setValue(a.getValue(i)*r+b.getValue(i)*(1-r),i);
    }

    return c;
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

void tracerRecursive(Camera ca, Scene s){

    double fov = 50, aspectratio = ca.getW() / double(ca.getH());
    double angle = tan(M_PI * 0.5 * fov / 180.);

    //code for Cimg found at
    //https://stackoverflow.com/questions/14914709/
    CImg<float> img(ca.getH(),ca.getW(),1,3); //create image
    cimg_forXY(img,x,y) {  //for each pixel of the image
        double yp = (2 * ((x + 0.5) * 1/ca.getW()) - 1) * angle * aspectratio;
        double zp = (1 - 2 * ((y + 0.5) * 1/ca.getH())) * angle;
        //Ray r = ca.Rayf(x*0.8,y*0.8);//rayon qui va de l'oeil au point (x.y)
        Ray r = Ray(Vector(1,yp,zp).normalize(), Vector(0,0,0));
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
            Vector rf = r.getVector().normalize();
            Vector n = s.getSphere(j).getNormal(res).normalize();
            rf = rf - 2 * (rf*n)*n;
            Color c = computeColor(ca, s, Ray(rf ,res), j, 2);
            for(int i = 0; i <3; i++){
                img(x,y,i) = c.getValue(i);
            }
        }

        else{//sinon couleur de fond noire
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
