view_0 = schnapps.getView("view_0");
Surface_Radiance = schnapps.enablePlugin("Surface_Radiance");

mesh1 = Surface_Radiance.importFromFile_SH("./stats_unfixed/0-Gong_28k_sh.ply");
mesh1.setBBVertexAttribute("position");
mesh1.createVBO("position");
mesh1.createVBO("normal");
mesh1.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh1.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh1.getName(), "position", "normal", "./stats/0-Gong_28k_sh.ply");

mesh2 = Surface_Radiance.importFromFile_SH("./stats_unfixed/0-Mask_48k_sh.ply");
mesh2.setBBVertexAttribute("position");
mesh2.createVBO("position");
mesh2.createVBO("normal");
mesh2.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh2.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh2.getName(), "position", "normal", "./stats/0-Mask_48k_sh.ply");

mesh3 = Surface_Radiance.importFromFile_SH("./stats_unfixed/1-Gong-quantize_256_sh.ply");
mesh3.setBBVertexAttribute("position");
mesh3.createVBO("position");
mesh3.createVBO("normal");
mesh3.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh3.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh3.getName(), "position", "normal", "./stats/1-Gong-quantize_256_sh.ply");

mesh4 = Surface_Radiance.importFromFile_SH("./stats_unfixed/1-Gong-simplif_14k_sh.ply");
mesh4.setBBVertexAttribute("position");
mesh4.createVBO("position");
mesh4.createVBO("normal");
mesh4.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh4.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh4.getName(), "position", "normal", "./stats/1-Gong-simplif_14k_sh.ply");

mesh5 = Surface_Radiance.importFromFile_SH("./stats_unfixed/1-Gong-smooth_10_sh.ply");
mesh5.setBBVertexAttribute("position");
mesh5.createVBO("position");
mesh5.createVBO("normal");
mesh5.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh5.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh5.getName(), "position", "normal", "./stats/1-Gong-smooth_10_sh.ply");

mesh6 = Surface_Radiance.importFromFile_SH("./stats_unfixed/1-Mask-quantize_256_sh.ply");
mesh6.setBBVertexAttribute("position");
mesh6.createVBO("position");
mesh6.createVBO("normal");
mesh6.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh6.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh6.getName(), "position", "normal", "./stats/1-Mask-quantize_256_sh.ply");

mesh7 = Surface_Radiance.importFromFile_SH("./stats_unfixed/1-Mask-simplif_24k_sh.ply");
mesh7.setBBVertexAttribute("position");
mesh7.createVBO("position");
mesh7.createVBO("normal");
mesh7.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh7.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh7.getName(), "position", "normal", "./stats/1-Mask-simplif_24k_sh.ply");

mesh8 = Surface_Radiance.importFromFile_SH("./stats_unfixed/1-Mask-smooth_10_sh.ply");
mesh8.setBBVertexAttribute("position");
mesh8.createVBO("position");
mesh8.createVBO("normal");
mesh8.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh8.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh8.getName(), "position", "normal", "./stats/1-Mask-smooth_10_sh.ply");

mesh9 = Surface_Radiance.importFromFile_SH("./stats_unfixed/2-Gong-quantize_192_sh.ply");
mesh9.setBBVertexAttribute("position");
mesh9.createVBO("position");
mesh9.createVBO("normal");
mesh9.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh9.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh9.getName(), "position", "normal", "./stats/2-Gong-quantize_192_sh.ply");

mesh10 = Surface_Radiance.importFromFile_SH("./stats_unfixed/2-Gong-simplif_7k_sh.ply");
mesh10.setBBVertexAttribute("position");
mesh10.createVBO("position");
mesh10.createVBO("normal");
mesh10.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh10.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh10.getName(), "position", "normal", "./stats/2-Gong-simplif_7k_sh.ply");

mesh11 = Surface_Radiance.importFromFile_SH("./stats_unfixed/2-Gong-smooth_20_sh.ply");
mesh11.setBBVertexAttribute("position");
mesh11.createVBO("position");
mesh11.createVBO("normal");
mesh11.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh11.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh11.getName(), "position", "normal", "./stats/2-Gong-smooth_20_sh.ply");

mesh12 = Surface_Radiance.importFromFile_SH("./stats_unfixed/2-Mask-quantize_192_sh.ply");
mesh12.setBBVertexAttribute("position");
mesh12.createVBO("position");
mesh12.createVBO("normal");
mesh12.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh12.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh12.getName(), "position", "normal", "./stats/2-Mask-quantize_192_sh.ply");

mesh13 = Surface_Radiance.importFromFile_SH("./stats_unfixed/2-Mask-simplif_12k_sh.ply");
mesh13.setBBVertexAttribute("position");
mesh13.createVBO("position");
mesh13.createVBO("normal");
mesh13.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh13.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh13.getName(), "position", "normal", "./stats/2-Mask-simplif_12k_sh.ply");

mesh14 = Surface_Radiance.importFromFile_SH("./stats_unfixed/2-Mask-smooth_20_sh.ply");
mesh14.setBBVertexAttribute("position");
mesh14.createVBO("position");
mesh14.createVBO("normal");
mesh14.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh14.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh14.getName(), "position", "normal", "./stats/2-Mask-smooth_20_sh.ply");

mesh15 = Surface_Radiance.importFromFile_SH("./stats_unfixed/3-Gong-quantize_128_sh.ply");
mesh15.setBBVertexAttribute("position");
mesh15.createVBO("position");
mesh15.createVBO("normal");
mesh15.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh15.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh15.getName(), "position", "normal", "./stats/3-Gong-quantize_128_sh.ply");

mesh16 = Surface_Radiance.importFromFile_SH("./stats_unfixed/3-Gong-simplif_3k_sh.ply");
mesh16.setBBVertexAttribute("position");
mesh16.createVBO("position");
mesh16.createVBO("normal");
mesh16.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh16.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh16.getName(), "position", "normal", "./stats/3-Gong-simplif_3k_sh.ply");

mesh17 = Surface_Radiance.importFromFile_SH("./stats_unfixed/3-Gong-smooth_30_sh.ply");
mesh17.setBBVertexAttribute("position");
mesh17.createVBO("position");
mesh17.createVBO("normal");
mesh17.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh17.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh17.getName(), "position", "normal", "./stats/3-Gong-smooth_30_sh.ply");

mesh18 = Surface_Radiance.importFromFile_SH("./stats_unfixed/3-Mask-quantize_128_sh.ply");
mesh18.setBBVertexAttribute("position");
mesh18.createVBO("position");
mesh18.createVBO("normal");
mesh18.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh18.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh18.getName(), "position", "normal", "./stats/3-Mask-quantize_128_sh.ply");

mesh19 = Surface_Radiance.importFromFile_SH("./stats_unfixed/3-Mask-simplif_6k_sh.ply");
mesh19.setBBVertexAttribute("position");
mesh19.createVBO("position");
mesh19.createVBO("normal");
mesh19.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh19.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh19.getName(), "position", "normal", "./stats/3-Mask-simplif_6k_sh.ply");

mesh20 = Surface_Radiance.importFromFile_SH("./stats_unfixed/3-Mask-smooth_30_sh.ply");
mesh20.setBBVertexAttribute("position");
mesh20.createVBO("position");
mesh20.createVBO("normal");
mesh20.createVBO("radiance");
Surface_Radiance.fixSHMap(mesh20.getName(), "position", "normal");
Surface_Radiance.exportPLY(mesh20.getName(), "position", "normal", "./stats/3-Mask-smooth_30_sh.ply");

