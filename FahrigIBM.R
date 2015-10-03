setwd("~/Desktop/Fahrig-Model_CNS")
# Função "wrapper" para a chamada em C:
#
# Os passos abaixo foram adaptados de http://users.stat.umn.edu/~geyer/rc/

#Sys.setenv("PKG_CPPFLAGS" = "-fopenmp -DPARALLEL") # liga biblioteca de paralelismo
system("rm FahrigIBM.{so,o}") #limpa sources velhos
system ("R CMD SHLIB FahrigIBM.cpp") ## compila no R
dyn.load("FahrigIBM.so") ## carrega os source resultantes como biblioteca dinamica no R

# Generates the landscape with specified conditions. 
# numb.cells represents both the lenght AND width of the landscape, so numb.cells=100 creates a 100x100 landscape
# Land.shape can be 0 = XXX or 1 = XXX.
# Bound.condition can be 0 = XXX or 1 = XXX. 
Landscape <- function (numb.cells = 100, cell.size = 1, land.shape = 1, type=c("random","blob","fahrig"), 
                       bound.condition=0, cover=1,FRAG=NULL) {
	type=match.arg(type)
	if(cover < 0 || cover > 1) {
		stop("Error creating landscape. Cover must be between 0 and 1")
	}
	# scape represents the actual landscape
	scape <- rep(1, numb.cells*numb.cells)
	if(cover < 1) {
		NtoRemove=round((1-cover)*numb.cells*numb.cells);
		if(type=="random") {
			while(NtoRemove>0)
			{
				i=round(runif(1,0,numb.cells-1));
				j=round(runif(1,0,numb.cells-1));
				# tests to see if this point has already been removed
				if(scape[1+numb.cells*j+i] == 1) {
					NtoRemove = NtoRemove - 1
					scape[1+numb.cells*j+i] = 0
				}
			}
		}
		if(type=="blob") {
			i=round(runif(1,0,numb.cells-1));
			j=round(runif(1,0,numb.cells-1));
			while(NtoRemove>0)
			{
				# tests to see if this point has already been removed
				if(scape[1+numb.cells*j+i] == 1) {
					NtoRemove = NtoRemove - 1
					scape[1+numb.cells*j+i] = 0
				}
				# Draft a new point to be removed (random walk!)
				if(sample(1:2,1) == 1) {
					i = i + (-1)**sample(1:2,1)
				} else {
					j = j + (-1)**sample(1:2,1)
				}
				if(i == -1) { i=numb.cells-1}
				if(i == numb.cells) { i=1}
				if(j == -1) { j=numb.cells-1}
				if(j == numb.cells) { j=1}
			}
		}
    if(type=="fahrig")
    {
      scape=matrix(rep(0, numb.cells*numb.cells),numb.cells,numb.cells)
      while(sum(scape)<round(numb.cells*numb.cells*cover))
      {
        chosen.cell=sample(1:numb.cells,2,replace=T)
        neigh.cells=matrix(c(chosen.cell[1],chosen.cell[2]+1,chosen.cell[1]-1,chosen.cell[2],chosen.cell[1],
                             chosen.cell[2]-1,chosen.cell[1]+1,chosen.cell[2]),4,2,byrow=T)
        out.land=which(neigh.cells==numb.cells+1 | neigh.cells==0,arr.ind=T)[,1]
        n.neigh=1:4
        if(scape[chosen.cell[1],chosen.cell[2]]==0) 
        {
          rnd=runif(1,0,1)
          is.habitat=0
          if(length(out.land)>0)
          {
            for(i in n.neigh[-out.land])
            {
              is.habitat=is.habitat+scape[neigh.cells[i,1],neigh.cells[i,2]]
            }
          }
          else
          {
            for(i in n.neigh)
            {
              is.habitat=is.habitat+scape[neigh.cells[i,1],neigh.cells[i,2]]
            }
          }
          if(rnd<FRAG | is.habitat>0) 
          {
            scape[chosen.cell[1],chosen.cell[2]]=1
          }
        }  
      }
      scape=as.numeric(scape)
    }
	}
	land <- list(numb.cells = numb.cells, cell.size=cell.size, land.shape=land.shape, type=type, bound.condition=bound.condition, cover=cover,
               scape=scape, FRAG=FRAG)
	class(land) <- "landscape"
	return(land)
}

FahrigIBM <- function (
					 GROWTH=0.1, 
					 offspringSize=1, 
					 Mortality=360, 
					 MOVEHABITAT=5, 
					 MATSURV=0.5, 
					 N=500, 
					 landscape,
           densLim=10,
					 maxTime=20,
           out.code=1) 
{
	if(class(landscape) != "landscape") {
		stop("Error in function FahrigIBM: you must provide a valid landscape. See ?Landscape")
	}
  saida.C <- .C("FahrigIBM", 
              as.double(GROWTH),# 1
              as.double(offspringSize),# 2
              as.double(Mortality),# 3
              as.double(MOVEHABITAT),# 4
              as.double(MATSURV),# 5
              as.integer(N),# 6
              as.integer(landscape$numb.cells),# 7
              as.integer(landscape$bound.condition),# 8
              as.integer(landscape$scape),# 9
              as.double(landscape$cover),# 10
              as.double(landscape$FRAG),# 11
              as.integer(densLim),# 12
              as.double(maxTime),# 13
              as.integer(out.code))
}

myTime=500
lands<-Landscape(type="fahrig",cell.size=1,numb.cells=30,FRAG=NULL,cover=1,bound.condition=0)
system.time(teste<-FahrigIBM(GROWTH=0.9,
		offspringSize=1,
		Mortality=0.05,
		MOVEHABITAT=1,
		MATSURV=0.05,
		N=500,
		lands,
		densLim=10,
		maxTime=myTime,
		out.code=1))
