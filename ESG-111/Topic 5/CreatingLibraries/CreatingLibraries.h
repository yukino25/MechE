#ifndef CREATINGLIBRARIES_H
#define CREATINGLIBRARIES_H

#include "Arduino.h"


class MyFoods {
  public:
    String getFruit(String c, String sz);
    String getVegetable(String c, String sz);
    String getStarch(String c, String sz);
};

#endif