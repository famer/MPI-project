#include "mpi.h"
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>
#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdlib>

using namespace std;
#define SIZE 8
#define QUEEN 2
#define BITE 1
#define FREE_CELL 0

int board[SIZE][SIZE] = { FREE_CELL };
struct combination {
	int q;
	int board[SIZE][SIZE];
};
list<combination> coms;
list<combination> stack;


bool freeCell(int x, int y, int board[SIZE][SIZE]);
bool boardIsFull(int board[SIZE][SIZE]);
void placeQueens(int board[SIZE][SIZE], int kk, int ii, int q);
void markDirections(int x, int y, int board[SIZE][SIZE]);
void markQueen(int x, int y, int board[SIZE][SIZE]);
void printBoard(int board[SIZE][SIZE]);
int CombineInt(int int1, int int2);
int findSmallestBoard(list<combination> list, int smallestBoard[SIZE][SIZE]);
list<combination> findSmallest(int root[SIZE][SIZE], int nq, bool main);
list<int> workingnodes;
int tag=1;
int flag;
int p;

int main( int argc,      // Number of strings in array argv
		char *argv[],   // Array of command-line argument strings
		char *envp[] )  // Array of environment variable strings
{
	int my_rank;
	int dest;
	int source;
	int length;
	MPI_Status status;

	/* start up MPI */
	MPI_Init( &argc, &argv );

	/* find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	/* find out number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if (my_rank != 0) {
		/* create message */
		dest=0;

		//if ( my_rank == 1 ) {
		while(true) {
			MPI_Iprobe(MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &flag, &status);
			if (flag) {
				printf("flag: %d\n", flag);
				break;
			}
			int root[SIZE][SIZE] = { FREE_CELL };
			int q;
			MPI_Recv(&root, sizeof(int)*SIZE*SIZE, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&q, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

			stack = findSmallest(root, q, false);
			int smallestBoard[SIZE][SIZE] = { FREE_CELL };
			int smallest = findSmallestBoard(stack, smallestBoard);
			MPI_Send (&smallestBoard, sizeof(int)*SIZE*SIZE, MPI_CHAR, 0, 3, MPI_COMM_WORLD);
			MPI_Send (&smallest, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
			

		}

	} else {
		/* my_rank == 0 */
		clock_t tStart = clock();
		printf("There are %d processes.\n", p);
		int root[SIZE][SIZE] = { FREE_CELL };
		int qq = 0;
		if (argc > 1 ) {
			for( int count = 1; count < argc; count+=2 ) {
				qq += 1;
				int x = atoi(argv[count]);
				int y = atoi(argv[count+1]);
				cout << "  x: " << x << " y:  "
					<< y << "\n";
				markDirections(x, y, root);
				markQueen(x, y, root);
			}
		}
		//cout << "pointed\n";
		//printBoard(root);
		//cout << qq << "\n";

		stack = findSmallest(root, qq, true);
		int smallestBoard[SIZE][SIZE] = { FREE_CELL };
		int smallest = SIZE*SIZE;
		int elem[SIZE][SIZE] = { FREE_CELL };
		int q = 0;
		if (workingnodes.size()==0 ) {
			smallest = findSmallestBoard(stack, smallestBoard);
			}
		while(workingnodes.size()!=0) {
			int source = workingnodes.front();
			//printf("source %d\n", source);
			workingnodes.pop_front();
			//int source = 1;
			MPI_Recv(&elem, sizeof(int)*SIZE*SIZE, MPI_CHAR, source, 3, MPI_COMM_WORLD, &status);
			MPI_Recv(&q, 1, MPI_INT, source, 4, MPI_COMM_WORLD, &status);

			if ( q < smallest ) {
				memcpy(smallestBoard, elem, sizeof(int)*SIZE*SIZE);
				smallest = q;

			}
		}
		printf("FINISH !\n");
		int stop = 1;
		for (int source=1; source<p; source++) {
			MPI_Send (&stop, 1, MPI_INT, source, 2, MPI_COMM_WORLD);
		}

		std::cout << "!!!queens: for smallest queen board " << smallest << std::endl;
		printBoard(smallestBoard);
		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


	}


	/* shut down MPI */
	MPI_Finalize();





	return 0;
}


bool freeCell(int x, int y, int  board[SIZE][SIZE]) {
	return board[x][y] == 0;
}

bool boardIsFull(int board[SIZE][SIZE]) {
	for ( int i = 0; i < SIZE; i++ ) {
		for ( int k = 0; k < SIZE; k++ ) {
			if ( board[k][i] == 0 ) {
				return false;
			}
		}
	}
	return true;
}


void markDirections(int x, int y, int board[SIZE][SIZE]) {
	int i = 0;
	int k = 0;
	board[x][y] = BITE;
	for ( int i = 0; i < SIZE; i++ ) {
		board[x][i] = BITE;
	}

	for ( int k = 0; k < SIZE; k++ ) {
		board[k][y] = BITE;
	}

	i = y;
	k = x;
	while ( i < SIZE && k < SIZE ) {
		board[k][i] = BITE;
		i++;
		k++;
	}

	i = y;
	k = x;
	while ( i >= 0 && k >= 0 ) {
		board[k][i] = BITE;
		i--;
		k--;
	}

	i = y;
	k = x;
	while ( i < SIZE && k >= 0 ) {
		board[k][i] = BITE;
		i++;
		k--;
	}

	i = y;
	k = x;
	while ( i >= 0 && k < SIZE ) {
		board[k][i] = BITE;
		i--;
		k++;
	}

}

void markQueen(int x, int y, int board[SIZE][SIZE]) {
	board[x][y] = QUEEN;
}


void printBoard(int board[SIZE][SIZE]) {
	for ( int i = 0; i < SIZE; i++ ) {
		for ( int k = 0; k < SIZE; k++ ) {
			printf ( "%d ", board[k][i] );
		}
		printf("\n");
	}
}

int CombineInt(int int1, int int2)
{
	char cResult[32];
	sprintf(cResult, "%d%d", int1, int2);
	return atoi(cResult);
}
list<combination> findSmallest(int root[SIZE][SIZE], int nq, bool main) {
	list<combination> stack;
	int pp = 0;
	combination com;
	com.q = nq;
	memcpy(com.board, root, sizeof (int) * SIZE * SIZE);
	stack.push_back(com);
	while(stack.size()!=0) {
		int elem[SIZE][SIZE] = { FREE_CELL };
		int q = stack.front().q;
		memcpy(elem, stack.front().board, sizeof (int) * SIZE * SIZE);
		if (boardIsFull(elem)) {
			break;
		}
		//printBoard(elem);
		stack.pop_front();
		for ( int i = 0; i < SIZE; i++ ) {
			for ( int k = 0; k < SIZE; k++ ) {
				if ( freeCell(k, i, elem) ) {
					int leaf[SIZE][SIZE] = { FREE_CELL };
					memcpy(leaf, elem, sizeof (int) * SIZE * SIZE);
					markDirections(k, i, leaf);
					markQueen(k, i, leaf);
					if ( q + 1 >= floor(SIZE/4) && main ) {
						int qq = q + 1;
						int dest = (pp % (p - 1)) + 1;
						workingnodes.push_back(dest);
						pp++;
						//printf("sent to %d processor\n", dest);
						MPI_Send (&leaf, sizeof(int)*SIZE*SIZE, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
						MPI_Send (&qq, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
					} else {
						combination com;
						memcpy(com.board, leaf, sizeof (int) * SIZE * SIZE);
						com.q = q + 1;
						stack.push_back(com);
					}
				}
			}
		}
	}
	return stack;
}
int findSmallestBoard(list<combination> list, int smallestBoard[SIZE][SIZE]) {
	//int smallestBoard[SIZE][SIZE] = { FREE_CELL };
	int smallest = SIZE*SIZE;
	for(std::list<combination>::iterator it = list.begin(); it != list.end(); it++) {
		if ( (it)->q < smallest ) {
			smallest = (it)->q;
			memcpy(smallestBoard, (it)->board, sizeof(int)*SIZE*SIZE);
		}
	}
	//std::cout << "queens: for this board " << smallest << std::endl;
	//printBoard(smallestBoard);
	return smallest;
	
}
