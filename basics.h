/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   basics.h
 * Author: gtaf
 *
 * Created on 21 mai 2017, 14:10
 */
#pragma once
#include <vector>
#include <iostream>
#include <string>
class Vector{
public:
    Vector();//vecteur nul
    Vector(double a, double b, double c);
    Vector multiply(double n)const ;//multiplication par un scalaire
    Vector add(Vector a)const ;//additionne 2 vecteurs
    double dotProduct(Vector a)const ;//produit scalaire
    Vector orth(Vector v)const ;
    Vector minus(Vector v)const ;
    double norm()const ;
    Vector normalize()const ;
    void print(std::ostream& out)const{out<< "Vector coordinates: " << std::endl<<"x = "<<x<<std::endl<<"y = "<<y<<std::endl<<"z = "<<z<<std::endl;}


private:
    double x, y ,z;
};

inline Vector operator+(Vector const& a, Vector const& b){return a.add(b);}
inline Vector operator-(Vector const& a, Vector const& b){return a.minus(b);}
inline Vector operator*(double const& a, Vector const& b){return b.multiply(a);}
inline Vector operator*(Vector const& b, double const& a){return b.multiply(a);}
inline double operator*(const Vector& a, const Vector& b){return a.dotProduct(b);}
inline Vector operator^(Vector const& a, Vector const& b){return a.orth(b);}
inline std::ostream& operator<<(std::ostream& out, const Vector& r){r.print(out);}

/**
* Classe représentant une couleur, selon ses trois composantes rouge, verte et bleue
*/
class Color{
public:
    Color(const double& r,const double& g,double const& b){rgb[0]=r;rgb[1]=g;rgb[2]=b;}
    Color(){rgb[0]=0.;rgb[1]=0.;rgb[2]=0.;}
    void setValue(const double value,const int channel);
    double getValue(const int channel)const;
private:
    double rgb[3];
};

/**
* Classe représentant un matériau. Un matériau possède quatre propriétés représentant sa réaction à la lumière ambiantede l'environnement,
* à la lumière diffuse émise par une source, à la lumière spéculaire émise par une source, et à son rapprochement du comportement d'un miroir.
*/
class Material{
public:
  Material(){};
  Material(const double& s,const double& d,const double& a,const double& alph):ks(s),kd(d),ka(a),alpha(alph){}
  double getSpecular()const{return ks;}
  double getDiffuse()const{return kd;}
  double getAmbient()const{return ka;}
  double getAlpha()const{return alpha;}

private:
  double ks;  //specular reflection constant, the ratio of reflection of the specular term of incoming light
  double kd;  //diffuse reflection constant, the ratio of reflection of the diffuse term of incoming light
  double ka;  //ambient reflection constant, the ratio of reflection of the ambient term present in all points in the scene rendered
  double alpha; //shininess constant for this material, which is larger for surfaces that are smoother and more mirror-like. When this constant is large the specular highlight is small
};

class Ray{
public:
    Ray(Vector gv, Vector gp);
    Vector getPoint()const;
    Vector getVector()const;


private:
    Vector v,p;//p est le point, v le vecteur
};

class Sphere{
public:
    Sphere();
    Sphere(int r, Vector c);
    Sphere(int r, Vector v , Color c , Material m):radius(r),p(v),color(c),material(m){}
    Vector getCenter()const;
    Vector getNormal(const Vector& pos)const{return (pos-p).normalize();}
    void setMaterial(const Material& m){material=m;}
    Material getMaterial()const{return material;}
    int getRadius()const;

private:
    int radius;
    Vector p;
    Color color;
    Material material;
};

class Source{
private:
    Vector p;
};

class Light{
public:
    Light(const Vector& P,const Color& c,const double& spec,const double& diff):p(P),color(c),specular(spec),diffuse(diff){}
    Vector getPoint()const{return p;};
    Color getColor()const{return color;}
    double getSpecular()const{return specular;}
    double getDiffuse()const{return diffuse;}

private:
    Vector p;
    Color color; //Les trois composantes de couleur
    double specular;
    double diffuse;
};

class Camera{
public:
    Camera(Vector Eye, Vector Target, Vector Up, int H, int W):
    eye(Eye),target(Target), up(Up), h(H), w(W){}
    Vector getEye()const;
    Vector getTarget()const;
    Vector getUp()const;
    int getW()const;
    int getH()const;
    Ray Rayf(double x, double y);//ray thas goes from eye to target + (x,y)


private:
    Vector eye, target, up;//up défini l'unité de base de l'image
    int h,w;//dimensions
};

class Scene{
public:
    Scene(int size);
    int size();
    void addSphere(Sphere s);
    void setAmbiantLighting(double l);
    double getAmbiantLighting()const{return ambiantLighting;}
    Sphere getSphere(int index)const;
private:
    std::vector<Sphere> set;
    double ambiantLighting;
};
