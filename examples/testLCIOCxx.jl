using LCIO

function testReader(fn::AbstractString)
	for event in LCIO.open(fn)
		println(event)
		for x in LCIO.getCollectionNames(event)
 			println(x)
		end
		# for item in getCollection(event, "MCParticle")
		# 	println(getP4(item))
		# end
		x = getCollection(event, "EcalBarrHits")
		for item in x
			# println(getPosition(item))
		end
		println(LCIO.CellIDDecoder(x))
	end
end
testReader(ARGS[1])
