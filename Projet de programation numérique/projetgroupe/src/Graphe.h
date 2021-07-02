#ifndef GRAPHE_H
#define GRAPHE_H
#include <iostream>
using namespace std;

class Graph
{
  private:
   int** g;
   int** d;
   int n;
   int * pred;
   int * inp;
   int* distmin;
   int* antecedent;
   bool* distrouve;
   int source;
   int destination;

  public:
    Graph();
    void initialiser_dijkstra();
    void dijkstra();
  //void afficher();
    void afficher_dijkstra();
    void imprimerchemin_dijkstra(int noeud);
};

class bell_ford
{
private:
 int nb;
 int** graph;
 int start;
 int* distance;
 int* predecessor;
public:
 void graphbell();
 void initialiser();
 void relacher();
 void afficher();
 void algorithm();
};
#endif//GRAPHE_H
