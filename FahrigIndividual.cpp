#include "FahrigIndividual.hpp"
#include <cmath>
#include "FahrigLandscape.hpp"
#include <cstdlib>
#include <iostream>
#include <R.h>
#include <Rmath.h>

using namespace std;

unsigned long individual::MAXID = 0;

individual::individual(int cellX, int cellY, double GROWTH, double offspringSize, double Mortality, double MOVEHABITAT, int stepLength, int direction,
                       double MATSURV):
    id(++MAXID), // pega o próximo ID livre
    cellX(cellX),
    cellY(cellY),
    Noffspring(0),
    GROWTH(GROWTH),
    offspringSize(offspringSize),
    Mortality(Mortality),
    MOVEHABITAT(MOVEHABITAT),
    stepLength(stepLength),
    direction(direction),
    MATSURV(MATSURV)
{
    this->birthProb=GROWTH/offspringSize;
}


/** \brief função para reprodução assexuada de um indivíduo
 * 
 * Faz uma cópia do indivíduo pai. Recebe um ponteiro dereferenciado (nomeado rhs) e usa isto para
 * Criar novo individuo com os mesmos valores de atributo do pai, exceto
 *  - id (veja \ref individuo::get_id)
 *  - vizinhos (veja \ref individuo::set_vizinhos) &&&&&&&& mudou já que os individuos nascem no mesmo local do pai
 *  - tempo para evento (veja \ref individuo::get_tempo) 
 * Usa notacao :atributo(valor) ao inves de atribuicão. 
 * Funcao chamada por paisagem::realiza_acao() quando a ação é um nascimentto
 * @param rhs ponteiro dereferenciado para o pai */
individual::individual(const individual& rhs):
    id(++MAXID),
    cellX(rhs.cellX),
    cellY(rhs.cellY),
    Noffspring(0),
    GROWTH(rhs.GROWTH),
    birthProb(rhs.birthProb),
    offspringSize(rhs.offspringSize),
    Mortality(rhs.Mortality),
    MOVEHABITAT(rhs.MOVEHABITAT),
    stepLength(rhs.stepLength),
    direction(rhs.direction),
    MATSURV(rhs.MATSURV)
{ //precisamos dessa chave e da que fecha ela?
	
}

/*
void individual::updateEvent()
{
    if(this->habitatType==1) {this->updateHabitat();}
    else {this->updateMatrix;}
}

void individual::updateHabitat()
{
    this->Noffspring=rbinom(offspringSize,birthProb);
    double rndM = runif(0,1);
    double rndMov = runif(0,1);
    if(this->Noffspring>0) {this->eventBirth=1;}
    if(rndM < this->Mortality) {this->eventMort=1;}
    if(rndMov < this->MOVEHABITAT) {this->eventMove=1;}
}


void individual::updateMatrix()
{
    double rndM = runif(0,1);
    if(rndM < (1-this->MATSURV)) {this->eventMort=1;}
}
*/