getMCPGenStatus(mcp::Ptr{Void}) = ccall((:lcmcpgetgeneratorstatus, libLCIO), Cint, (Ptr{Void}, ), mcp)

getMCPDGid(mcp::Ptr{Void}) = ccall((:lcmcpgetpdg, libLCIO), Cint, (Ptr{Void}, ), mcp)

function printMCParticle(mcp::Ptr{Void})
    id = getMCPDGid(mcp)
    genStatus = getMCPGenStatus(mcp)
    p4 = getMCP4(mcp)
    @printf("%-7d%-3d%s\n", id, genStatus, p4)
end

createMCParticle() = ccall((:lcmcpcreate, libLCIO), Ptr{Void})

setMCPDGid(mcp::Ptr{Void}) = ccall((:lcmcpsetpdg, libLCIO), Cint, (Ptr{Void}, ), mcp)

setMCPMass(mcp::Ptr{Void}) = ccall((:lcmcpsetmass, libLCIO), Cfloat, (Ptr{Void}, ), mcp)
