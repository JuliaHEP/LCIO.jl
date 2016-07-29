using LCIO

for (idx, event) in enumerate(LCIO.open(ARGS[1]))
	println("Event #", idx)
	@printf("%-30s%-20s%10s\n", "Collection Name", "Element Type", "Elements")
	println(repeat("=", 60))
	for name in getCollectionNames(event)
		c = getCollection(event, name)
		@printf("%-30s%-20s%10d\n", name, getCollectionTypeName(c), length(c))
		if contains(getCollectionTypeName(c), "Jets")
			jetsCollection = getCollectionTypeName(c)
		end
	end
end
