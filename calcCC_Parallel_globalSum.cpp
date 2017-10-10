//CSCI415; Saeed Salem, 9/6/2017
//To compile: g++ -O3 -w assign2Graph.cpp -lpthread -o assign2Graph
//To run: ./assign2Graph filename
//./assign2Graph networkDatasets/toyGraph1.txt
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>

using namespace std;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

long double globalCC;

pthread_mutex_t mutex;
pthread_mutex_t messsageMutex;

int numNodes;
const int MAX_THREADS = 64;
int threadCount;

void printAdjMatrix(AdjacencyMatrix adjMatrix)
{
  for (int i=0; i<adjMatrix.size(); i++)
  {
    for (int j=0; j<adjMatrix[i].size(); j++) cout<<adjMatrix[i][j]<<" ";
    cout<<endl;
  }
}

void *calcClusteringCoeff(void *rank)
{
  int start, end, offset;
  long id;
  id = (long) rank;
  offset = numNodes / threadCount;
  start = id * offset;
  if((int)id == (int)threadCount - 1) end = numNodes -1 ;
  else end = start + offset - 1;

  pthread_mutex_lock(&messsageMutex);
  cout << "Hi, I'm thread " << id << " and I'm working on nodes " << start << " to "  << end << endl << endl;
  pthread_mutex_unlock(&messsageMutex);

  for(int node = start; node <= end; node++)
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

    if(degree > 1)
    {
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
      pthread_mutex_lock(&mutex);
      globalCC += localCC;
      pthread_mutex_unlock(&mutex);

      //cout << "Node: " << node << "\n\tLocal Clustering Coefficient: " << localCC << "\n\tDegree: " << degree << "\n\tTriangles: " << triangles << endl << endl;
    }
  }
}

int main(int argc, char** argv)
{
  long thread;
  pthread_t* thread_handles;

  //get thread count from command line arguments
  threadCount = atoi(argv[2]);

  thread_handles = (pthread_t*) malloc(threadCount*sizeof(pthread_t));

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

  numNodes = adjMatrix.size();

  for(thread = 0; thread < threadCount; thread++){
    pthread_create(&thread_handles[thread], NULL, calcClusteringCoeff, (void*)thread);
  }

  for(thread = 0; thread < threadCount; thread++)
  {
    pthread_join(thread_handles[thread], NULL);
  }

  cout << "cumulative CC " << globalCC << endl << endl;


  globalCC /= numNodes;

  cout << "Global Clustering Coefficient: " << globalCC  << endl << endl;
  if(n<=10)
  printAdjMatrix(adjMatrix);

  return 0;
}
