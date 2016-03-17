getSimCaloHitId(hit::Ptr{Void}) = ccall((:lcschid, libLCIO), Cint, (Ptr{Void},), hit)
getCaloHitId(hit::Ptr{Void}) = ccall((:lccahid, libLCIO), Cint, (Ptr{Void},), hit)
getHitEnergy(hit::Ptr{Void}) = ccall((:lccahgetenergy, libLCIO), Cfloat, (Ptr{Void}, ), hit)

function getHitEnergyList(hit::Ptr{Void})
	nParticles = ccall((:lcschgetnmccontributions, libLCIO), Cint, (Ptr{Void},), hit)
	energyList = Cfloat[]
	for i in 1:nParticles[]
		push!(energyList, ccall((:lcschgetenergycont, libLCIO), Cint, (Ptr{Void}, Cint), hit, i-1))
	end
	return energyList
end

# reads the list of particles one by one. Does not allocate intermediate arrays on the C side
# since most hits only have one MCParticle contribution, allocating storage to reduce the number of calls to the C side is actually quite a bit slower than getting the MCParticles one-by-one.
function getHitMCParticles(hit::Ptr{Void})
	nParticles = ccall((:lcschgetnmccontributions, libLCIO), Cint, (Ptr{Void},), hit)
	particleList = Ptr{Void}[]
	for i in 1:nParticles[]
		part = ccall((:lcschgetparticlecont, libLCIO), Ptr{Void}, (Ptr{Void}, Cint), hit, i-1)
		push!(particleList, part)
	end
	return particleList
end


getHitType(hit::Ptr{Void}) = ccall((:lccahgettype, libLCIO), Cint, (hit,))
getSimCaloHit(hit::Ptr{Void}) = ccall((:lcschgetp4, libLCIO), CalHit, (Ptr{Void},), hit)
getCaloHit(hit::Ptr{Void}) = ccall((:lccahgetp4, libLCIO), CalHit, (Ptr{Void},), hit)
