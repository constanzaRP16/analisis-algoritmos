#include <vector>
#include <iostream>
using namespace std;

vector<vector<int>> sumar(const vector<vector<int>>& A, const vector<vector<int>>& B){
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n)); //n^2 memoria adicional

    for(int i = 0; i < n; i++){  
        for(int j = 0; j < n; j++){
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}
//n^2


vector<vector<int>> restar(const vector<vector<int>>& A, const vector<vector<int>>& B){
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n)); //n^2 memoria adicional

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}
//n^2

vector<vector<int>> strassen(const vector<vector<int>>& X, const vector<vector<int>>& Y){
    int n = X.size();
    if(n == 1){
        return {{X[0][0] * Y[0][0]}};
    }

    int mitad = n / 2;
    vector<vector<int>> A(mitad, vector<int>(mitad));
    vector<vector<int>> B(mitad, vector<int>(mitad));
    vector<vector<int>> C(mitad, vector<int>(mitad));
    vector<vector<int>> D(mitad, vector<int>(mitad));
    vector<vector<int>> E(mitad, vector<int>(mitad));
    vector<vector<int>> F(mitad, vector<int>(mitad));
    vector<vector<int>> G(mitad, vector<int>(mitad));
    vector<vector<int>> H(mitad, vector<int>(mitad));
    //matrices de tamño n/2 x n/2 = n^2 de memoria adicional

    for(int i = 0; i < mitad; i++){
        for(int j = 0; j < mitad; j++){
            A[i][j] = X[i][j];
            B[i][j] = X[i][j + mitad];
            C[i][j] = X[i + mitad][j];
            D[i][j] = X[i + mitad][j + mitad];
            E[i][j] = Y[i][j];
            F[i][j] = Y[i][j + mitad];
            G[i][j] = Y[i + mitad][j];
            H[i][j] = Y[i + mitad][j + mitad];
        }
    }
    //n/2 x n/2 = n^2


    auto P1 = strassen(A, restar(F, H));
    auto P2 = strassen(sumar(A, B), H);
    auto P3 = strassen(sumar(C, D),E); 
    auto P4 = strassen(D, restar(G, E));
    auto P5 = strassen(sumar(A, D), sumar(E, H));
    auto P6 = strassen(restar(B, D), sumar(G, H));
    auto P7 = strassen(restar(A, C), sumar(E, F));

    auto C11 = sumar(restar(sumar(P5, P4),P2),P6);
    auto C12 = sumar(P1, P2);
    auto C21 = sumar(P3, P4);
    auto C22 = restar(restar(sumar(P1, P5),P3),P7);
    //7 llamadas recursivas con matrices de tamaño n/2
    // sumas y restas son de tamaño n^2

    vector<vector<int>> resultado(n, vector<int>(n)); //n^2 memoria adicional
    for(int i = 0; i < mitad; i++){
        for(int j = 0; j < mitad; j++){
            resultado[i][j] = C11[i][j];
            resultado[i][j + mitad] = C12[i][j];
            resultado[i + mitad][j] = C21[i][j];
            resultado[i + mitad][j + mitad] = C22[i][j];
        }
    }
    //n/2 x n/2 = n^2

    return resultado;
}
// t(n) = 7T(n/2) + O(n^2) -> teorema maestro de la calse -> log 2 7 = 2.81 > 2 -> O(n^2.81)
//teoricamente se observa que strassen crece un poco mas lento que naive.