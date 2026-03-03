#ifndef CREATINGLIBRARIES_H
#define CREATINGLIBRARIES_H

#include <Arduino.h>
#include <String.h>


class Myfoods {
  public:
    Myfoods();
    void getFruit();
    void getVegetable();
    void getStarch();
    void printFoods();
    void readFoods();
    void begin();

  private:
    char Response[32];
    char c[8];
    char sz[4];
    bool spacefound;
    bool colorfound;
    bool sizefound;
    int color_val;
    int size_val;
    char size[3][4];
    char color[8][8];
    char food[20];
    char fruit[8][3][2][20];
    char vegetable[8][3][2][20];
    char starch[8][3][2][20];
    void compareFoods();
};

#endif