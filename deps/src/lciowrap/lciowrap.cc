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

using namespace std;

std::string open() {
    IOIMPL::LCFactory::getInstance()->createLCReader();
}

JULIA_CPP_MODULE_BEGIN(registry)
    cxx_wrap::Module& lciowrap = registry.create_module("lciowrap");
    lciowrap.method("open", &open);
JULIA_CPP_MODULE_END
