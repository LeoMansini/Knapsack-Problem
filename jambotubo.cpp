#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <set>
#include <map>

using namespace std;

int INFTY = -10e6; // Valor para indicar que no hubo solución.

// Información de la instancia a resolver.
int n, r;
vector<int> vectorPeso, vectorResistencia;



// i: posicion del elemento a considerar en este nodo.
// w: suma de los elementos seleccionados hasta este nodo.
// k: cantidad de elementos seleccionados hasta este nodo.
int FB(int r, int nroElemento)
{
    int res;
    if(nroElemento == n-1){//casos base
        if(r<0) res = INFTY;
        else if(vectorPeso[nroElemento] > r) res = 0;
        else res = 1;
    }else{//casos recursivos
        res = max(1+FB(min(r-vectorPeso[nroElemento], vectorResistencia[nroElemento]), nroElemento+1), FB(r, nroElemento+1));
    }
    return res;
}

// i: posicion del elemento a considerar en este nodo.
// k: cantidad de elementos seleccionados hasta este nodo.
// w: suma de los elementos seleccionados hasta este nodo.
bool poda_factibilidad = true; // define si la poda por factibilidad esta habilitada.
bool poda_optimalidad = true; // define si la poda por optimalidad esta habilitada.
int K = INFTY; // Mejor solucion hasta el momento.
int mejorResultado = 0;

int BT(int r, int nroElemento, int rtaActual)
{
    //Poda factibilidad: resistencia quedó negativa
    //Poda optimalidad:
    //mejorResultado > rtaActual+elementos restantes
    //mejorResultado > rtaActual+resistenciaActual
    int elementosRestantes = n-nroElemento-1;
    int res;
    // Poda por factibilidad.
    if (poda_factibilidad && r==0) return  0;//Podo ambas ramas porque nadie más se puede agregar

    if(nroElemento == n-1){//casos base
        if(r<0) res = INFTY;
        else if(vectorPeso[nroElemento] > r) res = 0;
        else {
            res = 1;
            rtaActual ++;
        }
    }else{//casos recursivos
        if(poda_factibilidad && vectorPeso[nroElemento] > r) res = BT(r, nroElemento+1, rtaActual);//Podo la rama "me agrego" porque sé que no es una respuesta válida
        else{
            int sinAgregar;
            int agregando;
            int resistenciaAgregandome = min(r-vectorPeso[nroElemento], vectorResistencia[nroElemento]);
            //PODA OPTIMALIDAD POR RESISTENCIA
            //Si al agregar un elemento la resistencia actual del sistema queda tan baja que incluso si todos los
            // elementos restantes pesan 1, no se pueden agregar suficientes para superar el mejor resultado actual
            // podamos
            if(poda_optimalidad && resistenciaAgregandome >= 0 && (rtaActual+1 + resistenciaAgregandome <= mejorResultado)){
                agregando = 0;
            }else {
                agregando = 1+BT(resistenciaAgregandome, nroElemento + 1,
                               rtaActual + 1);
            }
            //PODA OPTIMALIDAD POR ELEMENTOS RESTANTES
            //Si omitir el elemento actual causa que incluso agregando todos los restantes no alcanza para superar
            // el mejor resultado actual, podamos
            if(poda_optimalidad && mejorResultado >(rtaActual + elementosRestantes - 1))
                sinAgregar = 0;
            else sinAgregar = BT(r, nroElemento+1, rtaActual);
            res = max(agregando, sinAgregar);
        }
    }
    if(res >= 0 && rtaActual > mejorResultado){
        mejorResultado = rtaActual;
    }
    return res;
}

vector<vector<int>> M; // Memoria de PD.
const int UNDEFINED = -1;
// PD(i, w): minimo cardinal de un subconjunto de {Si, ... , Sn} que sume W−w.
int PD(int r, int nroElemento)
{
    int res;
    if(nroElemento == n-1){//casos base
        if(vectorPeso[nroElemento] > r) res = 0;
        else res = 1;
    }else{//casos recursivos
        int sinAgregar;
        int agregando;
        //Si el valor para el llamado recursivo sin agregar está precalculado, listo, sino, llamo
        if (M[r][nroElemento+1] == UNDEFINED) sinAgregar = PD(r, nroElemento+1);
        else sinAgregar = M[r][nroElemento+1];
        //si la resistencia agregando el actual elemento genera aplastamiento, descarto la opción
        //caso contrario si está precalculado el llamado lo evito y si no, lo hago
        int resistenciaAgregandome = min(r-vectorPeso[nroElemento], vectorResistencia[nroElemento]);
        if(resistenciaAgregandome > -1) {
            if (M[resistenciaAgregandome][nroElemento + 1] == UNDEFINED) agregando = 1 + PD(resistenciaAgregandome,
                                                                                            nroElemento + 1);
            else agregando = 1 + M[resistenciaAgregandome][nroElemento + 1];
        }else agregando = 0;
        res = max(agregando, sinAgregar);
    }
    M[r][nroElemento] = res;
    return res;
}

// Recibe por parámetro qué algoritmos utilizar para la ejecución separados por espacios.
// Imprime por clog la información de ejecución de los algoritmos.
// Imprime por cout el resultado de algun algoritmo ejecutado.
int main(int argc, char** argv)
{
	// Leemos el parametro que indica el algoritmo a ejecutar.
	map<string, string> algoritmos_implementados = {
		{"FB", "Fuerza Bruta"}, {"BT", "Backtracking con podas"}, {"BT-F", "Backtracking con poda por factibilidad"}, 
		{"BT-O", "Backtracking con poda por optimalidad"}, {"DP", "Programacion dinámica"}
	};

	// Verificar que el algoritmo pedido exista.
	if (argc < 2 || algoritmos_implementados.find(argv[1]) == algoritmos_implementados.end())
	{
		cerr << "Algoritmo no encontrado: " << argv[1] << endl;
		cerr << "Los algoritmos existentes son: " << endl;
		for (auto& alg_desc: algoritmos_implementados) cerr << "\t- " << alg_desc.first << ": " << alg_desc.second << endl;
		return 0;
	}
	string algoritmo = argv[1];

    // Leemos el input.
    cin >> n >> r;
    vectorPeso.assign(n, 0);
    vectorResistencia.assign(n, 0);
    for (int i = 0; i < n; ++i) cin >> vectorPeso[i] >> vectorResistencia[i];


    // Ejecutamos el algoritmo y obtenemos su tiempo de ejecución.
	int optimum;
	optimum = 0;
	auto start = chrono::steady_clock::now();

	if (algoritmo == "FB")
	{
		optimum = FB(r, 0);
	}
	else if (algoritmo == "BT")
	{
		K = INFTY;
		poda_optimalidad = poda_factibilidad = true;
		optimum = BT(r, 0, 0);
	}
	else if (algoritmo == "BT-F")
	{
		K = INFTY;
		poda_optimalidad = false;
		poda_factibilidad = true;
		optimum = BT(r, 0, 0);
	}
	else if (algoritmo == "BT-O")
	{
		K = INFTY;
		poda_optimalidad = true;
		poda_factibilidad = false;
		optimum = BT(r, 0, 0);
	}
	else if (algoritmo == "DP")
	{
		// Reservamos la estructura auxiliar de memoria dinámica
		M = vector<vector<int>>(r+1, vector<int>(n, UNDEFINED));
		// Obtenemos la solucion optima.
		optimum = PD(r, 0);
	}

	auto end = chrono::steady_clock::now();
	double total_time = chrono::duration<double, milli>(end - start).count();

	// Imprimimos el tiempo de ejecución por stderr.
	clog << total_time << endl;

    // Imprimimos el resultado por stdout.
    cout << (optimum == INFTY ? -1 : optimum) << endl;
    return 0;
}