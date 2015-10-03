#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <R.h>
#include <Rmath.h>
#include "FahrigLandscape.cpp"
#include "FahrigIndividual.cpp" 
#include <sstream>
#include <string>

using namespace std;

extern "C" void FahrigIBM (double * GROWTH, double * offspringSize, double * Mortality, double * MOVEHABITAT,
                           double * MATSURV, int * N, int * numb_cells, int * boundary_condition, int * scape, double * cover, double * FRAG, int * densLim,
                           double * maxTime, int * outCode)
{
	// This sequence creates an attribute containing the output file name. The template is output-00000.txt.
	string fileNAME = "output-00000.txt";
	stringstream tmps;
	tmps<<outCode[0];
	string addToName = tmps.str();
	int fnSize = fileNAME.size();
    int tmpsSize = addToName.size();
	fileNAME.erase(fileNAME.begin()+fnSize-4-tmpsSize,fileNAME.begin()+fnSize-4);
	fileNAME.insert(fnSize-4-tmpsSize,addToName);
	
	GetRNGstate(); /* (mudar para doxygen):  este comando chama o engine de numeros aleatorios do R
					Por causa dela nossa biblioteca nao eh standalone */
	
	landscape* forest = new landscape(GROWTH[0], offspringSize[0], Mortality[0], MOVEHABITAT[0], MATSURV[0], N[0], numb_cells[0], boundary_condition[0], scape, cover[0], FRAG[0], densLim[0]);
    
    
    ofstream outputSIM; // ofstream for the output file
	outputSIM.open(fileNAME.c_str());

    for(unsigned int i=0; i<forest->popSize();i++)
	{
		outputSIM << forest->simTime << " " << forest->get_indiv(i)->get_id() << " " << forest->get_indiv(i)->get_x() << " " << forest->get_indiv(i)->get_y() << endl;
	}
	
	while (forest->simTime < maxTime[0] && forest->popSize() > 0)
	{
		forest->updateLandscape();
		for(unsigned int i=0; i<forest->popSize();i++)
			{
				outputSIM << forest->simTime << " " << forest->get_indiv(i)->get_id() << " " << forest->get_indiv(i)->get_x() << " " << forest->get_indiv(i)->get_y() << endl;
			}
    }
	if(forest->popSize()==0){outputSIM << forest->simTime << " " << "NA" << " " << "NA" << " " << "NA" << endl;}
	outputSIM.close(); //end of output file
	
	delete forest;
	PutRNGstate();
}


