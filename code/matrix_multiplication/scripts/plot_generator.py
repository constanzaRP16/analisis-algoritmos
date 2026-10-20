import os
import pandas as pd
import matplotlib.pyplot as plt

datos = pd.read_csv("../data/measurements/tiempos.txt", sep=r"\s+")

os.makedirs("../data/plots", exist_ok=True)
tipos = ["dispersa", "diagonal", "densa"]
dominios = ["D0", "D10"]

for tipo in tipos:
    for dominio in dominios:
        filtrados = datos[(datos["tipo"] == tipo) & (datos["dominio"] == dominio)]
        promedios = filtrados.groupby("n").mean(numeric_only=True)

        plt.figure()
        plt.plot(promedios.index, promedios["tiempoNaive"], marker="o", label="Naive")
        plt.plot(promedios.index, promedios["tiempoStrassen"], marker="o", label="Strassen")
        plt.title("Tiempo " + tipo + " " + dominio)
        plt.xlabel("Tamaño")
        plt.ylabel("Tiempo (us)")
        plt.legend()
        plt.grid()
        plt.savefig("../data/plots/tiempo_" + tipo + "_" + dominio + ".png")

        plt.close()

        plt.figure()
        plt.plot(promedios.index, promedios["memoriaNaive"], marker="o", label="Naive")
        plt.plot(promedios.index, promedios["memoriaStrassen"], marker="o", label="Strassen")
        plt.title("Memoria " + tipo + " " + dominio)
        plt.xlabel("Tamaño")
        plt.ylabel("Memoria (bytes)")
        plt.legend()
        plt.grid()
        plt.savefig("../data/plots/memoria_" + tipo + "_" + dominio + ".png")

        plt.close()

print("Graficos generados")
#misma logica que con el plot de sorting