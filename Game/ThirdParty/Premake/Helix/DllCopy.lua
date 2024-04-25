dllCopyCommands = {}

dllCopyCommands["PIXWinDesktopx64"] = "{COPY} " .. heGetThirdPartyModule( "WinPixEventRuntime" ) .. "bin/x64/WinPixEventRuntime.dll %{cfg.targetdir}"
dllCopyCommands["PIXWinUniversalx64"] = "{COPY} " .. heGetThirdPartyModule( "WinPixEventRuntime" ) .. "bin/x64/WinPixEventRuntime_UAP.dll %{cfg.targetdir}"
