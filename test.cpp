/**
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: bengallou
 *
 * Created on 21 mai 2017, 14:05
 */

#include <cstdlib>
#include "basics.h"
#include <iostream>
#include "CImg.hpp"

using namespace std;
using namespace cimg_library;

int main(){
  cout<<"Hello world"<<endl;
  Vector center(1.,7.2,4.3);
  Sphere s(10,center);
  Material m;//4.,2.1,2.3,8.9);
  s.setMaterial(m);
  Vector a(2,4,6);
  Vector b(3,4,1);
  cout<<a<<b;
  cout<<(a^b);
  //cout<<s.getMaterial().getAmbient()<<endl;
}
