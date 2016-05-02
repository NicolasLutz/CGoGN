view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");

Mask1_decim005_degree1_lambda001 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree1_lambda001_fixed.ply");
Mask1_decim005_degree1_lambda001.createVBO("position");
Mask1_decim005_degree1_lambda001.createVBO("normal");
Mask1_decim005_degree1_lambda001.createVBO("radiance");
Mask1_decim005_degree1_lambda001.setBBVertexAttribute("position");

#FOURTH CALCULATIONS: DEGREE 1 -> DEGREE 0

#decimate the model
Surface_Radiance.changePositionVBO(Mask1_decim005_degree0_lambda001.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree0_lambda001.getName(), "normal");

Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

#decimate, save, calculate, redo
Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_2d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_3d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_4d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_5d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_6d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_7d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_8d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_9d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_10d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_11d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");

Surface_Radiance.decimate(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
Surface_Radiance.exportPLY(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_12d50.ply");
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");
