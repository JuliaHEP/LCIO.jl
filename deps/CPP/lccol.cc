#include "lcio.h"
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/CalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/TPCHit.h"
#include "EVENT/LCIO.h"

#include <iostream>

using namespace lcio ;
using namespace std ;


extern "C" {

// void* lccolcreate( const char* colname ){
//   return new LCCollectionVec( colname ) ;
// }

int lccoldelete( void* collection ){
  LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
  if (not col) {
	return LCIO::ERROR;
  }
  delete col ;
  return LCIO::SUCCESS ;
}

int lccolgetnumberofelements( void* collection ){
  LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
	if (not col) {
	return 0;
  }
  return col->getNumberOfElements();
}

void* lccolgetelementat( void* collection, int index ){
  LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
  if (not col) {
	return 0;
  }
  return col->getElementAt(index);
}

const char* lccolgettypename( void* collection ){
  const LCCollectionVec* col = static_cast<LCCollectionVec*>(collection);
  if (not col) {
	return "";
  }
  return col->getTypeName().c_str();
}


int lccolgetflag(void* collection){
   LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
   if (not col) {
	return -1;
   }
   return col->getFlag();
}

bool lccolistransient(void* collection){
   LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
   return col->isTransient() ;
}

bool lccolisdefault(void* collection){
   LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
   return col->isDefault() ;
}

int lccoladdelement(void* collection, void* object){
   try{
     LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
     col->addElement( static_cast<LCObject*>(object) ) ;
   }catch(...){ return LCIO::ERROR ; }
   return LCIO::SUCCESS ;
}

int lccolremoveelementat(void* collection, int i){
   try{
     LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
     col->removeElementAt( i ) ;
   }catch(...){ return LCIO::ERROR ; }
   return LCIO::SUCCESS ;
 }

}
