#include "lcio.h"
#include "IMPL/ClusterImpl.h"

using namespace lcio ;

#include <iostream>


extern "C" {

// create delete Cluster

 void* lcclucreate(){
   return new ClusterImpl;
 }

 int lccludelete( void* cluster ){
   ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
   delete clu ;
   return LCIO::SUCCESS ;
 }

// get Methods

int lccluid( void* cluster )  {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return clu->id() ;
}

int lcclugettype( void* cluster )  {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return clu->getType() ;
}

float lcclugetenergy( void* cluster ) {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return clu->getEnergy() ;
}

float lcclugetenergyerr( void* cluster )  {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return clu->getEnergyError() ;
}

const float* lcclugetposition( void* cluster )  {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return clu->getPosition();
}

const float* lcclugetpositionerror( void* cluster )  {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return &clu->getPositionError()[0];
}

float lcclugetitheta( void* cluster ) {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return clu->getITheta() ;
}

float lcclugetiphi( void* cluster ) {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return clu->getIPhi() ;
}

const float* lcclugetdirectionerror( void* cluster)  {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return &clu->getDirectionError()[0];
}

const float* lcclugetshape( void* cluster )  {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return &clu->getShape()[0];
}

const void* lcclugetparticleids( void* cluster ) {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return &clu->getParticleIDs();
}

const void* lcclugetclusters( void* cluster ) {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return &clu->getClusters();
}

const void* lcclugetcalorimeterhits( void* cluster ) {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return &clu->getCalorimeterHits();
}

const void* lcclugetsubdetectorenergies( void* cluster ) {
  ClusterImpl* clu = static_cast<ClusterImpl*>( cluster ) ;
  return &clu->getSubdetectorEnergies();
}
}
