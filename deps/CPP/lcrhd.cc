#include "lcio.h" 
#include "Exceptions.h"
#include "IOIMPL/LCFactory.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCTOOLS.h"
#include <iostream>

using namespace lcio ;

extern "C" {

void* lcrhdcreate(){
  return new LCRunHeaderImpl ;
}

int lcrhddelete( void* runHeader ){
  LCRunHeaderImpl* rhd = static_cast<LCRunHeaderImpl*>(runHeader) ;
  delete rhd ;
  return LCIO::SUCCESS ;
}

int lcrhdgetrunnumber( void* runHeader ){
  LCRunHeaderImpl* rhd = static_cast<LCRunHeaderImpl*>(runHeader) ;
	if (not rhd) {
	return -1;
	}
  return rhd->getRunNumber() ;
}

const char* lcrhdgetdetectorname( void* runHeader  ){
  LCRunHeaderImpl* rhd = static_cast<LCRunHeaderImpl*>(runHeader) ;
	if (not rhd) {
	return "";
	}
  return rhd->getDetectorName().c_str();
}

const char* lcrhdgetdescription( void* runHeader ){
  LCRunHeaderImpl* rhd = static_cast<LCRunHeaderImpl*>(runHeader) ;
	if (not rhd) {
	return "";
	}
  return  rhd->getDescription().c_str();
}

void* lcrhdgetactivesubdetectors(void* runHeader){
  LCRunHeaderImpl* rhd = static_cast<LCRunHeaderImpl*>(runHeader) ;
 	if (not rhd) {
	return 0;
}
  return rhd->getActiveSubdetectors();
}
}


