using Compat
using BinDeps

lcioversion = "02-08"

libdir_opt = ""
@BinDeps.setup

jlcxx_dir = Pkg.dir("CxxWrap","deps","usr","share","cmake","JlCxx")

liblcio = library_dependency("liblcio")
libsio = library_dependency("libsio")
liblciowrap = library_dependency("liblciowrap")
prefix=joinpath(BinDeps.depsdir(liblciowrap),"usr")
# if the LCIO environment has been sourced, use it. Otherwise, download from svn
lciowrap_builddir = joinpath(BinDeps.depsdir(liblciowrap),"builds","lciowrap")
lciowrap_srcdir = joinpath(BinDeps.depsdir(liblciowrap), "src", "lciowrap")
# use a generic, unversioned dir name to simplify installation and keep travis installation
# reasonably symmetric with this version
lcio_srcdir = joinpath(BinDeps.depsdir(liblcio), "src", "LCIO-$(lcioversion)")
lcio_destdir = haskey(ENV, "LCIO") ? ENV["LCIO"] : prefix
lib_prefix = @static is_windows() ? "" : "lib"
lib_suffix = @static is_windows() ? "dll" : (@static is_apple() ? "dylib" : "so")

# LCIOConfig.cmake needs to be available. If not there, check out the LCIO source
# unfortunately, this only works on POSIX at the moment
lcio_cmake = joinpath(lcio_destdir, "LCIOConfig.cmake")
genopt = "Unix Makefiles"

# remove the previous libraries -- always build at least the library
isfile(joinpath(prefix, "lib$libdir_opt", "$(lib_prefix)lciowrap.$(lib_suffix)")) && rm(joinpath(prefix, "lib$(libdir_opt)", "$(lib_prefix)lciowrap.$(lib_suffix)"))

provides(Sources, URI("https://github.com/iLCSoft/LCIO/archive/v$(lcioversion).tar.gz"), [liblcio, libsio], unpacked_dir="LCIO-$(lcioversion)")
provides(BuildProcess,
  (@build_steps begin
    FileRule(lcio_cmake, @build_steps begin
        CreateDirectory(lciowrap_builddir)
        @build_steps begin
            ChangeDirectory(lciowrap_builddir)
						GetSources(liblcio)
						CreateDirectory(joinpath(lcio_srcdir, "build"))
						@build_steps begin
							ChangeDirectory(joinpath(lcio_srcdir, "build"))
							`cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX=$(lcio_destdir) $(lcio_srcdir)`
							`make`
							`make install`
						end
        end
    end)
	end), [liblcio, libsio]
	)

provides(BuildProcess,
  (@build_steps begin
    CreateDirectory(lciowrap_builddir)
    @build_steps begin
      ChangeDirectory(lciowrap_builddir)
      FileRule(joinpath(prefix, "lib$libdir_opt", "$(lib_prefix)lciowrap.$lib_suffix"), @build_steps begin
      	`cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX="$prefix" -DCMAKE_BUILD_TYPE="Release" -DJlCxx_DIR="$jlcxx_dir" -DLIBDIR_SUFFIX="$libdir_opt" -DLCIO_DIR="$lcio_srcdir"  $lciowrap_srcdir`
        `make`
        `make install`
      end)
    end
  end),liblciowrap)

# deps = [liblcio, libsio, liblciowrap]

@BinDeps.install Dict([(:liblciowrap, :_l_lciowrap)])
