using Compat
using BinDeps

libdir_opt = ""
@static if is_windows()
  libdir_opt = Sys.WORD_SIZE==32 ? "32" : ""
end

@static if is_windows()
  # prefer building if requested
  if haskey(ENV, "BUILD_ON_WINDOWS") && ENV["BUILD_ON_WINDOWS"] == "1"
    saved_defaults = deepcopy(BinDeps.defaults)
    empty!(BinDeps.defaults)
    append!(BinDeps.defaults, [BuildProcess])
  end
end

@BinDeps.setup

cxx_wrap_dir = Pkg.dir("CxxWrap","deps","usr","lib","cmake")

lciowrap = library_dependency("lciowrap", aliases=["liblciowrap.so"])
prefix=joinpath(BinDeps.depsdir(lciowrap),"usr")
lciowrap_srcdir = joinpath(BinDeps.depsdir(lciowrap),"src","lciowrap")
lciowrap_builddir = joinpath(BinDeps.depsdir(lciowrap),"builds","lciowrap")
lib_prefix = @windows ? "" : "lib"
lib_suffix = @windows? "dll" : (@osx? "dylib" : "so")

# Set generator if on windows
genopt = "Unix Makefiles"
@windows_only begin
  if WORD_SIZE == 64
    genopt = "Visual Studio 14 2015 Win64"
  else
    genopt = "Visual Studio 14 2015"
  end
end

lcio_steps = @build_steps begin
	`cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_BUILD_TYPE="Release" -DCxxWrap_DIR="$cxx_wrap_dir" -DLIBDIR_SUFFIX=$libdir_opt $lciowrap_srcdir`
	`cmake --build . --config Release --target install`
end

# If built, always run cmake, in case the code changed
if isdir(lciowrap_builddir)
  BinDeps.run(@build_steps begin
    ChangeDirectory(lciowrap_builddir)
    lcio_steps
  end)
end

provides(BuildProcess,
  (@build_steps begin
    CreateDirectory(lciowrap_builddir)
    @build_steps begin
      ChangeDirectory(lciowrap_builddir)
      FileRule(joinpath(prefix, "lib$libdir_opt", "$(lib_prefix)lciowrap.$lib_suffix"),lcio_steps)
    end
  end),lciowrap)

deps = [lciowrap]

provides(Binaries, Dict(URI("https://github.com/jstrube/LCIO.jl/releases/download/v0.3.0/LCIO-julia-$(VERSION.major).$(VERSION.minor)-win$(WORD_SIZE).zip") => deps), os = :Windows)

@BinDeps.install Dict([(:lciowrap, :_l_lcio_wrap)])

@static if is_windows()
  if haskey(ENV, "BUILD_ON_WINDOWS") && ENV["BUILD_ON_WINDOWS"] == "1"
    empty!(BinDeps.defaults)
    append!(BinDeps.defaults, saved_defaults)
  end
end
