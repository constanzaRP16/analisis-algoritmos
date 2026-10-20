#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <thread>
#include <atomic>
#include <filesystem>

using namespace std;

void mergeSort(vector<int>& vec, int left, int right);
void quickSort(vector<int>& vec, int left, int right);
void patienceSort(vector<int>& vec);

vector<int> sortArray(vector<int>& arr);
vector<int> leerArchivo(string nombreArchivo, int n){
    vector<int> vec;
    vec.reserve(n);
    ifstream archivo(nombreArchivo);

    if(!archivo.is_open()){
        cout << "No se pudo abrir: " << nombreArchivo << endl;
        return vec;
    }

    int numero;
    while(archivo >> numero){
        vec.push_back(numero); //lee los n numeros

    }

    archivo.close();
    return vec;
}

void guardarArchivo(string nombreArchivo, vector<int>& vec){
    ofstream archivo(nombreArchivo);
    if(!archivo.is_open()){
        cout << "No se pudo guardar: " << nombreArchivo << endl;
        return;
    }

    for(int numero : vec){  
        archivo << numero << " ";

    }
    archivo.close();
}

auto obtenerMemoria(){
    PROCESS_MEMORY_COUNTERS memoria;
    GetProcessMemoryInfo(GetCurrentProcess(), &memoria, sizeof(memoria));
    return memoria.WorkingSetSize;  //devuelve la cantidad de bytes que ocupa la memoria en el proceso
}

void medirMemoria(atomic<bool>& ejecutando, atomic<bool>& comenzar, long long& memoriaMaxima){
    while(!comenzar){
        this_thread::yield();
    }

    while(ejecutando){
        auto memoriaActual = obtenerMemoria();

        if(memoriaActual > memoriaMaxima){  //se queda con la memoria amxima observada
            memoriaMaxima = memoriaActual;
        } 
        this_thread::sleep_for(chrono::milliseconds(5));
    }

}

int main(){
    filesystem::create_directories("data/array_output");
    filesystem::create_directories("data/measurements");
    ofstream mediciones("data/measurements/tiempos.txt");

    if(!mediciones.is_open()){
        cout << "No se pudo crear el archivo de mediciones" << endl;
        return 1;
    }

    vector<int> tamanos = {10, 1000, 100000, 10000000};
    vector<string> tipos = {"ascendente", "descendente", "aleatorio"};
    vector<string> dominios = {"D1", "D7"};
    vector<string> muestras = {"a", "b", "c"};
    int caso = 0;
    int total = 72;

    for(int n : tamanos){   //recorre todas las combinaciones
        for(string tipo : tipos){
            for(string dominio : dominios){
                for(string muestra : muestras){
                    caso++;

                    string base = to_string(n) + "_" + tipo + "_" + dominio + "_" + muestra;
                    string entrada = "data/array_input/" + base + ".txt";
                    string salida = "data/array_output/" + base + "_out.txt";
                    cout << endl;
                    cout << "Caso " << caso << "/" << total << endl;
                    cout << "Arreglo: " << base << endl;
                    vector<int> original = leerArchivo(entrada, n);

                    if(original.size() != n){
                        cout << "Error al leer el archivo" << endl;
                        continue;
                    }
                    bool resultadosIguales = true; 
                    vector<int> vecMerge = original;  //copia
                    cout << "Iniciando MergeSort..." << endl;
                    long long memoriaMaximaMerge = 0;   
                    atomic<bool> ejecutandoMerge(true);
                    atomic<bool> comenzarMerge(false);

                    thread hiloMemoriaMerge([&](){
                        medirMemoria(ejecutandoMerge, comenzarMerge, memoriaMaximaMerge);
                    //medir la memoria mientras mergesort se ejecuta
                    });

                    long long memoriaBaseMerge = obtenerMemoria();  
                    memoriaMaximaMerge = memoriaBaseMerge;
                    comenzarMerge = true;
                    auto inicio = chrono::steady_clock::now();

                    if(!vecMerge.empty()){
                        mergeSort(vecMerge, 0, vecMerge.size() - 1);
                    }

                    auto fin = chrono::steady_clock::now();  //guardar cuanto tardo
                    ejecutandoMerge = false;
                    hiloMemoriaMerge.join();
                    long long tiempoMerge = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                    long long memoriaMerge = memoriaMaximaMerge - memoriaBaseMerge; //calcular cuanto aumento la memoria
                    cout << "MergeSort terminado" << endl;

                    //lo mismo con los demas algoritmos
                    long long tiempoQuick = 0;  
                    long long memoriaQuick = 0;
                    {
                        vector<int> vecQuick = original;
                        cout << "Iniciando QuickSort..." << endl;
                        long long memoriaMaximaQuick = 0;
                        atomic<bool> ejecutandoQuick(true);
                        atomic<bool> comenzarQuick(false);

                        thread hiloMemoriaQuick([&](){
                            medirMemoria(ejecutandoQuick, comenzarQuick, memoriaMaximaQuick);
                        });

                        long long memoriaBaseQuick = obtenerMemoria();
                        memoriaMaximaQuick = memoriaBaseQuick;
                        comenzarQuick = true;
                        inicio = chrono::steady_clock::now();

                        if(!vecQuick.empty()){
                            quickSort(vecQuick, 0, vecQuick.size() - 1);
                        }

                        fin = chrono::steady_clock::now();
                        ejecutandoQuick = false;
                        hiloMemoriaQuick.join();
                        tiempoQuick = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                        memoriaQuick = memoriaMaximaQuick - memoriaBaseQuick;
                        cout << "QuickSort terminado" << endl;
                        if(vecQuick != vecMerge){
                            resultadosIguales = false;
                        }
                    }

                    long long tiempoPatience = 0;
                    long long memoriaPatience = 0;
                    {
                        vector<int> vecPatience = original;
                        cout << "Iniciando PatienceSort..." << endl;
                        long long memoriaMaximaPatience = 0;
                        atomic<bool> ejecutandoPatience(true);
                        atomic<bool> comenzarPatience(false);

                        thread hiloMemoriaPatience([&](){
                            medirMemoria(ejecutandoPatience, comenzarPatience, memoriaMaximaPatience);
                        });

                        long long memoriaBasePatience = obtenerMemoria();
                        memoriaMaximaPatience = memoriaBasePatience;
                        comenzarPatience = true;
                        inicio = chrono::steady_clock::now();
                        patienceSort(vecPatience);
                        fin = chrono::steady_clock::now();
                        ejecutandoPatience = false;
                        hiloMemoriaPatience.join();
                        tiempoPatience = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                        memoriaPatience = memoriaMaximaPatience - memoriaBasePatience;

                        cout << "PatienceSort terminado" << endl;

                        if(vecPatience != vecMerge){
                            resultadosIguales = false;
                        }
                    }

                    long long tiempoSort = 0;
                    long long memoriaSort = 0;
                    {
                        vector<int> vecSort = original;
                        cout << "Iniciando std::sort..." << endl;
                        long long memoriaMaximaSort = 0;
                        atomic<bool> ejecutandoSort(true);
                        atomic<bool> comenzarSort(false);

                        thread hiloMemoriaSort([&](){
                            medirMemoria(ejecutandoSort, comenzarSort, memoriaMaximaSort);
                        });

                        long long memoriaBaseSort = obtenerMemoria();
                        memoriaMaximaSort = memoriaBaseSort;
                        comenzarSort = true;
                        inicio = chrono::steady_clock::now();
                        sortArray(vecSort);
                        fin = chrono::steady_clock::now();
                        ejecutandoSort = false;
                        hiloMemoriaSort.join();
                        tiempoSort = chrono::duration_cast<chrono::microseconds>(fin - inicio).count();
                        memoriaSort = memoriaMaximaSort - memoriaBaseSort;
                        cout << "std::sort terminado" << endl;

                        if(vecSort != vecMerge){
                            resultadosIguales = false;
                        }
                    }

                    cout << "MergeSort: " << tiempoMerge << " us" << endl;
                    cout << "QuickSort: " << tiempoQuick << " us" << endl;
                    cout << "PatienceSort: " << tiempoPatience << " us" << endl;
                    cout << "std::sort: " << tiempoSort << " us" << endl;
                    cout << "Memoria MergeSort: " << memoriaMerge << " bytes" << endl;
                    cout << "Memoria QuickSort: " << memoriaQuick << " bytes" << endl;
                    cout << "Memoria PatienceSort: " << memoriaPatience << " bytes" << endl;
                    cout << "Memoria std::sort: " << memoriaSort << " bytes" << endl;


                    if(resultadosIguales){
                        cout << "Resultados iguales" << endl; //verificar que terminaron con el mismo arregko
                        guardarArchivo(salida, vecMerge);
                    }
                    else{
                        cout << "Resultados distintos" << endl;
                    }

                    mediciones << n << " "  << tipo << " "  << dominio << " "  << muestra << " "  << tiempoMerge << " "
                                << tiempoQuick << " " << tiempoPatience << " " << tiempoSort << " "  << memoriaMerge << " "
                                << memoriaQuick << " " << memoriaPatience << " " << memoriaSort << endl;
                    cout << "Medicion guardada" << endl;
                }
            }
        }
    }
    mediciones.close();
    cout << "Todos los arreglos terminados" << endl;
    return 0;
}

//Para implementar la medicion de tiempo y memoria busque y revise documentacion de internet sobre su forma de implementacion.
//Tambien di uso de apoyo de inteligencia artificial, ya que no tenia experiencia previa implementando este tipo de mediciones y me vi complicada al intentarlo.
//Aun asi, dejo en claro que intente ser consciente de lo que se estaba implementando y entender el funcionamiento de cada parte para mejorarlo en una proxima tarea.