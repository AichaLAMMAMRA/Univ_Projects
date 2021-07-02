#include "Graphe.h"
#include <iostream>
using namespace std;

int main()
{
  cout<<endl;
  int continuer=0;
  int a;
  do
   {
    cout << "Saisie 1:dijkstra et 2:bellman ford "<< endl;
    cin >> a;

    if (a==1)
    {
      Graph G;
      G.initialiser_dijkstra();
      G.dijkstra();
      cout<<endl;
      cout<<"*****Resultats******"<<endl;
      G.afficher_dijkstra();
      cout<<"Continuer(0)?"<<endl;
      cin>>continuer;
     
    }
    else if (a==2)
    {
      Graphe Gb;
      Gb.initialiser_bellman();
      Gb.bellman();
      cout<<endl;
      cout<<"*****Resultats******"<<endl;
      Gb.afficher_bellman();
      cout<<"Continuer(0)?"<<endl;
      cin>>continuer;
    }
    else
    {
      cout<<"Continuer(0)?"<<endl;
      cin>>continuer;
    }
     
  } while (continuer==0);
   
 return 0;
}
