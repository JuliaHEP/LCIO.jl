push!(LOAD_PATH, "..")
using LCIO
using DataStructures

nEvents = 0
for (idx, i) in enumerate(LCIO.open(ARGS[1]))
    outfile = open("particles_500GeV_$idx.csv", "w")
    particleHits = DefaultDict(Ptr{Void}, Dict, Dict("ECAL" => LCIO.CalHit[], "HCAL" => LCIO.CalHit[]))
    ee = getCollection(i, "EcalEndcapHits")
    eb = getCollection(i, "EcalBarrelHits")
    he = getCollection(i, "HcalEndcapHits")
    hb = getCollection(i, "HcalBarrelHits")
    for relation in getCollection(i, "CalorimeterHitRelations")
	calibratedHit = getRelationFrom(relation)
	simCaloHit = getRelationTo(relation)
	pList = getHitMCParticles(simCaloHit)
	p4 = getCaloHit(calibratedHit)
	energyList = getHitEnergyList(simCaloHit)
	simCaloHitEnergy = sum(energyList)	
	# TODO map the particles to the Hits, but correct the hit energy by the sampling fraction
	# maybe look at energy spread within the cluster
        if (simCaloHit in eb) || (simCaloHit in ee)
            collectionName = "ECAL"
        elseif (simCaloHit in hb) || (simCaloHit in he)
            collectionName = "HCAL"
        else
            # skip muonhits
            continue
        end
        for (kdx, particle) in enumerate(pList)
            h = LCIO.CalHit(p4, energyList[kdx]*p4.E/simCaloHitEnergy)
            push!(particleHits[particle][collectionName], h)
        end
    end

    for (p, hitDictionary) in particleHits
        p4 = getMCP4(p)
        pdg = getMCPDGid(p)
        write(outfile, "particle: $pdg, momentum: $(p4.x), $(p4.y), $(p4.z), $(p4.t), #ECalHits: $length(eCollection), #HCalHits: $length(hCollection)\n")
        write(outfile, "  E  ,  x  ,  y  , z\n")
	for hit in hitDictionary["ECAL"]
            write(outfile, "$(hit.E), $(hit.x), $(hit.y), $(hit.z)\n")
	end
        write(outfile, "\n")
	for hit in hitDictionary["HCAL"]
            write(outfile, "$(hit.E), $(hit.x), $(hit.y), $(hit.z)\n")
	end
        write(outfile, "\n\n\n")	
    end
    close(outfile)
    if idx > 100
        break
    end
end
