using LCIO
using StaticArrays
using BenchmarkTools

function testNew()
	LCIO.open("/home/jstrube/.julia/dev/LCIO/test/test_DST.slcio") do reader
		for event in reader
		    mcparts = getCollection(event, "MCParticlesSkimmed")
		    for p in mcparts
			println(@btime getMomentum($p))
			break
		    end
		    break
		end
	end
end
function testTuple()
	LCIO.open("/home/jstrube/.julia/dev/LCIO/test/test_DST.slcio") do reader
		for event in reader
		    mcparts = getCollection(event, "MCParticlesSkimmed")
		    for p in mcparts
			println(@btime LCIO._getMomentum($p))
			break
		    end
		    break
		end
	end
end

function getMomentumOld(particle)
	p3 = Array{Float64,1}(undef, 3)
	valid = LCIO.getMomentum3(particle, p3)
	return p3
end

function testOld()
	LCIO.open("/home/jstrube/.julia/dev/LCIO/test/test_DST.slcio") do reader
		for event in reader
		    mcparts = getCollection(event, "MCParticlesSkimmed")
		    for p in mcparts
			println(@btime getMomentumOld($p))
			break
		    end
		    break
		end
	end
end
function testOldSA()
	LCIO.open("/home/jstrube/.julia/dev/LCIO/test/test_DST.slcio") do reader
		for event in reader
		    mcparts = getCollection(event, "MCParticlesSkimmed")
		    for p in mcparts
			println(@btime SVector{3, Float64}(getMomentumOld($p)))
			break
		    end
		    break
		end
	end
end
function testNewSA()
	LCIO.open("/home/jstrube/.julia/dev/LCIO/test/test_DST.slcio") do reader
		for event in reader
		    mcparts = getCollection(event, "MCParticlesSkimmed")
		    for p in mcparts
			println(@btime SVector{3, Float64}(getMomentum($p)))
			break
		    end
		    break
		end
	end
end
function testTupleSA()
	LCIO.open("/home/jstrube/.julia/dev/LCIO/test/test_DST.slcio") do reader
		for event in reader
		    mcparts = getCollection(event, "MCParticlesSkimmed")
		    for p in mcparts
			println(@btime SVector{3, Float64}(LCIO._getMomentum($p)))
			break
		    end
		    break
		end
	end
end

testOld()
testNew()
testTuple()
testOldSA()
testNewSA()
testTupleSA()
