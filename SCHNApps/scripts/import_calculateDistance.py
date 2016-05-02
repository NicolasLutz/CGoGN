################################################################
#Imports a model twice, decimates one of the two versions at 50% and calculate the distance between them (output currently code-dependant).
#It then restarts with 75% of decimation, and then 87%.
################################################################

#replace every occurence of <model> with the name of your model (Mask1_decim005_degree2_lambda001_1.ply)
#replace every occurence of <path> with the relative path to your model (don't end it with /).
#uncomment and copy as many sections as you want to add models for comparing. To fix : removing some models seem to crash the app.

view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");

#same model, but not decimated
Mask1_decim005_degree2_lambda001 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda001_fixed.ply");
Mask1_decim005_degree2_lambda001.createVBO("position");
Mask1_decim005_degree2_lambda001.createVBO("normal");
Mask1_decim005_degree2_lambda001.createVBO("radiance");
Mask1_decim005_degree2_lambda001.setBBVertexAttribute("position");

#Another model, this time with degree 0
Mask1_decim005_degree0_lambda001 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda001_fixed.ply");
Mask1_decim005_degree0_lambda001.createVBO("position");
Mask1_decim005_degree0_lambda001.createVBO("normal");
Mask1_decim005_degree0_lambda001.createVBO("radiance");
Mask1_decim005_degree0_lambda001.setBBVertexAttribute("position");

#FIRST CALCULATIONS: DEGREE 2 -> DEGREE 0

#decimate the model
Surface_Radiance.changePositionVBO(Mask1_decim005_degree0_lambda001.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree0_lambda001.getName(), "normal");

Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "distance", Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "distance");
