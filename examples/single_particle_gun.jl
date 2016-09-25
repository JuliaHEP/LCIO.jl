#####################################
#
# simple script to create lcio files with single particle
# events - modify as needed
# @author J. Strube, PNNL
# @date 2016-09-21
#
#####################################
using LCIO

#---- number of events per momentum bin -----
nevt = 1000

outfile = "mcparticles.slcio"

#--------------------------------------------


wrt = LCIO.createLCWriter()

LCIO.open(wrt, outfile, 0)

# ========== particle properties ===================

momenta = [ 5. ]

genstat  = 1
pdg = -13
charge = +1.f0
mass =  0.105658f0
theta = 85./180.f0 * pi

decayLen = 1.e32
# =================================================

# write a RunHeader
run = LCIO.LCRunHeaderImpl()
LCIO.setRunNumber(run, 0)
LCIO.setValue(LCIO.parameters(run),"Generator","\${lcgeo_DIR}/examples/lcio_particle_gun.py")
LCIO.setValue(LCIO.parameters(run), "PDG", pdg )
LCIO.setValue(LCIO.parameters(run), "Charge", charge )
LCIO.setValue(LCIO.parameters(run), "Mass", mass )
LCIO.writeRunHeader(wrt, run)
# ================================================
for p in momenta
    for j in 1:nevt
        col = LCIO.LCCollectionVec(LCIO.MCPARTICLE)
        evt = LCIO.LCEventImpl()
        LCIO.setEventNumber(evt, j)
        LCIO.addCollection(evt, col, "genParticles")
        phi = rand() * 2pi
        energy   = sqrt( mass*mass  + p * p )
        px = p * cos( phi ) * sin( theta )
        py = p * sin( phi ) * sin( theta )
        pz = p * cos( theta )

        momentum  = [ px, py, pz ]

        epx = decayLen * cos( phi ) * sin( theta )
        epy = decayLen * sin( phi ) * sin( theta )
        epz = decayLen * cos( theta )
        endpoint = [ epx, epy, epz ]

#--------------- create MCParticle -------------------
        mcp = LCIO.MCParticleImpl()
        LCIO.setGeneratorStatus(mcp, genstat )
        LCIO.setMass(mcp, mass )
        LCIO.setPDG(mcp, pdg )
        LCIO.setMomentum(mcp, momentum )
        LCIO.setCharge(mcp, charge )
        LCIO.addElement(col, mcp)
        LCIO.writeEvent(wrt, evt )
    end
end
LCIO.close(wrt)
