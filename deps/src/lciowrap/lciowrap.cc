#include <cxx_wrap.hpp>
#include <string>
#include <vector>
#include <iostream>

#include "EVENT/Cluster.h"
#include "EVENT/LCCollection.h"
#include "EVENT/LCEvent.h"
#include "EVENT/LCGenericObject.h"
#include "EVENT/LCRelation.h"
#include "EVENT/MCParticle.h"
#include "EVENT/ParticleID.h"
#include "EVENT/RawCalorimeterHit.h"
#include "EVENT/ReconstructedParticle.h"
#include "EVENT/SimCalorimeterHit.h"
#include "EVENT/SimTrackerHit.h"
#include "EVENT/Track.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/Vertex.h"
#include "IO/LCReader.h"
#include "IOIMPL/LCFactory.h"
#include "UTIL/BitField64.h"
#include "UTIL/CellIDDecoder.h"

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
    lciowrap.add_type<vector<string>>("StringVec")
        .method("size", &EVENT::StringVec::size);
    lciowrap.method("at", [](const vector<string>* vec, size_t i) {
        return vec->at(i);
    });
    lciowrap.add_type<vector<float>>("FloatVec")
        .method("size", &EVENT::FloatVec::size);
    lciowrap.method("at", [](const vector<float>* vec, size_t i) {
        return vec->at(i);
    });
    lciowrap.add_type<vector<int>>("IntVec")
        .method("size", &EVENT::IntVec::size);
    lciowrap.method("at", [](const vector<int>* vec, size_t i) {
        return vec->at(i);
    });

    lciowrap.add_type<EVENT::LCParameters>("LCParameters")
        .method("getIntVal", &EVENT::LCParameters::getIntVal)
        .method("getFloatVal", &EVENT::LCParameters::getFloatVal)
        .method("getStringVal", &EVENT::LCParameters::getStringVal)
        .method("getIntVals", &EVENT::LCParameters::getIntVals)
        .method("getFloatVals", &EVENT::LCParameters::getFloatVals)
        .method("getStringVals", &EVENT::LCParameters::getStringVals)
        .method("getIntKeys", &EVENT::LCParameters::getStringKeys)
        .method("getFloatKeys", &EVENT::LCParameters::getFloatKeys)
        .method("getStringKeys", &EVENT::LCParameters::getStringKeys)
        .method("getNInt", &EVENT::LCParameters::getNInt)
        .method("getNFloat", &EVENT::LCParameters::getNFloat)
        .method("getNString", &EVENT::LCParameters::getNString);

    lciowrap.add_type<EVENT::LCRunHeader>("LCRunHeader")
        .method("getRunNumber", &EVENT::LCRunHeader::getRunNumber)
        .method("getDetectorName", &EVENT::LCRunHeader::getDetectorName)
        .method("getDescription", &EVENT::LCRunHeader::getDescription)
        .method("getParameters", &EVENT::LCRunHeader::getParameters);

    lciowrap.add_type<EVENT::LCObject>("LCObject");

    lciowrap.add_type<EVENT::ParticleID>("ParticleID")
        .method("getType", &EVENT::ParticleID::getType)
        .method("getPDG", &EVENT::ParticleID::getPDG)
        .method("getLikelihood", &EVENT::ParticleID::getLikelihood)
        .method("getAlgorithmType", &EVENT::ParticleID::getAlgorithmType)
        .method("getParameters", &EVENT::ParticleID::getParameters);
    lciowrap.add_type<EVENT::ParticleIDVec>("ParticleIDVec")
        .method("size", &EVENT::ParticleIDVec::size);
    lciowrap.method("at", [](const EVENT::ParticleIDVec& vec, size_t i) {
        return vec.at(i);
    });

    #include "MCParticle.icc"

    #include "CalorimeterHitTypes.icc"

    lciowrap.add_type<EVENT::SimTrackerHit>("SimTrackerHit");

    lciowrap.add_type<EVENT::TrackerHit>("TrackerHit");
    lciowrap.add_type<EVENT::TrackerHitVec>("TrackerHitVec")
        .method("size", &EVENT::TrackerHitVec::size);
    lciowrap.method("at", [](const EVENT::TrackerHitVec& vec, size_t i) {
        return vec.at(i);
    });

    lciowrap.add_type<EVENT::LCRelation>("LCRelation")
        .method("getFrom", &EVENT::LCRelation::getFrom)
        .method("getTo", &EVENT::LCRelation::getTo)
        .method("getWeight", &EVENT::LCRelation::getWeight);

    lciowrap.add_type<EVENT::Vertex>("Vertex")
        .method("isPrimary", &EVENT::Vertex::isPrimary)
        .method("getAlgorithmType", &EVENT::Vertex::getAlgorithmType)
        .method("getChi2", &EVENT::Vertex::getChi2)
        .method("getProbability", &EVENT::Vertex::getProbability)
        .method("getCovMatrix", &EVENT::Vertex::getCovMatrix)
        .method("getParameters", &EVENT::Vertex::getParameters);
    lciowrap.method("getPosition3", [](const EVENT::Vertex* v, ArrayRef<double> x)->bool {
        const float* p3 = v->getPosition();
        if (not p3) {return false;}
        x[0] = p3[0];
        x[1] = p3[1];
        x[2] = p3[2];
        return true;
    });

    #include "Track.icc"
    #include "Cluster.icc"

    lciowrap.add_type<EVENT::LCGenericObject>("LCGenericObject");

    #include "ReconstructedParticle.icc"

    // most of the functionality is forwarded to the TypedCollection
    lciowrap.add_type<EVENT::LCCollection>("LCCollection")
        .method("getNumberOfElements", &EVENT::LCCollection::getNumberOfElements)
        .method("getElementAt", &EVENT::LCCollection::getElementAt)
        .method("getTypeName", &EVENT::LCCollection::getTypeName);

    lciowrap.add_type<EVENT::LCEvent>("LCEvent")
        .method("getEventCollection", &EVENT::LCEvent::getCollection)
        .method("getCollectionNames", &EVENT::LCEvent::getCollectionNames)
        .method("getDetectorName", &EVENT::LCEvent::getDetectorName)
        .method("getEventNumber", &EVENT::LCEvent::getEventNumber)
        .method("getRunNumber", &EVENT::LCEvent::getRunNumber);
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
