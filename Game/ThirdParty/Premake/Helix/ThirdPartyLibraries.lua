libraryDirectories = {}
libraryDirectories["PIXx64"] = heGetThirdPartyModule( "WinPixEventRuntime" ) .. "bin/x64/"


libraries = {}
libraries["PIXWinDesktop"] = "WinPixEventRuntime.lib"
libraries["PIXWinUniversal"] = "WinPixEventRuntime_UAP.lib"
