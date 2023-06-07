
filter ("configurations:ShippingGame")
    postbuildcommands
    {
        "DEL \"$(TargetDir)/*.exp\"",
        "DEL \"$(TargetDir)/*.lib\"",
    }
