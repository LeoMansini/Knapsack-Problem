# Knapsack Problem

Trabajo Práctico 1 de la materia Algoritmos y Estructuras de Datos III.

Integrantes:
- Leo Mansini
- Camilo Semeria

En este trabajo se implementaron algoritmos para resolver el [Knapsack Problem][1].

Al ser este un problema de Optimización Combinatoria, uno de los métodos es Fuerza Bruta. También se implementó un algoritmo de Backtracking, y uno de Programación Dinámica.

[1]:https://es.wikipedia.org/wiki/Problema_de_la_mochila

El código que resuelve el problema del enunciado está en el archivo subsetsum.cpp.
Para compilarlo se debe ejecutar el comando 

	make

en la consola.

Al ejecutarlo se le debe pasar un parámetro que indique cuál algoritmo se desea utilizar entre las siguientes opciones:

	- FB: Fuerza bruta
	- BT: Backtracking
	- BT-F: Backtracking solamente con poda por factibilidad.
	- BT-O: Backtracking solamente con poda por optimalidad.
	- DP: Programación dinámica.

Además se debe pasar la instancia por entrada estándar. Por ejemplo, para ejecutar backtracking solamente con podas por factibilidad en una instancia INST1.TXT se debe correr el siguiente comando:

	cat INST1.txt | ./subsetsum BT-F
La salida del programa es doble. Por la salida estándar se devuelve lo pedido por el enunciado. Por la STDERR se devuelve el tiempo de ejecución del algoritmo.

La generación de instancias, corrida de experimentos y visualización de resultados está programada en Python utilizando Jupyter (https://jupyter.org/). Para ver los notebooks se debe instalar Jupyter Labs/Notebook siguiendo las instrucciones en su web.

Una vez instalado Jupyter Lab, abrir una consola en la carpeta python y correr el comando:

	jupyter lab

Allí, existen tres archivos:

	- generar-instancias.ipynb: genera las instancias descriptas en el trabajo y las ubica en la carpeta python/instancias.
	- experimentos.ipynb: corre los experimentos del trabajo y deja el resultado de todas las ejecuciones en un CSV en la carpeta python/resultados
	- analysis.ipynb: toma los resultados de las ejecuciones y genera todos los gráficos correspondientes.