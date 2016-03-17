#include "lcio.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/SimTrackerHitImpl.h"
#include "IMPL/LCTOOLS.h"
#include "lcvecutil.h"
#include <iostream>

using namespace lcio ;
using namespace std ;

extern "C" {
 void* lcsthcreate(){
   return new SimTrackerHitImpl ;
 }

 int lcsthdelete( void* hit ){
   SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit ) ;
   delete sth ;
   return LCIO::SUCCESS ;
 }


int lcsthgetcellid( void* hit ){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit );
  if (not sth) {
      return 0;
  }
  return sth->getCellID() ;
}

int lcsthgetcellid0( void* hit ){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit ) ;
  if (not sth) {
      return 0;
  }
  return sth->getCellID0() ;
}

int lcsthgetcellid1( void* hit ){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit ) ;
  if (not sth) {
      return 0;
  }
  return sth->getCellID1() ;
}

ThreeVec lcsthgetpositionvec( void* hit ){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit ) ;
  if (not sth) {
      return ThreeVec{0,0,0};
  }
  const double* pos = sth->getPosition();
  ThreeVec v;
  v.x = pos[0];
  v.y = pos[1];
  v.z = pos[2];
  return v;
}

const double* lcsthgetposition( void* hit ){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit ) ;
  if (not sth) {
      return 0;
  }
  return sth->getPosition();
}


ThreeVec lcsthgetmomentum( void* hit){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit ) ;
  if (not sth) {
      return ThreeVec{0,0,0};
  }
  const float* mom = sth->getMomentum();
  ThreeVec v;
  v.x = mom[0];
  v.y = mom[1];
  v.z = mom[2];
  return v;
}

float lcsthgetpathlength( void* hit ){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit );
  if (not sth) {
      return 0;
  }
  return sth->getPathLength() ;
}

float lcsthgetedep( void* hit ){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit ) ;
  if (not sth) {
      return 0;
  }
  return sth->getEDep() ;
}

float lcsthgettime( void* hit ){
  SimTrackerHitImpl* sth = static_cast<SimTrackerHitImpl*>( hit ) ;
  if (not sth) {
      return 0;
  }
  return sth->getTime() ;
}


}
