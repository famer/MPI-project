#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <iostream>
#include <time.h>
#include <vector>
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

int main( int argc,      // Number of strings in array argv
		char *argv[],   // Array of command-line argument strings
		char *envp[] )  // Array of environment variable strings
{
	/*int n;

	if (argc > 1 ) {
		n = atoi(argv[1]);
		cout << "\nBoard size: " << n << "\n";
		#define SIZE n

		for( int count = 2; count < argc; count+=2 ) {
			int x = atoi(argv[count]);
			int y = atoi(argv[count+1]);
			cout << "  x: " << x << " y:  "
				<< y << "\n";
		}
	} 
	*/
	int root[SIZE][SIZE] = { FREE_CELL };
	combination com;
	com.q = 0;
	if (argc > 1 ) {
		for( int count = 1; count < argc; count+=2 ) {
			com.q += 1;
			int x = atoi(argv[count]);
			int y = atoi(argv[count+1]);
			cout << "  x: " << x << " y:  "
				<< y << "\n";
			markDirections(x, y, root);
			markQueen(x, y, root);
		}
	}


	memcpy(com.board, root, sizeof (int) * SIZE * SIZE);
	stack.push_back(com);

	clock_t tStart = clock();
	while(stack.size()!=0) {
		int elem[SIZE][SIZE] = { FREE_CELL };
		int q = stack.front().q;
		memcpy(elem, stack.front().board, sizeof (int) * SIZE * SIZE);
		//delete stack.front().q;
		//delete stack.front().board;
		if (boardIsFull(elem)) {
			break;
		}
		//printBoard(elem);
		stack.pop_front();
		for ( int i = 0; i < SIZE; i++ ) {
			for ( int k = 0; k < SIZE; k++ ) {
				if ( freeCell(k, i, elem) ) {
					combination com;
					memcpy(com.board, elem, sizeof (int) * SIZE * SIZE);
					markDirections(k, i, com.board);
					markQueen(k, i, com.board);
					com.q = q + 1;
					stack.push_back(com);
					//printBoard(leaf);
					//cout<< "\n";
					//if number of queens (q) == 2
					//send board to slave
					//and do not add it to stack
				}
			}
		}
	}
	int smallestBoard[SIZE][SIZE] = { FREE_CELL };
	int smallest = SIZE*SIZE;
	for(std::list<combination>::iterator it = stack.begin(); it != stack.end(); it++) {
		if ( (it)->q < smallest ) {

			smallest = (it)->q;
			memcpy(smallestBoard, (it)->board, sizeof(int)*SIZE*SIZE);
		}
	}
	std::cout << "queens: " << smallest << std::endl;
	printBoard(smallestBoard);
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);




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

void placeQueens(int board[SIZE][SIZE], int kk, int ii, int q) {
	if ( boardIsFull(board) ) {
		combination com;
		com.q = q;
		memcpy(com.board, board, sizeof (int) * SIZE * SIZE);
		coms.push_back(com);
		//printBoard(board);
		std::cout << "\n";
		return;
	}
	int num = CombineInt(ii, kk);
	for ( int i = 0; i < SIZE; i++ ) {
		for ( int k = 0; k < SIZE; k++ ) {
			int curNum = CombineInt(i, k);
			if ( freeCell(k, i, board) && (curNum >= num) ) {
				int board1[SIZE][SIZE] = { FREE_CELL };
				memcpy(board1, board, sizeof (int) * SIZE * SIZE);
				int q1 = q;
				markDirections(k, i, board);
				markQueen(k, i, board);
				q++;
				placeQueens(board, k, i, q);
				memcpy(board, board1, sizeof (int) * SIZE * SIZE);
				q = q1;


			}
		}
	}
	q = 0;
	return;

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
