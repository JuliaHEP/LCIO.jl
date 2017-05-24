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

using namespace std;
using namespace jlcxx;

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
    // TypedCollection(IMPL::LCCollectionVec* collection) {
    //     m_coll = collection;
    // }
    inline T* getElementAt(size_t i) {
        return static_cast<T*>(m_coll->getElementAt(i));
    }
    inline size_t getNumberOfElements() {
        return m_coll->getNumberOfElements();
    }
    inline EVENT::LCCollection* coll() {
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
    template<> struct SuperType<IMPL::LCEventImpl> { typedef EVENT::LCEvent type; };
    template<> struct SuperType<IMPL::LCCollectionVec> { typedef EVENT::LCCollection type; };
    template<> struct SuperType<IMPL::MCParticleImpl> { typedef EVENT::MCParticle type; };
    template<> struct SuperType<IMPL::LCRunHeaderImpl> { typedef EVENT::LCRunHeader type; };
}


JULIA_CPP_MODULE_BEGIN(registry)
    jlcxx::Module& lciowrap = registry.create_module("LCIO");

    lciowrap.add_type<EVENT::LCObject>("LCObject");
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
    lciowrap.add_type<vector<short>>("ShortVec")
        .method("size", &EVENT::ShortVec::size);
    lciowrap.method("at", [](const vector<short>* vec, size_t i) {
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
    lciowrap.method("setValue", [](EVENT::LCParameters* parms, const std::string& key, int value) {
        return parms->setValue(key, value);
    });
    lciowrap.method("setValue", [](EVENT::LCParameters* parms, const std::string& key, float value) {
        return parms->setValue(key, value);
    });
    lciowrap.method("setValue", [](EVENT::LCParameters* parms, const std::string& key, const std::string& value) {
        return parms->setValue(key, value);
    });
    // most of the functionality is forwarded to the TypedCollection
    lciowrap.add_type<EVENT::LCCollection>("LCCollection")
        .method("getNumberOfElements", &EVENT::LCCollection::getNumberOfElements)
        .method("getElementAt", &EVENT::LCCollection::getElementAt)
        .method("getTypeName", &EVENT::LCCollection::getTypeName)
        .method("getParameters", &EVENT::LCCollection::getParameters);

    lciowrap.add_type<IMPL::LCCollectionVec>("LCCollectionVec", jlcxx::julia_type<EVENT::LCCollection>())
        .constructor<const string&>()
        .method("setTransient", &IMPL::LCCollectionVec::setTransient);

    lciowrap.add_type<EVENT::LCRunHeader>("LCRunHeader")
        .method("getRunNumber", &EVENT::LCRunHeader::getRunNumber)
        .method("getDetectorName", &EVENT::LCRunHeader::getDetectorName)
        .method("getDescription", &EVENT::LCRunHeader::getDescription)
        .method("getParameters", &EVENT::LCRunHeader::getParameters);

    lciowrap.add_type<IMPL::LCRunHeaderImpl>("LCRunHeaderImpl", jlcxx::julia_type<EVENT::LCRunHeader>())
        .method("setRunNumber", &IMPL::LCRunHeaderImpl::setRunNumber)
        .method("setDetectorName", &IMPL::LCRunHeaderImpl::setDetectorName)
        .method("setDescription", &IMPL::LCRunHeaderImpl::setDescription)
        .method("parameters", &IMPL::LCRunHeaderImpl::parameters);

    lciowrap.add_type<EVENT::LCObjectVec>("LCObjectVec")
        .method("size", &EVENT::LCObjectVec::size);
    lciowrap.method("at", [](const EVENT::LCObjectVec& vec, size_t i) {
        return vec.at(i);
    });

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
    #include "TrackerHitTypes.icc"

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

    lciowrap.add_type<EVENT::LCGenericObject>("LCGenericObject")
        .method("getNInt", &EVENT::LCGenericObject::getNInt)
        .method("getNFloat", &EVENT::LCGenericObject::getNFloat)
        .method("getNDouble", &EVENT::LCGenericObject::getNDouble)
        .method("getIntVal", &EVENT::LCGenericObject::getIntVal)
        .method("getFloatVal", &EVENT::LCGenericObject::getFloatVal)
        .method("getDoubleVal", &EVENT::LCGenericObject::getDoubleVal)
        .method("isFixedSize", &EVENT::LCGenericObject::isFixedSize)
        .method("getTypeName", &EVENT::LCGenericObject::getTypeName)
        .method("getDataDescription", &EVENT::LCGenericObject::getDataDescription)
        .method("id", &EVENT::LCGenericObject::id);

    #include "ReconstructedParticle.icc"

    lciowrap.add_type<EVENT::LCEvent>("LCEvent")
        .method("getEventCollection", &EVENT::LCEvent::getCollection)
        .method("getCollectionNames", &EVENT::LCEvent::getCollectionNames)
        .method("getDetectorName", &EVENT::LCEvent::getDetectorName)
        .method("getEventNumber", &EVENT::LCEvent::getEventNumber)
        .method("getRunNumber", &EVENT::LCEvent::getRunNumber)
        .method("getParameters", &EVENT::LCEvent::getParameters)
        .method("getWeight", &EVENT::LCEvent::getWeight);

    lciowrap.add_type<IMPL::LCEventImpl>("LCEventImpl", jlcxx::julia_type<EVENT::LCEvent>())
        .method("setEventNumber", &IMPL::LCEventImpl::setEventNumber);
    lciowrap.method("addCollection", [](IMPL::LCEventImpl* event, IMPL::LCCollectionVec* col, const std::string& name) {
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
        .apply<TypedCollection<EVENT::CalorimeterHit>
             , TypedCollection<EVENT::Cluster>
             , TypedCollection<EVENT::MCParticle>
             , TypedCollection<EVENT::LCGenericObject>
             , TypedCollection<EVENT::LCRelation>
             , TypedCollection<EVENT::ReconstructedParticle>
             , TypedCollection<EVENT::SimCalorimeterHit>
             , TypedCollection<EVENT::SimTrackerHit>
             , TypedCollection<EVENT::Track>
             , TypedCollection<EVENT::TrackerHit>
             , TypedCollection<EVENT::TrackerRawData>
             , TypedCollection<EVENT::Vertex>>([](auto wrapped)
        {
        typedef typename decltype(wrapped)::type WrappedColl;
        wrapped.template constructor<EVENT::LCCollection*>();
        // wrapped.template constructor<IMPL::LCCollectionVec*>();
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
        .apply<UTIL::CellIDDecoder<EVENT::SimCalorimeterHit>
             , UTIL::CellIDDecoder<EVENT::RawCalorimeterHit>
             , UTIL::CellIDDecoder<EVENT::CalorimeterHit>
             , UTIL::CellIDDecoder<EVENT::TrackerHit>
             , UTIL::CellIDDecoder<EVENT::SimTrackerHit>>([](auto wrapped)
    {
        typedef typename decltype(wrapped)::type WrappedT;
        wrapped.template constructor<const EVENT::LCCollection*>();
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
    .apply<CastOperator<EVENT::CalorimeterHit>
         , CastOperator<EVENT::Cluster>
         , CastOperator<EVENT::LCGenericObject>
         , CastOperator<EVENT::MCParticle>
         , CastOperator<EVENT::ReconstructedParticle>
         , CastOperator<EVENT::SimCalorimeterHit>
         , CastOperator<EVENT::SimTrackerHit>
         , CastOperator<EVENT::Track>
         , CastOperator<EVENT::TrackerHit>
         , CastOperator<EVENT::Vertex>>([](auto wrapped)
    {
        typedef typename decltype(wrapped)::type LCType;
        wrapped.method("cast", &LCType::cast);
    });
JULIA_CPP_MODULE_END
