using LCIO
using HEP
using PyPlot
using HDF5

nBins = 1000
xaxis = linspace(-63,63,nBins+1)
yaxis = linspace(0,2*π,nBins+1)
layer1hist = Hist2D((xaxis, yaxis), zeros(nBins, nBins), zeros(nBins, nBins), :left)
layer2hist = Hist2D((xaxis, yaxis), zeros(nBins, nBins), zeros(nBins, nBins), :left)
layer3hist = Hist2D((xaxis, yaxis), zeros(nBins, nBins), zeros(nBins, nBins), :left)
layer4hist = Hist2D((xaxis, yaxis), zeros(nBins, nBins), zeros(nBins, nBins), :left)
layer5hist = Hist2D((xaxis, yaxis), zeros(nBins, nBins), zeros(nBins, nBins), :left)

r(pos) = sqrt(pos[1]*pos[1]+pos[2]*pos[2])
phi(pos) = atan2(pos[2], pos[1]) + pi

function plotHist(histogram, label)
	filename = "$(label)barrelHitmap.h5"
	h5write(filename, "hitmap", histogram.weights)
	h5write(filename, "xaxis", collect(xaxis))
	h5write(filename, "yaxis", collect(yaxis))
	#p = pcolormesh(xaxis, yaxis, histogram.weights')
	#colorbar(p)
	#savefig(" $(label)barrelHitmap.png" )
	#clf()
	#data = histogram.weights[:]
	#plt[:hist](data, bins=linspace(1, maximum(data)), log=true)
	#savefig(" $(label)barrelHitDistribution.svg" )
	#clf()
end

const LCIOPATH="/pic/projects/ilc/users/stru821/DATA/GuineaPig/mergedSLCIO/"
for lciofile in readdir(LCIOPATH)
	for evt in LCIO.open(joinpath(LCIOPATH, lciofile))
	    # tb = getCollection(evt, "SiTrackerBarrelHits")
	    # te = getCollection(evt, "SiTrackerEndcapHits")
	    # tf = getCollection(evt, "SiTrackerForwardHits")
	    vb = getCollection(evt, "SiVertexBarrelHits")
	    # ve = getCollection(evt, "SiVertexEndcapHits")
	    for hit in vb
		pos = getPosition(hit)
		radius = r(pos)
		if radius < 17
		    push!(layer1hist, (pos[3], phi(pos)), 1)
		elseif radius < 25
		    push!(layer2hist, (pos[3], phi(pos)), 1)
		elseif radius < 38
		    push!(layer3hist, (pos[3], phi(pos)), 1)
		elseif radius < 50.6
		    push!(layer4hist, (pos[3], phi(pos)), 1)
		else
		    push!(layer5hist, (pos[3], phi(pos)), 1)
		end
	    end
	end
end
plotHist(layer1hist, "layer1")
plotHist(layer2hist, "layer2")
plotHist(layer3hist, "layer3")
plotHist(layer4hist, "layer4")
plotHist(layer5hist, "layer5")
