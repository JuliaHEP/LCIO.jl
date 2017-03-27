using Compat
using BinDeps

libdir_opt = ""

@BinDeps.setup

cxx_wrap_dir = Pkg.dir("CxxWrap","deps","usr","lib","cmake")

liblciowrap = library_dependency("liblciowrap")
prefix=joinpath(BinDeps.depsdir(liblciowrap),"usr")
# if the LCIO environment has been sourced, use it. Otherwise, download from svn
lciowrap_builddir = joinpath(BinDeps.depsdir(liblciowrap),"builds","lciowrap")
lciowrap_srcdir = joinpath(BinDeps.depsdir(liblciowrap), "src", "lciowrap")
# use a generic, unversioned dir name to simplify installation and keep travis installation
# reasonably symmetric with this version
lcio_libdir = haskey(ENV, "LCIO") ? ENV["LCIO"] : joinpath(lciowrap_builddir, "LCIO_LIB", "lib")
lcio_srcdir = haskey(ENV, "LCIO") ? ENV["LCIO"] : joinpath(lciowrap_builddir, "LCIO_LIB")
lib_prefix = @static is_windows() ? "" : "lib"
lib_suffix = @static is_windows() ? "dll" : (@static is_apple() ? "dylib" : "so")
lcio_library = joinpath(lcio_libdir, "$(lib_prefix)lcio.$(lib_suffix)")

genopt = "Unix Makefiles"

# LCIOConfig.cmake needs to be available. If not there, check out the LCIO source
# unfortunately, this only works on POSIX at the moment
# remove the previous libraries -- always build at least the library
isfile(joinpath(prefix, "lib$libdir_opt", "$(lib_prefix)lciowrap.$(lib_suffix)")) && rm(joinpath(prefix, "lib$(libdir_opt)", "$(lib_prefix)lciowrap.$(lib_suffix)"))
isfile(lcio_library) && rm(lcio_library)

lcioversion = "02-07-04"
provides(BuildProcess,
  (@build_steps begin
    FileRule(lcio_library, @build_steps begin
        CreateDirectory(lciowrap_builddir)
        @build_steps begin
            ChangeDirectory(lciowrap_builddir)
                `sh $(joinpath(BinDeps.depsdir(liblciowrap), "install_lcio.sh")) $(lcioversion)`
        end
    end)
    CreateDirectory(lciowrap_builddir)
        @build_steps begin
            ChangeDirectory(lciowrap_builddir)
            FileRule(joinpath(prefix, "lib$libdir_opt", "$(lib_prefix)lciowrap.$lib_suffix"), @build_steps begin
                `cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_BUILD_TYPE="Release" -DCxxWrap_DIR="$cxx_wrap_dir" -DLIBDIR_SUFFIX="$libdir_opt" -DLCIO_INSTALLDIR="$lcio_srcdir" $lciowrap_srcdir`
                `make`
                `make install`
            end)
        end
    end),liblciowrap)
deps = [liblciowrap]

@BinDeps.install Dict([(:liblciowrap, :_l_lciowrap)])
