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

liblciowrap = library_dependency("liblciowrap")
prefix=joinpath(BinDeps.depsdir(liblciowrap),"usr")
lciowrap_srcdir = joinpath(BinDeps.depsdir(liblciowrap),"src","lciowrap")
lciowrap_builddir = joinpath(BinDeps.depsdir(liblciowrap),"builds","lciowrap")
lib_prefix = @static is_windows() ? "" : "lib"
lib_suffix = @static is_windows() ? "dll" : (@static is_apple() ? "dylib" : "so")

# Set generator if on windows
genopt = "Unix Makefiles"
@static if is_windows()
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
  end),liblciowrap)

deps = [liblciowrap]

@BinDeps.install Dict([(:liblciowrap, :_l_lciowrap)])

@static if is_windows()
  if haskey(ENV, "BUILD_ON_WINDOWS") && ENV["BUILD_ON_WINDOWS"] == "1"
    empty!(BinDeps.defaults)
    append!(BinDeps.defaults, saved_defaults)
  end
end
