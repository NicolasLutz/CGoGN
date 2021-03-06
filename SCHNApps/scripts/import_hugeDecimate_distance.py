################################################################
#Imports a model twice, decimates one of the two versions at 95%, calculates the distance between the two and retries with 99%. The difference between the two should be of factor 5.
################################################################

#replace every occurence of <model> with the name of your model (Mask1_decim005_degree2_lambda005.ply)
#replace every occurence of <path> with the relative path to your model (don't end it with /).
#uncomment and copy as many sections as you want to add models for comparing. To fix : removing some models seem to crash the app.

view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");

#model
Mask1_decim005_degree2_lambda005 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda005.ply");
Mask1_decim005_degree2_lambda005.createVBO("position");
Mask1_decim005_degree2_lambda005.createVBO("normal");
Mask1_decim005_degree2_lambda005.createVBO("radiance");
Mask1_decim005_degree2_lambda005.setBBVertexAttribute("position");

#another model
#<model> = Surface_Radiance.importFromFile_SH("<model>.ply");
#<model>.setBBVertexAttribute("position");
#<model>.createVBO("position");
#<model>.createVBO("normal");
#<model>.createVBO("radiance");

#decimate the model
Surface_Radiance.changePositionVBO(Mask1_decim005_degree2_lambda005.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree2_lambda005.getName(), "normal");
Surface_Radiance.decimate(Mask1_decim005_degree2_lambda005.getName(), "position", "normal", 0.05, 1); #replace 0.5 by the percentage of decimation you want

#same model, but not decimated
Mask1_decim005_degree2_lambda005_1 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda005.ply");
Mask1_decim005_degree2_lambda005_1.createVBO("position");
Mask1_decim005_degree2_lambda005_1.createVBO("normal");
Mask1_decim005_degree2_lambda005_1.createVBO("radiance");
Mask1_decim005_degree2_lambda005_1.setBBVertexAttribute("position");

Surface_Radiance.changePositionVBO(Mask1_decim005_degree2_lambda005_1.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree2_lambda005_1.getName(), "normal");

#bellow, always put the model with the most points as first argument (with the best radiance if equals)
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda005_1.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda005.getName(), "position", "normal", "distance");

#re-decimate and re-calculate
Surface_Radiance.decimate(Mask1_decim005_degree2_lambda005.getName(), "position", "normal", 0.20, 1);
Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda005_1.getName(), "position", "normal", "distance", Mask1_decim005_degree2_lambda005.getName(), "position", "normal", "distance");

#Now let's compare with other models
#<model> = Surface_Radiance.importFromFile_SH("<model>.ply");
#<model>.setBBVertexAttribute("position");
#<model>.createVBO("position");
#<model>.createVBO("normal");
#<model>.createVBO("radiance");
#Surface_Radiance.changePositionVBO(<model>.getName(), "position");
#Surface_Radiance.changeNormalVBO(<model>.getName(), "normal");
#Surface_Radiance.computeRadianceDistance(Mask1_decim005_degree2_lambda005.getName(), "position", "normal", "distance", <model>.getName(), "position", "normal", "distance");
