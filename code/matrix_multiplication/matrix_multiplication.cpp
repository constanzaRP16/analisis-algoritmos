#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <thread>
#include <atomic>

using namespace std;

vector<vector<int>> naive(const vector<vector<int>>& A, const vector<vector<int>>& B);
vector<vector<int>> strassen(const vector<vector<int>>& A, const vector<vector<int>>& B);

vector<vector<int>> leerMatriz(string nombre, int n){
    ifstream archivo(nombre);
    vector<vector<int>> matriz(n, vector<int>(n));

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            archivo >> matriz[i][j];
        }
    }
    archivo.close();
    return matriz;
}

void guardarMatriz(string nombre, vector<vector<int>>& matriz){
    ofstream archivo(nombre);

    for(int i = 0; i < matriz.size(); i++){
        for(int j = 0; j < matriz[i].size(); j++){
            archivo << matriz[i][j] << " ";
        }
        archivo << endl;
    }
    archivo.close();
}

auto obtenerMemoria(){
    PROCESS_MEMORY_COUNTERS memoria;
    GetProcessMemoryInfo(GetCurrentProcess(), &memoria, sizeof(memoria));
    return memoria.WorkingSetSize;
}

void medirMemoria(atomic<bool>& ejecutando, long long& memoriaMaxima){
    while(ejecutando){
        auto memoriaActual = obtenerMemoria();
        if(memoriaActual > memoriaMaxima){
            memoriaMaxima = memoriaActual;
        }
        this_thread::sleep_for(chrono::milliseconds(5));
    }
}

int main(){  
    vector<int> tamanos = {16, 64, 256, 1024}; 
    vector<string> tipos = {"dispersa", "diagonal", "densa"}; 
    vector<string> dominios = {"D0", "D10"}; 
    vector<string> muestras = {"a", "b", "c"}; 
    int caso = 0; 
    int total = 72; 
     
    for(int n : tamanos){ 
        for(string tipo : tipos){ 
            for(string dominio : dominios){ 
                for(string muestra : muestras){ 
                    caso++; 

                    string base = to_string(n) + "_" + tipo + "_" + dominio + "_" + muestra; 
                    string archivo1 = "data/matrix_input/" + base + "_1.txt"; 
                    string archivo2 = "data/matrix_input/" + base + "_2.txt"; 
                    string salida = "data/matrix_output/" + base + "_out.txt"; 
 
                    cout << endl; 
                    cout << "Caso " << caso << "/" << total << endl; 
                    cout << "Matriz: " << base << endl; 
 
                    vector<vector<int>> A = leerMatriz(archivo1, n); 
                    vector<vector<int>> B = leerMatriz(archivo2, n); 
                    cout << "Iniciando Naive..." << endl; 
                    auto memoriaBaseNaive = obtenerMemoria(); 
                    long long memoriaMaximaNaive = memoriaBaseNaive; 
                    atomic<bool> ejecutandoNaive(true); 
 
                    thread hiloMemoriaNaive([&](){ 
                        medirMemoria(ejecutandoNaive, memoriaMaximaNaive); 
                    }); 
 
                    auto inicio = chrono::high_resolution_clock::now(); 
                    vector<vector<int>> resultadoNaive = naive(A, B); 
                    auto fin = chrono::high_resolution_clock::now(); 
                    ejecutandoNaive = false; 
                    hiloMemoriaNaive.join(); 
                    auto tiempoNaive = chrono::duration_cast<chrono::microseconds>(fin - inicio).count(); 
                    auto memoriaNaive = memoriaMaximaNaive - memoriaBaseNaive; 
 
                    cout << "Naive terminado" << endl; 
 
 
                    cout << "Iniciando Strassen..." << endl; 
                    auto memoriaBaseStrassen = obtenerMemoria(); 
                    long long memoriaMaximaStrassen = memoriaBaseStrassen; 
                    atomic<bool> ejecutandoStrassen(true); 
 
                    thread hiloMemoriaStrassen([&](){ 
                        medirMemoria(ejecutandoStrassen, memoriaMaximaStrassen); 
                    }); 
 
                    inicio = chrono::high_resolution_clock::now(); 
                    vector<vector<int>> resultadoStrassen = strassen(A, B); 
                    fin = chrono::high_resolution_clock::now(); 
                    ejecutandoStrassen = false; 
                    hiloMemoriaStrassen.join(); 
                    auto tiempoStrassen = chrono::duration_cast<chrono::microseconds>(fin - inicio).count(); 
                    auto memoriaStrassen = memoriaMaximaStrassen - memoriaBaseStrassen; 
 
                    cout << "Strassen terminado" << endl; 
 
     
                    cout << "Naive: " << tiempoNaive << " us" << endl; 
                    cout << "Strassen: " << tiempoStrassen << " us" << endl; 
                    cout << "Memoria Naive: " << memoriaNaive << " bytes" << endl; 
                    cout << "Memoria Strassen: " << memoriaStrassen << " bytes" << endl; 
 
                    if(resultadoNaive == resultadoStrassen){ 
                        cout << "Resultados iguales" << endl; 
                        guardarMatriz(salida, resultadoNaive); 
                    } 
                    else{ 
                        cout << "Resultados distintos" << endl; 
                    } 
 
                    ofstream mediciones("data/measurements/tiempos.txt", ios::app); 
                    mediciones << n << " "<< tipo << " "<< dominio << " "<< muestra << " "<< tiempoNaive << " "<< tiempoStrassen << " "<< memoriaNaive << " "<< memoriaStrassen << endl; 
 
                    mediciones.close(); 
                    cout << "Medicion guardada" << endl; 
                } 
            } 
        } 
    } 
    cout << "Todas las matrices terminadas" << endl; 
    return 0; 
}

//misma explicacion que sorting , solo cambian los algoritmos