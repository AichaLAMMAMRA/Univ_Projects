#ifndef GRAPHE_H
#define GRAPHE_H
#include <iostream>
using namespace std;

class Graph
{
  private:
   int** g;
   int n;
   int * pred;
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

class Graphe
{
private:
  int** g;
  int n;
  int * pred;
  int* distmin;
  int* antecedent;
  bool* distrouve;
  int source;
  int destination;

public:
  Graphe();
  void initialiser_bellman();
  //void afficher();
  void afficher_bellman();
  void imprimerchemin_bellman(int noeud);
  void bellman();
  
};
#endif//GRAPHE_H
