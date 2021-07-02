#include<iostream>
#include <list>
#include <stack>
#include <limits.h>
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
  cout<<"Enter les distances entre les noeuds reliés par un arc"<<endl;
  cout<<"Entrer la valeur 100 s'il n'existe pas d'arc entre deux noeux du graphe"<<endl;
  cout<<"Entrer 0 pour la distance d'un noeud à lui-même"<<endl;
  for(i=0;i<n;i++)
  {
      for(j=0;j<n;j++)
      {
        g[i][j]=0;
        if (i!=j)
        {
          cout<<"Donner la distance arc"<<"("<<i<<","<<j<<"):"<<endl;
          cin >> g[i][j];;
        }
        else
        {
           
        }
      }
	}

    pred= new  int[n];
    distmin= new int[n];
    distrouve= new bool[n];
}

void Graph :: initialiser()
{
   int i;
   cout<<"Donner le noeu source : "<<endl;
   cin>>source;
   //std::cout<<"Donner la destinations : "<<endl;
   //std::cin>>destination;
   cout<<endl;
   for(i=0;i<n;i++)
     distmin[i]=INF;
      //for(i=0;i<n;i++)
        //distrouve[i]=false;
        for(i=0;i<n;i++)
          pred[i]=-1;
}


void Graph::bell_ford()
{
	int i,j;
	for(j=0;j<n;j++)
  	{
     	distmin[j]=g[source][j];
     	pred[j]=source;  
  	}
   
  //distrouve[source]=true;
  	distmin[source]=0;
  	pred[source]=source;
 
 	for(int k=0; k<n-2 ; k++)
 	{
 		for(int i=0;i<n-1;i++)
 		{
   			int mind=INF;
   			int v=source;
     	/*for(int j=0;j<n;j++)
       		if(!distrouve[j])
        	if(distmin[j]<mind)
         	{
          		v=j;
          		mind=distmin[v];
         	}*/
  //distrouve[v]=true;
     		for(int j=0;j<n;j++)
     		{
      //if(!distrouve[j])
       			if(mind+g[v][j]<distmin[j])
       			{
                	distmin[j]=mind+g[v][j];
                	pred[j]=v;
       			}
       		}
		}
 	}

 	for (int i = 0; i < n-1; ++i)
 	{
 		for (int j = 0; j < n-1; ++j)
 		{
 			if (distmin[j] > mind + g[v][j])
 			{
 				afficher();
 			}
 		}
 	}

     
 } 
 

}

void Graph :: afficher()
{
  int i;
  for(i=0;i<n;i++)
   {
     if(i==source)
      cout<<source<<" "<<source;
     else
      imprimerchemin(i);
      //imprimerchemin(destination);
     if(distmin[i]!=INF)
     cout<<"Distance minimale:"<<distmin[i]<<endl;
     else
     cout<<" Pas de chemin"<<endl;
   } 

}

void Graph :: imprimerchemin(int noeud)
{  

     if(noeud==source)
     cout<<noeud<<" ";
     else
    {
     imprimerchemin(pred[noeud]);
     cout<<noeud<<" ";
    }

}