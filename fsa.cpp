#include <algorithm>
#include <utility> 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Node
{
public:
	int initialState;
	int finalState;
	std::vector< std::pair< int,std::string >  > conections;
	Node(int isInitial, int isFinal)
	{
		initialState = isInitial;
		finalState = isFinal;
	}
	~Node()
	{}

	/* data */
	void AddConnection(int state, std::string transition)
	{
		std::pair< int,std::string > newcon;
		newcon = std::make_pair(state,transition);
		conections.push_back(newcon);
	}
};


class Fsa
{
public:
	std::vector< std::vector< std::string > > matrizAdyacencia;
	std::vector<Node> states;
	Fsa(int vertices)
	{
		matrizAdyacencia.resize(vertices);
		//states.resize(states);
		for (int i = 0; i < vertices; ++i)
		{
			matrizAdyacencia[i].resize(vertices);
		}
	}
	~Fsa()
	{

	}

	/* data */

	/*
	Escribe un archivo con la descripcion del fsa y el nombre dado
	*/
	void GuardarFsa(std::string nombre)
	{
		std::ofstream saveFile;
		nombre += ".fsa";
		saveFile.open (nombre.c_str());
		int size = matrizAdyacencia.size();
  		saveFile << size << "\n";
  		for (int i = 0; i < states.size(); ++i)
  		{
  			if (states[i].initialState == 1)
  			{
  				saveFile << states[i].initialState << "\n";
  			}
  			if (states[i].finalState == 1)
  			{
  				saveFile << states[i].finalState << "\n";
  			}
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
		int size,tmp1, tmp2,initial, final;
		std::string transition;
		if (loadFile.is_open())
  		{
  			loadFile >> size >> initial >> final;
  			matrizAdyacencia.resize(size);
			for (int i = 0; i < size; ++i)
			{
				matrizAdyacencia[i].resize(size);
			}
  			std::cout << size << " " << initial << " "<< final << '\n';
  		  	while ( loadFile >> tmp1 )
  		  	{
  		  		loadFile >> tmp2 >> transition;	
	  		    std::cout << tmp1 << " " << tmp2 << " "<< transition<< '\n';
  			}
  			loadFile.close();
  		}
	}
};

int main(int argc, char const *argv[])
{
	Fsa prueba(5);
 	prueba.CargarFsa("pruebasave.fsa"); 	
	return 0;
}