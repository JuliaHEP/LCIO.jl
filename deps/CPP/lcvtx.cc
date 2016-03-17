#include "lcio.h"
#include "lcvecutil.h"
#include "IMPL/VertexImpl.h"
#include "IMPL/ReconstructedParticleImpl.h"

using namespace lcio ;

extern "C" {

// create delete Vertex

void* lcvtxcreate(){
   return new VertexImpl ;
}

 int lcvtxdelete( void* vertex ){
   VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
   delete vtx ;
   return LCIO::SUCCESS ;
 }


// get Methods

int lcvtxid( void* vertex ) {
  VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
  return vtx->id() ;
}

bool lcvtxisprimary ( void* vertex ) {
  VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
  return vtx->isPrimary() ;
}

const char* lcvtxgetalgorithmtype ( void* vertex ) {
  VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
  return  vtx->getAlgorithmType().c_str() ;
}

float lcvtxgetchi2( void* vertex ) {
  VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
  return vtx->getChi2() ;
}

float lcvtxgetprobability( void* vertex ) {
  VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
  return vtx->getProbability() ;
}

ThreeVec lcvtxgetposition( void* vertex ) {
  VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
  const float* p = vtx->getPosition();
  return ThreeVec{p[0], p[1], p[2]};
}

const float* lcvtxgetcovmatrix( void* vertex )  {
  VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
  return vtx->getCovMatrix() ;
}

// return is address of floatvec. the number of elements is modified as input argument
const float* lcvtxgetparameters( void* vertex, size_t* nvec ) {
  VertexImpl* vtx = static_cast<VertexImpl*>( vertex ) ;
  const FloatVec& floatVec = vtx->getParameters() ;
  *nvec = floatVec->size();
  return &floatVec[0];
}

void* lcvtxgetassociatedparticle( void* vertex ) {
  Vertex* vtx = static_cast<Vertex*>( vertex ) ;
  ReconstructedParticle* recP = vtx->getAssociatedParticle();
  return recP ;
}
}
