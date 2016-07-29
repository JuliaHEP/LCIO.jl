function printMCParticle(mcp)
    id = getPDG(mcp)
    genStatus = getMCPGenStatus(mcp)
    p4 = getP4(mcp)
    @printf("%-7d%-3d%s\n", id, genStatus, p4)
end
