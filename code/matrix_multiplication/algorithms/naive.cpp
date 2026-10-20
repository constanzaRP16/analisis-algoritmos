#include <vector>
#include <iostream>

using namespace std;

vector<vector<int>> naive(const vector<vector<int>>& A, const vector<vector<int>>& B,bool progreso){
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n, 0)); //crea una mariz. n^2 memoria adicional

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    // O(n^3)

    return C;
}
