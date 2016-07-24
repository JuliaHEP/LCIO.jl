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

void deleteReader(IO::LCReader* reader) {
    delete reader;
}

// This is just a simple wrapper class around the lccollection pointer
// This will be constructed together with the type, which can be inferred from
// the collection name parameter
template<typename T>
struct TypedCollection
{
    TypedCollection(EVENT::LCCollection* collection) {
        m_coll = collection;
    }
    EVENT::LCCollection* m_coll;
    inline T* getElementAt(size_t i) {
        return static_cast<T*>(m_coll->getElementAt(i));
    }
    inline size_t getNumberOfElements() {
        return m_coll->getNumberOfElements();
    }
};

JULIA_CPP_MODULE_BEGIN(registry)
    cxx_wrap::Module& lciowrap = registry.create_module("lciowrap");
    lciowrap.method("createLCReader", &createReader);
    lciowrap.method("deletLCReader", &deleteReader);

    // most of the functionality is forwarded to the TypedCollection
    lciowrap.add_type<EVENT::LCCollection>("LCCollection")
        .method("getNumberOfElements", &EVENT::LCCollection::getNumberOfElements)
        .method("getElementAt", &EVENT::LCCollection::getElementAt)
        .method("getTypeName", &EVENT::LCCollection::getTypeName);

    lciowrap.add_type<Parametric<TypeVar<1>>>("TypedCollection")
        .apply<TypedCollection<EVENT::SimCalorimeterHit>
             , TypedCollection<EVENT::CalorimeterHit>
             , TypedCollection<EVENT::MCParticle>
             , TypedCollection<EVENT::ReconstructedParticle>
             , TypedCollection<EVENT::TrackerHit>
             , TypedCollection<EVENT::SimTrackerHit>
             , TypedCollection<EVENT::LCRelation>
             , TypedCollection<EVENT::LCGenericObject>
             , TypedCollection<EVENT::Track>
             , TypedCollection<EVENT::Vertex>>([](auto wrapped)
        {
        typedef typename decltype(wrapped)::type WrappedT;
        wrapped.template constructor<EVENT::LCCollection*>();
        wrapped.method("getElementAt", &WrappedT::getElementAt);
        wrapped.method("getNumberOfElements", &WrappedT::getNumberOfElements);
    });
    lciowrap.add_type<EVENT::LCEvent>("LCEvent")
        .method("getCollection", &EVENT::LCEvent::getCollection)
        .method("getCollectionNames", &EVENT::LCEvent::getCollectionNames);

    lciowrap.add_type<IO::LCReader>("LCReader");
        // .method("open", &IO::LCReader::open)
        // .method("readNextEvent", &IO::LCReader::readNextEvent)
        // .method("close", &IO::LCReader::close);

    lciowrap.add_type<EVENT::SimCalorimeterHit>("SimCalorimeterHit")
        .method("getEnergy", &EVENT::SimCalorimeterHit::getEnergy);

    lciowrap.add_type<UTIL::BitField64>("BitField64")
        .constructor<const string&>()
        .method("getValue", &UTIL::BitField64::getValue);

    lciowrap.add_type<Parametric<TypeVar<1>>>("CellIDDecoder")
        .apply<UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>>([](auto wrapped)
    {
        typedef typename decltype(wrapped)::type WrappedT;
        wrapped.template constructor<const string&>();
        wrapped.template constructor<const EVENT::LCCollection*>();
        wrapped.method("get", &WrappedT::operator());
    });

JULIA_CPP_MODULE_END
