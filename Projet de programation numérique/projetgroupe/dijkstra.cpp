#include "Graphe.h"
#include <iostream>
#include <fstream>
#define INF 100
using namespace std;

Graph :: Graph()
{
  cout<<" Donner le nombre de sommets du graphe"<<endl;
  cin>>n;  
  g=new int*[n];
  int i,j;
  for(i=0; i<n; i++)
   g[i]=new int[n];

  
  //Saisie de la matrice des distances
  //cout<<"Enter les distances entre les noeuds reliés par un arc"<<endl;
  //cout<<"Entrer la valeur 100 s'il n'existe pas d'arc entre deux noeux du graphe"<<endl;
  //cout<<"Entrer 0 pour la distance d'un noeud à lui-même"<<endl;

  //int m=100;
  //int n=10;
  //int val[m];
  //g = new int*[n];
  //ifstream fichier("testdefaut.txt", ios::in);
  /*for (int i = 0; i <= m-1; ++i)
  {
    fichier >> val[i];
  }
  fichier.close();*/
  //for (int i = 0; i < n; ++i)
    

  //ifstream fichier("testdefaut.txt", ios::in);
  for( int i=0;i<n;i++)
  {
      for(int j=0;j<n;j++)
      {
        g[i][j]=0;
        //fichier >> g[i][j];//=val[(i-1)*((2*n-i)/2)+j];
        if (i!=j)
        {
          g[i][j]=g[j][i]=rand()%100 + 1;

        //   cout<<"Donner la distance arc"<<"("<<i<<","<<j<<"):"<<endl;
        //   cin >> g[i][j];;
        }
         else
        {
        
        }
      }
  }
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      cout << g[i][j] <<" ";
    }
    cout << endl;
  }
  //fichier.close();

    pred= new  int[n];
    distmin= new int[n];
    distrouve= new bool[n];

}

void Graph :: initialiser_dijkstra()
{
   int i;
   cout<<"Donner le noeu source : "<<endl;
   cin>>source;
   //std::cout<<"Donner la destinations : "<<endl;
   //std::cin>>destination;
   cout<<endl;
   for(i=0;i<n;i++)
     distmin[i]=INF;
      for(i=0;i<n;i++)
        distrouve[i]=false;
        for(i=0;i<n;i++)
          pred[i]=-1;
}

void Graph :: dijkstra()
{
  int i,j;
  for(j=0;j<n;j++)
  {
     distmin[j]=g[source][j];
     pred[j]=source;  
  }
   
  distrouve[source]=true;
  distmin[source]=0;
  pred[source]=source;
  
 for(int i=0;i<n-1;i++)
 {
   int mind=INF;
   int v=source;
   for(int j=0;j<n;j++)
    if(!distrouve[j])
      if(distmin[j]<mind)
      {
        v=j;
        mind=distmin[v];
      }
      distrouve[v]=true;
      for(int j=0;j<n;j++)
        if(!distrouve[j])
          if(mind+g[v][j]<distmin[j])
          {
            distmin[j]=mind+g[v][j];
            pred[j]=v;
          }
  } 
}

void Graph :: afficher_dijkstra()
{
  int i;
  for(i=0;i<n;i++)
  {
     if(i==source)
      cout<<source<<" "<<source;
     else
      imprimerchemin_dijkstra(i);
      //imprimerchemin_dijkstra(destination);
     if(distmin[i]!=INF)
     cout<<"Distance minimale:"<<distmin[i]<<endl;
     else
     cout<<" Pas de chemin"<<endl;
  } 

}

void Graph :: imprimerchemin_dijkstra(int noeud)
{  

     if(noeud==source)
     cout<<noeud<<" ";
     else
    {
     imprimerchemin_dijkstra(pred[noeud]);
     cout<<noeud<<" ";
    }

}











 





