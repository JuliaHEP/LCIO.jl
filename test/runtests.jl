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
LCIO.iterate("test.slcio") do event
    @test length(getCollectionNames(event)) == 17
    @test getDetectorName(event) == "sidaug05"
end
