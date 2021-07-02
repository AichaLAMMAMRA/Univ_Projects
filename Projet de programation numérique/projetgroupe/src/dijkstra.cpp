#include "Graphe.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#define INF 100
using namespace std;

Graph :: Graph()
{

  ifstream monFlux_entrer("/home/alainkonan/TP/projet/projet/projetgroupe/src/test.txt");  //Ouverture d'un fichier en lecture

  if(monFlux_entrer)
  {
    monFlux_entrer >> n;  
    g=new int*[n];
    d=new int*[n];
    int i,j;
    for(i=0; i<n; i++)
      g[i]=new int[n];

    for (int i = 0; i < n; i++)
      d[i]=new int [n];
    for (int i = 0; i < n; ++i)
    {
      for (int j = 0; j < n; ++j)
      {
        monFlux_entrer >> d[i][j];
      }
    }
    //Tout est prêt pour la lecture.
  }
  else
  {
    cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
  }
   
  //srand(time(NULL));
  for( int i=0;i<n;i++)
  {
      for(int j=0;j<n;j++)
      {
        g[i][j]=0;
        //fichier >> g[i][j];//=val[(i-1)*((2*n-i)/2)+j];
        if (i!=j)
        {
          if (d[i][j]==1 && d[j][i]==1 )
          {
            g[i][j]=g[j][i]=1;
          }

          //auto r = rand()%100;
          //if ((r>=10 && r<=20)||(r>=40 && r<=60)){
          else{
            g[i][j]=g[j][i]=INF;
          }

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
      if (g[i][j]==INF)
      {
        std::cout<< 0 <<" ";
      }
      else{
         cout << g[i][j] <<" ";
      }
      //cout << g[i][j] <<" ";
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
   std::cout<<endl;
   cout<<"Donner le noeud source : "<<endl;
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
          cout <<source<<" "<<source;
        else
          imprimerchemin_dijkstra(i);
        if(distmin[i]!= INF)
          cout <<"\t\t\t\tDistance minimale:"<<distmin[i]<<endl;
        else
          cout <<" \tPas de chemin"<<endl;
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











 





