#include "importSurface.h"

#include "mapHandler.h"

#include "Algo/Import/import.h"

#include <QFileDialog>
#include <QFileInfo>

bool ImportSurfacePlugin::enable()
{
	importAction = new QAction("import", this);
	addMenuAction("Surface;Import", importAction);
	connect(importAction, SIGNAL(triggered()), this, SLOT(importFromFileDialog()));
	return true;
}

MapHandlerGen* ImportSurfacePlugin::importFromFile(const QString& fileName)
{
	QFileInfo fi(fileName);
	if(fi.exists())
	{
		MapHandlerGen* mhg = m_window->addMap(fi.baseName(), 2);
		if(mhg)
		{
			MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(mhg);
			PFP2::MAP* map = mh->getMap();

			std::vector<std::string> attrNames;
			Algo::Surface::Import::importMesh<PFP2>(*map, fileName.toStdString(), attrNames);

			// get vertex position attribute
			VertexAttribute<PFP2::VEC3> position = map->getAttribute<PFP2::VEC3, CGoGN::VERTEX>(attrNames[0]);
			mh->registerAttribute<PFP2::VEC3, VERTEX>(position);

			// create VBO for vertex position attribute
			mh->createVBO(position);

			// compute map bounding box
			mh->updateBB(position);
		}
		return mhg;
	}
	else
		return NULL;
}

void ImportSurfacePlugin::importFromFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(m_window, "Import file", m_window->getAppPath(), "Mesh Files (*.ply *.off *.trian)");
	importFromFile(fileName);
}

#ifndef DEBUG
Q_EXPORT_PLUGIN2(ImportSurfacePlugin, ImportSurfacePlugin)
#else
Q_EXPORT_PLUGIN2(ImportSurfacePluginD, ImportSurfacePlugin)
#endif