module LCIO
using Cxx
import Base: getindex, start, done, next, length, +
export Vec, getCollection, getCollectionNames, getP4, LCIOopen, getPosition

addHeaderDir(joinpath(ENV["LCIO"], "include"), kind=C_User)
Libdl.dlopen(joinpath(ENV["LCIO"], "lib", "liblcio"), Libdl.RTLD_GLOBAL)

cxx"""
#include "lcio.h"
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
#include <vector>
#include <iostream>
#include <string>
IO::LCReader* reader = IOIMPL::LCFactory::getInstance()->createLCReader();
"""

function __init__()
	global reader = icxx"reader;"
	atexit() do
		# delete reader
		icxx"delete reader;"
	end
end


immutable Vec
	x::Cdouble
	y::Cdouble
	z::Cdouble
	t::Cdouble
end

immutable ThreeVec
	x::Cdouble
	y::Cdouble
	z::Cdouble
end

immutable CalHit
	x::Cfloat
	y::Cfloat
	z::Cfloat
	E::Cfloat
end

typealias VectorStringP cxxt"const std::vector<std::string>*"

start(it::VectorStringP) = 0
next(it::VectorStringP,i) = (it[i], i+1)
done(it::VectorStringP,i) = i >= length(it)
getindex(it::VectorStringP,i) = String( icxx"$(it)->at($i).c_str();" )
length(it::VectorStringP) = icxx"$(it)->size();"

# The iterators use the events as state
# the lcio reader returns NULL at the end of the file
# Because the LCIO getNext function could re-use the memory for the next event,
# the next function should not hold the current and next event at the same time.
# Returning current and reading nextEvent as the new state causes memory corruption

type EventIterator
	current::cxxt"EVENT::LCEvent*"
end
start(it::EventIterator) = C_NULL
next(it::EventIterator, state) = (it.current, C_NULL)
function done(it::EventIterator,state)
	it.current = icxx"reader->readNextEvent();"
	isdone = icxx"not $(it.current);"
	if isdone
		icxx"reader->close();"
	end
	isdone
end

# open file with reader, returns iterator
function LCIOopen(fn::AbstractString)
	icxx"reader->open($fn);"
	# returns an iterator, initialized with a nullptr
	# the iterator knows about the global reader object
	return EventIterator( icxx"(EVENT::LCEvent*)NULL;" )
end

immutable LCCollection{T}
	coll::cxxt"EVENT::LCCollection*"
end

typealias SimCalorimeterHit cxxt"EVENT::SimCalorimeterHit*"
typealias TrackerHit cxxt"EVENT::TrackerHit*"
typealias SimTrackerHit cxxt"EVENT::SimTrackerHit*"
typealias MCParticle cxxt"EVENT::MCParticle*"
typealias Track cxxt"EVENT::Track*"

# map from names stored in collection to actual types
LCIOTypemap = Dict(
	"SimCalorimeterHit" => SimCalorimeterHit,
	"TrackerHit" => TrackerHit,
	"SimTrackerHit" => SimTrackerHit,
	"MCParticle" => MCParticle,
	"Track" => Track,
)

start(it::LCCollection) = 0
done(it::LCCollection, i) = i >= length(it)
next{T}(it::LCCollection{T}, i) = icxx"static_cast<$(T)>($(it.coll)->getElementAt($(i)));", i+1
length(it::LCCollection) = icxx"$(it.coll)->getNumberOfElements();"

function getCollection(event, collectionName)
	name =  pointer(collectionName)
	collection = icxx"""$(event)->getCollection($(name));"""
	collectionType = icxx"$(collection)->getTypeName().c_str();"
	return LCCollection{LCIOTypemap[String(collectionType)]}(collection)
end

getCollectionTypeName(collection::LCCollection) = String( icxx"$(collection)->getTypeName().c_str();" )
getCollectionNames(event) = icxx"$(event)->getCollectionNames();"

getEnergy(particle) = icxx"$(particle)->getEnergy();"
function getMomentum(particle)
	p3 = icxx"$(particle)->getMomentum();"
	ThreeVec(unsafe_load(p3, 1), unsafe_load(p3, 2), unsafe_load(p3, 3))
end
function getPosition(hit)
	pos = icxx"$(hit)->getPosition();"
	ThreeVec(unsafe_load(pos, 1), unsafe_load(pos, 2), unsafe_load(pos, 3))
end

function getP4(particle)
	p3 = icxx"$(particle)->getMomentum();"
	e = icxx"$(particle)->getEnergy();"
	Vec(unsafe_load(p3, 1), unsafe_load(p3, 2), unsafe_load(p3, 3), e)
end


include("MCParticle.jl")

end # module
