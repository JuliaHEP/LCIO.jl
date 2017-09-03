using Compat
using BinDeps

lcioversion = "02-09"

@BinDeps.setup

jlcxx_dir = Pkg.dir("CxxWrap","deps","usr","share","cmake","JlCxx")

liblciowrap = library_dependency("liblciowrap")
prefix = BinDeps.depsdir(liblciowrap)
downloads = joinpath(prefix, "downloads")
lciowrap_builddir = joinpath(prefix, "builds", "lciowrap")
lciowrap_srcdir = joinpath(prefix, "src", "lciowrap")
lciowrap_destdir = joinpath(prefix, "usr")
lcio_srcdir = joinpath(prefix, "src", "LCIO-$(lcioversion)")
# if the LCIO environment has been sourced, use it. Otherwise, download the source and build
lcio_destdir = haskey(ENV, "LCIO") ? ENV["LCIO"] : joinpath(prefix, "usr")

libdir_opt = ""
lib_prefix = @static is_windows() ? "" : "lib"
lib_suffix = @static is_windows() ? "dll" : (@static is_apple() ? "dylib" : "so")

lcio_library = joinpath(lcio_destdir, "lib", "$(lib_prefix)lcio.$(lib_suffix)")
lciowrap_library = joinpath(prefix, "usr", "lib$libdir_opt", "$(lib_prefix)lciowrap.$(lib_suffix)")

genopt = "Unix Makefiles"

# remove the previous libraries -- always build at least the library
isfile(lciowrap_library) && rm(lciowrap_library)
isfile(joinpath(lciowrap_builddir, "lib$libdir_opt", "$(lib_prefix)lciowrap.$(lib_suffix)")) && rm(joinpath(lciowrap_builddir, "lib$(libdir_opt)", "$(lib_prefix)lciowrap.$(lib_suffix)"))

provides(BuildProcess,
  (@build_steps begin
		# we're looking for the cmake file, even though the dependencies are the libs
    FileRule(lcio_library, @build_steps begin
        CreateDirectory(lciowrap_builddir)
        @build_steps begin
            ChangeDirectory(lciowrap_builddir)
            FileDownloader("https://github.com/iLCSoft/LCIO/archive/v$(lcioversion).tar.gz", joinpath(downloads, "v$(lcioversion).tar.gz"))
						CreateDirectory(joinpath(prefix, "src"))
            FileUnpacker(joinpath(downloads, "v$(lcioversion).tar.gz"), joinpath(prefix, "src"), "LCIO-$(lcioversion)")
						# GetSources(liblcio)
						CreateDirectory(joinpath(lcio_srcdir, "build"))
						@build_steps begin
							ChangeDirectory(joinpath(lcio_srcdir, "build"))
							`cmake -G "$genopt" -DCMAKE_INSTALL_PREFIX=$(lcio_destdir) $(lcio_srcdir)`
							`make`
							`make install`
						end
        end
    end)
    CreateDirectory(lciowrap_builddir)
    @build_steps begin
      ChangeDirectory(lciowrap_builddir)
      FileRule(lciowrap_library, @build_steps begin
      	`cmake -G "$(genopt)" -DCMAKE_INSTALL_PREFIX="$(lciowrap_destdir)" -DCMAKE_BUILD_TYPE="Release" -DJlCxx_DIR="$(jlcxx_dir)" -DLCIO_DIR="$(lcio_destdir)"  $(lciowrap_srcdir)`
        `make`
        `make install`
      end)
    end
  end),liblciowrap)

# deps = [liblcio, liblciowrap]

@BinDeps.install Dict([(:liblciowrap, :_l_lciowrap)])
