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

class Graph_b
{
   int V; // nbr de vecteurs
   int * pred;
   int src;
   // Pointeur sur un tableau contenant des listes adjacentes
   list<AdjListNode> *adj;
   
   public:
   Graph_b(int V); // Constructeur
   //~Graph_b();
   // pour ajouter un bord au graphique
   void addEdge(int u, int v, int poids);
   //void afficher
   //void bell_ford(int src);
   void bell_ford();
   void imprimer_chemin(int noeud);
   void afficher_graphe();
   void graphe_test();
}; 

Graph_b::Graph_b(int V)
{
   this->V = V;
   pred= new int[V];
   adj = new list<AdjListNode>[V];
}

void Graph_b::addEdge(int u, int v, int poids)
{
   AdjListNode noeud(v, poids);
   adj[u].push_back(noeud); // Ajouter v à la liste de u
  
}

void Graph_b:: afficher_graphe()
{
  std::cout << endl;
  std::cout<< "Representation du graphe de 10 sommets par des listes d'adjacence :"<< std::endl;
  std::cout << endl;
    for(int u=0; u<V; ++u)
    {  
      if(!adj[u].empty())
      {  
        cout << "Sucesseus de"<<" "<< u<<" sont:"<<" ";
        for(list<AdjListNode> :: iterator i=adj[u].begin(); i!=adj[u].end(); ++i)
         cout <<(*i).getsommet()<<" ";
         
      }
      else
      {
             cout <<"Sucesseurs de"<<" "<< u <<" "<<"aucun";
      }
    cout << endl;
        
    }
    std::cout << endl;
}


void Graph_b::bell_ford()
{
  std::cout << "Entrer la source : " << std::endl;
  std:: cin >> src;
  cout << "Les distances les plus courtes par rapport à la source sont les suivantes " << src <<" \n";
  cout << "******Resultats******" << endl;
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
            pred[i->getsommet()]= u;
          }
        }
     }
   }
    
   for(int u=0; u<V; u++)
   {
        for(list<AdjListNode> :: iterator i=adj[u].begin(); i!=adj[u].end(); ++i)
        {
          if(dist[i->getsommet()] > dist[u]+ i->getpoids())
          {
            a++;
          }
          
        }
    }

    if(a == 0)
    {
      cout<<"Sommet\t\tDistance minimale\t\tPlus court chemin"<<endl;
        for(int i=0; i<V; i++)
        {
          if (i==src)
          {
            cout << src << "\t\t "<<dist[src] <<"\t\t"<<"\t\t";
          }
          else
          cout<<i<<"\t\t"<<dist[i]<<"\t\t"<<"\t\t";
          imprimer_chemin(i);
          cout<<endl;
        }
   
      }
      else
      { 
        cout << "Cycle absorbant détecté" << endl;
      }
}

void Graph_b :: imprimer_chemin(int noeud)
{
  //int src=0;
  if(noeud==src){
     cout << noeud <<" ";
  }
  else{
    imprimer_chemin(pred[noeud]);
      cout << noeud <<" ";
  }
}
/*void Graph_b :: graphe_test()
{
  //Graph_b g(26);
  Graph_b g(10);
  g.addEdge(0, 1, 1);
  g.addEdge(0, 2, 1);
  g.addEdge(1, 0, 1);
  g.addEdge(1, 5, 1);
  g.addEdge(1, 2, 1);
  g.addEdge(2, 1, 1);
  g.addEdge(2, 4, 1);
  g.addEdge(2, 0, 1);
  g.addEdge(3, 4, 1);
  g.addEdge(3, 8, 1);
  g.addEdge(4, 2, 1);
  g.addEdge(4, 3, 1);
  g.addEdge(4, 5, 1);
  g.addEdge(5, 1, 1);
  g.addEdge(5, 4, 1);
  g.addEdge(5, 6, 1);
  g.addEdge(5, 7, 1);
  g.addEdge(6, 5, 1);
  g.addEdge(6, 7, 1);
  g.addEdge(7, 5, 1);
  g.addEdge(7, 6, 1);
  g.addEdge(7, 8, 1);
  g.addEdge(7, 9, 1);
  g.addEdge(8, 3, 1);
  g.addEdge(8, 7, 1);
  g.addEdge(9, 7, 1);

}
*/