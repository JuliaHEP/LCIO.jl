#include "lcio.h"
#include "IMPL/LCRelationImpl.h"
#include <iostream>

using namespace lcio ;

extern "C" {

 void* lcrelcreate( void* objectfrom, void* objectto, float weight ){
   LCObject* objf      = static_cast<LCObject*>( objectfrom ) ;
   LCObject* objt      = static_cast<LCObject*>( objectto ) ;
	if (not objf or not objt) {
	return 0;
	}
   return new LCRelationImpl( objf, objt, weight ) ;
 }

 int lcreldelete( void* relation ){
   LCRelationImpl* rel = static_cast<LCRelationImpl*>( relation ) ;
   delete rel ;
   return LCIO::SUCCESS ;
 }

int lcrelid( void* relation ){
  LCRelationImpl* rel = static_cast<LCRelationImpl*>( relation ) ;
  return rel->id() ;
}

void* lcrelgetfrom( void* relation ){
  LCRelationImpl* rel = static_cast<LCRelationImpl*>( relation ) ;
  LCObject* object    = rel->getFrom() ;
  return object;
}

void* lcrelgetto( void* relation ){
  LCRelationImpl* rel = static_cast<LCRelationImpl*>( relation ) ;
  LCObject* object    = rel->getTo() ;
  return object;
}

float lcrelgetweight( void* relation ){
  LCRelationImpl* rel = static_cast<LCRelationImpl*>( relation ) ;
  return rel->getWeight() ;
}

void lcrelsetfrom(void* relation, void* object ){
  LCRelationImpl* rel = static_cast<LCRelationImpl*>( relation ) ;
  LCObject* obj       = static_cast<LCObject*>( object ) ;
  rel->setFrom( obj ) ;
}

void lcrelsetto(void* relation, void* object ){
  LCRelationImpl* rel = static_cast<LCRelationImpl*>( relation ) ;
  LCObject* obj       = static_cast<LCObject*>( object ) ;
  rel->setTo( obj ) ;
}

void lcrelsetweight(void* relation, float weight ){
  LCRelationImpl* rel = static_cast<LCRelationImpl*>( relation ) ;
  rel->setWeight( weight ) ;
}
}
