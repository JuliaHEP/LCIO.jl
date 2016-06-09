#include "lcio.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


using namespace lcio ;

extern "C" {

void* lcevtcreate(){
  LCEventImpl*  event = new LCEventImpl() ;
  return event ;
}

int lcevtdelete( void* event ){
  LCEventImpl* evt = static_cast<LCEventImpl*>(event);
  delete evt ;
  return LCIO::SUCCESS ;
}

int lcevtgetrunnumber( void* event ){
  LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
  return evt->getRunNumber()  ;
}

int lcevtgeteventnumber( void* event ){
  LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
  return evt->getEventNumber()  ;
}

const char* lcevtgetdetectorname( void* event ){
  LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
  return evt->getDetectorName().c_str();
}

int lcevtgetcollectionnamecount( void* event ) {
    LCEventImpl* evt = static_cast<LCEventImpl*>(event);
    const std::vector<std::string>* nameVec = evt->getCollectionNames();
    return nameVec->size();
}

const char* lcevtgetcollectionname(void* event, size_t i) {
    LCEventImpl* evt = static_cast<LCEventImpl*>(event);
    const std::vector<std::string>* nameVec = evt->getCollectionNames();
    return nameVec->at(i).c_str();
}


long lcevtgettimestamp( void* event )
{
  LCEventImpl* evt = static_cast<LCEventImpl*>(event);
  return evt->getTimeStamp();
}

void* lcevtgetcollection(void* event, const char* colname){
  try{
    LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
    return evt->getCollection(colname);
  }catch(...){ return 0 ;}
}


 int lcevtaddcollection( void* event, void* collection, const char* colname ){
   try{
     LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
     LCCollectionVec* col = static_cast<LCCollectionVec*>(collection) ;
     evt->addCollection( col , colname ) ;
   }catch(...){ return LCIO::ERROR ; }
   return LCIO::SUCCESS ;
 }


 int lcevtsetrunnumber( void* event, int rn ){
   LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
	if (not evt) {
	return LCIO::ERROR;
	}
   evt->setRunNumber( rn ) ;
   return LCIO::SUCCESS ;
 }

 int lcevtseteventnumber( void* event, int en ){
   LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
    if (not evt) {
	return LCIO::ERROR;
	}
   evt->setEventNumber( en ) ;
   return LCIO::SUCCESS ;
 }

 // int lcevtsetdetectorname( void* event,  char* dn ){
 //   LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
 //   evt->setDetectorName( dn ) ;
 //   return LCIO::SUCCESS ;
 // }

 int lcevtsettimestamp( void* event,  long ts ){
   LCEventImpl* evt = static_cast<LCEventImpl*>(event) ;
   evt->setTimeStamp( ts ) ;
   return LCIO::SUCCESS ;
 }
}
