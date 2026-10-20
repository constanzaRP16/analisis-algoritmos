import os
import pandas as pd
import matplotlib.pyplot as plt

datos = pd.read_csv("../data/measurements/tiempos.txt", sep=r"\s+")

os.makedirs("../data/plots", exist_ok=True)
tipos = ["ascendente", "descendente", "aleatorio"]
dominios = ["D1", "D7"]

for tipo in tipos:
    for dominio in dominios:
        filtrados = datos[(datos["tipo"] == tipo) & (datos["dominio"] == dominio)] # solo me quedo con el tipo y dominio
        promedios = filtrados.groupby("n").mean(numeric_only=True) #promedio de a b y c de tiempo y memoria para cada tamaño n
        
        plt.figure()
        plt.plot(promedios.index, promedios["tiempoMerge"], marker="o", label="MergeSort")
        plt.plot(promedios.index, promedios["tiempoQuick"], marker="o", label="QuickSort")
        plt.plot(promedios.index, promedios["tiempoPatience"], marker="o", label="PatienceSort")
        plt.plot(promedios.index, promedios["tiempoSort"], marker="o", label="std::sort")
        plt.title("Tiempo " + tipo + " " + dominio)
        plt.xlabel("Tamaño")
        plt.ylabel("Tiempo (us)")
        plt.xscale("log")
        plt.legend()
        plt.grid()
        plt.savefig("../data/plots/tiempo_" + tipo + "_" + dominio + ".png")

        plt.close()

        plt.figure()
        plt.plot(promedios.index, promedios["memoriaMerge"], marker="o", label="MergeSort")
        plt.plot(promedios.index, promedios["memoriaQuick"], marker="o", label="QuickSort")
        plt.plot(promedios.index, promedios["memoriaPatience"], marker="o", label="PatienceSort")
        plt.plot(promedios.index, promedios["memoriaSort"], marker="o", label="std::sort")
        plt.title("Memoria " + tipo + " " + dominio)
        plt.xlabel("Tamaño")
        plt.ylabel("Memoria (bytes)")
        plt.xscale("log")
        plt.legend()
        plt.grid()
        plt.savefig("../data/plots/memoria_" + tipo + "_" + dominio + ".png")

        plt.close()


print("Graficos generados")