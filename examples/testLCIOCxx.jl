using Cxx
using LCIO

function testReader(fn::AbstractString)
	for event in LCIO.open(fn)
		println(event)
		for x in getCollectionNames(event)
 			println(x, "\t", String( icxx"$(event)->getCollection($(x))->getTypeName().c_str();") )
		end
		for item in getCollection(event, "MCParticle")
			println(getP4(item))
		end
		for item in getCollection(event, "EcalBarrHits")
			println(getPosition(item))
		end
	end
end
testReader("../test/test.slcio")
