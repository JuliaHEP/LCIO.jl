#include "lcio.h"
#include "lcvecutil.h"
#include "IMPL/MCParticleImpl.h"

using namespace lcio ;

#include <iostream>

extern "C" {

MCParticle* lcmcpcreate() {
    return new MCParticleImpl;
}

void lcmcpdelete( void* mcparticle ){
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  delete mcp ;
}

int lcmcpgetnumberofparents( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  return mcp->getParents().size() ;
}

void* lcmcpgetparent( void* mcparticle, int i ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  return mcp->getParents()[i-1];
}

void* lcmcpgetdaughter( void* mcparticle, int i ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  return mcp->getDaughters()[i-1];
}

ThreeVec lcmcpgetendpoint( void* mcparticle) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  const double* p = mcp->getEndpoint();
  return ThreeVec{p[0], p[1], p[2]};
}

int lcmcpgetnumberofdaughters( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return -1;
  }
  return mcp->getDaughters().size() ;
}

int lcmcpgetpdg( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return -1;
  }
  return mcp->getPDG() ;
}

int lcmcpgetgeneratorstatus( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return -1;
  }
  return mcp->getGeneratorStatus() ;
}

int lcmcpgetsimulatorstatus( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return -1;
  }
  return mcp->getSimulatorStatus() ;
}

ThreeVec lcmcpgetvertex( void* mcparticle)  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return ThreeVec{0,0,0};
  }
  const double* p = mcp->getVertex();
  return ThreeVec{p[0], p[1], p[2]};
}

float lcmcpgettime( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return -1;
  }
  return mcp->getTime() ;
}

ThreeVec lcmcpgetP3( void* mcparticle)  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return ThreeVec{0,0,0};
  }
  const double* p = mcp->getMomentum();
  return ThreeVec{p[0], p[1], p[2]};
}

const double* lcmcpgetmomentum( void* mcparticle)  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return 0;
  }
  return mcp->getMomentum();
}

FourVec lcmcpgetp4( void* mcparticle)  {
  MCParticle* mcp = static_cast<MCParticle*>( mcparticle ) ;
  if (not mcp) {
      std::cout << "can't cast" << std::endl;
      return FourVec{0,0,0,0};
  }
  const double* p = mcp->getMomentum();
  // std::cout << "p[0]: " << p[0] << "\tp[1]: " << p[1] << "\tp[2]: " << p[2] << "\tp[4]: " << mcp->getEnergy() << std::endl;
  return FourVec{p[0], p[1], p[2], mcp->getEnergy()};
}

double lcmcpgetenergy( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return -1;
  }
  return mcp->getEnergy() ;
}

double lcmcpgetmass( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return -1;
  }
  return mcp->getMass() ;
}

float lcmcpgetcharge( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return 100;
  }
  return mcp->getCharge() ;
}

const float* lcmcpgetspin( void* mcparticle)  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return 0;
  }
  return mcp->getSpin();
}

const int* lcmcpgetcolorflow( void* mcparticle )  {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return 0;
  }
  return mcp->getColorFlow();
}

int lcmcpaddparent(  void* mcparticle, void* parent ) {
    MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
    if (not mcp) {
        return LCIO::ERROR;
    }
    MCParticle* mom = static_cast<MCParticle*>( parent ) ;
    if (not mom) {
        return LCIO::ERROR;
    }
    mcp->addParent( mom ) ;
    return LCIO::SUCCESS ;
}

int lcmcpsetpdg( void* mcparticle, int pdg ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setPDG( pdg ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetgeneratorstatus( void* mcparticle, int status ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setGeneratorStatus( status ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetsimulatorstatus( void* mcparticle, int status ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setSimulatorStatus( status ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetvertex( void* mcparticle, double vtx[3] ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setVertex( vtx ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetendpoint( void* mcparticle, double pnt[3] ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setEndpoint( pnt ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetmomentum( void* mcparticle,  float p[3] ){
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setMomentum( p ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetmass( void* mcparticle, float m) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setMass( m ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetcharge( void* mcparticle, float c ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setCharge( c ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetspin( void* mcparticle, float spin[3] ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setSpin( spin ) ;
  return LCIO::SUCCESS ;
}

int lcmcpsetcolorflow( void* mcparticle, int cflow[2] ) {
  MCParticleImpl* mcp = static_cast<MCParticleImpl*>( mcparticle ) ;
  if (not mcp) {
      return LCIO::ERROR;
  }
  mcp->setColorFlow( cflow ) ;
  return LCIO::SUCCESS ;
}
}
