module LCIO
import Base: start, done, next, length
export Vec, getCollection, getCollectionNames, getP4, getSimCaloHit, getCollectionTypeName, numberOfElements, getHitMCParticles, getMCPGenStatus, getMCPDGid, getRelationFrom, getRelationTo, getHitEnergy, getHitEnergyList, getCaloHit, getSimCaloHitId, getCaloHitId, getMCP4, getPosition

const libLCIO = joinpath(dirname(@__FILE__), "..", "deps", "lcio_jl.so")

function __init__()
	global LCIOReader = ccall((:lcrdrcreate, libLCIO), Ptr{Void}, ())
	atexit() do
		# delete reader
		if ccall((:lcrdrdelete, libLCIO), Cint, (Ptr{Void},), LCIOReader) == 0
			println("Delete Reader failed")
		end
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

# constructor to set energy to new value explicitly
# "calibration"
CalHit(h::CalHit, e) = CalHit(h.x, h.y, h.z, e)

# The iterators use the events as state
# the lcio reader returns NULL at the end of the file
# Because the LCIO getNext function could re-use the memory for the next event,
# the next function should not hold the current and next event at the same time.
# Returning current and reading nextEvent as the new state causes memory corruption
type LCIOIterator
	current::Ptr{Void}
end

# open file with reader, returns iterator
function open(fn::AbstractString)
	if ccall((:lcrdropen, libLCIO), Cint, (Ptr{Void}, Ptr{UInt8}), LCIOReader, fn) == 0
		println("File open failed")
	end
	return LCIOIterator(C_NULL)
end

function start(r::LCIOIterator)
	return "start"
end

function done(r::LCIOIterator, state)
	r.current = readNext()
	if r.current == C_NULL
		# close file
		if ccall((:lcrdrclose, libLCIO), Cint, (Ptr{Void},), LCIOReader) == 0
			println("File close failed")
		end
	end
	return r.current == C_NULL
end

function next(r::LCIOIterator, state)
	return r.current, "OK"
end

# read one event
function readNext()
	return ccall((:lcrdrreadnextevent, libLCIO), Ptr{Void}, (Ptr{Void},), LCIOReader)
end

# returns names for collections in event
function getCollectionNameArray(event::Ptr{Void})
	nameArray = AbstractString[]
	nNames = ccall((:lcevtgetcollectionnamecount, libLCIO), Cint, (Ptr{Void},), event)
	for n in 1:nNames
		name = ccall((:lcevtgetcollectionname, libLCIO), Ptr{UInt8}, (Ptr{Void}, Csize_t), event, n-1)
		push!(nameArray, bytestring(name))
	end
	return nameArray
end

immutable LCCollectionIterator
	coll::Ptr{Void}
	numberOfElements::Cint
end

function start(i::LCCollectionIterator)
	return 1
end

function done(i::LCCollectionIterator, state)
	return state > i.numberOfElements
end

function next(i::LCCollectionIterator, state)
	return ccall((:lccolgetelementat, libLCIO), Ptr{Void}, (Ptr{Void}, Cint), i.coll, state-1), state+1
end

length(i::LCCollectionIterator) = numberOfElements(i.coll)

# access a collection
function getCollection(event::Ptr{Void}, name::AbstractString)
	coll = ccall((:lcevtgetcollection, libLCIO), Ptr{Void}, (Ptr{Void}, Ptr{UInt8}), event, name)
	return LCCollectionIterator(coll, numberOfElements(coll))
end

function getCollectionTypeName(collection::LCCollectionIterator)
	return bytestring(ccall((:lccolgettypename, libLCIO), Ptr{UInt8}, (Ptr{Void},), collection.coll))
end

# print number of elements
numberOfElements(coll::Ptr{Void}) = ccall((:lccolgetnumberofelements, libLCIO), Cint, (Ptr{Void}, ), coll)

getMCP4(mcp::Ptr{Void}) = ccall((:lcmcpgetp4, libLCIO), Vec, (Ptr{Void},), mcp)
function getMCMomentum(mcp::Ptr{Void})
	p = ccall((:lcmcpgetmomentum, libLCIO), Ptr{Cdouble}, (Ptr{Void},), mcp)
	return pointer_to_array(p, 3)
end

getP4(particle::Ptr{Void}) = ccall((:lcgetp4, libLCIO), Vec, (Ptr{Void},), particle)

# returns the covarianceMatrix from the lower diagonal representation of the symmetric matrix on the c side
function getCovMatrix(vtx::Ptr{Void})
	cov = ccall((:lcvtxgetcovmatrix, libLCIO), Ptr{Cfloat}, (vtx,))
	return [unsafe_load(cov, 1) unsafe_load(cov, 2) unsafe_load(cov, 4);
			unsafe_load(cov, 2) unsafe_load(cov, 3) unsafe_load(cov, 5);
			unsafe_load(cov, 4) unsafe_load(cov, 5) unsafe_load(cov, 6)]
end

# returns a Julia array with the floats from the float* on the C side
# the correct length is obtained from another call on the c side
function getVtxParameters(vtx::Ptr{Void})
	nParams = Ref{Csize_t}(0)
	parVec = ccall((:lcvtxgetparameters, libLCIO), Ptr{Cfloat}, (Ptr{Void}, Ref{Csize_t}), vtx, nParams)
	parameters = Cfloat[]
	for i in 1:nParams[]
		push!(parameters, unsafe_load(parVec, i))
	end
	return parameters
end

# returns the list of ReconstructedParticles from a ReconstructedParticle (e.g. a jet)
getHitType(hit::Ptr{Void}) = ccall((:lccahgettype, libLCIO), Cint, (hit,))


# converts the std::vector to a Julia Array
function getParticles(rp::Ptr{Void})
	nParticles = Ref{Csize_t}(0)
	pList = ccall((:lcrcpgetparticles, libLCIO), Ptr{Ptr{Void}}, (Ptr{Void}, Ref{Csize_t}), rp, nParticles)
	particleList = Ptr{Void}[]
	for i in 1:nParticles[]
		push!(particleList, unsafe_load(pList, i))
	end
	return particleList
end

include("CaloHit.jl")
include("MCParticle.jl")

getRelationFrom(mcp::Ptr{Void}) = ccall((:lcrelgetfrom, libLCIO), Ptr{Void}, (Ptr{Void}, ), mcp)
getRelationTo(mcp::Ptr{Void}) = ccall((:lcrelgetto, libLCIO), Ptr{Void}, (Ptr{Void}, ), mcp)


getPositionVec(hit::Ptr{Void}) = ccall((:lcsthgetposition, libLCIO), ThreeVec, (Ptr{Void}, ), hit)

function getPosition(hit::Ptr{Void})
	pos = ccall((:lcsthgetposition, libLCIO), Ptr{Cdouble}, (Ptr{Void},), hit)
	return [unsafe_load(pos, 1) unsafe_load(pos, 2) unsafe_load(pos, 3)]
end


function getParticleHits(rp::Ptr{Void})
	hitList = CalHit[]
	nClusters = Ref{Csize_t}(0)
	cList = ccall((:lcrcpgetclusters, libLCIO), Ptr{Ptr{Void}}, (Ptr{Void}, Ref{Csize_t}), rp, nClusters)
	for i in 1:nClusters[]
		nHits = Ref{Csize_t}(0)
		hitCollection = ccall((:lcclugetcalorimeterhits, libLCIO), Ptr{Ptr{Void}}, (Ptr{Void}, Ref{Csize_t}), unsafe_load(cList, i), nHits)
		for j in 1:nHits[]
			push!(hitList, getCaloHit(unsafe_load(hitCollection, j)))
		end
	end
	return hitList
end


end # module
