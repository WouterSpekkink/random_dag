#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <sstream>

// We will create a global matrix so it can be accessed by both functions below.
std::vector<std::vector<int> > matrix;
// The same for a boolean we use to check whether a cycle has been found.
bool found = false;
// Output file. Put here for debugging reasons.

/* 
   This recursive function figures out whether there is a path from the current
   target (or head) to the current source (or tail) being considered for the addition
   of an edge. If there is this path, then the boolean found is switched to true, 
   which prevents an edge from begin created between the source (tail) and target (head).
*/
void findPaths(int currentSource, int currentTarget, int matsize) {
  // We walk through the columns of the adjacency matrix.
  for (std::vector<int>::size_type k = 0; k != matsize; k++) {   
    if (matrix[currentTarget][k] == 1) { // If we find an edge
      /* 
	 We first check if the edge is the current source (tail).
	 If yes, then we set found to true and we return (no more work to be done).
      */
      if (k == currentSource) { 
	found = true;
	return;
	/* 
	   If no, then we check the other nodes that the last node we found might be
	   connected to 
	*/
      } else {
	int newSource = currentSource;
	int newTarget = k;
	int newsize = matsize;
	findPaths(newSource, newTarget, newsize); // By making the function recursive, we can check longer paths.
      }
    }
  }
}

// The main body of the program
int main (int argc, char *argv[]) {

  std::cout << "Number of nodes?\n";
  std::string input = std::string();
  std::getline(std::cin, input); 
  int n = 0;
  std::stringstream ss(input);
  ss >> n;
    
  std::ofstream fileOut("Output.csv");
  // First let us create an empty 10-by-10 matrix;
  for (int i = 0; i != n; i++) {
    std::vector<int> currentRow;
    for (int j = 0; j != n; j++) {
      currentRow.push_back(0);
    }
    matrix.push_back(currentRow);
  }
  // Then we start filling the matrix, but constrain it to be a DAG.
  for (std::vector<std::vector<int> >::size_type i = 0; i != n; i++) {
    for (std::vector<int>::size_type j = 0; j != n; j++) {
      if (i != j) {
	/* 
	   We also make sure that we only consider an edge with some random chance 
	   This makes sure that the graph is not very dense.
	*/
	int value = rand() % 100;
	if (value < 20) { // If this edge is considered for addition.
	  findPaths(i, j, n); // Recursive function is called here.
	  if (found == true) { // If found = true, we do not add this edge.
	    matrix[i][j] = 0;
	    found = false; // Switch back to false for next iteration.
	  } else { // If it is not true, we add it.
	    matrix[i][j] = 1;
	  }
	}
      }
    }
  }
  
  // Now let us write the results to an adjacency matrix, for testing. 
  fileOut << "Source,Target\n";
  for (std::vector<std::vector<int> >::size_type i = 0; i != n; i++) {
    for (std::vector<int>::size_type j = 0; j != n; j++) {
      if (matrix[i][j] == 1) {
	fileOut << i << "," << j << "\n";
      }
    }
  }
  return 0;
}


