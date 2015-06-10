# MPI-project
Project of programming for parallel cluster using MPI library and C++

Problem definition and description of sequential solution
##Problem QUC: queens on chessboard

###Task:

For chessboard S of size n x n, find a placement of the minimal number of queens such that they can attack all chessboard positions and they do not threaten themselves. Queens can attack in all 8 directions, i.e., horizon- tally, vertically, and diagonally. The sequential algorithm is of type SB-DFS with the search depth bounded by n*n. An internal state is defined as a two-dimensional array, which represents the board with placed queens and attacking positions. State considered as a solution if there is no space to place more queens on the board(i.e. all positions in array is marked as taken or attacked). Input data are size of the board and placements of starting queens with attacked positions(i.e. two-dimensional array with marked po- sitions). Output data is a two-dimensional array with placed queens and attacked positions.

