__precompile__(false)
module LCIO
using CxxWrap
import Base: getindex, start, done, next, length, +, convert
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
        # deletLCReader(reader)
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
	# returns an iterator
	# the iterator knows about the global reader object
	return EventIterator(getNumberOfEvents(reader))
end

# map from names stored in collection to actual types
LCIOTypemap = Dict(
	"SimCalorimeterHit" => SimCalorimeterHit,
    "CalorimeterHit" => CalorimeterHit,
	"TrackerHit" => TrackerHit,
	"SimTrackerHit" => SimTrackerHit,
	"MCParticle" => MCParticle,
	"Track" => Track,
	"LCGenericObject" => LCGenericObject,
)

start(it::TypedCollection) = length(it)
done(it::TypedCollection, i) = i <= 0
next{T}(it::TypedCollection{T}, i) = getElementAt(it, i-1), i-1
length(it::TypedCollection) = getNumberOfElements(it)

CellIDDecoder{T}(t::TypedCollection{T}) = CellIDDecoder{T}(coll(t))

# to get the typed collection, one needs to read the typename
# then we can return the right type from the LCIOTypemap
function getCollection(event, collectionName)
	collection = getEventCollection(event, collectionName)
	collectionType = getTypeName(collection)
	return TypedCollection{LCIOTypemap[collectionType]}(collection)
end

function getPosition(hit)
    p3 = Array{Float64,1}(3)
    getP3(hit, p3)
    return p3
end

end
