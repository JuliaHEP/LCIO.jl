#include "lcio.h" 
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include <iostream>

using namespace lcio ;

extern "C" {
void* lcwrtcreate(){
  return LCFactory::getInstance()->createLCWriter() ;
}

int lcwrtdelete(void* writer){
  LCWriter* lcWriter = static_cast<LCWriter*>(writer)  ;
  delete lcWriter ;
  return LCIO::SUCCESS ;
}

int lcwrtopen(void* writer, const char* filename, int writeMode ){
  try{ 
    LCWriter* wrt = static_cast<LCWriter*>(writer); 
    wrt->open( filename , writeMode );
    
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}


int lcwrtclose(void* writer){
  try{
    LCWriter* wrt = static_cast<LCWriter*>(  writer ) ; 
    wrt->close() ;
    }catch(...){ return LCIO::ERROR ; }

  return LCIO::SUCCESS ;
}

int lcwrtwriterunheader( void* writer, void* header){
  try{
    LCWriter* wrt = static_cast<LCWriter*>(  writer ) ; 
    LCRunHeader* hdr = static_cast<LCRunHeader*>(  header ) ; 
    wrt->writeRunHeader( hdr ) ;
  }catch(...){ return LCIO::ERROR ; }
  return LCIO::SUCCESS ;
}

int lcwrtwriteevent( void* writer, void* event){
  try{
    LCWriter* wrt = static_cast<LCWriter*>(  writer ) ; 
    LCEvent* evt = static_cast<LCEvent*>( event ) ; 
    
    wrt->writeEvent( evt ) ;
    
  }catch(...){ return LCIO::ERROR ; }
  
  return LCIO::SUCCESS ;
}
}
