//CSCI415; Saeed Salem, 9/6/2017
//To compile: g++ -O3 -w assign2Graph.cpp -lpthread -o assign2Graph
//To run: ./assign2Graph filename
//./assign2Graph networkDatasets/toyGraph1.txt
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;


void printAdjMatrix(AdjacencyMatrix adjMatrix)
{
    for (int i=0; i<adjMatrix.size(); i++)
    {
        for (int j=0; j<adjMatrix[i].size(); j++) cout<<adjMatrix[i][j]<<" ";
        cout<<endl;
    }

}

void calcClusteringCoeff()
{
    int numNodes = adjMatrix.size();
    cout << "Number of Nodes: " << numNodes << endl << endl;
    long double cumulativeCC = 0.0;

    for(int node = 0; node < numNodes; node++)
    {
        long double localCC = 0;
        int degree = 0;
        int triangles = 0;

        std::vector<int> neighbors;
        for(int i = 0; i < numNodes; i++)
        {
            if(adjMatrix[node][i] == 1)
            {
              neighbors.push_back(i);
              degree++;
            }

        }


        if(degree > 1){
          for(int j = 0; j < neighbors.size(); j++)
          {
              int pal1 = neighbors[j];
              for(int k = j + 1; k < neighbors.size(); k++)
              {
                  int pal2 = neighbors[k];
                  if(adjMatrix[pal1][pal2] == 1)
                  {
                    triangles++;
                  }
              }
          }

          localCC = (long double) (2 * triangles) / (degree * (degree - 1));
          cumulativeCC += localCC;
          //cout << "Node: " << node << "\n\tLocal Clustering Coefficient: " << localCC << "\n\tDegree: " << degree << "\n\tTriangles: " << triangles << endl << endl;
        }

    }

    cout << "cumulative cc: " << cumulativeCC << endl << endl;

    long double  globalCC = cumulativeCC / numNodes;
    cout << "Global Clustering Coefficient: " << globalCC << endl << endl;

}

int main(int argc, char** argv)
{
    if(argc<2){
      cout<<"To run: ./assign2Graph filename"<<endl;
      cout<<"./assign2Graph networkDatasets/toyGraph1.txt"<<endl;
      return 0;
    }

    fstream myfile(argv[1],std::ios_base::in);
    int u,v;
    int maxNode = 0;
    vector<pair<int,int> > allEdges;
    while(myfile >> u >> v)
    {
        allEdges.push_back(make_pair(u,v));
        if(u > maxNode)
          maxNode = u;

        if(v > maxNode)
          maxNode = v;
    }

    int n = maxNode +1;  //Since nodes starts with 0
    cout<<"Graph has "<< n <<" nodes"<<endl;

    adjMatrix = AdjacencyMatrix(n,vector<int>(n));
    //populate the matrix
    for(int i =0; i<allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
       adjMatrix[v][u] = 1;
    }

    calcClusteringCoeff();

    if(n<=10)
      printAdjMatrix(adjMatrix);

	//You can also make a list of neighbors for each node if you want.

return 0;
}
