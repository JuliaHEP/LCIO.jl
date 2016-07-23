#include <cxx_wrap.hpp>
#include <string>
#include <vector>
#include <iostream>

#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCCollection.h"
#include "EVENT/MCParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/Track.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/Vertex.h"
#include "EVENT/LCRelation.h"
#include "EVENT/LCGenericObject.h"
#include "UTIL/CellIDDecoder.h"
#include "UTIL/BitField64.h"

using namespace std;
using namespace cxx_wrap;
IO::LCReader* createReader() {
    return IOIMPL::LCFactory::getInstance()->createLCReader();
}

JULIA_CPP_MODULE_BEGIN(registry)
    cxx_wrap::Module& lciowrap = registry.create_module("lciowrap");
    lciowrap.method("createReader", &createReader);
    
    lciowrap.add_type<EVENT::SimCalorimeterHit>("SimCalorimeterHit")
      .method("getEnergy", &EVENT::SimCalorimeterHit::getEnergy);
    
    lciowrap.add_type<EVENT::LCCollection>("LCCollection");

    lciowrap.add_type<UTIL::BitField64>("BitField64")
      .constructor<const string&>();
    
    lciowrap.add_type<Parametric<TypeVar<1>>>("CellIDDecoder")
      .apply<UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>>([](auto wrapped)
    {
      // wrapped.constructor<const string&>();
      typedef typename decltype(wrapped)::type WrappedT;
      // wrapped.constructor<const EVENT::LCCollection*>();
      // wrapped.method("find", &WrappedT::operator());
    });

JULIA_CPP_MODULE_END
