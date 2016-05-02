view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");
0-Mask_48k_sh = Surface_Radiance.importFromFile_SH(0-Mask_48k_sh.getName());
1-Mask-quantize_256_sh = Surface_Radiance.importFromFile_SH(1-Mask-quantize_256_sh.getName());
1-Mask-simplif_24k_sh = Surface_Radiance.importFromFile_SH(1-Mask-simplif_24k_sh.getName());
1-Mask-smooth_10_sh = Surface_Radiance.importFromFile_SH(1-Mask-smooth_10_sh.getName());
2-Mask-quantize_192_sh = Surface_Radiance.importFromFile_SH(2-Mask-quantize_192_sh.getName());
2-Mask-simplif_12k_sh = Surface_Radiance.importFromFile_SH(2-Mask-simplif_12k_sh.getName());
2-Mask-smooth_20_sh = Surface_Radiance.importFromFile_SH(2-Mask-smooth_20_sh.getName());
3-Mask-quantize_128_sh = Surface_Radiance.importFromFile_SH(3-Mask-quantize_128_sh.getName());
3-Mask-simplif_6k_sh = Surface_Radiance.importFromFile_SH(3-Mask-simplif_6k_sh.getName());
3-Mask-smooth_30_sh = Surface_Radiance.importFromFile_SH(3-Mask-smooth_30_sh.getName());
schnapps.setSelectedMap(0-Mask_48k_sh.getName());
0-Mask_48k_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(0-Mask_48k_sh.getName());
0-Mask_48k_sh.createVBO("position");
0-Mask_48k_sh.createVBO("normal");
0-Mask_48k_sh.createVBO("radiance");
schnapps.setSelectedMap(1-Mask-quantize_256_sh.getName());
1-Mask-quantize_256_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(1-Mask-quantize_256_sh.getName());
1-Mask-quantize_256_sh.createVBO("position");
1-Mask-quantize_256_sh.createVBO("normal");
1-Mask-quantize_256_sh.createVBO("radiance");
schnapps.setSelectedMap(1-Mask-simplif_24k_sh.getName());
1-Mask-simplif_24k_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(1-Mask-simplif_24k_sh.getName());
1-Mask-simplif_24k_sh.createVBO("position");
1-Mask-simplif_24k_sh.createVBO("normal");
1-Mask-simplif_24k_sh.createVBO("radiance");
schnapps.setSelectedMap(1-Mask-smooth_10_sh.getName());
1-Mask-smooth_10_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(1-Mask-smooth_10_sh.getName());
1-Mask-smooth_10_sh.createVBO("position");
1-Mask-smooth_10_sh.createVBO("normal");
1-Mask-smooth_10_sh.createVBO("radiance");
schnapps.setSelectedMap(2-Mask-quantize_192_sh.getName());
2-Mask-quantize_192_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(2-Mask-quantize_192_sh.getName());
2-Mask-quantize_192_sh.createVBO("position");
2-Mask-quantize_192_sh.createVBO("normal");
2-Mask-quantize_192_sh.createVBO("radiance");
schnapps.setSelectedMap(2-Mask-smooth_20_sh.getName());
schnapps.setSelectedMap(2-Mask-simplif_12k_sh.getName());
2-Mask-simplif_12k_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(2-Mask-simplif_12k_sh.getName());
2-Mask-simplif_12k_sh.createVBO("position");
2-Mask-simplif_12k_sh.createVBO("normal");
2-Mask-simplif_12k_sh.createVBO("radiance");
schnapps.setSelectedMap(2-Mask-quantize_192_sh.getName());
schnapps.setSelectedMap(2-Mask-smooth_20_sh.getName());
2-Mask-smooth_20_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(2-Mask-smooth_20_sh.getName());
2-Mask-smooth_20_sh.createVBO("position");
2-Mask-smooth_20_sh.createVBO("normal");
2-Mask-smooth_20_sh.createVBO("radiance");
schnapps.setSelectedMap(3-Mask-quantize_128_sh.getName());
3-Mask-quantize_128_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(3-Mask-quantize_128_sh.getName());
3-Mask-quantize_128_sh.createVBO("position");
3-Mask-quantize_128_sh.createVBO("normal");
3-Mask-quantize_128_sh.createVBO("radiance");
schnapps.setSelectedMap(3-Mask-simplif_6k_sh.getName());
3-Mask-simplif_6k_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(3-Mask-simplif_6k_sh.getName());
3-Mask-simplif_6k_sh.createVBO("position");
3-Mask-simplif_6k_sh.createVBO("normal");
3-Mask-simplif_6k_sh.createVBO("radiance");
schnapps.setSelectedMap(3-Mask-smooth_30_sh.getName());
3-Mask-smooth_30_sh.setBBVertexAttribute("position");
schnapps.setSelectedMap(3-Mask-smooth_30_sh.getName());
3-Mask-smooth_30_sh.createVBO("position");
3-Mask-smooth_30_sh.createVBO("normal");
3-Mask-smooth_30_sh.createVBO("radiance");
schnapps.setSelectedMap(3-Mask-simplif_6k_sh.getName());
schnapps.setSelectedMap(3-Mask-quantize_128_sh.getName());
schnapps.setSelectedMap(2-Mask-simplif_12k_sh.getName());
schnapps.setSelectedMap(1-Mask-smooth_10_sh.getName());
schnapps.setSelectedMap(1-Mask-simplif_24k_sh.getName());
schnapps.setSelectedMap(1-Mask-quantize_256_sh.getName());
schnapps.setSelectedMap(0-Mask_48k_sh.getName());
Surface_Radiance.computeRadianceDistance(0-Mask_48k_sh.getName(), "position", "normal", "distance", 1-Mask-quantize_256_sh.getName(), "position", "normal", "distance");
schnapps.setSelectedMap(1-Mask-simplif_24k_sh.getName());
schnapps.setSelectedMap(0-Mask_48k_sh.getName());
Surface_Radiance.changePositionVBO(0-Mask_48k_sh.getName(), "position");
Surface_Radiance.changeNormalVBO(0-Mask_48k_sh.getName(), "position");
Surface_Radiance.changeNormalVBO(0-Mask_48k_sh.getName(), "normal");
view_0.linkPlugin("Surface_Radiance");
view_0.linkMap(0-Mask_48k_sh.getName());
schnapps.setSelectedMap(1-Mask-simplif_24k_sh.getName());
schnapps.setSelectedMap(1-Mask-quantize_256_sh.getName());
Surface_Radiance.changePositionVBO(1-Mask-quantize_256_sh.getName(), "position");
Surface_Radiance.changeNormalVBO(1-Mask-quantize_256_sh.getName(), "normal");
view_0.unlinkMap(0-Mask_48k_sh.getName());
view_0.linkMap(1-Mask-quantize_256_sh.getName());

schnapps.restoreSplitViewPositions("26 0 0 0 -1 0 0 0 0 0 0 0 1 0 0 2 88 1 0 0 0 6 1 0 0 0 1 ")
schnapps.getCamera("camera_0").fromString("19.1305 70.7012 15.6732 0.481271 0.404665 0.638496 -0.443788")
0-Mask_48k_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
1-Mask-quantize_256_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
1-Mask-simplif_24k_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
1-Mask-smooth_10_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
2-Mask-quantize_192_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
2-Mask-simplif_12k_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
2-Mask-smooth_20_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
3-Mask-quantize_128_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
3-Mask-simplif_6k_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
3-Mask-smooth_30_sh.frameFromString("1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 ")
schnapps.setWindowSize(1855, 1056)
schnapps.setFloatingControlDock(0)
schnapps.setFloatingPluginDock(0)
schnapps.setFloatingPythonDock(0)
