module LCIO

using LCIO_jll
using CxxWrap
using LCIO_Julia_Wrapper_jll

function open(f::Function, fn::AbstractString)
    reader = createLCReader()
    if isnull(reader)
        return nothing
    end
    try
        openFile(reader, fn)
        f(reader)
    finally
        closeFile(reader)
        deleteLCReader(reader)
    end
end

@wrapmodule(lciowrap)

function __init__()
  @initcxx
end

import Base: getindex, length, convert, iterate, eltype
export CalHit, getP4, getPosition, CellIDDecoder,
    getEventNumber, getRunNumber, getDetectorName, getCollection, getCollectionNames, # LCEvent
    getTypeName, # LCCollection
    getEnergy, getParents, getDaughters, getPDG, getGeneratorStatus, getSimulatorStatus, isCreatedInSimulation, isBackScatter, vertexIsNotEndpointOfParent, isDecayedInCalorimeter, hasLeftDetector, isStopped, isOverlay, getVertex, getTime, getEndpoint, getMomentum, getMomentumAtEndpoint, getMass, getCharge, # MCParticle
    getCalorimeterHits, # Cluster
    getClusters, getType, isCompound, getMass, getCharge, getReferencePoint, getParticleIDs, getParticleIDUsed, getGoodnessOfPID, getParticles, getClusters, getTracks, getStartVertex, getEndVertex # ReconstructedParticle

struct CalHit
	x::Cfloat
	y::Cfloat
	z::Cfloat
	E::Cfloat
end

const MCPARTICLE = "MCParticle"
const WRITE_NEW = 0
const WRITE_APPEND = 1

function iterate(it::CxxPtr{LCReader})
    # get the length first. Inverting the order of these two calls
    # results in the first event being read twice.
    nEvents = length(it)
    event = readNextEvent(it)
    if isnull(event)
        return nothing
    end
    return (event, nEvents - 1)
end

function iterate(it::CxxPtr{LCReader}, state)
    if state < 1
        return nothing
    end
    event = readNextEvent(it)
    if isnull(event)
        return nothing 
    end
    return (event, state - 1)
end

# FIXME: Upstream bug: getNumberOfEvents resets the state of the reader
# to one before this event, so that events can be read twice
length(it::LCReader) = getNumberOfEvents(it)
eltype(::Type{CxxPtr{LCReader}}) = LCEvent

# map from names stored in collection to actual types
LCIOTypemap = Dict(
    Symbol("CalorimeterHit") => CalorimeterHit,
    Symbol("Cluster") => Cluster,
	Symbol("LCGenericObject") => LCGenericObject,
    Symbol("LCRelation") => LCRelation,
	Symbol("MCParticle") => MCParticle,
    Symbol("RawCalorimeterHit") => RawCalorimeterHit,
    Symbol("ReconstructedParticle") => ReconstructedParticle,
	Symbol("SimCalorimeterHit") => SimCalorimeterHit,
	Symbol("SimTrackerHit") => SimTrackerHit,
	Symbol("Track") => Track,
	Symbol("TrackerHit") => TrackerHit,
    Symbol("TrackerRawData") => TrackerRawData,
    Symbol("Vertex") => Vertex,
    Symbol("TrackerHitPlane") => TrackerHitPlane,
)

# This version of the iteration runs length() multiple times during the iteration
# if this becomes a speed problem, the length could be memoized, or iteration order could be inverted
iterate(it::TypedCollection) = length(it) > 0 ? (it[1], 2) : nothing 
iterate(it::TypedCollection, i) = i <= length(it) ? (it[i], i+1) : nothing
length(it::TypedCollection) = getNumberOfElements(it)
# getindex uses Julia counting, getElementAt uses C counting
getindex(it::TypedCollection, i) = getElementAt(it, convert(UInt64, i-1))
CellIDDecoder(t::TypedCollection{T}) where {T} = CellIDDecoder{T}(coll(t))
getTypeName(coll::TypedCollection{T}) where {T} = "$T"
eltype(::Type{TypedCollection{T}}) where {T} = T

# to get the typed collection, one needs to read the typename
# then we can return the right type from the LCIOTypemap
function getCollection(event, collectionName)
	collection = getEventCollection(event, collectionName)
	collectionType = Symbol(getTypeName(collection)[])
	return TypedCollection{LCIOTypemap[collectionType]}(collection)
end

mutable struct LCStdHepRdr
    r::_LCStdHepRdrCpp
    e::LCEventImpl
end
LCStdHepRdr(filename) = LCStdHepRdr(_LCStdHepRdrCpp(filename), LCEventImpl())

# the LCStdHepRdr implementation in C++ is not consistent with the LCIO reader
# this is me trying to make things a bit better
function iterate(it::LCStdHepRdr)
    l = length(it)
    if l == 0
        return nothing
    else
        return readNextEvent(it), l - 1
    end
end 
function iterate(it::LCStdHepRdr, state)
    if state > 0
        return readNextEvent(it), state-1
    else
        return nothing
    end
end
length(it::LCStdHepRdr) = getNumberOfEvents(it.r)
eltype(::Type{LCStdHepRdr}) = TypedCollection{MCParticle}


function openStdhep(f::Function, fn::AbstractString)
    reader = LCStdHepRdr(fn)
    try
        f(reader)
    finally
    end
end

getDetectorName(header_or_event) = getDetectorName_cxx(header_or_event)[]

# stdhep files only contain one specific type of collection: MCParticle
function readNextEvent(r::LCStdHepRdr)
    removeCollection(r.e, "MCParticle")
    updateNextEvent(r.r, r.e, "MCParticle")
    getCollection(r.e, "MCParticle")
end

function getPosition(hit)
    p3 = Array{Float64,1}(undef, 3)
    valid = getPosition3(hit, p3)
    return p3
end

function getMomentum(particle)
    p3 = Array{Float64,1}(undef, 3)
    valid = getMomentum3(particle, p3)
    return p3
end

function getVertex(particle)
    p3 = Array{Float64,1}(undef, 3)
    valid = getVertex3(particle, p3)
    return valid, p3
end

function getEndpoint(particle)
    p3 = Array{Float64,1}(undef, 3)
    valid = getEndpoint3(particle, p3)
    return valid, p3
end

function getMomentumAtEndpoint(particle)
    p3 = Array{Float64,1}(undef, 3)
    valid = getMomentumAtEndpoint3(particle, p3)
    return valid, p3
end

# the navigator gets initialized with a collection
# it defers the actual work to the C++ implementation
struct LCRelationNavigator
    relnav
    fromType
    toType
    LCRelationNavigator(coll::TypedCollection) = _completeNavigator(new(LCRelNav(coll.coll)))
end

function _completeNavigator(nav)
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

# should work for all particle types
function getP4(x)
    p3 = getMomentum(x)
    E = getEnergy(x)
    return (E, p3)
end

Base.:(==)(x::CxxWrap.ConstCxxRef{CxxWrap.StdLib.StdString}, y) = x[] == y

getP3(x) = getPosition(x)

# converters to keep older code working
const CalHits = Union{SimCalorimeterHit, CalorimeterHit, RawCalorimeterHit}

function CalHit(h::CalHits)
    p = getPosition(h)
    E = getEnergy(h)
    return CalHit(p[1], p[2], p[3], E)
end

getEnergyCont(hit, i) = _getEnergyCont(hit, i-1)
getTimeCont(hit, i) = _getTimeCont(hit, i-1)
getPDGCont(hit, i) = _getPDGCont(hit, i-1)
getParticleCont(hit, i) = _getParticleCont(hit, i-1)

function printParameters(p::LCParameters)
    println("strings:")
    for k in getStringKeys(p, StdVector{StdString}())
        println(k, "\t", getStringVal(p, k))
    end
    println("floats:")
    for k in getFloatKeys(p, StdVector{StdString}())
        println(k, "\t", getFloatVal(p, k))
    end
    println("ints:")
    for k in getIntKeys(p, StdVector{StdString}())
        println(k, "\t", getIntVal(p, k))
    end
end

include("precompile_LCIO.jl")
    _precompile_()
end # module
