#include <algorithm>
#include <utility> 
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

const std::string epsilon = ":";

class Fsa
{
public:
	std::vector< std::vector< std::string > > matrizAdyacencia; // Matriz para representar el Grafo
	std::string lenguaje; // Cadena que contiene todos los simbolos del lenguaje
	std::vector<int> initialStates; // Lista de estados iniciales
	std::vector<int> finalStates; // Lista de estado finales
	std::string name; // Nombre del archivo, esta vacio hasta que se guarda o se carga
	int tipo; // Tipo de fsa, 0 - no-determinista, 1 - determinista.

	Fsa()
		: lenguaje(""), tipo(1), name("") 
	{}

	/*
		Copy constructor
	*/
	Fsa(const Fsa& x)
        : lenguaje(x.lenguaje), initialStates(x.initialStates), finalStates(x.finalStates), name(x.name), tipo(x.tipo)
    {
    	int size = x.matrizAdyacencia.size();
    	matrizAdyacencia.resize(size);
    	for (int i = 0; i < size; ++i)
    	{
    		matrizAdyacencia[i].resize(size);
    		for (int j = 0; j < size; ++j)
    		{
    			matrizAdyacencia[i][j] = x.matrizAdyacencia[i][j];
    		}
    	}
    }

	/*
		Crea un Fsa con n nodos 
	*/
	Fsa(int nodos)
		: lenguaje(""), tipo(1), name("") 
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
		: lenguaje(""), tipo(1), name("") 
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
		: lenguaje(""), tipo(1), name("") 
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
		matrizAdyacencia[n1-1][n2-1] = matrizAdyacencia[n1-1][n2-1] + transition;	
		// Agregamos el caracter al lenguaje
		if(lenguaje.find(transition) == std::string::npos)
			lenguaje += transition;
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
		int sizeAU = matrizAdyacencia.size();
		int sizeIS = initialStates.size();
		int sizeFS = finalStates.size();
  		saveFile << sizeAU << "\n"<< sizeIS << "\n"<< sizeFS << "\n";
  		for (int i = 0; i < sizeIS; ++i)
  		{
  			saveFile << initialStates[i] << "\n";
  		}

  		for (int i = 0; i < sizeFS; ++i)
  		{
  			saveFile << finalStates[i] << "\n";	
  		}

  		for (int i = 0; i < sizeAU; ++i)
  		{
  			for (int j = 0; j < sizeAU; ++j)
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
		int sizeAU, sizeIS, sizeFS,tmp1, tmp2;
		std::string transition;
		if (loadFile.is_open())
  		{
  			loadFile >> sizeAU >> sizeIS >> sizeFS;
  			matrizAdyacencia.clear();
  			matrizAdyacencia.resize(sizeAU);
			for (int i = 0; i < sizeAU; ++i)
			{
				matrizAdyacencia[i].resize(sizeAU);
			}
			initialStates.clear();
			for (int i = 0; i < sizeIS; ++i)
  			{	
  				loadFile >> tmp1;
  				initialStates.push_back(tmp1);
  			
  			}
  			finalStates.clear();
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
	int ComprobarCadena(std::string entrada) const
	{
		int estadoActual = initialStates[0]-1, size = matrizAdyacencia.size(), tmp = 0;	

		for(int i = 0; i < entrada.size(); ++i)
		{	
			for (int j = 0; j < size; ++j)
			{				
				if (matrizAdyacencia[estadoActual][j] != "" &&  matrizAdyacencia[estadoActual][j].find(entrada[i]) != std::string::npos )
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
	void MostrarFsa() const
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

	/*
		Retorna 1 si el FSA es determinista, 0 de lo contrario.
	*/
	int EsDeterminista() const
	{
		int size = matrizAdyacencia.size();
		// Descartamos el caso trivial
		if (initialStates.size() != 1) return 0;
		//Revisamos las transiciones del automata
		for (int j = 0; j < size; ++j)
		{
			std::string tmp = "";
			for (int k = 0; k < size; k++)
			{	
				if(matrizAdyacencia[j][k] == epsilon) return 0;
				if (matrizAdyacencia[j][k] != "")
				{
					for (int i = 0; i < matrizAdyacencia[j][k].size(); ++i)
					{
						if ( tmp.find(matrizAdyacencia[j][k][i]) != std::string::npos)
							return 0;
						else
							tmp += matrizAdyacencia[j][k][i];
					}
				}
			}
		}
		return 1;
	}
};

/*
	Crea un FSA determinista a partir un FSA no determinista
*/
Fsa DeterminarFsa (const Fsa& afsa)
{
	int aestados = afsa.matrizAdyacencia.size(),
		nestados = aestados * aestados,
		aini = afsa.initialStates.size(),
		afin = afsa.finalStates.size();

	Fsa respuesta(nestados);

	for (int i = 0; i < aestados; ++i)
	{
		for (int j = 0; j < aestados; ++j)
		{
			respuesta.AddTran(i+2, j+2, afsa.matrizAdyacencia[i][j]);
		}
	}

	respuesta.AddIni(1);

	for (int i = 0; i < aini; ++i)
	{
		respuesta.AddIni(afsa.initialStates[i]+1);
		respuesta.AddTran(1,afsa.initialStates[i]+1,":");
	}

	for (int i = 0; i < afin; ++i)
	{
		respuesta.AddFin(afsa.finalStates[i]+1);
	}

	respuesta.name += "Deter";

	return respuesta;
}

/*
	Crea un FSA a partir de la union de dos FSA
*/
Fsa UnirFsa (const Fsa& afsa, const Fsa& bfsa)
{
	int aestados = afsa.matrizAdyacencia.size(),
		bestados = bfsa.matrizAdyacencia.size(),
		nestados = aestados + bestados + 1,
		aini = afsa.initialStates.size(),
		afin = afsa.finalStates.size(),
		bini = bfsa.initialStates.size(),
		bfin = bfsa.finalStates.size();

	Fsa respuesta(nestados);

	for (int i = 0; i < aestados; ++i)
	{
		for (int j = 0; j < aestados; ++j)
		{
			respuesta.AddTran(i+2, j+2, afsa.matrizAdyacencia[i][j]);
		}
	}
	for (int i = 0; i < bestados; ++i)
	{
		for (int j = 0; j < bestados; ++j)
		{
			respuesta.AddTran(i+aestados+2, j+aestados+2, bfsa.matrizAdyacencia[i][j]);
		}
	}

	respuesta.AddIni(1);

	for (int i = 0; i < aini; ++i)
	{
		respuesta.AddIni(afsa.initialStates[i]+1);
		respuesta.AddTran(1,afsa.initialStates[i]+1,":");
	}
	for (int i = 0; i < bini; ++i)
	{
		respuesta.AddIni(bfsa.initialStates[i]+aestados+1);
		respuesta.AddTran(1,bfsa.initialStates[i]+aestados+1,":");
	}

	for (int i = 0; i < afin; ++i)
	{
		respuesta.AddFin(afsa.finalStates[i]+1);
	}
	for (int i = 0; i < bfin; ++i)
	{
		respuesta.AddFin(bfsa.finalStates[i]+aestados+1);
	}

	respuesta.name = afsa.name + bfsa.name;
	respuesta.tipo = 0;

	return respuesta;
}

/*
	Crea un FSA a partir del complemento de otro FSA
*/
Fsa CompFsa (const Fsa& afsa)
{
	int aestados = afsa.matrizAdyacencia.size(),
		nestados = aestados + 1,
		aini = afsa.initialStates.size(),
		afin = afsa.finalStates.size();

	Fsa respuesta(nestados);

	for (int i = 0; i < aestados; ++i)
	{
		for (int j = 0; j < aestados; ++j)
		{
			respuesta.AddTran(i+2, j+2, afsa.matrizAdyacencia[i][j]);
		}
	}

	respuesta.AddIni(1);

	for (int i = 0; i < aini; ++i)
	{
		respuesta.AddIni(afsa.initialStates[i]+1);
		respuesta.AddTran(1,afsa.initialStates[i]+1,":");
	}

	for (int i = 0; i < afin; ++i)
	{
		respuesta.AddFin(afsa.finalStates[i]+1);
	}

	respuesta.name += "Comp";

	return respuesta;
}

/*
	Crea un FSA a partir de la minimizacion de otro FSA
*/
Fsa MinFsa (const Fsa& afsa)
{
	int aestados = afsa.matrizAdyacencia.size(),
		nestados = aestados + 1,
		aini = afsa.initialStates.size(),
		afin = afsa.finalStates.size();

	Fsa respuesta(nestados);

	for (int i = 0; i < aestados; ++i)
	{
		for (int j = 0; j < aestados; ++j)
		{
			respuesta.AddTran(i+2, j+2, afsa.matrizAdyacencia[i][j]);
		}
	}

	respuesta.AddIni(1);

	for (int i = 0; i < aini; ++i)
	{
		respuesta.AddIni(afsa.initialStates[i]+1);
		respuesta.AddTran(1,afsa.initialStates[i]+1,":");
	}

	for (int i = 0; i < afin; ++i)
	{
		respuesta.AddFin(afsa.finalStates[i]+1);
	}

	respuesta.name += "Min";

	return respuesta;
}

/*
	Crea un FSA a partir de la entrada por consola del usuario
*/
Fsa CrearCLI()
{
	int nestados, x, y;
	std::string entrada, tipoAutomata;

	std::cout << "Ingrese el numero total de estados: ";
	std::cin >> nestados;

	Fsa automata(nestados);


	std::cout << "Ingrese las transiciones(estado estado caracter), : es epsilon' y 0 para terminar" << std::endl;
	std::cout << "Los estados se enumeran desde 1 hasta total de estados." << std::endl;
	while(std::cin >> x && x != 0)
	{
		std::cin >> y >> entrada;
		automata.AddTran(x, y, entrada);
	}

	std::cout << "Ingrese el numero de estados iniciales: ";
	std::cin >> nestados;

	for (int i = 0; i < nestados; ++i)
	{
		std::cout << "Ingrese un estado: ";
		std::cin >> x;
		automata.AddIni(x);
	}

	std::cout << "Ingrese el numero de estados finales: ";
	std::cin >> nestados;

	for (int i = 0; i < nestados; ++i)
	{
		std::cout << "Ingrese un estado: ";
		std::cin >> x;
		automata.AddFin(x);
	}

	std::cout << "Determinando tipo de automata..." << std::endl;

	automata.tipo = automata.EsDeterminista();

	if(automata.tipo == 1)
		tipoAutomata = "determinista";
	else
		tipoAutomata = "no determinista";

	std::cout << "Automata " << tipoAutomata << " creado" << std::endl;

	std::cout << "Agregar nombre: ";
	std::cin >> automata.name;

	return automata;
}

int main(int argc, char const *argv[])
{
/*	Fsa prueba(5), prueba2;

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
	respuesta = (prueba.ComprobarCadena("011") == 1) ? "Yes\n" : "No\n";
	std::cout<< respuesta;

	std::cout << "Caso 2\n";
	respuesta = (prueba.ComprobarCadena("010") == 1) ? "Yes\n" : "NO\n";
	std::cout<< respuesta;
	std::cout << "Fin de Aceptacion de cadenas\n";

	prueba.GuardarFsa("pruebasave");
 	prueba.CargarFsa("pruebasave.fsa"); 	
	prueba.MostrarFsa();

	prueba2 = CrearCLI();
	std::string test_entrada;

	std::cout << "Inrgese una cadena a probar: ";
	std::cin >> test_entrada;
	std::cout << ((prueba.ComprobarCadena(test_entrada) == 1) ? "Yes" : "No") << std::endl;
*/

	int entrada = 10;
	std::vector<Fsa> automatas;

	while(true)
	{
		std::cout << "Que desea hacer:\n";
		std::cout << "1 Crear un automata\n";
		std::cout << "2 Unir dos automatas\n";
		std::cout << "3 Complemento de un automata\n";
		std::cout << "4 Probar una cadena con un automata\n";
		std::cout << "5 Comparar dos automatas\n";
		std::cout << "6 Transformar en expresion regular\n";
		std::cout << "7 Transformar NFSA en DFSA\n";
		std::cout << "8 Minimizar un DFSA\n";
		std::cout << "9 Ver un FSA\n";
		std::cout << "0 Salir\n";
		std::cout << "Ingresar opcion: ";
		std::cin >> entrada;

		if (entrada == 0) break;
		if (entrada == 1)
		{
			Fsa nuevaFSA = CrearCLI();
			automatas.push_back(nuevaFSA);
		}
		if(automatas.size() == 0)
			std::cout << "No hay automatas creados" << std::endl;
		else
		{
			std::cout << "Automatas:\n";
			for (int i = 0; i < automatas.size(); ++i)
			{
				std::cout << i+1 << " " << automatas[i].name << std::endl;
			}
		}

		if (entrada == 2)
		{
			int in1, in2;
			std::cout << "Ingrese el primer automata: ";
			std::cin >> in1;
			std::cout << "Ingrese el segundo automata: ";
			std::cin >> in2;

			if(in1 == in2)
				std::cout << "Automatas iguales!" << std::endl;
			else if(in1 < 1 || in2 < 1 || in1 > automatas.size() || in2 > automatas.size())
				std::cout << "No existen los automatas!" << std::endl;
			else
			{
				Fsa nuevaFSA = UnirFsa(automatas[in1 - 1], automatas[in2 - 1]);
				automatas.push_back(nuevaFSA);
				std::cout << "Automata " << nuevaFSA.name << " creado" << std::endl;
			}
		}
		else if(entrada == 3)
		{
			int in1;
			std::cout << "Ingrese el automata: ";
			std::cin >> in1;

			if(in1 < 1 || in1 > automatas.size())
				std::cout << "No existe el automata!" << std::endl;
			else
			{
				Fsa nuevaFSA = CompFsa(automatas[in1 - 1]);
				automatas.push_back(nuevaFSA);
				std::cout << "Automata " << nuevaFSA.name << " creado" << std::endl;
				/* Implementar CompFsa */
			}
		}
		else if(entrada == 4)
		{
			int in1;
			std::string entrada_texto;
			std::cout << "Ingrese el automata: ";
			std::cin >> in1;

			if(in1 < 1 || in1 > automatas.size())
				std::cout << "No existe el automata!" << std::endl;
			else
			{
				std::cout << "Ingrese el texto: ";
				std::cin >> entrada_texto;
				std::cout << ((automatas[in1-1].ComprobarCadena(entrada_texto) == 1)? "Yes":"No") << std::endl;
				/* Implementar para NFSA */
			}
		}
		else if(entrada == 5)
		{
			int in1, in2;
			std::cout << "Ingrese el primer automata: ";
			std::cin >> in1;
			std::cout << "Ingrese el segundo automata: ";
			std::cin >> in2;

			if(in1 == in2)
				std::cout << "Automatas iguales!" << std::endl;
			else if(in1 < 1 || in2 < 1 || in1 > automatas.size() || in2 > automatas.size())
				std::cout << "No existen los automatas!" << std::endl;
			else
			{
				/* Terminar */
			}
		}
		else if(entrada == 6)
		{
			int in1;
			std::cout << "Ingrese el automata: ";
			std::cin >> in1;

			if(in1 < 1 || in1 > automatas.size())
				std::cout << "No existe el automata!" << std::endl;
			else
			{
				/* Implementar Regex */
			}
		}
		else if(entrada == 7)
		{
			int in1;
			std::cout << "Ingrese el automata: ";
			std::cin >> in1;

			if(in1 < 1 || in1 > automatas.size())
				std::cout << "No existe el automata!" << std::endl;
			else
			{
				if (automatas[in1 - 1].tipo == 1)
					std::cout << "Automata determinista!" << std::endl;
				else
				{
					Fsa nuevaFSA = DeterminarFsa(automatas[in1 - 1]);
					automatas.push_back(nuevaFSA);
					std::cout << "Automata " << nuevaFSA.name << " creado" << std::endl;
					/* Implementar DeterminarFsa */
				}
			}
		}
		else if(entrada == 8)
		{
			int in1;
			std::cout << "Ingrese el automata: ";
			std::cin >> in1;

			if(in1 < 1 || in1 > automatas.size())
				std::cout << "No existe el automata!" << std::endl;
			else
			{
				if (automatas[in1 - 1].tipo == 0)
					std::cout << "Automata no-determinista!" << std::endl;
				else
				{
					Fsa nuevaFSA = MinFsa(automatas[in1 - 1]);
					automatas.push_back(nuevaFSA);
					std::cout << "Automata " << nuevaFSA.name << " creado" << std::endl;
					/* Implementar MinFsa */
				}
			}
		}
		else if(entrada == 9)
		{
			int in1;
			std::cout << "Ingrese el automata: ";
			std::cin >> in1;

			if(in1 < 1 || in1 > automatas.size())
				std::cout << "No existe el automata!" << std::endl;
			else
				automatas[in1 - 1].MostrarFsa();
		}

	}

	return 0;
}