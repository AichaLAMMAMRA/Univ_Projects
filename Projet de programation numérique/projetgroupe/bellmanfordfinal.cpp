#include <iostream>
#include <list>
#include <limits.h>
#include <cstdlib>

using namespace std;

class AdjListNode
{
   int sommet; 
   int poids;

   public:
   AdjListNode(int _v, int _w) { sommet = _v; poids = _w;}
   int getsommet() { return sommet; }
   int getpoids() { return poids; }
};

class Graph
{
   int V; // No. of vertices'
   int * pred;
   // Pointer to an array containing adjacency lists
   list<AdjListNode> *adj;
   
   public:
   Graph(int V); // Constructor
   // function to add an edge to graph
   void addEdge(int u, int v, int poids);
   void bell_ford(int src);
   void imprimer_chemin(int noeud);
}; 

Graph::Graph(int V)
{
   this->V = V;
   pred= new int[V];
   adj = new list<AdjListNode>[V];
}

void Graph::addEdge(int u, int v, int poids)
{
   AdjListNode noeud(v, poids);
   adj[u].push_back(noeud); // Add v to u's list
  
}

void Graph::bell_ford(int src)
{
   int dist[V];
   for (int i = 0; i < V; i++){
      dist[i] = INT_MAX;
      pred[i]=INT_MAX;
   }
   
   dist[src] = 0;
   int a=0;
   for(int k=0; k < V-1 ; ++k){ 
    for (int u = 0; u < V; u++)
    {

      
      list<AdjListNode>::iterator i;
        if (dist[u] != INT_MAX)
        {
          for (i = adj[u].begin(); i != adj[u].end(); ++i)
          if (dist[i->getsommet()] > dist[u] + i->getpoids())
          {
            dist[i->getsommet()] = dist[u] + i->getpoids();
            pred[i->getsommet()]=u;
          }
        }
     }
   }
    
   for(int u=0; u<V; u++){
        for(list<AdjListNode> :: iterator i=adj[u].begin(); i!=adj[u].end(); ++i){
           if(dist[i->getsommet()]>dist[u]+ i->getpoids()){
           a++;
            
           }
          
        }
  
   }
      if(a==0){
        cout<<"Sommet\t\tDistance minimale\t\tPlus court chemin"<<endl;
        for(int i=0; i<V; i++)
        {
          cout<<i<<"\t\t"<<dist[i]<<"\t\t";
          cout<<endl;
        }
   
      }else{ cout << "Cycle absorbant détecté" << endl;}
}

void Graph :: imprimer_chemin(int noeud)
{
  int src=0;
  if(noeud==src){
     cout << noeud <<" ";
  }
  else{
    imprimer_chemin(pred[noeud]);
      cout << noeud <<" ";
  }
}

int main()
{ 
   
  Graph g(6);
  g.addEdge(0, 1, 10);
  g.addEdge(0, 5, 8);
  g.addEdge(1, 3, 2);
  g.addEdge(2, 1, 1);
  g.addEdge(3, 2, -2);
  g.addEdge(4, 3, -1);
  g.addEdge(4, 1, -4);
  g.addEdge(5, 4, 1);
  int s = 0;
  cout << "******Resultats******" << endl;
  g.bell_ford(s);
  
  return 0;
}


