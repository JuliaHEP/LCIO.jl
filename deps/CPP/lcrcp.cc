#include "lcio.h"
#include "lcvecutil.h"
#include "IMPL/ReconstructedParticleImpl.h"
#include "IMPL/VertexImpl.h"

using namespace lcio ;

#include <iostream>


extern "C" {
// create delete ReconstructedParticle

 void* lcrcpcreate(){
   return new ReconstructedParticleImpl ;
 }

 int lcrcpdelete( void* recopart ){
   ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
   delete rcp ;
   return LCIO::SUCCESS ;
 }


// get Methods
FourVec lcgetp4( void* part)  {
    ReconstructedParticleImpl* particle = static_cast<ReconstructedParticleImpl*>( part ) ;
    if (not particle) {
        return FourVec{0,0,0,0};
    }
  const double* p = particle->getMomentum();
  FourVec v;
  v.x = p[0];
  v.y = p[1];
  v.z = p[2];
  v.t = particle->getEnergy();
  return v;
}


int lcrcpid( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return rcp->id() ;
}

int lcrcpgettype( void* recopart )  {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return rcp->getType() ;
}

bool lcrcpiscompound ( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return rcp->isCompound() ;
}

float lcrcpgetenergy( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return rcp->getEnergy() ;
}

const float* lcrcpgetcovmatrix( void* recopart )  {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return &rcp->getCovMatrix()[0];
}

float lcrcpgetmass( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return rcp->getMass() ;
}

float lcrcpgetcharge( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return rcp->getCharge() ;
}

ThreeVec lcrcpgetreferencepoint( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  const float* p = rcp->getReferencePoint() ;
  return ThreeVec{p[0], p[1], p[2]};
}

const void* lcrcpgetparticleids( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return &rcp->getParticleIDs();
}

float lcrcpgetgoodnessofpid( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>(recopart ) ;
  return rcp->getGoodnessOfPID() ;
}

const void* lcrcpgetparticles( void* recopart, size_t* nParticles ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  *nParticles = rcp->getParticles().size();
  return &rcp->getParticles()[0];
}

const void* lcrcpgetclusters( void* recopart, size_t* nClusters ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart );
  *nClusters = rcp->getClusters().size();
  return &rcp->getClusters()[0];
}


const void* lcrcpgettracks( void* recopart ) {
  ReconstructedParticleImpl* rcp = static_cast<ReconstructedParticleImpl*>( recopart ) ;
  return &rcp->getTracks();
}

void* lcrcpgetstartvertex( void* recopart ) {
  ReconstructedParticle* rcp = static_cast<ReconstructedParticle*>( recopart ) ;
  return rcp->getStartVertex();
}

void* lcrcpgetendvertex( void* recopart ) {
  ReconstructedParticle* rcp = static_cast<ReconstructedParticle*>( recopart ) ;
  return rcp->getEndVertex();
}


}
