#include <string>
#include <vector>
#include <iostream>

#include "jlcxx/jlcxx.hpp"
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
#include "EVENT/TrackerData.h"
#include "EVENT/TrackerPulse.h"
#include "EVENT/TrackerRawData.h"
#include "EVENT/TrackerHit.h"
#include "EVENT/Vertex.h"
#include "IMPL/LCCollectionVec.h"
#include "IMPL/LCEventImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IO/LCReader.h"
#include "IO/LCWriter.h"
#include "IOIMPL/LCFactory.h"
#include "UTIL/BitField64.h"
#include "UTIL/CellIDDecoder.h"
#include "UTIL/LCRelationNavigator.h"
#include "UTIL/LCStdHepRdr.h"
#include "UTIL/LCTrackerConf.h"

using namespace std;
using namespace jlcxx;
using namespace EVENT;
using namespace IMPL;

// This is just a simple wrapper class around the lccollection pointer
// This will be constructed together with the type, which can be inferred from
// the collection name parameter
template<typename T>
struct TypedCollection
{
    LCCollection* m_coll;
    TypedCollection(LCCollection* collection) {
        m_coll = collection;
    }
    // TypedCollection(LCCollectionVec* collection) {
    //     m_coll = collection;
    // }
    inline T* getElementAt(size_t i) {
        return static_cast<T*>(m_coll->getElementAt(i));
    }
    inline size_t getNumberOfElements() {
        return m_coll->getNumberOfElements();
    }
    inline LCCollection* coll() {
        return m_coll;
    }
};


// This is just a functor to cast an LCObject to the right type
template<typename T>
struct CastOperator
{
    T* cast(LCObject* orig) {
        return static_cast<T*>(orig);
    }
};

namespace jlcxx
{
    template<> struct SuperType<LCEventImpl> { typedef LCEvent type; };
    template<> struct SuperType<LCCollectionVec> { typedef LCCollection type; };
    template<> struct SuperType<MCParticleImpl> { typedef MCParticle type; };
    template<> struct SuperType<LCRunHeaderImpl> { typedef LCRunHeader type; };
}


JULIA_CPP_MODULE_BEGIN(registry)
    jlcxx::Module& lciowrap = registry.create_module("LCIO");
    lciowrap.add_type<LCObject>("LCObject");
    lciowrap.add_type<vector<string>>("StringVec")
        .method("size", &StringVec::size);
    lciowrap.method("at", [](const vector<string>* vec, size_t i) {
        return vec->at(i);
    });
    lciowrap.add_type<vector<float>>("FloatVec")
        .method("size", &FloatVec::size);
    lciowrap.method("at", [](const vector<float>* vec, size_t i) {
        return vec->at(i);
    });
    lciowrap.add_type<vector<int>>("IntVec")
        .method("size", &IntVec::size);
    lciowrap.method("at", [](const vector<int>* vec, size_t i) {
        return vec->at(i);
    });
    lciowrap.add_type<vector<short>>("ShortVec")
        .method("size", &ShortVec::size);
    lciowrap.method("at", [](const vector<short>* vec, size_t i) {
        return vec->at(i);
    });

    lciowrap.add_type<LCParameters>("LCParameters")
        .method("getIntVal", &LCParameters::getIntVal)
        .method("getFloatVal", &LCParameters::getFloatVal)
        .method("getStringVal", &LCParameters::getStringVal)
        .method("getIntVals", &LCParameters::getIntVals)
        .method("getFloatVals", &LCParameters::getFloatVals)
        .method("getStringVals", &LCParameters::getStringVals)
        .method("getIntKeys", &LCParameters::getStringKeys)
        .method("getFloatKeys", &LCParameters::getFloatKeys)
        .method("getStringKeys", &LCParameters::getStringKeys)
        .method("getNInt", &LCParameters::getNInt)
        .method("getNFloat", &LCParameters::getNFloat)
        .method("getNString", &LCParameters::getNString);
    lciowrap.method("setValue", [](LCParameters* parms, const std::string& key, int value) {
        return parms->setValue(key, value);
    });
    lciowrap.method("setValue", [](LCParameters* parms, const std::string& key, float value) {
        return parms->setValue(key, value);
    });
    lciowrap.method("setValue", [](LCParameters* parms, const std::string& key, const std::string& value) {
        return parms->setValue(key, value);
    });
    // most of the functionality is forwarded to the TypedCollection
    lciowrap.add_type<LCCollection>("LCCollection")
        .method("getNumberOfElements", &LCCollection::getNumberOfElements)
        .method("getElementAt", &LCCollection::getElementAt)
        .method("getTypeName", &LCCollection::getTypeName)
        .method("getParameters", &LCCollection::getParameters);

    lciowrap.add_type<LCCollectionVec>("LCCollectionVec", jlcxx::julia_type<LCCollection>())
        .constructor<const string&>()
        .method("setTransient", &LCCollectionVec::setTransient);

    lciowrap.add_type<LCRunHeader>("LCRunHeader")
        .method("getRunNumber", &LCRunHeader::getRunNumber)
        .method("getDetectorName", &LCRunHeader::getDetectorName)
        .method("getDescription", &LCRunHeader::getDescription)
        .method("getParameters", &LCRunHeader::getParameters);

    lciowrap.add_type<LCRunHeaderImpl>("LCRunHeaderImpl", jlcxx::julia_type<LCRunHeader>())
        .method("setRunNumber", &LCRunHeaderImpl::setRunNumber)
        .method("setDetectorName", &LCRunHeaderImpl::setDetectorName)
        .method("setDescription", &LCRunHeaderImpl::setDescription)
        .method("parameters", &LCRunHeaderImpl::parameters);

    lciowrap.add_type<LCObjectVec>("LCObjectVec")
        .method("size", &LCObjectVec::size);
    lciowrap.method("at", [](const LCObjectVec& vec, size_t i) {
        return vec.at(i);
    });

    lciowrap.add_type<ParticleID>("ParticleID")
        .method("getType", &ParticleID::getType)
        .method("getPDG", &ParticleID::getPDG)
        .method("getLikelihood", &ParticleID::getLikelihood)
        .method("getAlgorithmType", &ParticleID::getAlgorithmType)
        .method("getParameters", &ParticleID::getParameters);
    lciowrap.add_type<ParticleIDVec>("ParticleIDVec")
        .method("size", &ParticleIDVec::size);
    lciowrap.method("at", [](const ParticleIDVec& vec, size_t i) {
        return vec.at(i);
    });

    #include "MCParticle.icc"
    #include "CalorimeterHitTypes.icc"
    #include "TrackerHitTypes.icc"

    lciowrap.add_type<LCRelation>("LCRelation")
        .method("getFrom", &LCRelation::getFrom)
        .method("getTo", &LCRelation::getTo)
        .method("getWeight", &LCRelation::getWeight);

    lciowrap.add_type<Vertex>("Vertex")
        .method("isPrimary", &Vertex::isPrimary)
        .method("getAlgorithmType", &Vertex::getAlgorithmType)
        .method("getChi2", &Vertex::getChi2)
        .method("getProbability", &Vertex::getProbability)
        .method("getCovMatrix", &Vertex::getCovMatrix)
        .method("getParameters", &Vertex::getParameters);
    lciowrap.method("getPosition3", [](const Vertex* v, ArrayRef<double> x)->bool {
        const float* p3 = v->getPosition();
        if (not p3) {return false;}
        x[0] = p3[0];
        x[1] = p3[1];
        x[2] = p3[2];
        return true;
    });

    #include "Track.icc"
    #include "Cluster.icc"

    lciowrap.add_type<LCGenericObject>("LCGenericObject")
        .method("getNInt", &LCGenericObject::getNInt)
        .method("getNFloat", &LCGenericObject::getNFloat)
        .method("getNDouble", &LCGenericObject::getNDouble)
        .method("getIntVal", &LCGenericObject::getIntVal)
        .method("getFloatVal", &LCGenericObject::getFloatVal)
        .method("getDoubleVal", &LCGenericObject::getDoubleVal)
        .method("isFixedSize", &LCGenericObject::isFixedSize)
        .method("getTypeName", &LCGenericObject::getTypeName)
        .method("getDataDescription", &LCGenericObject::getDataDescription)
        .method("id", &LCGenericObject::id);

    #include "ReconstructedParticle.icc"

    lciowrap.add_type<LCEvent>("LCEvent")
        .method("getEventCollection", &LCEvent::getCollection)
        .method("getCollectionNames", &LCEvent::getCollectionNames)
        .method("getDetectorName", &LCEvent::getDetectorName)
        .method("getEventNumber", &LCEvent::getEventNumber)
        .method("getRunNumber", &LCEvent::getRunNumber)
        .method("getParameters", &LCEvent::getParameters)
        .method("getWeight", &LCEvent::getWeight);

    lciowrap.add_type<LCEventImpl>("LCEventImpl", jlcxx::julia_type<LCEvent>())
        .method("setEventNumber", &LCEventImpl::setEventNumber);
    lciowrap.method("addCollection", [](LCEventImpl* event, LCCollectionVec* col, const std::string& name) {
        event->addCollection(col, name);
        // TODO this is necessary for the time being, otherwise the event tries to delete the collection, but the julia finalizers also try to kill the collection
        // the event tries to make the collection non-transient after takeCollection, but we may still want to write it out, so keep the state
        bool isTransient = col->isTransient();
        event->takeCollection(name);
        col->setTransient(isTransient);
    });

    lciowrap.add_type<IO::LCReader>("LCReader")
      .method("getNumberOfEvents", &IO::LCReader::getNumberOfEvents)
      .method("getNumberOfRuns", &IO::LCReader::getNumberOfRuns);
    lciowrap.method("readNextEvent", [](IO::LCReader* reader) {
        return reader->readNextEvent();
    });
    lciowrap.method("readNextRunHeader", [](IO::LCReader* reader) {
        return reader->readNextRunHeader();
    });
    lciowrap.method("createLCReader", [](){
        return IOIMPL::LCFactory::getInstance()->createLCReader();
    });
    lciowrap.method("deleteLCReader", [](IO::LCReader* reader){
        delete reader;
    });
    lciowrap.method("openFile", [](IO::LCReader* reader, const std::string& filename) {
        reader->open(filename);
    });
    lciowrap.method("closeFile", [](IO::LCReader* reader) {
        reader->close();
    });

    lciowrap.add_type<IO::LCWriter>("LCWriter")
        .method("setCompressionLevel", &IO::LCWriter::setCompressionLevel)
        .method("close", &IO::LCWriter::close)
        .method("flush", &IO::LCWriter::flush)
        .method("writeRunHeader", &IO::LCWriter::writeRunHeader)
        .method("writeEvent", &IO::LCWriter::writeEvent);
    lciowrap.method("open", [](IO::LCWriter* writer, const std::string& filename, int writeMode) {
        writer->open(filename, writeMode);
    });
    lciowrap.method("createLCWriter", [](){
        return IOIMPL::LCFactory::getInstance()->createLCWriter();
    });
    lciowrap.method("deleteLCWriter", [](IO::LCWriter* writer){
        delete writer;
    });


    lciowrap.add_type<Parametric<TypeVar<1>>>("TypedCollection")
        .apply<TypedCollection<CalorimeterHit>
             , TypedCollection<Cluster>
             , TypedCollection<MCParticle>
             , TypedCollection<LCGenericObject>
             , TypedCollection<LCRelation>
             , TypedCollection<ReconstructedParticle>
             , TypedCollection<SimCalorimeterHit>
             , TypedCollection<SimTrackerHit>
             , TypedCollection<Track>
             , TypedCollection<TrackerHit>
             , TypedCollection<TrackerRawData>
             , TypedCollection<Vertex>>([](auto wrapped)
        {
        typedef typename decltype(wrapped)::type WrappedColl;
        wrapped.template constructor<LCCollection*>();
        // wrapped.template constructor<LCCollectionVec*>();
        wrapped.method("getElementAt", &WrappedColl::getElementAt);
        wrapped.method("getNumberOfElements", &WrappedColl::getNumberOfElements);
        wrapped.method("coll", &WrappedColl::coll);
    });

    lciowrap.add_type<UTIL::BitField64>("BitField64")
        .constructor<const string&>()
        .method("getValue", &UTIL::BitField64::getValue)
        .method("size", &UTIL::BitField64::size)
        .method("index", &UTIL::BitField64::index)
        .method("lowWord", &UTIL::BitField64::lowWord)
        .method("highWord", &UTIL::BitField64::highWord)
        .method("fieldDescription", &UTIL::BitField64::fieldDescription)
        .method("valueString", &UTIL::BitField64::valueString);
    lciowrap.method("getindex", [](const UTIL::BitField64& b, const string s)->long long {
        return b[s].value();
    });
    lciowrap.method("getindex", [](const UTIL::BitField64& b, size_t index)->long long {
        return b[index].value();
    });

    lciowrap.add_type<Parametric<TypeVar<1>>>("CellIDDecoder")
        .apply<UTIL::CellIDDecoder<SimCalorimeterHit>
             , UTIL::CellIDDecoder<RawCalorimeterHit>
             , UTIL::CellIDDecoder<CalorimeterHit>
             , UTIL::CellIDDecoder<TrackerHit>
             , UTIL::CellIDDecoder<SimTrackerHit>>([](auto wrapped)
    {
        typedef typename decltype(wrapped)::type WrappedT;
        wrapped.template constructor<const LCCollection*>();
        wrapped.method(&WrappedT::operator());
    });

    lciowrap.add_type<UTIL::LCRelationNavigator>("LCRelNav")
        .method("getFromType", &UTIL::LCRelationNavigator::getFromType)
        .method("getToType", &UTIL::LCRelationNavigator::getToType)
        .method("getRelatedToObjects", &UTIL::LCRelationNavigator::getRelatedToObjects)
        .method("getRelatedFromObjects", &UTIL::LCRelationNavigator::getRelatedFromObjects)
        .method("getRelatedFromWeights", &UTIL::LCRelationNavigator::getRelatedFromWeights)
        .method("getRelatedToWeights", &UTIL::LCRelationNavigator::getRelatedToWeights);

    lciowrap.add_type<Parametric<TypeVar<1>>>("CastOperator")
    .apply<CastOperator<CalorimeterHit>
         , CastOperator<Cluster>
         , CastOperator<LCGenericObject>
         , CastOperator<MCParticle>
         , CastOperator<ReconstructedParticle>
         , CastOperator<SimCalorimeterHit>
         , CastOperator<SimTrackerHit>
         , CastOperator<Track>
         , CastOperator<TrackerHit>
         , CastOperator<Vertex>>([](auto wrapped)
    {
        typedef typename decltype(wrapped)::type LCType;
        wrapped.method("cast", &LCType::cast);
    });
JULIA_CPP_MODULE_END
