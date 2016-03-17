#include "lcio.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "lcvecutil.h"


using namespace lcio ;

extern "C" {

 void* lcschcreate(){
  return new SimCalorimeterHitImpl ;
 }

 int lcschdelete(void* simcalhit ) {
   SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
   delete hit ;
   return LCIO::SUCCESS ;
 }

// get Methods
CalHit lcschgetp4( void* hit)  {
    SimCalorimeterHitImpl* h = static_cast<SimCalorimeterHitImpl*>( hit ) ;
    CalHit c;
    const float* p = h->getPosition();
    c.x = p[0];
    c.y = p[1];
    c.z = p[2];
    c.E = h->getEnergy();
    return c;
}

int lcschid( void* simcalhit ) {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>(simcalhit);
  return hit->id() ;
}

int lcschgetcellid0( void* simcalhit )  {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
  return hit->getCellID0() ;
}

int lcschgetcellid1( void* simcalhit )  {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
  return hit->getCellID1() ;
}

float lcschgetenergy( void* simcalhit )  {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
  return hit->getEnergy() ;
}

int lcschgetnmccontributions( void* simcalhit )  {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
  return hit->getNMCContributions() ;
}

const void* lcschgetparticlecont( void* simcalhit, int i)  {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
  return hit->getParticleCont( i ) ;
}

float lcschgetenergycont( void* simcalhit, int i)  {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
  return hit->getEnergyCont( i ) ;
}

float lcschgettimecont( void* simcalhit, int i)  {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
  return hit->getTimeCont( i ) ;
}

int lcschgetpdgcont( void* simcalhit, int i)  {
  SimCalorimeterHitImpl* hit = static_cast<SimCalorimeterHitImpl*>( simcalhit ) ;
  return hit->getPDGCont( i ) ;
}

// set,add Methods

// int lcschsetcellid0( PTRTYPE simcalhit, int id0) {
//   SimCalorimeterHitImpl* hit = f2c_pointer<SimCalorimeterHitImpl,LCObject>( simcalhit ) ;
//   hit->setCellID0( id0 ) ;
//   return  LCIO::SUCCESS ;
// }
// int lcschsetcellid1( PTRTYPE simcalhit, int id1) {
//   SimCalorimeterHitImpl* hit = f2c_pointer<SimCalorimeterHitImpl,LCObject>( simcalhit ) ;
//   hit->setCellID1( id1 ) ;
//   return  LCIO::SUCCESS ;
// }
// int lcschsetenergy( PTRTYPE simcalhit, float en) {
//   SimCalorimeterHitImpl* hit = f2c_pointer<SimCalorimeterHitImpl,LCObject>( simcalhit ) ;
//   hit->setEnergy( en ) ;
//   return  LCIO::SUCCESS ;
// }
// int lcschsetposition( PTRTYPE simcalhit, float pos[3])  {
//   SimCalorimeterHitImpl* hit = f2c_pointer<SimCalorimeterHitImpl,LCObject>( simcalhit ) ;
//   hit->setPosition( pos ) ;
//   return  LCIO::SUCCESS ;
// }
// int lcschaddmcparticlecontribution( PTRTYPE simcalhit, PTRTYPE mcparticle, float en, float t, int pdg ) {
//   SimCalorimeterHitImpl* hit = f2c_pointer<SimCalorimeterHitImpl,LCObject>( simcalhit ) ;
//   MCParticleImpl* mcp = f2c_pointer<MCParticleImpl,LCObject>( mcparticle ) ;
//   hit->addMCParticleContribution( mcp, en, t, pdg ) ;
//   return  LCIO::SUCCESS ;
// }
}
