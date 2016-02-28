################################################################
#Imports a model twice, decimates one of the two versions (at 50%) and calculate the distance between them (output currently code-dependant)
################################################################

#replace every occurence of <model> with the name of your model (<model>.ply)
#replace every occurence of <path> with the directory path to your model (don't end it with /).

view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");
<model> = Surface_Radiance.importFromFile_SH("<path>/<model>.ply");
schnapps.setSelectedMap(<model>.getName());
<model>.setBBVertexAttribute("position");
schnapps.setSelectedMap(<model>.getName());
<model>.createVBO("position");
<model>.createVBO("normal");
<model>.createVBO("radiance");
Surface_Radiance.changePositionVBO(<model>.getName(), "position");
Surface_Radiance.changeNormalVBO(<model>.getName(), "normal");
view_0.linkMap(<model>.getName());
view_0.linkPlugin("Surface_Radiance");
Surface_Radiance.decimate(<model>.getName(), "position", "normal", 0.5, 1); #replace 0.5 by the percentage of decimation you want
<model>_1 = Surface_Radiance.importFromFile_SH("<path>/<model>.ply");
schnapps.setSelectedMap(<model>_1.getName());
schnapps.setSelectedMap(<model>.getName());
schnapps.setSelectedMap(<model>_1.getName());
<model>_1.createVBO("position");
<model>_1.createVBO("normal");
<model>_1.createVBO("radiance");
view_1 = schnapps.getView("view_1");
view_1 = schnapps.splitView("view_0", 1);
view_1.linkMap(<model>_1.getName());
view_1.linkPlugin("Surface_Radiance");
Surface_Radiance.changePositionVBO(<model>_1.getName(), "position");
Surface_Radiance.changeNormalVBO(<model>_1.getName(), "normal");
<model>_1.setBBVertexAttribute("position");
schnapps.setSelectedMap(<model>_1.getName());
view_1.setCurrentCamera("camera_0");
#bellow, trade <model> and <model>_1 for different results
Surface_Radiance.computeRadianceDistance(<model>.getName(), "position", "normal", "distance", <model>_1.getName(), "position", "normal", "distance");
Surface_RenderScalar = schnapps.enablePlugin("Surface_RenderScalar");
schnapps.setSelectedMap(<model>.getName());
schnapps.setSelectedMap(<model>_1.getName());
<model>_1.createVBO("distance");
schnapps.setSelectedMap(<model>.getName());
<model>.createVBO("distance");
view_0.unlinkPlugin("Surface_Radiance");
view_0.linkPlugin("Surface_RenderScalar");
view_1.unlinkPlugin("Surface_Radiance");
view_1.linkPlugin("Surface_RenderScalar");
Surface_RenderScalar.changePositionVBO("view_1", <model>.getName(), "position");
Surface_RenderScalar.changeScalarVBO("view_1", <model>.getName(), "distance");
Surface_RenderScalar.changePositionVBO("view_0", <model>.getName(), "position");
Surface_RenderScalar.changeScalarVBO("view_0", <model>.getName(), "distance");
schnapps.setSelectedMap(<model>_1.getName());
Surface_RenderScalar.changePositionVBO("view_1", <model>_1.getName(), "position");
schnapps.setSelectedMap(<model>.getName());
schnapps.setSelectedMap(<model>_1.getName());
Surface_RenderScalar.changeScalarVBO("view_1", <model>_1.getName(), "distance");
Surface_RenderScalar.changeColorMap("view_1", <model>_1.getName(), 2);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 1);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 2);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 3);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 4);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 5);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 6);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 7);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 8);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 9);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 10);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 9);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 8);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 7);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 6);
Surface_RenderScalar.changeExpansion("view_1", <model>_1.getName(), 5);

schnapps.restoreSplitViewPositions("30 0 0 0 -1 0 0 0 0 0 0 0 2 0 0 2 -99 0 0 2 -100 1 0 0 0 6 1 0 0 0 1 ")
schnapps.setFloatingControlDock(0)
schnapps.setFloatingPluginDock(0)
schnapps.setFloatingPythonDock(0)
