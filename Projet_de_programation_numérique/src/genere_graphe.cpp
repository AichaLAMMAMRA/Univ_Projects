#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(int argc, char const *argv[])
{
	int n;
	
	//int ** g ;
	//g =new int *[n];
	//for (int i = 0; i < n; ++i)
		//g[i]=new int[n];

	std::cout << "entre le nombre de sommets : ";
	std::cin >> n;

	int mat [n][n];

	srand(time(NULL));
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			//g[i][j]=0;
			mat[i][j]=0;

			auto r = rand()%100;
			
			if (i!=j)
			{
				
				if ((r>=10 && r<=30)||(r>=40 && r<=70))
				{
     				//g[i][j]=g[j][i]=0;
     				mat[i][j]=mat[j][i]=0;
     			}
     			else
     			{
     				//g[i][j]=g[j][i]=1;
     				mat[i][j]=mat[j][i]=1;
     			}
     			
			}
			else
			{
				 
			}
     		
			
		}
	}

	 /*for (int i = 0; i < n; ++i)
  		{
    		for (int j = 0; j < n; ++j)
   		 	{
         		cout << mat[i][j] <<" ";
      		}

    		cout << endl;
  		}*/



	string const nomFichier("/home/alainkonan/TP/projet/projet/projetgroupe/src/.txt");

    ofstream monFlux(nomFichier.c_str());


    if(monFlux)    

    {
    	monFlux << n << endl;

      	for (int i = 0; i < n; ++i)
  		{
    		for (int j = 0; j < n; ++j)
    		{
         		monFlux << mat[i][j] <<" ";
    		}

    		monFlux << std::endl;
  		} 

    }

    else

    {

        std::cout << "ERREUR: Impossible d'ouvrir le fichier." << std::endl;

    }
	
	return 0;
}