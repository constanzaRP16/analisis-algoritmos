#include <vector>

using namespace std;

void merge(vector<int>& vec, int left, int mid, int right);
void mergeSort(vector<int>& vec, int left, int right) {
    if(left < right){
        int mid = left + (right - left) / 2;  //calcular la mitad
        mergeSort(vec, left, mid);            //recursion con la primera mitad
        mergeSort(vec, mid + 1, right);       //recursion con la segunda mitad
        merge(vec, left, mid, right);         
    }
}
//log n porque se a dividiendo por la mitad

void merge(vector<int>& vec, int left, int mid, int right){
    vector<int> izquierda; //memoria adicional n/2
    vector<int> derecha; //memoria adicional n/2

    for(int i = left; i <= mid; i++){  
        izquierda.push_back(vec[i]);  
    }
    
    // n/2
    for(int i = mid + 1; i <= right; i++){
        derecha.push_back(vec[i]);
    }
    // n/2
    

    int i = 0;      //inicio de la izq
    int j = 0;      //incio de la der
    int k = left;

    while(i < izquierda.size() && j < derecha.size()){
        if(izquierda[i] <= derecha[j]){   
            vec[k] = izquierda[i];    //ordena
            i++;
        }
        else{
            vec[k] = derecha[j];
            j++;
        }

        k++;
    }
    

    while(i < izquierda.size()){
        vec[k] = izquierda[i];     //si sobra a la izq
        i++;
        k++;
    }

    while(j < derecha.size()){
        vec[k] = derecha[j];       //si sobra a la der
        j++; 
        k++;
    }
    // entre  los 3 n
}

// log n * (n/2 + n/2 + n) = O(nlog n)