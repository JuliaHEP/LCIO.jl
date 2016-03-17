#include "lcio.h"
#include "lcvecutil.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "lcvecutil.h"

using namespace lcio ;

extern "C" {

 void* lccahcreate(){
   return new CalorimeterHitImpl ;
 }

 int lccahdelete( void* calhit ) {
   CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
   delete hit ;
   return LCIO::SUCCESS ;
 }

CalHit lccahgetp4(void* hit)  {
    CalorimeterHitImpl* h = static_cast<CalorimeterHitImpl*>( hit ) ;
    CalHit c;
    const float* p = h->getPosition();
    c.x = p[0];
    c.y = p[1];
    c.z = p[2];
    c.E = h->getEnergy();
    return c;
}

int lccahid( void* calhit )  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return -1;
	}
  return hit->id() ;
}

int lccahgetcellid0( void* calhit )  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return -1;
	}
  return hit->getCellID0() ;
}

int lccahgetcellid1( void* calhit )  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return -1;
	}
  return hit->getCellID1() ;
}

float lccahgetenergy( void* calhit )  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return -1;
	}
  return hit->getEnergy() ;
}

float lccahgetenergyerr( void* calhit )  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return -1;
	}
  return hit->getEnergyError() ;
}

float lccahgettime( void* calhit )  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return -1;
	}
  return hit->getTime() ;
}

const float* lccahgetposition( void* calhit)  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return 0;
	}
  return hit->getPosition() ;
}

int lccahgettype( void* calhit )  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return -1;
	}
  return hit->getType() ;
}

void* lccahgetrawhit( void* calhit )  {
  CalorimeterHitImpl* hit = static_cast<CalorimeterHitImpl*>( calhit ) ;
	if (not hit) {
	return 0;
	}
  return hit->getRawHit();
}

}
