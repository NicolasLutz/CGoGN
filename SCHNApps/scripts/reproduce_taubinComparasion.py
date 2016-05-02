view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");
Mask1_decim005_degree2_lambda001 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda001_2d50.ply");
Mask1_decim005_degree2_lambda001_1 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda001_2d50.ply");
Mask1_decim005_degree2_lambda001_2 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda001_2d50.ply");
Mask1_decim005_degree2_lambda001_3 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda001_2d50.ply");
view_1 = schnapps.getView("view_1");
view_1 = schnapps.splitView("view_0", 1);
view_2 = schnapps.getView("view_2");
view_2 = schnapps.splitView("view_1", 1);
view_3 = schnapps.getView("view_3");
view_3 = schnapps.splitView("view_2", 1);
view_0.linkMap(Mask1_decim005_degree2_lambda001.getName());
view_1.linkMap(Mask1_decim005_degree2_lambda001_1.getName());
view_2.linkMap(Mask1_decim005_degree2_lambda001_2.getName());
view_3.linkMap(Mask1_decim005_degree2_lambda001_3.getName());
view_0.linkPlugin("Surface_Radiance");
view_1.linkPlugin("Surface_Radiance");
view_2.linkPlugin("Surface_Radiance");
view_3.linkPlugin("Surface_Radiance");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001.getName());
Mask1_decim005_degree2_lambda001.setBBVertexAttribute("position");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001.getName());
Mask1_decim005_degree2_lambda001.createVBO("position");
Mask1_decim005_degree2_lambda001.createVBO("normal");
Mask1_decim005_degree2_lambda001.createVBO("radiance");
Surface_Radiance.changePositionVBO(Mask1_decim005_degree2_lambda001.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree2_lambda001.getName(), "normal");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_1.getName());
Mask1_decim005_degree2_lambda001_1.setBBVertexAttribute("position");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_1.getName());
Mask1_decim005_degree2_lambda001_1.createVBO("position");
Mask1_decim005_degree2_lambda001_1.createVBO("normal");
Mask1_decim005_degree2_lambda001_1.createVBO("radiance");
Surface_Radiance.changePositionVBO(Mask1_decim005_degree2_lambda001_1.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree2_lambda001_1.getName(), "normal");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_2.getName());
Mask1_decim005_degree2_lambda001_2.setBBVertexAttribute("position");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_2.getName());
Mask1_decim005_degree2_lambda001_2.createVBO("position");
Mask1_decim005_degree2_lambda001_2.createVBO("normal");
Mask1_decim005_degree2_lambda001_2.createVBO("radiance");
Surface_Radiance.changePositionVBO(Mask1_decim005_degree2_lambda001_2.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree2_lambda001_2.getName(), "normal");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_3.getName());
Mask1_decim005_degree2_lambda001_3.setBBVertexAttribute("position");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_3.getName());
Mask1_decim005_degree2_lambda001_3.createVBO("position");
Mask1_decim005_degree2_lambda001_3.createVBO("normal");
Mask1_decim005_degree2_lambda001_3.createVBO("radiance");
Surface_Radiance.changePositionVBO(Mask1_decim005_degree2_lambda001_3.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree2_lambda001_3.getName(), "normal");
view_1.setCurrentCamera("camera_0");
view_2.setCurrentCamera("camera_0");
view_3.setCurrentCamera("camera_0");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001.getName());
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_1.getName());
Surface_Radiance.applyTaubinFilter(Mask1_decim005_degree2_lambda001_1.getName(), "position", 10);
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_2.getName());
Surface_Radiance.applyTaubinFilter(Mask1_decim005_degree2_lambda001_2.getName(), "position", 20);
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_3.getName());
Surface_Radiance.applyTaubinFilter(Mask1_decim005_degree2_lambda001_3.getName(), "position", 30);
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001.getName());
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_1.getName(), "position", "normal", "distance", "radianceDistance.csv");
Mask1_decim005_degree2_lambda001.createVBO("distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_2.getName(), "position", "normal", "distance", "radianceDistance.csv");
Mask1_decim005_degree2_lambda001.createVBO("distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_3.getName(), "position", "normal", "distance", "radianceDistance.csv");

schnapps.restoreSplitViewPositions("38 0 0 0 -1 0 0 0 0 0 0 0 4 0 0 0 -112 0 0 0 -112 0 0 0 -112 0 0 0 -112 1 0 0 0 6 1 0 0 0 1 ")
schnapps.getCamera("camera_0").fromString("6.13595 124.64 -215.558 -0.5 -0.5 -0.5 0.5")
schnapps.getCamera("camera_1").fromString("0 0 265.045 0 0 0 1")
schnapps.getCamera("camera_2").fromString("0 0 265.832 0 0 0 1")
schnapps.getCamera("camera_3").fromString("0 0 265.832 0 0 0 1")
Mask1_decim005_degree2_lambda001.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_1.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_2.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_3.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
schnapps.setWindowSize(1855, 1056)
schnapps.setFloatingControlDock(0)
schnapps.setFloatingPluginDock(0)
schnapps.setFloatingPythonDock(0)
view_0 = schnapps.getView("view_0");
view_1 = schnapps.getView("view_1");
view_2 = schnapps.getView("view_2");
view_3 = schnapps.getView("view_3");
Mask1_decim005_degree2_lambda001_2d50 = schnapps.getMap(Mask1_decim005_degree2_lambda001_2d50.getName());  #replace by  = previous_var if appending
Mask1_decim005_degree2_lambda001_2d50_1 = schnapps.getMap(Mask1_decim005_degree2_lambda001_2d50_1.getName());  #replace by  = previous_var if appending
Mask1_decim005_degree2_lambda001_2d50_2 = schnapps.getMap(Mask1_decim005_degree2_lambda001_2d50_2.getName());  #replace by  = previous_var if appending
Mask1_decim005_degree2_lambda001_2d50_3 = schnapps.getMap(Mask1_decim005_degree2_lambda001_2d50_3.getName());  #replace by  = previous_var if appending
Mask1_decim005_degree2_lambda001_3d50 = Surface_Radiance.importFromFile_SH(Mask1_decim005_degree2_lambda001_3d50.getName());
Mask1_decim005_degree2_lambda001_4d50 = Surface_Radiance.importFromFile_SH(Mask1_decim005_degree2_lambda001_4d50.getName());
Mask1_decim005_degree2_lambda001_5d50 = Surface_Radiance.importFromFile_SH(Mask1_decim005_degree2_lambda001_5d50.getName());
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_2d50_1.getName());
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_2d50_3.getName());
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_3d50.getName());
Mask1_decim005_degree2_lambda001_3d50.setBBVertexAttribute("position");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_3d50.getName());
Mask1_decim005_degree2_lambda001_3d50.createVBO("position");
Mask1_decim005_degree2_lambda001_3d50.createVBO("normal");
Mask1_decim005_degree2_lambda001_3d50.createVBO("radiance");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_4d50.getName());
Mask1_decim005_degree2_lambda001_4d50.setBBVertexAttribute("position");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_4d50.getName());
Mask1_decim005_degree2_lambda001_4d50.createVBO("position");
Mask1_decim005_degree2_lambda001_4d50.createVBO("normal");
Mask1_decim005_degree2_lambda001_4d50.createVBO("radiance");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_5d50.getName());
Mask1_decim005_degree2_lambda001_5d50.setBBVertexAttribute("position");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_5d50.getName());
Mask1_decim005_degree2_lambda001_5d50.createVBO("position");
Mask1_decim005_degree2_lambda001_5d50.createVBO("normal");
Mask1_decim005_degree2_lambda001_5d50.createVBO("radiance");
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_3d50.getName());
schnapps.setSelectedMap(Mask1_decim005_degree2_lambda001_5d50.getName());
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_3d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_4d50.getName(), "position", "normal", "distance");
Mask1_decim005_degree2_lambda001_2d50.createVBO("distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_5d50.getName(), "position", "normal", "distance");
Mask1_decim005_degree2_lambda001_2d50_1.createVBO("distance");
Mask1_decim005_degree2_lambda001_2d50_2.createVBO("distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_1.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_2d50_2.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_1.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_2d50_3.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_1.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_3d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_1.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_4d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_1.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_5d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_2.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_2d50_3.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_2.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_3d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_2.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_4d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_2.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_5d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_3.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_3d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_3.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_4d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_2d50_3.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_5d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_3d50.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_4d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_3d50.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_5d50.getName(), "position", "normal", "distance");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001_4d50.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda001_5d50.getName(), "position", "normal", "distance");

schnapps.restoreSplitViewPositions("38 0 0 0 -1 0 0 0 0 0 0 0 4 0 0 0 -112 0 0 0 -112 0 0 0 -112 0 0 0 -112 1 0 0 0 6 1 0 0 0 1 ")
schnapps.getCamera("camera_0").fromString("6.14321 273.319 -215.559 -0.5 -0.5 -0.5 0.5")
schnapps.getCamera("camera_1").fromString("0 0 265.045 0 0 0 1")
schnapps.getCamera("camera_2").fromString("0 0 265.832 0 0 0 1")
schnapps.getCamera("camera_3").fromString("0 0 265.832 0 0 0 1")
Mask1_decim005_degree2_lambda001_2d50.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_2d50_1.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_2d50_2.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_2d50_3.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_3d50.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_4d50.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
Mask1_decim005_degree2_lambda001_5d50.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
schnapps.setWindowSize(1855, 1056)
