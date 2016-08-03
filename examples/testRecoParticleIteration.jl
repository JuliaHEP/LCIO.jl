using LCIO

function getParticleHits(p)
    hitList = CalHit[]
    for c in getClusters(p)
        for h in getCalorimeterHits(c)
            push!(hitList, CalHit(h))
        end
    end
    hitList
end

LCIO.open(ARGS[1]) do reader
for event in reader
	for part in getCollection(event, "PandoraPFOCollection")
		println(getParticleHits(part))
	end
	break
end
end
