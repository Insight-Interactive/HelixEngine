-- Copyright 2021 Insight Interactive. All Rights Reserved.

postBuildCommands = {}
postBuildCommands["debugContentDir"]			= "IF NOT EXIST \"$(TargetDir)Content\" mklink /D \"$(TargetDir)Content\" \"%{wks.location}Game/Content\""
postBuildCommands["releaseContentDir"]			= "{COPY} %{wks.location}Game/Content %{cfg.targetdir}/Content"
postBuildCommands["gameConfigCopy"]				= "{COPY} %{wks.location}Game/Config/* $(TargetDir)"
postBuildCommands["clearModuleLibs"]			= "DEL \"$(TargetDir)/*.lib\""
postBuildCommands["clearModuleLibLinkExtras"]	= "DEL \"$(TargetDir)/*.exp\""

