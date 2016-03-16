################################################################
#Imports and decimates a single model.
################################################################

#replace every occurence of <model> with the name of your model (Mask1_decim005_degree2_lambda005.ply)
#replace every occurence of <path> with the relative path to your model (don't end it with /).

view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");

#model
Mask1_decim005_degree2_lambda005 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda005.ply");
Mask1_decim005_degree2_lambda005.setBBVertexAttribute("position");
Mask1_decim005_degree2_lambda005.createVBO("position");
Mask1_decim005_degree2_lambda005.createVBO("normal");
Mask1_decim005_degree2_lambda005.createVBO("radiance");

#decimate the model
Surface_Radiance.changePositionVBO(Mask1_decim005_degree2_lambda005.getName(), "position");
Surface_Radiance.changeNormalVBO(Mask1_decim005_degree2_lambda005.getName(), "normal");
Surface_Radiance.decimate(Mask1_decim005_degree2_lambda005.getName(), "position", "normal", 0.125, 1); #replace 0.125 by the percentage of decimation you want

#export the model
Surface_Radiance.exportPLY(Mask1_decim005_degree2_lambda005.getName(), "position", "normal", "exportedModel.ply");
