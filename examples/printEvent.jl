using LCIO
using HEP
using PyPlot

nBins = 20
xyaxis = linspace(0,1,nBins+1)
thetaaxis = linspace(-π/2, π/2, nBins+1)
phiaxis = linspace(0,2*π,nBins+1)

xyplot = Hist2D((xyaxis, xyaxis), zeros(nBins, nBins), zeros(nBins, nBins), :left)
rphiplot = Hist2D((phiaxis, thetaaxis), zeros(nBins, nBins), zeros(nBins, nBins), :left)
energies = Float32[]

r(p) = sqrt(p.x*p.x+p.y*p.y)
ϕ(p) = atan2(p.y, p.x)
θ(p) = if -1 < p.z/r(p) < 1 acos(p.z/r(p)) else 0 end

for (idx, event) in enumerate(LCIO.open(ARGS[1]))
	# println("Event #", idx)
	# @printf("%-30s%-20s%10s\n", "Collection Name", "Element Type", "Elements")
	# println(repeat("=", 60))
	for c in getCollection(event, "MCParticlesSkimmed")
		if abs(getMCPDGid(c)) == 13
			p4 = getMCP4(c)
			push!(xyplot, (p4.x, p4.y), 1)
			push!(rphiplot, (θ(p4), ϕ(p4)), 1)
			push!(energies, p4.t)
		end
	end
end

p = pcolormesh(xyaxis, xyaxis, xyplot.weights')
colorbar(p)
savefig("xyplot.png")
clf()
p = pcolormesh(thetaaxis, phiaxis, rphiplot.weights')
colorbar(p)
savefig("rphiplot.png")
clf()
plt[:hist](energies)
savefig("energyHist.png")
