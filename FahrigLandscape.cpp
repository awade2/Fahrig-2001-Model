#include "FahrigLandscape.hpp"
#include "FahrigIndividual.hpp"
#include <R.h>
#include <Rmath.h>

landscape::landscape(double GROWTH, double offspringSize, double Mortality, double MOVEHABITAT, double MATSURV, int N,
				   int numb_cells, int bound_condition, int scape[], double cover, double FRAG, int densLim):

N(N),
simTime(0),
numb_cells(numb_cells),
//cellHabitat(0),
boundary_condition(bound_condition),
cover(cover),
FRAG(FRAG),
densLim(densLim)
{
    for(int i=0;i<this->numb_cells;i++)
    {
        for(int j=0;j<this->numb_cells;j++)
        {
            // transforma o vetor scape recebido do construtor em uma matriz
            this->land[i][j]=scape[j*numb_cells+i];
        }
    }
    
    this->populating(N, GROWTH, offspringSize, Mortality, MOVEHABITAT, MATSURV);
    int cX, cY;
    for(int k=0; k<this->popInd.size();k++)
    {
        cX = this->popInd[k]->get_x();
        cY = this->popInd[k]->get_y();
        this->cellDensity[cX][cY]=this->cellDensity[cX][cY]+1;
    }
}


void landscape::populating(int N, double GROWTH, double offspringSize, double Mortality, double MOVEHABITAT, double MATSURV)
{
    individual::reset_id(); // reinicia o contador de id dos individuos
    
    for(int i=0; i<this->N; i++)
    {
        this->popInd.push_back(new individual(
                                             (int)runif(0,this->numb_cells), //posicao x
                                             (int)runif(0,this->numb_cells), //posicao y
                                             GROWTH,
                                             offspringSize,
                                             Mortality,
                                             MOVEHABITAT,
                                             (int)runif(1,5),
                                             (int)runif(1,5),
                                             MATSURV));
    }
}


void landscape::updateLandscape()
{
    if(this->popInd.size()>0)
    {
        /* Following the Fahrig (1997) algorithm. But, I did not understand why it is necessary to update individuals by cells. The same results are expected 
            if we make the update by individuals themselves, for which case two loops can be removed. However, .... habitat type variable ....
        
        for(unsigned int i=0; i<this->numb_cells; i++)
        {
            for(unsigned int j=0; j<this->numb_cells; j++)
            {
                this->cellHabitat=this->landscape[i][j];
                for(unsigned int k=0; k<this->popInd.size(); k++)
                {
                    if(this->popInd[k]->get_x()==i && this->popInd[k]->get_y()==j)
                    {
                        this->popInd[k]->update();
                    }
                }
            }
        } */
        int nPop = this->popInd.size();
        vector <int> toRemove;
        int nRemoved = 0;
        //int counter = 0;
        
        for(unsigned int i=0; i<nPop; i++)
        {
            int cXX = this->popInd[i]->get_x();
            int cYY = this->popInd[i]->get_y();
            int isDeath = 0;
            int habitat = this->land[cXX][cYY];
            if(habitat==1)
            {
                int orderOfActions[3];
                this->smpArrayInt(orderOfActions,3,3,1);
                for(int a=0;a<3;a++)
                {
                    //int action = orderOfActions[i];
                    switch(orderOfActions[a])
                    {
                        case 1: // BIRTH
                        {
                            int Noffspring=rbinom(this->popInd[i]->get_offspringSize(),this->popInd[i]->get_birthProb());
                            if(Noffspring>0)
                            {
                                //int cX1 = this->popInd[i]->get_x();
                                //int cY1 = this->popInd[i]->get_y();
                                for(int u=0;u<Noffspring;u++)
                                {
                                    individual* chosen;
                                    chosen = new individual(*this->popInd[i]);
                                    this->popInd.push_back(chosen);
                                   // this->cellDensity[cX1][cY1]=this->cellDensity[cX1][cY1]+1;
                                }
                            }
                        }
                            break;
                            
                        case 2: // DEATH
                        {
                            double rndM = runif(0,1);
                            if(rndM < this->popInd[i]->get_Mortality())
                            {
                                toRemove.push_back(i);
                                isDeath=1;
                            }
                        }
                            break;
                            
                        case 3: // MOVEMENT
                        {
                            double rndMov = runif(0,1);
                            if(rndMov < this->popInd[i]->get_MOVEHABITAT())
                            {
                                int cX = this->popInd[i]->get_x();
                                int cY = this->popInd[i]->get_y();
                                //this->cellDensity[cX][cY]=this->cellDensity[cX][cY]-1;
                                int direc = (int)runif(1,5);
                                int step = (int)runif(1,5);
                                switch (direc) // 1 = right; 2 = up; 3 = left; 4 = down.
                                {
                                    case 1:
                                        this->popInd[i]->set_y(cY+step);
                                        if(this->popInd[i]->get_y()>=this->numb_cells)
                                        {
                                            if(this->boundary_condition==1)
                                            {
                                                int newCy = cY + step - this->numb_cells;
                                                this->popInd[i]->set_y(newCy);
                                                //this->cellDensity[cX][newCy]=this->cellDensity[cX][newCy]+1;
                                            }
                                        }
                                        //else {this->cellDensity[cX][(cY+step)]=this->cellDensity[cX][(cY+step)]+1;}
                                        break;
                                        
                                    case 2:
                                        this->popInd[i]->set_x(cX-step);
                                        if(this->popInd[i]->get_x()<0)
                                        {
                                            if(this->boundary_condition==1)
                                            {
                                                int newCx = this->numb_cells - step + cX;
                                                this->popInd[i]->set_x(newCx);
                                                //this->cellDensity[newCx][cY]=this->cellDensity[newCx][cY]+1;
                                            }
                                        }
                                        //else {this->cellDensity[(cX-step)][cY]=this->cellDensity[(cX-step)][cY]+1;}
                                        break;
                                        
                                    case 3:
                                        this->popInd[i]->set_y(cY-step);
                                        if(this->popInd[i]->get_y()<0)
                                        {
                                            if(this->boundary_condition==1)
                                            {
                                                int newCy = this->numb_cells - step + cY;
                                                this->popInd[i]->set_y(newCy);
                                                //this->cellDensity[cX][newCy]=this->cellDensity[cX][newCy]+1;
                                            }
                                        }
                                        //else {this->cellDensity[cX][(cY-step)]=this->cellDensity[cX][(cY-step)]+1;}
                                        break;
                                        
                                    case 4:
                                        this->popInd[i]->set_x(cX+step);
                                        if(this->popInd[i]->get_x()>=this->numb_cells)
                                        {
                                            if(this->boundary_condition==1)
                                            {
                                                int newCx = cX + step - this->numb_cells;
                                                this->popInd[i]->set_x(newCx);
                                                //this->cellDensity[newCx][cY]=this->cellDensity[newCx][cY]+1;
                                            }
                                        }
                                        //else {this->cellDensity[(cX+step)][cY]=this->cellDensity[(cX+step)][cY]+1;}
                                        break;
                                }
                            }
                        }
                            break;
                    } // end of switch (orderOfActions)
                    if(orderOfActions[a]==2 && isDeath==1)
                        break;
                } // end of for();
            }// end if (habitat == 1)
            else // if the  individual is on matrix environment.
            {
                int orderOfActions[2];
                this->smpArrayInt(orderOfActions,2,2,1);
                for(int a=0;a<2;a++)
                {
                    //int action = orderOfActions[i];
                    switch(orderOfActions[a])
                    {
                        case 1: // DEATH
                        {
                            double rndM = runif(0,1);
                            if(rndM < (1-this->popInd[i]->get_MATSURV()))
                            {
                                toRemove.push_back(i);
                                isDeath=1;
                            }
                        }
                            break;
                            
                        case 2: // MOVEMENT
                        {
                            int cX = this->popInd[i]->get_x();
                            int cY = this->popInd[i]->get_y();
                            //this->cellDensity[cX][cY]=this->cellDensity[cX][cY]-1;
                            int direc = (int)runif(1,5);
                            int step = (int)runif(1,5);
                            switch (direc) // 1 = right; 2 = up; 3 = left; 4 = down.
                            {
                                case 1:
                                    this->popInd[i]->set_y(cY+step);
                                    if(this->popInd[i]->get_y()>=this->numb_cells)
                                    {
                                        if(this->boundary_condition==1)
                                        {
                                            int newCy = cY + step - this->numb_cells;
                                            this->popInd[i]->set_y(newCy);
                                            //this->cellDensity[cX][newCy]=this->cellDensity[cX][newCy]+1;
                                        }
                                    }
                                    //else {this->cellDensity[cX][(cY+step)]=this->cellDensity[cX][(cY+step)]+1;}
                                    break;
                                    
                                case 2:
                                    this->popInd[i]->set_x(cX-step);
                                    if(this->popInd[i]->get_x()<0)
                                    {
                                        if(this->boundary_condition==1)
                                        {
                                            int newCx = this->numb_cells - step + cX;
                                            this->popInd[i]->set_x(newCx);
                                            //this->cellDensity[newCx][cY]=this->cellDensity[newCx][cY]+1;
                                        }
                                    }
                                    //else {this->cellDensity[(cX-step)][cY]=this->cellDensity[(cX-step)][cY]+1;}
                                    break;
                                    
                                case 3:
                                    this->popInd[i]->set_y(cY-step);
                                    if(this->popInd[i]->get_y()<0)
                                    {
                                        if(this->boundary_condition==1)
                                        {
                                            int newCy = this->numb_cells - step + cY;
                                            this->popInd[i]->set_y(newCy);
                                            //this->cellDensity[cX][newCy]=this->cellDensity[cX][newCy]+1;
                                        }
                                    }
                                    //else {this->cellDensity[cX][(cY-step)]=this->cellDensity[cX][(cY-step)]+1;}
                                    break;
                                    
                                case 4:
                                    this->popInd[i]->set_x(cX+step);
                                    if(this->popInd[i]->get_x()>=this->numb_cells)
                                    {
                                        if(this->boundary_condition==1)
                                        {
                                            int newCx = cX + step - this->numb_cells;
                                            this->popInd[i]->set_x(newCx);
                                            //this->cellDensity[newCx][cY]=this->cellDensity[newCx][cY]+1;
                                        }
                                    }
                                    //else {this->cellDensity[(cX+step)][cY]=this->cellDensity[(cX+step)][cY]+1;}
                                    break;
                            }
                        }
                            break;
                    }
                    if(orderOfActions[a]==2 && isDeath==1)
                        break;
                }
            } // end else.
            //this->popInd[i]->set_habitat(this->landscape[cX][cY]);
            //this->popInd[i]->updateEvent();
        } // end of for (loop of indivuduals).
        int index;
        for(int z=0;z<toRemove.size();z++)
        {
            /*int cellx, celly;
            cellx = this->popInd[index]->get_x();
            celly = this->popInd[index]->get_y();
            this->cellDensity[cellx][celly] = this->cellDensity[cellx][celly]-1;//update Cell Density after the death */
            index = toRemove[z] - nRemoved;
            delete this->popInd[index];
            this->popInd.erase(this->popInd.begin()+index);
            ++nRemoved;
        }
        if(this->boundary_condition==0) // Remove according to absorbing boundary condition.
        {
            vector <int> toRemove2;
            int nRemoved2 =0;
            for(int w=0; w<this->popInd.size(); w++)
            {
                if(this->popInd[w]->get_x()<0 || this->popInd[w]->get_x()>=this->numb_cells || this->popInd[w]->get_y()<0 || this->popInd[w]->get_y()>=this->numb_cells)
                {
                    toRemove2.push_back(w);
                }
            }
            for(int z=0; z<toRemove2.size();z++)
            {
                index = toRemove2[z] - nRemoved2;
                delete this->popInd[index];
                this->popInd.erase(this->popInd.begin()+index);
                ++nRemoved2;
            }
        }
        this->updateCellDens();
        this->overKill();
        this->simTime=this->simTime+1;
    }
}

void landscape::smpArrayInt(int array[], int setLength, int sampleQty, int iniVal) // setLength = size of the set of integers to be sampled; // sampleQty = number of integers to be sampled (with replacement) // iniVal = initial value of the set of integers.
{
    int x=1,y=0;//ensure x is 1 so that first number is generated outside of loops
    srand(time(NULL));//set random # seed
    
    array[0]=(int)runif(0,setLength)+iniVal;//rand()%52+1;//first number assigned
    do
    {
        array[x]=(int)runif(0,setLength)+iniVal;//rand()%52+1; //value generated for x's position
        y=0;
        while (y<x) //iterate through array while checking for duplicate values
        {
            if (array[y]==array[x]) //if duplicate is found, generate another value for this position
            {
                --x;
                break;
            }
            ++y; //increment y to check additional array positions
        }
        ++x; //increment x to assign values to additional array positions
    }
    while (x<sampleQty);
}


void landscape::overKill()
{
    for(int i=0; i<this->numb_cells;i++)
    {
        for(int j=0; j<this->numb_cells;j++)
        {
            if(this->cellDensity[i][j] > this->densLim)
            {
                int index2 = 0;
                int nToRem = this->cellDensity[i][j] - this->densLim;
                vector<int> listInd;
                for(int k=0;k<this->popInd.size();k++)
                {
                    if(this->popInd[k]->get_x()==i && this->popInd[k]->get_y()==j)
                    {
                        listInd.push_back(k);
                    }
                }
                int remVec[nToRem];
                this->smpArrayInt(remVec,this->cellDensity[i][j],nToRem,0);
                vector <int> removVec (remVec,remVec+nToRem);
                sort(removVec.begin(),removVec.begin()+nToRem);
                int nRemoved3=0;
                int index3;
                for(int u=0; u<removVec.size();u++)
                {
                    index3 = listInd[removVec[u]] - nRemoved3;
                    /*cout << "u = " << u << "---->Size(removeVec) = " << removVec.size() << " ----> removVec[u] = " << removVec[u] << " ----> size(listInd) = " << listInd.size() << " = " << this->cellDensity[i][j] << " ----> listInd[removVec[u]] = " << listInd[removVec[u]] << "----> nRemoved3 = " << nRemoved3 << " ----> Index3 = " << index3 << "----> size(popInd) = " << this->popInd.size() << endl; */
                    delete this->popInd[index3];
                    this->popInd.erase(this->popInd.begin()+index3);
                    ++nRemoved3;
                }
            }
        }
    }
}


void landscape::updateCellDens()
{
    for(int i=0;i<this->numb_cells;i++)
    {
        for(int j=0;j<this->numb_cells;j++)
        {
            // transforma o vetor scape recebido do construtor em uma matriz
            this->cellDensity[i][j]=0;
        }
    }
    
    int cellX, cellY;
    for(int k=0; k<this->popInd.size();k++)
    {
        cellX = this->popInd[k]->get_x();
        cellY = this->popInd[k]->get_y();
        this->cellDensity[cellX][cellY]=this->cellDensity[cellX][cellY]+1;
    }
    
}
