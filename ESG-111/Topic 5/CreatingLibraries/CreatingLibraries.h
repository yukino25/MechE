#ifndef CREATINGLIBRARIES_H
#define CREATINGLIBRARIES_H

#include "Arduino.h"


class MyFoods {
  public:
    void getFruit();
    void getVegetable();
    void getStarch();
    void printFoods();
    void readFoods();
    void begin();
    MyFoods();
  private:
    Char Response;
    Char c;
    Char sz;
    Bool spacefound;
};

#endif