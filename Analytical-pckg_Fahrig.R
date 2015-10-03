multiRunFAHRIG=function(PATH,nrep=20,growth,offs.size=1,Mort=0.05,movehab,matsurv,N0=500,dens.lim=10,
                        BC=1,tm=500, cover, frag, ini.rep=1,save.land=F,save.codes=NULL,save.path=NULL)
{
  oldpath <- getwd()
  dir.create(PATH, recursive=TRUE)
  setwd(PATH)
  for(i in 1:nrep)
  {
    land=Landscape(type="fahrig",cell.size=1,numb.cells=30,bound.condition=BC,FRAG=frag,cover=cover)
    if(save.land) write(land$scape,paste(save.path,"/Comb-",save.codes,"Rep",ini.rep,sep=""))
    
    FahrigIBM(GROWTH=growth,
              offspringSize=offs.size,
              Mortality=Mort,
              MOVEHABITAT=movehab,
              MATSURV=matsurv,
              N=N0,
              lands=land,
              densLim=dens.lim,
              maxTime=tm,
              out.code=ini.rep)
    ini.rep=ini.rep+1
  }
  #Nt.reps=Nt.data(wdir=PATH,intervals=1,tmax=tm)
  #plot.Nt(data=Nt.reps,growth=b0-d0,sum.incl=inc.b+inc.d)
  #return(Nt.reps)
  setwd(oldpath)
}

# land <- Landscape(cover=1,type="b",cell.size=100)
# Dcoef=10000*0.8/4
# sim1=multiRun(PATH="~/Desktop/Comb-0023",
#               nrep=20,
#               b0=0.4,
#               d0=0.1,
#               m0=0.8,
#               inc.b=100450,
#               inc.d=0,
#               step=100,
#               radius=1460,
#               dens.t=1,
#               config=0,
#               N0=50,
#               lands=land,
#               tm=2000
# )
####################
#  Get N(t) data
####################
Nt.data.FAHRIG=function(wdir,intervals=1,tmax,NREPS=20){
  oldpath <- getwd()
  setwd(wdir)
  files=dir()
  Nt.arr=array(0,c(tmax+1,2,NREPS))
  for(i in 1:NREPS)
  {
    t1=read.table(files[i],sep=" ")
    Nt=as.data.frame(table(t1[,1]))
    if(is.na(t1[dim(t1)[1],2])) # condicao que indica se houve extincao.
    {
      Nt[dim(Nt)[1],2]=0 # Corrije o data.frame Nt para que N = 0 quando t (coluna 1) = t de extincao. Isso se deve ao fato
      # do table levar em conta o tempo de extincao como um fator representado uma vez na planilha da dados.  
    }
  Nt.arr[,1,i]=0:tmax
  Nt.arr[,2,i]=Nt[1:(tmax+1),2]
  }
setwd(oldpath)
return(Nt.arr)  
}

Surv.Prob=function(dat)#### dat must be an array such as the output of function NT.data.FAHRIG
{
  pop.surv=numeric()
  for(i in 1:dim(dat)[3])
  {
    if(min(dat[,2,i],na.rm=T)==0)
    {
      pop.surv[i]=0
    }
    else {pop.surv[i]=1}
  }
  return(list(Extinction.prob=sum(pop.surv)/length(pop.surv),survival=pop.surv))
}

timeToExtinction=function(dat)
{
  tte=numeric()
  for(i in 1:dim(dat)[3])
  {
    aux=which(dat[,2,i]==0)
    if(length(aux)==0){tte[i]=NA}
    else {tte[i]=dat[aux,1,i]}
  }  
  return(tte)
}
####################
#  Plot N(t) data
####################
plot.Nt.FAHRIG=function(dataSim,name="N(t)",yrange=c(0,max(dataSim[,2,],na.rm=T)+10),
                        xrange=c(0,dim(dataSim)[1]))
{
  plot(dataSim[,1,1],dataSim[,2,1],type="n",xlab="t",ylab="N (population size)",xlim=xrange, ylim=yrange,
       cex=1.5, main = name)
  for(i in 1:dim(dataSim)[3])
  {
    is.NA=which(is.na(dataSim[,2,i]))
    if(length(is.NA)>0)
    {
      dataSim[is.NA,2,i]=0
    }
    lines(dataSim[,1,i],dataSim[,2,i],col="gray50")
  }
  lines(0:(dim(dataSim)[1]-1),apply(dataSim[,2,],1,mean),col=1,lwd=4)
}

Threshold.data=function(direc="/Volumes/DADOS/Fahrig-outputs",combs,tmax=500,parameters,nreps=20)
{
  dirs=dir(direc)
  probs=0
  index=1
  N.t=array(NA,dim = c(tmax+1,2,nreps,length(combs)))
  surv.data=matrix(NA,nreps,length(combs))
 
  for(i in combs)
  {
    Ns=Nt.data.FAHRIG(paste(direc,dirs[combs[index]],sep="/"),tmax=tmax)
    N.t[,,,index]=Ns
    survival=Surv.Prob(Ns)
    probs[index]=survival$Extinction.prob
    surv.data[,index]=survival$survival
    index=index+1
  }
  return(list(Pop.Size=N.t,Survival.Prob=probs,Survival.data=surv.data,params.values=params[combs[1],2:5]))
}

plot.threshold=function(name="Extinction Threshold", point.style=0,parameters,combs,probs,add=F,pcol=1)
{
  if(add==F)
  {
    plot(parameters[combs,1],1:length(combs),type="n",xlab="Landscape Cover",ylab="Survival Probability",xlim=c(0,1), ylim=c(0,1.1),
         cex=1.5, main = name, sub=paste("FRAG = ",parameters$frag[combs[1]],", MATSURV = ",
                                         parameters$matsurv[combs[1]],", MOVEHAB = ", parameters$movehab[combs[1]],
                                         ", GROWTH = ",parameters$growth[combs[1]]))
  }
  points(parameters[combs,1],probs,pch=point.style,col=pcol)
  lines(parameters[combs,1],probs,lwd=2,col=pcol)
}

plot.threshold(probs=test1$Survival.Prob,parameters=params,comb=1:21)

undebug(Threshold.data)




# which(dataSim[dim(dataSim)[1],2,]!=0)

# plot.Nt(Nt.test,growth=0,0,10^8)
# plot.Nt(growth=0.3,sum.incl=100450)
# sim1=Nt.data(wdir="~/Desktop/Comb-0022",tmax=2000)

