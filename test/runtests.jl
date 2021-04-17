using LCIO
using Test
using LinearAlgebra
import CxxWrap: isnull

# test iteration
LCIO.open("test_DST.slcio") do reader
for event in reader
    # for c in getCollectionNames(event)
    #     println(c)
    # end
    for vtx in getCollection(event, "BuildUpVertex")
        cv = LCIO.getCovMatrix(vtx)
        cvm = getCovarianceMatrix(vtx)
        for i in size(cvm, 1)
            @test cv[i] == cvm[1, i]
        end
    end
    for trk in getCollection(event, "MarlinTrkTracks")
        cv = LCIO.getCovMatrix(trk)
        cvm = getCovarianceMatrix(trk)
        for i in size(cvm, 1)
            @test cv[i] == cvm[1, i]
        end
    end
    mcparts = getCollection(event, "MCParticlesSkimmed")
    for p in mcparts
        e = getEnergy(p)
        @test e > 0
        momentum = getMomentum(p)
        if getPDG(p) != 22
            # tolerance, since we're dealing with floats
            @test getEnergy(p)^2 - sum(momentum.^2) > -1e-11
        else
            @test abs(getEnergy(p)^2 - sum(momentum.^2)) < 1e-12
        end
        parentList = getParents(p)
        for parent in parentList
            @test getEnergy(parent) > 0
        end
        #direct access
        nParents = length(parentList)
        for i = 1:nParents
            @test getEnergy(parentList[i]) > 0
        end
    end
end
end
println("DST test successful")

# test that everything is closed and opened properly
LCIO.open("test_hits.slcio") do reader
iEvent = 0
for event in reader
    iEvent += 1
    collectionList = getCollectionNames(event)
    # for c in collectionList
    #     println(c)
    # end
    @test collectionList[1] == "BeamCalHits"
    @test collectionList[length(collectionList)] == "VertexJets"
    @test length(getCollectionNames(event)) == 69
    @test getDetectorName(event) == "SiD_o2_v03"
    HcalBarrelHits = getCollection(event, "HCalBarrelHits")
    @test getTypeName(HcalBarrelHits) == "LCIO.SimCalorimeterHit"
    decode = CellIDDecoder(HcalBarrelHits)
    l = length(HcalBarrelHits)
    # getNumberOfElements should not be exported
    @test l == LCIO.getNumberOfElements(HcalBarrelHits)
    iHit = 0
    for h in HcalBarrelHits
        @test 0 <= decode(h)["layer"] <= 39
        iHit += 1
        # HcalBarrelHits is of type SimCalorimeterHit
        @test LCIO.getNMCContributions(h) > 0
        for iMCP = 1:LCIO.getNMCContributions(h)
            # we're just running over the hits to make sure there's no segfault due to off-by-one access
            # we're translating the index by -1 before calling the C++ function.
            LCIO.getPDGCont(h, iMCP)
        end
    end
    # test iteration -- julia counting vs. C counting
    @test l == iHit
end
@test iEvent == length(reader)
end
println("Hit test successful")

# test that everything is closed and opened properly
LCIO.open("test_miniDST.slcio") do reader
    iEvent = 0
    for event in reader
        iEvent += 1
        recoParticles = getCollection(event, "PandoraPFOs")
        mcParticles = getCollection(event, "MCParticlesSkimmed")
        reco2mcpRelation = getCollection(event, "RecoMCTruthLink")
        relationNavigator = LCIO.LCRelationNavigator(reco2mcpRelation)
        diff = 0.0
        nParticles = 0
        for rp in recoParticles
            mcpList = getRelatedToObjects(relationNavigator, rp)
            @test length(mcpList) > 0
            mcpWeights = getRelatedToWeights(relationNavigator, rp)
            @test length(mcpList) == length(mcpWeights)
            diff += norm(getMomentum(rp) - getMomentum(mcpList[1]))
            nParticles += 1
            # isnull(getParticleIDUsed(rp)) || println(getParticleIDUsed(rp))
        end
        # println("Average difference between Reco and MCParticle momentum: ", diff/nParticles, " GeV")
        jets = getCollection(event, "Refined2Jets")
        if length(jets) < 2
            continue
        end
        jetPIDh = PIDHandler(jets)
        ilcfi = LCIO.getAlgorithmID(jetPIDh, "lcfiplus")
        # @show ilcfi
        ibtag = getParameterIndex(jetPIDh, ilcfi, "BTag") # algorithm 0 is "lcfiplus"
        ictag = getParameterIndex(jetPIDh, ilcfi, "CTag") # algorithm 0 is "lcfiplus"
        iotag = getParameterIndex(jetPIDh, ilcfi, "OTag") # algorithm 0 is "lcfiplus"
        # @show ibtag
        # @show ictag
        # @show iotag
        pfoPIDh = PIDHandler(recoParticles)
        idEdx = LCIO.getAlgorithmID(pfoPIDh, "dEdxPID")
        iLikelihood = LCIO.getAlgorithmID(pfoPIDh, "LikelihoodPID")
        # @show iLikelihood
        iKaonLike = getParameterIndex(pfoPIDh, iLikelihood, "kaonLikelihood") # algorithm 3 is "LikelihoodPID"
        iPionLike = getParameterIndex(pfoPIDh, iLikelihood, "pionLikelihood")
        # @show iKaonLike
        # @show iPionLike
        for j in jets
            tagList = getParameters(getParticleID(jetPIDh, j, ilcfi))
            if !isnull(tagList)
                btag = tagList[ibtag]
                ctag = tagList[ictag]
                otag = tagList[iotag]
                # println(btag, "\t", ctag, "\t", otag)
            end
            parts = getParticles(j)
            for p in parts
                pidList = getParameters(getParticleID(pfoPIDh, p, iLikelihood))
                # print(length(pidList), "\t")
                if length(pidList) > 0
                    L_pi = pidList[iPionLike]
                    L_K = pidList[iPionLike]
                    # @show L_pi
                    # @show L_K
                    # print("Kaon PID: ", L_K/(L_pi+L_K)) 
                end
                # println()
            end
        end
    end
    @test iEvent == length(reader)
end
println("MiniDST test successful")
    
# test the stdhep reader
iEvent = 0
LCIO.openStdhep("test.stdhep") do reader
    global iEvent
    for event in reader
        iEvent += 1
        iParticle = 0
        for particle in event
            iParticle += 1
        end
        @test iParticle == length(event)
    end
end
@test iEvent > 0
println("Stdhep test successful: ", iEvent, " events")

# test creating a new file and writing out a particle
wrt = LCIO.createLCWriter()

LCIO.open(wrt, "writeTest.slcio", LCIO.WRITE_NEW)
run = LCIO.LCRunHeaderImpl()
LCIO.setRunNumber(run, 0)
LCIO.setValue(LCIO.parameters(run),"Purpose","runTest")
LCIO.setValue(LCIO.parameters(run), "intval", 1 )
LCIO.setValue(LCIO.parameters(run), "floatval", 3.14f0)
LCIO.writeRunHeader(wrt, run)
p = 5.0
pdg = -13
charge = +1.f0
mass =  0.105658f0
theta = 85. / 180.f0 * pi
for i in 1:1
    evt = LCIO.LCEventImpl()
    col = LCIO.LCCollectionVec(LCIO.MCPARTICLE)
    LCIO.setTransient(col, false)
    LCIO.setEventNumber(evt, i)
    phi = rand() * 2pi
    energy = sqrt(mass^2 + p^2)
    px = p * cos(phi) * sin(theta)
    py = p * sin(phi) * sin(theta)
    pz = p * cos(theta)
    momentum  = [ px, py, pz ]
#--------------- create MCParticle -------------------
    mcp = LCIO.MCParticleImpl()
    LCIO.setGeneratorStatus(mcp, 1)
    LCIO.setMass(mcp, mass)
    LCIO.setPDG(mcp, pdg)
    LCIO.setMomentum(mcp, momentum)
    LCIO.setCharge(mcp, charge)
    LCIO.addElement(col, mcp)
    LCIO.addCollection(evt, col, "genParticles")
    LCIO.writeEvent(wrt, evt)
end
LCIO.close(wrt)
println("WriteEvent successful")
