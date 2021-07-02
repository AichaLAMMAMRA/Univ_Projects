#include "Graphe.h"
#include "bellmanfordfinal.h"
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

int main()
{
  cout<<endl;
  int continuer=0;
  int a;
  time_t deb_d,fin_d,deb_b,fin_b;
  double duree_d,duree_b;
  do
   {
    cout << "Saisie 1:dijkstra et 2:bellman ford "<< endl;
    cin >> a;

    if (a==1)
    {
      
      Graph G;
      deb_d=time(NULL);
      G.initialiser_dijkstra();
      G.dijkstra();
      fin_d=time(NULL);
      cout<<endl;
      cout<<"*****Resultats******"<<endl;
      cout << "Les distances les plus courtes par rapport à la source sont les suivantes " <<std::endl;
      std::cout<< "Chemin\t\tDistance"<<std::endl;
      G.afficher_dijkstra();
      cout<<endl;
      duree_d=difftime(fin_d,deb_d);
      std::cout<<"le temps d'execution de dijkstra est :" << duree_d << "s" <<endl;
      std::cout << endl;
      cout<<"Continuer(0)?"<<endl;
      cin>>continuer;
     
    }
    else if (a==2)
    {
        //graphe test 
     
      ifstream monFlux("/home/alainkonan/TP/projet/projet/projetgroupe/src/test.txt");  //Ouverture d'un fichier en lecture
      if(monFlux)
      {
        int nb;
        monFlux >> nb; 
        Graph_b g(nb);
        int in;

        for (int i = 0; i < nb; ++i)
        {
          for (int j = 0; j < nb; ++j)
          {
            monFlux >> in;
            if (in==1)
              g.addEdge(i,j,1);
            
          }
        }
      
        g.afficher_graphe();
        deb_b=time(NULL);
        g.bell_ford();
        fin_b=time(NULL);
        duree_b=difftime(fin_b,deb_b);
        std::cout << std::endl;
        std::cout<<"le temps d'execution de bellman ford est :" << duree_b<< "s" <<endl;

        cout<<"Continuer(0)?"<<endl;
        cin>>continuer;
  
      }
    }
  else
  {
    cout<<"Continuer(0)?"<<endl;
    cin>>continuer;
  }

     
  } while (continuer==0);
   
 return 0;
}
