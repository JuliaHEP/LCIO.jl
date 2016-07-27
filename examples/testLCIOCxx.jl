using LCIO

function testReader(fn::AbstractString)
	for event in LCIO.open(fn)
		println(event)
		# for x in getCollectionNames(event)
 	# 		println(x)
		# end
		# for item in getCollection(event, "MCParticle")
		# 	println(getP4(item))
		# end
		for item in getCollection(event, "EcalBarrHits")
			println(getPosition(item))
		end
	end
end
testReader(ARGS[1])
