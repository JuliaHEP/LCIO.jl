#include "lcio.h"
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;

extern "C" {
void* lcrdrcreate(){
  LCReader* lcReader = IOIMPL::LCFactory::getInstance()->createLCReader();
  return lcReader;
}

int lcrdrdelete(void* reader){
  LCReader* lcReader = static_cast<LCReader*>(reader);
  delete lcReader;
  return LCIO::SUCCESS;
}

int lcrdropen(void* reader, const char* filename ){
  try {
    LCReader* lcReader = static_cast<LCReader*>(reader);
    lcReader->open(filename);
  } catch(...) {
      return LCIO::ERROR;
  }
  return LCIO::SUCCESS;
}

int lcrdrclose(void* reader) {
  try {
    LCReader* lcReader = static_cast<LCReader*>(reader);
    lcReader->close() ;
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

void* lcrdrreadnextrunheader(void* reader, int accessMode){
   LCReader* rdr = static_cast<LCReader*>(reader) ;
    if (not rdr) {
	return 0;
    }
   return rdr->readNextRunHeader( accessMode ) ;
 }

void* lcrdrreadnextevent(void* reader)
{
  LCReader* rdr = static_cast<LCReader*>(reader) ;
  return rdr->readNextEvent();
}

void* lcrdrreadevent(void* reader, int runNumber, int evtNumber ){
   LCReader* rdr = static_cast<LCReader*>(reader) ;
	if (not rdr) {
	return 0;
}
   return rdr->readEvent( runNumber, evtNumber  );
 }

 int lcrdrskipnevents( void* reader, int n ) {
   LCReader* rdr = static_cast<LCReader*>(reader) ;
	if (not rdr) {
	return LCIO::ERROR;
	}
   rdr->skipNEvents( n ) ;
   return LCIO::SUCCESS ;
 }
}
