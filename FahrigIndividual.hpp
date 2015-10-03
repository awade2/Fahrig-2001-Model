#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <vector>

using namespace std;

/** \brief A classe individuo representa um agente da simulação.
 *
 *  Esta classe contém informações referentes a cada indivíduo, incluindo sua localização e estado, uma lista contendo ponteiros para os vizinhos próximos, tamanho do passo de movimentação, etc. Esta classe NÃO contém métodos que sejam de responsabilidade do ambiente, como o método de atualizar os vizinhos */
class individual
{
private:
    //propriedades
	
    /** Identificador único de cada indivíduo */
    const unsigned long id;
	/** Identificador máximo já usado */
    static unsigned long MAXID;
	/** Posição X da localização do indivíduo */
    int cellX;
	/** Posição Y da localização do indivíduo */
	int cellY;
    int habitatType; // The cell where the individual is located is breeding habitat or matrix?
    const double GROWTH;//mean of a binomial distribution
    double birthProb;// probability of birth
    const double offspringSize; // maximum clutch size
    const double Mortality;//taxa intrínseca de morte quando no habitat e na ausencia de outros individuos na vizinhanca local
    const double MOVEHABITAT; //movement probability in breeding habitat
    int stepLength;//length of the step given by a moving individual
    int direction;//moving direction (4-neighbour rule)
    const double MATSURV;// probability of survival in the matrix habitat
    /*
    int eventMort;
    int eventBirth;
    int eventMove;
    */
    int Noffspring;
	
public:
    /** Construtor da classe individuo. Deve ser chamado pela paisagem para posicionar os
     * individuos que já estão na paisagem no início da simulação. */
    individual(
              /** Posição X do indivíduo */
              int cellX,
              /** Posição Y do indivíduo */
              int cellY,
              /**  */
              const double GROWTH,
              /**  */
              const double offspringSize,
              /**  */
              const double Mortality,
              /**  */
              const double MOVEHABITAT,
              /**  */
              int stepLength,
              /**  */
              int direction,
              /**  */
              const double MATSURV);
    /** Construtor de c√≥pia, usado para gerar novos indiv√≠duos por reprodu√ß√£o ASSEXUADA.
     *  Todos os dados do individuo pai ser√£o copiados, com a exce√ß√£o de:
     *  - id (veja \ref individuo::get_id)
     *  - vizinhos (veja \ref individuo::set_vizinhos)
     *  - tempo para evento (veja \ref individuo::get_tempo) 
     *  */
    individual(/** Individuo pai */ const individual& rhs);

    /** Reinicia o contador de indivíduos **/
    static void reset_id() {MAXID = 0;}
    
    /** Atualiza o tipo de hábitat no qual o indivíduo está. Deve ser chamada a cada passo de tempo pela \ref paisagem. */
    void set_habitat (int typeH){this->habitatType=typeH;}
    /** returns the X coordinate of the cell of an individual */
    inline const int get_x() const {return this->cellX;}
    /** returns the Y coordinate of the cell of an individual */
    inline const int get_y() const {return this->cellY;}
    void set_x(double i){this->cellX =i;}
    void set_y(double i){this->cellY =i;}
    inline const double get_offspringSize() const {return this->offspringSize;}
    inline const double get_birthProb() const {return this->birthProb;}
    void set_stepLength(int lgth){this->stepLength = lgth;}
    void set_direction(int dir){this->direction = dir;}
    const unsigned long get_id() const {return this->id;}
    inline const double get_Mortality() const {return this->Mortality;}
    inline const double get_MOVEHABITAT() const {return this->MOVEHABITAT;}
    inline const double get_MATSURV() const {return this->MATSURV;}
    /*
    void updateEvent();
    void updateHabitat();
    void updateMatrix();
    */
};

#endif // INDIVIDUAL_H
