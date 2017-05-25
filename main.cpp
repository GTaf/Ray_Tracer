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

using namespace std;
using namespace cimg_library;


/*
 * 
 */
bool ray_sphere_intersect(Ray r, Sphere s, double *dist);
void tracer(Camera ca, Scene s);

int main(int argc, char** argv) {
    
    Sphere s = Sphere(250, Vector(500,0,0), Color(255,50,0), Material(1,1,1,1));
    Ray r = Ray(Vector(1,0,0), Vector(0,5,0));
    double dist;
    bool b = ray_sphere_intersect(r,s,&dist);
    cout << b << endl;
    
    Camera ca = Camera(Vector(0,0,0), Vector(100,0,0), Vector(0,1,0), 1000, 1000);
    Scene sc = Scene(0);
    sc.addSphere(s);
    
    tracer(ca,sc);
    
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
            if(ray_sphere_intersect(r,s.getSphere(i),&dist)){//compute color
                Vector intersect = ca.getEye()+r.getVector().normalize().multiply(dist);
                cout << intersect;
                std::vector<Light> lights;
                lights.push_back(Light(Vector(0,0,1), Color(255,255,255), 0, 1));
                Color c = phongColor(ca, s, lights, s.getSphere(i), intersect);
                for(int i = 0; i <3; i++){
                    //cout << "   " << c.getValue(i)*255;
                    img(x,y,i) = c.getValue(i)*255;
                    cout << c.getValue(i)*255 << "     ";
                }
                cout << endl << endl;
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
    img.save("file.bmp");
    
}

