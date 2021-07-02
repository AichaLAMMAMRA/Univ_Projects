#include "Graphe.h"
#include <iostream>
#define INF 100
using namespace std;

Graphe :: Graphe()
{
  cout<<" Donner le nombre de sommets du graphe"<<endl;
  cin>>n;  
  g=new int*[n];
  int i,j;
  for(i=0; i<n; i++)
   g[i]=new int[n];
  //Saisie de la matrice des distances
  cout<<"Enter les distances entre les noeuds reliés par un arc"<<endl;
  cout<<"Entrer la valeur 100 s'il n'existe pas d'arc entre deux noeux du graphe"<<endl;
  cout<<"Entrer 0 pour la distance d'un noeud à lui-même"<<endl;
  for(i=0;i<n;i++)
  {
      for(j=0;j<n;j++)
      {
        g[i][j]=0;
        if (i !=j )
        {
          cout<<"Donner la distance arc"<<"("<<i<<","<<j<<"):"<<endl;
          cin >> g[i][j];
        }
         
      }
  }
    pred= new  int[n];
    distmin= new int[n];
    distrouve= new bool[n];

}

void Graphe :: initialiser_bellman()
{
   int i;
   cout<<"Donner le noeud source"<<endl;
   cin>>source;
   cout<<endl;
   for(i=0;i<n;i++)
     distmin[i]=INF;
      for(i=0;i<n;i++)
        distrouve[i]=false;
        for(i=0;i<n;i++)
          pred[i]=-1;
}

void Graphe :: bellman()
{   
     for(int i=0;i<n;i++)
     {
        distmin[i]=g[source][i];
        pred[i]=source;
     }
         distrouve[source]=true;
         distmin[source]=0;
         pred[source]=source;

         for(int k=0; k<n-2; ++k)
         {
            for (int i = 0; i < n-1; ++i)
            {
              int mind = INF;
              int v;
              cout << k+1 << "ième itération " << endl;
              for(int j=0; j<n-1; ++j)
              {
                if(!distrouve[j])
                {
                  v=j;
                  mind=distmin[v];
                }

                distrouve[v]=true;
              }

              for(int j=0;j<n;++j)
              {
                if(!distrouve[j])
                if(distmin[j]>mind+g[v][j])
                {
                distmin[j]=g[v][j]+mind;
                pred[j]=v;
                }
              }
              imprimerchemin_bellman(i);
              cout<<endl;
            }
            
            /*cout << distmin[v] <<endl;
   
            for(int i=0;i<n;++i)
            {
              for(int j=0;j<n;++j)
              {
                if(distmin[j]>distmin[j]+g[i][j])
                  cout << "Circuit absorbant: le plus court chemin est mal defini " << endl;
              }
            }*/
          }
}

void Graphe :: afficher_bellman()
{
  int i;
  for(i=0;i<n;i++)
   {
     if(i==source)
      cout<<source<<" "<<source;
     else
     imprimerchemin_bellman(i);
     //if(distmin[i]!=INF)
     cout<<"Distance minimale:"<<distmin[i]<<endl;
     //else
     //cout<<" Pas de chemin"<<endl;
   } 

}

void Graphe :: imprimerchemin_bellman(int noeud)
{  

     if(noeud==source)
     cout<<noeud<<" ";
     else
    {
     imprimerchemin_bellman(pred[noeud]);
     cout<<noeud<<" ";
    }

}











 




       













 



























 

