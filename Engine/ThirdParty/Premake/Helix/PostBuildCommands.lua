postBuildCommands = {}

postBuildCommands["debugContentDir"] = "IF NOT EXIST $(TargetDir)Content mklink /D $(TargetDir)Content %{wks.location}Content"
postBuildCommands["releaseContentDir"] = "{COPY} %{wks.location}Content %{cfg.targetdir}/Content"