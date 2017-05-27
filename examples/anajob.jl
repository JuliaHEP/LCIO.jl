using LCIO

LCIO.iterate(ARGS[1]) do event
	println(repeat("/", 35))
	println("EVENT: ", getEventNumber(event))
	println("RUN: ", getRunNumber(event))
	println("DETECTOR: ", getDetectorName(event))
	println("COLLECTIONS: (see below)")
	println(repeat("/", 35))
	println()

	println(repeat("-", 70))
	@printf("%-30s%-30s%-10s\n", "Collection name", "Element type", "Elements")
	println(repeat("=", 70))
	for name in getCollectionNames(event)
		c = getCollection(event, name)
		@printf("%-30s%-30s%-10d\n", name, getTypeName(c), length(c))
	end
	println(repeat("-", 70))
	println()
	println()
end
