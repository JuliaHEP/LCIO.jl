using LCIO
using Base.Test

# test iteration
LCIO.iterate("test.slcio") do event
    mcparts = getCollection(event, "MCParticle")
    for p in mcparts
        e = getEnergy(p)
        @test e > 0
        momentum = getMomentum(p)
        if getPDG(p) != 22
            # tolerance, since we're dealing with floats
            @test getEnergy(p)^2 - sum(momentum.^2) > -1e-12
        else
            @test abs(getEnergy(p)^2 - sum(momentum.^2)) < 1e-12
        end
    end
end

# test that everything is closed and opened properly
LCIO.open("test.slcio") do reader
for event in reader
    @test length(getCollectionNames(event)) == 23
    @test getDetectorName(event) == "sidloi3_scint1x1"
    HcalBarrelHits = getCollection(event, "HcalBarrelHits")
    @test getTypeName(HcalBarrelHits) == "LCIO.SimCalorimeterHit"
    decode = CellIDDecoder(HcalBarrelHits)
    for h in HcalBarrelHits
        @test 0 <= decode(h)["layer"] <= 39
    end
end
end
