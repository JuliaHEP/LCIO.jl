getGeneratorStatus(mcp) = icxx"$(mcp)->getGeneratorStatus();"
getSimulatorStatus(mcp) = icxx"$(mcp)->getSimulatorStatus();"
getPDG(mcp) = icxx"$(mcp)->getPDG();"

function printMCParticle(mcp)
    id = getPDG(mcp)
    genStatus = getMCPGenStatus(mcp)
    p4 = getP4(mcp)
    @printf("%-7d%-3d%s\n", id, genStatus, p4)
end
<<<<<<< HEAD

setMCPDGid(mcp::Ptr{Void}) = ccall((:lcmcpsetpdg, libLCIO), Cint, (Ptr{Void}, ), mcp)

setMCPMass(mcp::Ptr{Void}) = ccall((:lcmcpsetmass, libLCIO), Cfloat, (Ptr{Void}, ), mcp)
=======
>>>>>>> Cxx
