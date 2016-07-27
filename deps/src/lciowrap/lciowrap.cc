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

inline IO::LCReader*
createReader() {
    return IOIMPL::LCFactory::getInstance()->createLCReader();
}

inline void
deleteReader(IO::LCReader* reader) {
    delete reader;
}

inline void
openFile(IO::LCReader* reader, const std::string& filename) {
    reader->open(filename);
}

inline EVENT::LCEvent*
readNextEvent(IO::LCReader* reader) {
    return reader->readNextEvent();
}

inline void
closeFile(IO::LCReader* reader) {
    reader->close();
}

// This is just a simple wrapper class around the lccollection pointer
// This will be constructed together with the type, which can be inferred from
// the collection name parameter
template<typename T>
struct TypedCollection
{
    EVENT::LCCollection* m_coll;
    TypedCollection(EVENT::LCCollection* collection) {
        m_coll = collection;
    }
    inline T* getElementAt(size_t i) {
        return static_cast<T*>(m_coll->getElementAt(i));
    }
    inline size_t getNumberOfElements() {
        return m_coll->getNumberOfElements();
    }
    inline EVENT::LCCollection*
    coll() {
        return m_coll;
    }
};

JULIA_CPP_MODULE_BEGIN(registry)
    cxx_wrap::Module& lciowrap = registry.create_module("LCIO");
    lciowrap.add_type<std::vector<std::string>>("StringVector");

    lciowrap.add_type<EVENT::LCObject>("LCObject");

    lciowrap.add_type<EVENT::SimCalorimeterHit>("SimCalorimeterHit")
        .method("getEnergy", &EVENT::SimCalorimeterHit::getEnergy);
    lciowrap.method("getP3", [](EVENT::SimCalorimeterHit* hit, ArrayRef<double> x) {
            const float* p = hit->getPosition();
            x[0] = p[0];
            x[1] = p[1];
            x[2] = p[2];
        });

    lciowrap.add_type<EVENT::SimTrackerHit>("SimTrackerHit");

    lciowrap.add_type<EVENT::CalorimeterHit>("CalorimeterHit");

    lciowrap.add_type<EVENT::TrackerHit>("TrackerHit");

    lciowrap.add_type<EVENT::MCParticle>("MCParticle");

    lciowrap.add_type<EVENT::LCRelation>("LCRelation");

    lciowrap.add_type<EVENT::Vertex>("Vertex");

    lciowrap.add_type<EVENT::Track>("Track");

    lciowrap.add_type<EVENT::LCGenericObject>("LCGenericObject");

    lciowrap.add_type<EVENT::ReconstructedParticle>("ReconstructedParticle");

    // most of the functionality is forwarded to the TypedCollection
    lciowrap.add_type<EVENT::LCCollection>("LCCollection")
        .method("getNumberOfElements", &EVENT::LCCollection::getNumberOfElements)
        .method("getElementAt", &EVENT::LCCollection::getElementAt);
    // FIXME This is a workaround for a bug that prevents const string& return types
    lciowrap.method("getTypeName", [](EVENT::LCCollection* c){
        return string(c->getTypeName());
    });

    lciowrap.add_type<EVENT::LCEvent>("LCEvent")
        .method("getEventCollection", &EVENT::LCEvent::getCollection);
    // .method("getCollectionNames", &EVENT::LCEvent::getCollectionNames);
    //
    lciowrap.add_type<IO::LCReader>("LCReader")
      .method("getNumberOfEvents", &IO::LCReader::getNumberOfEvents);
        // .method("open", &IO::LCReader::open)
        // .method("readNextEvent", &IO::LCReader::readNextEvent)
        // .method("close", &IO::LCReader::close);
    lciowrap.method("createLCReader", &createReader);
    lciowrap.method("deletLCReader", &deleteReader);
    lciowrap.method("openFile", &openFile);
    lciowrap.method("readNextEvent", &readNextEvent);
    lciowrap.method("closeFile", &closeFile);


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
        typedef typename decltype(wrapped)::type WrappedColl;
        wrapped.template constructor<EVENT::LCCollection*>();
        wrapped.method("getElementAt", &WrappedColl::getElementAt);
        wrapped.method("getNumberOfElements", &WrappedColl::getNumberOfElements);
        wrapped.method("coll", &WrappedColl::coll);
    });

    lciowrap.add_type<UTIL::BitField64>("BitField64")
        .constructor<const string&>()
        .method("getValue", &UTIL::BitField64::getValue);

    lciowrap.add_type<Parametric<TypeVar<1>>>("CellIDDecoder")
        .apply<UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>
             , UTIL::CellIDDecoder<EVENT::CalorimeterHit>
             , UTIL::CellIDDecoder<EVENT::TrackerHit>
             , UTIL::CellIDDecoder<EVENT::SimTrackerHit>>([](auto wrapped)
    {
        typedef typename decltype(wrapped)::type WrappedT;
        wrapped.template constructor<const EVENT::LCCollection*>();
        // wrapped.template constructor<const string&>();
        wrapped.method("get", &WrappedT::operator());
    });

JULIA_CPP_MODULE_END
