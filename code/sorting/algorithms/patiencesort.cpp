#include <vector>
#include <algorithm>
#include <queue>
#include <functional>

using namespace std;

void patienceSort(vector<int>& vec){
    vector<vector<int>> piles; // n memoria adicional
    vector<int> tops;   // p memoria adicional

    for(int x : vec) {     //recorre n elementos
        auto it = lower_bound(tops.begin(), tops.end(), x);   //busqueda binaria, log p
        int index = it - tops.begin();

        if(it == tops.end()){
            piles.push_back({x});
            tops.push_back(x);
        }
        else{
            piles[index].push_back(x);
            tops[index] = x;
        }
    }
    
    // n log n

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> heap; //p memoria adicional

    for(int i = 0; i < piles.size(); i++){  //p
        heap.push({piles[i].back(), i});    //log p
    }
    //peor caso n log n
    

    int k = 0;
    while(!heap.empty()){   //n
        auto actual = heap.top();  //constante
        heap.pop();                //log p
        int valor = actual.first;
        int pileIndex = actual.second;
        vec[k] = valor;
        k++;
        piles[pileIndex].pop_back();  //constante

        if(!piles[pileIndex].empty()){
            heap.push({piles[pileIndex].back(), pileIndex}); // log p
        }
    }
    //n log n
}

//n logn + n logn + n logn = O(n log n)