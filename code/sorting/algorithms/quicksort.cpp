#include <vector>
#include <utility>
#include <random>

using namespace std;

void quickSort(vector<int>& vec, int left, int right){
    static mt19937 generador(2026);  //pivote aleatoreo para optimizar mejor el algoritmo.

    while(left < right){
        uniform_int_distribution<int> distribucion(left, right);  //genera una posicion entre la iz y der
        int posicionPivot = distribucion(generador);  //elige la posicion aleatoria entre esa restriccion
        int pivot = vec[posicionPivot];
        int menor = left;
        int actual = left;
        int mayor = right;
    // constante 

        while(actual <= mayor){ //n
            if(vec[actual] < pivot){        //menores que el pivote
                swap(vec[menor], vec[actual]);
                menor++;
                actual++;
            }

            else if(vec[actual] > pivot){    //mayores que el pivote
                swap(vec[actual], vec[mayor]);
                mayor--;
            }

            else{                           //iguales
                actual++;
            }

        }
        // n

        if(menor - left < right - mayor){
            quickSort(vec, left, menor - 1);
            left = mayor + 1;
        }

        
        else{
            quickSort(vec, mayor + 1, right);
            right = menor - 1;
        }

        // n
       
    }

}

//caso promedio O(nlog n)
//peor caso O(n^2)

//En este caso tuve que cambiar esas 3 cosas porque con el QuickSort normal no lograba terminar con los arreglos de 10 millones, 
//sobre todo por como hacia las particiones y por la recursion.

//Pivote pseudoaleatorio para no usar siempre un pivote malo, especialmente en arreglos ya ordenados.
//Partición en 3 vías para manejar mejor los valores repetidos de D1 y no volver a procesarlos una y otra vez.
//Recursión solo en la parte pequeña para evitar demasiadas llamadas recursivas y que el programa se cayera.