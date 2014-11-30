#include <algorithm>
#include <utility> 
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

class Fsa
{
public:
	std::vector< std::vector< std::string > > matrizAdyacencia; //Matriz para representar el Grafo
	std::vector<int> initialStates; //Lista de estados iniciales
	std::vector<int> finalStates; //Lista de estado finales
	std::string name; //Nombre del archivo, esta vacio hasta que se guarda o se carga
	Fsa()
	{

	}
	/*
		Crea un Fsa con n nodos 
	*/
	Fsa(int nodos)
	{
		matrizAdyacencia.resize(nodos);
		for (int i = 0; i < nodos; ++i)
		{
			matrizAdyacencia[i].resize(nodos);
		}
	}
	/*
		Crea un Fsa con n nodos con un estado inicial y un estado final
	*/
	Fsa(int nodos, int inicial, int final)
	{
		matrizAdyacencia.resize(nodos);
		for (int i = 0; i < nodos; ++i)
		{
			matrizAdyacencia[i].resize(nodos);
		}
		initialStates.push_back(inicial);
		finalStates.push_back(final);
	}

	Fsa(int nodos, std::vector<int> iniciales, std::vector<int> finales)
	{
		matrizAdyacencia.resize(nodos);
		for (int i = 0; i < nodos; ++i)
		{
			matrizAdyacencia[i].resize(nodos);
		}
		initialStates = iniciales;
		finalStates = finales;
	}
	~Fsa()
	{

	}
	/*
		Añade un nodo inicial
	*/
	void AddIni(int nodo)
	{
		initialStates.push_back(nodo);
	}
	/*
		Añade un nodo final
	*/
	void AddFin(int nodo)
	{
		finalStates.push_back(nodo);
	}
	/*
		Añade una transicion del nodo n1 al nodo n2 con el caracter transition
	*/
	void AddTran(int n1, int n2, std::string transition)
	{
		matrizAdyacencia[n1-1][n2-1] = transition;
	}


	/*
	Escribe un archivo con la descripcion del fsa y el nombre dado
	*/
	void GuardarFsa(std::string nombre)
	{
		std::ofstream saveFile;
		name = nombre;
		nombre += ".fsa";
		saveFile.open (nombre.c_str());
		int size = matrizAdyacencia.size();
		int sizeIS = initialStates.size();
		int sizeFS = finalStates.size();
  		saveFile << size << "\n"<< sizeIS << "\n"<< sizeFS << "\n";
  		for (int i = 0; i < sizeIS; ++i)
  		{
  			saveFile << initialStates[i] << "\n";
  			
  		}
  		for (int i = 0; i < sizeFS; ++i)
  		{
  			saveFile << finalStates[i] << "\n";
  			
  		}
  		for (int i = 0; i < size; ++i)
  		{
  			for (int j = 0; j < size; ++j)
  			{
  				if (matrizAdyacencia[i][j] != "")
  				{
  					saveFile << i << " " << j << " " << matrizAdyacencia[i][j]<< "\n";
  				}
  			}
  		}

  		saveFile.close();
	}

	/*
	Lee un archivo previamente guardado
	*/
	void CargarFsa(std::string path)
	{
		std::ifstream loadFile;	
		loadFile.open(path.c_str());
		int size, sizeIS, sizeFS,tmp1, tmp2;
		std::string transition;
		if (loadFile.is_open())
  		{
  			loadFile >> size >> sizeIS >> sizeFS;
  			matrizAdyacencia.resize(size);
			for (int i = 0; i < size; ++i)
			{
				matrizAdyacencia[i].resize(size);
			}
			for (int i = 0; i < sizeIS; ++i)
  			{	
  				loadFile >> tmp1;
  				initialStates.push_back(tmp1);
  			
  			}
  			for (int i = 0; i < sizeFS; ++i)
  			{	
  				loadFile >> tmp1;
  				finalStates.push_back(tmp1);
  			
  			}
  		  	while ( loadFile >> tmp1 )
  		  	{
  		  		loadFile >> tmp2 >> transition;	
	  		    matrizAdyacencia[tmp1][tmp2] = transition;
  			}
  			loadFile.close();
  		}
	}
	/*
		Revisa si la cadena es aceptada por el fsa. Devuelve 1 en caso de que sea aceptada, 0 en los demas casos
	*/
	int ComprobarCadena(std::string entrada)
	{
		int estadoActual = initialStates[0]-1, size = matrizAdyacencia.size(), tmp = 0;	

		for(int i = 0; i < entrada.size(); ++i)
		{	
			for (int j = 0; j < size; ++j)
			{				
				if (matrizAdyacencia[estadoActual][j][0] == entrada[i] )
				{
					estadoActual = j;
					break;
					
				}
			}
		}	
		for (int i = 0; i < finalStates.size(); ++i)
		{
			if (finalStates[i]-1 == estadoActual)
			{
				return 1;
			}
		}
		return 0;
	}


	/*
		Muestra la matriz de adyacencia y los estado iniciales y finales
	*/
	void MostrarFsa()
	{
		std::cout << "FSA\n Estados Iniciales: \n";
		for(auto& a : initialStates)
			std::cout << a << "\n";
		std::cout << "Estados Finales: \n";
		for(auto& a : finalStates)
			std::cout << a << "\n";
		std::cout << "Transiciones\n";
		int size = matrizAdyacencia.size();
		for (int i = 0; i < size; ++i)
  		{
  			for (int j = 0; j < size; ++j)
  			{
  				if (matrizAdyacencia[i][j] != "")
  				{
  					std::cout << "Se va de "<<i+1 << " a " << j+1 << " con " << matrizAdyacencia[i][j]<< "\n";
  				}
  			}
  		}
	}
};

int main(int argc, char const *argv[])
{
	Fsa prueba(5);
	prueba.AddIni(1);
	prueba.AddFin(5);
	prueba.AddTran(1,2,"0");
	prueba.AddTran(2,3,"1");
	prueba.AddTran(4,2,"0");
	prueba.AddTran(3,5,"1");
	prueba.MostrarFsa();
	std::string respuesta;
	std::cout << "Aceptacion de cadenas\n";
	std::cout << "Caso 1\n";
	respuesta = (prueba.ComprobarCadena("011") == 1) ? "Yes\n" : "NO\n";
	std::cout<< respuesta;
	std::cout << "Caso 2\n";
	respuesta = (prueba.ComprobarCadena("010") == 1) ? "Yes\n" : "NO\n";
	std::cout<< respuesta;
	std::cout << "Fin de Aceptacion de cadenas\n";
	prueba.GuardarFsa("pruebasave");
 	prueba.CargarFsa("pruebasave.fsa"); 	
	prueba.MostrarFsa();
	return 0;
}