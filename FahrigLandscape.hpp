#ifndef PAISAGEM_H
#define PAISAGEM_H
#include "FahrigIndividual.hpp"
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<cmath>
///** Dimensão maxima do mapa, usada para rotinas de fragmentação TBI */
#define dim 10000 /*Em algum momento pode ser alterado para um argumento do
		    construtor. No momento não é prioritário. */

using namespace std;

/** \brief A classe paisagem implementa o mundo onde os agentes estão.
 *
 * Esta classe é responsável pela criação dos indivíduos, por manter o relógio do mundo, e por estabelecer a comunicação entre os indivíduos \sa \ref paisagem::update */
class landscape
{
  private:
    
    //propriedades privadas
    const unsigned long N; // numero de inds no comeco da simulacao
    vector <individual*> popInd;//vetor dos individuos da populacao
    const int numb_cells; // em quantos pedacos cada dimensao do mundo esta dividida (para fragmentacao)
    const int boundary_condition; // tipo de condição de contorno (0 = absortiva, 1 = periódica)
    int land[dim][dim];//[linha][coluna] temporariamente substituido or valor fixo
    int cellDensity[dim][dim]={{0}};
    double cover;
    double FRAG;
    const int densLim;
    //int cellHabitat;
    //const int initialPos;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //metodos privados
    
    void populating(
                    /* Numero de individuos que começam a simulação espalhados no ambiente */
                    const int N,
                    /**  */
                    const double GROWTH,
                    /**  */
                    const double offspringSize,
                    /**  */
                    const double Mortality,
                    /**  */
                    const double MOVEHABITAT,
                    /**  */
                    const double MATSURV
                    );
    
    /*
     void atualiza_vizinhos(individuo * const ind) const;//contabilizador de vizinhos
     void atualiza_habitat(individuo * const i) const;//vai informar o individuo em que tipo de habitat ele esta
     //int define_tempo();
     void apply_boundary(individuo * const ind); //const; // metodo para aplicação da condicao de contorno
     int shortTime(); // Select the individual who will make the action. This individual is the one with the shortest time ("tempo_eventp")
     
     */
    
  public:
    
    /** Contador de quanto tempo já transcorreu no mundo simulado */
    double simTime;
    
    //metodos
    
    /** Construtor da classe paisagem */
    landscape(
              /**  */
              const double GROWTH,
              /**  */
              const double offspringSize,
              /**  */
              const double Mortality,
              /**  */
              const double MOVEHABITAT,
              /**  */
              const double MATSURV,
              /** Numero de individuos que começam a simulação espalhados no ambiente */
              const int N,
              /** Ângulo de visada dos individuos */
              const int numb_cells,
              /** How individuals are initially set into the landscpae **/
              //const int inipos,
              /** Condição de contorno */
              const int bound_condition,
              /** Vetor de cobertura de habitat na paisagem */
              int scape[],
              double cover,
              double FRAG,
              const int densLim
              );
    
    
    void updateLandscape();//atualizador
    void overKill();
    void smpArrayInt(int array[], int setLength, int sampleQty, int iniVal);
    const int popSize() const{return popInd.size();}
    individual* get_indiv(int i) const {return popInd[i];}
    void updateCellDens();
};

#endif // PAISAGEM_H
