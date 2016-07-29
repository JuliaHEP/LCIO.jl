function getEnergyList(hit)
	nParticles = icxx"$(hit)->getNMCParticles();"
	energyList = Float64[]
	for i in 1:nParticles
		push!(energyList, icxx"$(hit)->getEnergyCont($(i)-1);")
	end
	return energyList
end

function getTimeList(hit)
	nParticles = icxx"$(hit)->getNMCParticles();"
	timeList = Float64[]
	for i in 1:nParticles
		push!(timeList, icxx"$(hit)->getTimeCont($(i)-1);")
	end
	return timeList
end

function getPDGList(hit)
	nParticles = icxx"$(hit)->getNMCParticles();"
	PDGList = Float64[]
	for i in 1:nParticles
		push!(PDGList, icxx"$(hit)->getPDGCont($(i)-1);")
	end
	return PDGList
end
