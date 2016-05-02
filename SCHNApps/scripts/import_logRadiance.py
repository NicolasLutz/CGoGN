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
Mask1_decim005_degree0_lambda001 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree1_lambda001_fixed.ply");
Mask1_decim005_degree0_lambda001.createVBO("position");
Mask1_decim005_degree0_lambda001.createVBO("normal");
Mask1_decim005_degree0_lambda001.createVBO("radiance");
Mask1_decim005_degree0_lambda001.setBBVertexAttribute("position");

Surface_Radiance.logRadianceDetails(Mask1_decim005_degree2_lambda001.getName(), "outlvl2.log");
Surface_Radiance.logRadianceDetails(Mask1_decim005_degree0_lambda001.getName(), "outlvl1.log");
