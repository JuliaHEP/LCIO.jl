using Cxx
using LCIO

function testReader(fn::AbstractString)
	for event in LCIOopen(fn)
		println(event)
		for x in getCollectionNames(event)
 			println(x, "\t", String( icxx"$(event)->getCollection($(x))->getTypeName().c_str();") )
		end
		for item in getCollection(event, "MCParticle")
			println(getP4(item))
		end
		for item in getCollection(event, "SiVertexBarrelHits")
			println(getPosition(item))
		end
	end
end
testReader("../SingleParticles/singlemu+_10__scint3x3_mu+_SIM.slcio")
