__precompile__(false)
module LCIO
using CxxWrap
import Base: getindex, start, done, next, length, +, convert
export Vec, getP4, getPosition,
    getEventNumber, getRunNumber, getDetectorName, getCollection, getCollectionNames, # LCEvent
    getTypeName, # LCCollection
    getEnergy, getParents, getDaughters, getPDG, getGeneratorStatus, getSimulatorStatus, # MCParticle
    isCreatedInSimulation, isBackScatter, vertexIsNotEndpointOfParent, isDecayedInCalorimeter, # MCParticle
    hasLeftDetector, isStopped, isOverlay, getVertex, getTime, getEndpoint, getMomentum,
    getMomentumAtEndpoint, getMass, getCharge # MCParticle
const depsfile = joinpath(dirname(dirname(@__FILE__)), "deps", "deps.jl")
if !isfile(depsfile)
  error("$depsfile not found, CxxWrap did not build properly")
end
include(depsfile)

wrap_module(_l_lciowrap)

# function __init__()
#     atexit() do
#     end
# end

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

# uses Julia counting, 1..n
start(it::StringVec) = convert(UInt64, 1)
next(it::StringVec, i) = (it[i], i+1)
done(it::StringVec, i) = i > length(it)
length(it::StringVec) = size(it)
# 'at' uses C counting, 0..n-1
getindex(it::StringVec, i) = at(it, i-1)

start(it::LCReader) = getNumberOfEvents(it)
next(it::LCReader, state) = readNextEvent(it), state-1
done(it::LCReader, state) = state <= 1

function iterate(f::Function, fn::AbstractString)
    reader = createLCReader()
    openFile(reader, fn)
    try
        for event in reader
            f(event)
        end
    finally
        closeFile(reader)
        deleteLCReader(reader)
    end
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

getTypeName{T}(coll::TypedCollection{T}) = "$T"

# to get the typed collection, one needs to read the typename
# then we can return the right type from the LCIOTypemap
function getCollection(event, collectionName)
	collection = getEventCollection(event, collectionName)
	collectionType = getTypeName(collection)
	return TypedCollection{LCIOTypemap[collectionType]}(collection)
end

function getPosition(hit)
    p3 = Array{Float64,1}(3)
    valid = getPosition3(hit, p3)
    return p3
end

function getMomentum(particle)
    p3 = Array{Float64,1}(3)
    valid = getMomentum3(particle, p3)
    return p3
end

function getVertex(particle)
    p3 = Array{Float64,1}(3)
    valid = getVertex3(particle, p3)
    return valid, p3
end

function getEndpoint(particle)
    p3 = Array{Float64,1}(3)
    valid = getEndpoint3(particle, p3)
    return valid, p3
end

function getMomentumAtEndpoint(particle)
    p3 = Array{Float64,1}(3)
    valid = getMomentumAtEndpoint3(particle, p3)
    return valid, p3
end

# the navigator gets initialized with a collection
# it defers the actual work to the C++ implementation
immutable LCRelationNavigator
    relnav
    fromType
    toType
    LCRelationNavigator(coll::TypedCollection) = _completNavigator(new(LCRelNav(coll.coll)))
end

function _completNavigator(nav)
    nav.fromType = LCIOTypemap[nav.relnav.getFromType()]
    nav.toType = LCIOTypemap[nav.relnav.getToType()]
    nav
end

# this ensures that the types are appropriately cast
function getRelatedToObjects(nav::LCRelationNavigator, obj)
    [CastOperator{nav.toType}.cast(x) for x in getRelatedToObjects(nav.relnav)]
end

# this ensures that the types are appropriately cast
function getRelatedFromObjects(nav::LCRelationNavigator, obj)
    [CastOperator{nav.fromType}.cast(x) for x in getRelatedFromObjects(nav.relnav)]
end

end
