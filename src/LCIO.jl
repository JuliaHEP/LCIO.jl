module LCIO
using CxxWrap
import Base: getindex, start, done, next, length, +
export Vec, getCollection, getCollectionNames, getCollectionTypeName, getP4, getPosition

const depsfile = joinpath(dirname(dirname(@__FILE__)), "deps", "deps.jl")
if !isfile(depsfile)
  error("$depsfile not found, CxxWrap did not build properly")
end
include(depsfile)

wrap_module(_l_lciowrap)

function __init__()
    global reader = createLCReader()
    atexit() do
        # CxxWrap'ed objects automatically call delete in the finalizer
        #deletLCReader(reader)
    end
end

immutable Vec
	x::Cdouble
	y::Cdouble
	z::Cdouble
	t::Cdouble
end
+(a::Vec, b::Vec) = Vec(a.x+b.x, a.y+b.y, a.z+b.z, a.t+b.t)

immutable ThreeVec
	x::Cdouble
	y::Cdouble
	z::Cdouble
end
+(a::ThreeVec, b::ThreeVec) = ThreeVec(a.x+b.x, a.y+b.y, a.z+b.z)

immutable CalHit
	x::Cfloat
	y::Cfloat
	z::Cfloat
	E::Cfloat
end

# typealias VectorStringP cxxt"const std::vector<std::string>*"
#
# start(it::VectorStringP) = 0
# next(it::VectorStringP,i) = (it[i], i+1)
# done(it::VectorStringP,i) = i >= length(it)
# getindex(it::VectorStringP,i) = String( icxx"$(it)->at($i).c_str();" )
# length(it::VectorStringP) = icxx"$(it)->size();"

# The iterators use the events as state
# the lcio reader returns NULL at the end of the file
# Because the LCIO getNext function could re-use the memory for the next event,
# the next function should not hold the current and next event at the same time.
# Returning current and reading nextEvent as the new state causes memory corruption

type EventIterator
    nEvents
end
start(it::EventIterator) = 0
function next(it::EventIterator, state)
    it.nEvents -= 1
    (readNextEvent(reader), it.nEvents-1)
end
function done(it::EventIterator, state)
	isdone = 0 == it.nEvents
	if isdone
		closeFile(reader)
	end
	isdone
end

# open file with reader, returns iterator
function open(fn::AbstractString)
	openFile(reader, fn)
	# returns an iterator, initialized with a nullptr
	# the iterator knows about the global reader object
	return EventIterator(getNumberOfEvents(reader))
end

# We would like to have a typed collection, but what getCollection returns is unfortunately untyped
# The type is established by reading its name from the collection and mapping it in the LCIOTypemap
# immutable LCCollection{T}
# 	coll::LCCollection
# end

# typealias SimCalorimeterHit cxxt"EVENT::SimCalorimeterHit*"
# typealias TrackerHit cxxt"EVENT::TrackerHit*"
# typealias SimTrackerHit cxxt"EVENT::SimTrackerHit*"
# typealias MCParticle cxxt"EVENT::MCParticle*"
# typealias Track cxxt"EVENT::Track*"
# typealias LCGenericObject cxxt"EVENT::LCGenericObject*"
#

# map from names stored in collection to actual types
# LCIOTypemap = Dict(
# 	"SimCalorimeterHit" => SimCalorimeterHit,
# 	"TrackerHit" => TrackerHit,
# 	"SimTrackerHit" => SimTrackerHit,
# 	"MCParticle" => MCParticle,
# 	"Track" => Track,
# 	"LCGenericObject" => LCGenericObject,
# )
#
# start(it::LCCollection) = 0
# done(it::LCCollection, i) = i >= length(it)
# next{T}(it::LCCollection{T}, i) = getElementAt{T}(it.coll, i), i+1
# length(it::LCCollection) = getNumberOfElements(it.coll)
#
function getCollection(event, collectionName)
	collection = getEventCollection(event, collectionName)
	collectionType = getTypeName(collection)
	return TypedCollection{LCIOTypemap[String(collectionType)]}(collection)
end

# getEnergy(particle) = icxx"$(particle)->getEnergy();"
# function getMomentum(particle)
# 	p3 = icxx"$(particle)->getMomentum();"
# 	ThreeVec(unsafe_load(p3, 1), unsafe_load(p3, 2), unsafe_load(p3, 3))
# end
# function getPosition(hit)
# 	pos = icxx"$(hit)->getPosition();"
# 	ThreeVec(unsafe_load(pos, 1), unsafe_load(pos, 2), unsafe_load(pos, 3))
# end
#
# function getP4(particle)
# 	p3 = icxx"$(particle)->getMomentum();"
# 	e = icxx"$(particle)->getEnergy();"
# 	Vec(unsafe_load(p3, 1), unsafe_load(p3, 2), unsafe_load(p3, 3), e)
# end


# include("MCParticle.jl")
# include("CaloHit.jl")

# getRelationFrom(mcp::Ptr{Void}) = ccall((:lcrelgetfrom, libLCIO), Ptr{Void}, (Ptr{Void}, ), mcp)
# getRelationTo(mcp::Ptr{Void}) = ccall((:lcrelgetto, libLCIO), Ptr{Void}, (Ptr{Void}, ), mcp)


# getPositionVec(hit::Ptr{Void}) = ccall((:lcsthgetposition, libLCIO), ThreeVec, (Ptr{Void}, ), hit)
#
# function getPosition(hit::Ptr{Void})
# 	pos = ccall((:lcsthgetposition, libLCIO), Ptr{Cdouble}, (Ptr{Void},), hit)
# 	return [unsafe_load(pos, 1) unsafe_load(pos, 2) unsafe_load(pos, 3)]
# end
#
#
# function getParticleHits(rp::Ptr{Void})
# 	hitList = CalHit[]
# 	nClusters = Ref{Csize_t}(0)
# 	cList = ccall((:lcrcpgetclusters, libLCIO), Ptr{Ptr{Void}}, (Ptr{Void}, Ref{Csize_t}), rp, nClusters)
# 	for i in 1:nClusters[]
# 		nHits = Ref{Csize_t}(0)
# 		hitCollection = ccall((:lcclugetcalorimeterhits, libLCIO), Ptr{Ptr{Void}}, (Ptr{Void}, Ref{Csize_t}), unsafe_load(cList, i), nHits)
# 		for j in 1:nHits[]
# 			push!(hitList, getCaloHit(unsafe_load(hitCollection, j)))
# 		end
# 	end
# 	return hitList
# end


end
