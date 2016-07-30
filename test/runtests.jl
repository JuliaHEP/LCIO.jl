using LCIO
using Base.Test

# test iteration
for (idx, event) in enumerate(LCIO.open("test.slcio"))
    @test getEventNumber(event) == idx-1
    mcparts = getCollection(event, "MCParticle")
    for p in mcparts
        e = getEnergy(p)
        @test e > 0
        ok, momentum = getMomentum(p)
        if getPDG(p) != 22
            # tolerance, since we're dealing with floats
            @test getEnergy(p)^2 - sum(momentum.^2) > -1e-12
        else
            @test abs(getEnergy(p)^2 - sum(momentum.^2)) < 1e-12        
        end
    end
end

# test that everything is closed and opened properly
for (idx, event) in enumerate(LCIO.open("test.slcio"))
    @test getEventNumber(event) == idx-1
    @test length(getCollectionNames(event)) == 17
    @test getDetectorName(event) == "sidaug05"
end
