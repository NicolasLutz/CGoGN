################################################################
#Imports and decimates a single model.
################################################################

#replace every occurence of <model> with the name of your model (Mask1_decim005_degree2_lambda005.ply)
#replace every occurence of <path> with the relative path to your model (don't end it with /).

view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");

mesh = Surface_Radiance.importFromFile_SH("./stats/1-Mask-quantize_256_sh.ply");
mesh.setBBVertexAttribute("position");
mesh.createVBO("position");
mesh.createVBO("normal");
mesh.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh.getName(), "position", "normal", "./stats/1-Mask-quantize_256_sh.ply");

#model
#Mask1_decim005_degree0_lambda001 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree0_lambda001.ply");
#Mask1_decim005_degree0_lambda001.setBBVertexAttribute("position");
#Mask1_decim005_degree0_lambda001.createVBO("position");
#Mask1_decim005_degree0_lambda001.createVBO("normal");
#Mask1_decim005_degree0_lambda001.createVBO("radiance");
#Surface_Radiance.exportPLY_superiorResolution(Mask1_decim005_degree0_lambda001.getName(), "position", "normal", "Mask1_decim005_degree0_lambda001_fixed.ply", 2);

#model
#Mask1_decim005_degree2_lambda001 = Surface_Radiance.importFromFile_SH("Mask1_decim005_degree2_lambda001.ply");
#Mask1_decim005_degree2_lambda001.setBBVertexAttribute("position");
#Mask1_decim005_degree2_lambda001.createVBO("position");
#Mask1_decim005_degree2_lambda001.createVBO("normal");
#Mask1_decim005_degree2_lambda001.createVBO("radiance");
#Surface_Radiance.exportPLY(Mask1_decim005_degree2_lambda001.getName(), "position", "normal", "Mask1_decim005_degree2_lambda001_fixed.ply");

