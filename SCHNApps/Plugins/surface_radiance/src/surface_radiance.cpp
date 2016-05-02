#include "surface_radiance.h"

#include "meshTableSurfaceRadiance.h"
#include "meshTableSurfaceRadiance_P.h"

#include "halfEdgeSelectorRadiance.h"
#include "edgeSelectorRadiance.h"

#include "mapHandler.h"
#include "camera.h"

#include "Algo/Geometry/distances.h"
#include "Algo/Geometry/plane.h"

#include "SphericalFunctionIntegratorCartesian.h"

#include <QFileDialog>
#include <QFileInfo>

#include <fstream>
#define IN_DEV

namespace CGoGN
{

namespace SCHNApps
{

//FaceCluster

FaceCluster::FaceCluster() : m_cluster(), m_mark(false)
{}

FaceCluster::~FaceCluster()
{}

FaceCluster::FaceCluster(const PFP2::VEC3& bbMin, const PFP2::VEC3& bbMax, unsigned int x, unsigned int y, unsigned int z):
    m_cluster(), m_bb(bbMin), m_mark(false), m_x(x), m_y(y), m_z(z)
{
    m_bb.addPoint(bbMax);
    assert(m_bb.isInitialized() || "BoundingBox was not initialized correctly.");
}

bool FaceCluster::contains(PFP2::MAP &map, Face f, const VertexAttribute<PFP2::VEC3, PFP2::MAP> &position)
{
    PFP2::VEC3 s1=position[f.dart];
    PFP2::VEC3 s2=position[map.phi1(f.dart)];
    PFP2::VEC3 s3=position[map.phi_1(f.dart)];

    //bounding boxes intersect? Find the bounding box of the face
    PFP2::VEC3 minBB, maxBB;
    for(unsigned int i=0; i<3; ++i)
    {
        minBB[i]=std::min(s1[i], std::min(s2[i], s3[i]));
        maxBB[i]=std::max(s1[i], std::min(s2[i], s3[i]));
    }
    Geom::BoundingBox<PFP2::VEC3> bbTriangle(minBB);
    bbTriangle.addPoint(maxBB);

    if(!m_bb.intersects(bbTriangle))
        return false;

    //edges of the triangle intersect the bounding box?
    if(!contains(s1, s2) && !contains(s2,s3) && !contains(s3,s1))
    {
        //Note: this case is very rare.
        //do the diagonals of the bounding box intersect the triangle? (triangle is bigger than bounding box)

        //find bounding box vertices
        PFP2::VEC3 garbageInter;
        PFP2::VEC3 p2=m_bb.min(), p3=m_bb.min(), p4=m_bb.min(), p5=m_bb.max(), p6=m_bb.max(), p7=m_bb.max();
        p2[0]=m_bb.max()[0];
        p3[1]=m_bb.max()[1];
        p4[2]=m_bb.max()[2];
        p5[0]=m_bb.min()[0];
        p6[1]=m_bb.min()[1];
        p7[2]=m_bb.min()[2];

        //test diagonal intersections to check if the bounding box is inside the triangle
        return Algo::Surface::Geometry::intersectionSegmentConvexFace<PFP2>(map, f, position, m_bb.min(), m_bb.max(), garbageInter)
        || Algo::Surface::Geometry::intersectionSegmentConvexFace<PFP2>(map, f, position, p2, p5, garbageInter)
        || Algo::Surface::Geometry::intersectionSegmentConvexFace<PFP2>(map, f, position, p3, p6, garbageInter)
        || Algo::Surface::Geometry::intersectionSegmentConvexFace<PFP2>(map, f, position, p4, p7, garbageInter);
    }
    else
        return true;
}

bool FaceCluster::closestFaceInCluster(PFP2::MAP& map, const PFP2::VEC3& p, const VertexAttribute<PFP2::VEC3, PFP2::MAP>& position,
                                       Face& face, PFP2::REAL& distance2) const
{
    bool foundBetter=false;
    for(Face f : m_cluster)
    {
        PFP2::REAL dist = Algo::Geometry::squaredDistancePoint2Face<PFP2>(map, f, position, p);
        if (dist < distance2)
        {
            foundBetter=true;
            distance2 = dist;
            face = f;
        }
    }
    return foundBetter;
}

//FaceClusterAggregation

FaceClustersAggregation::FaceClustersAggregation(const Geom::BoundingBox<PFP2::VEC3>& boundingBox,
                                                 size_t width, size_t height, size_t depth):
    m_clusters(), m_nbFaces(0), m_width(width), m_height(height), m_depth(depth)
{
    m_clusters.reserve(m_width*m_height*m_depth);
    PFP2::VEC3 minBB=boundingBox.min();
    PFP2::VEC3 maxBB=boundingBox.max();
    PFP2::VEC3 singleClusterSize=(maxBB-minBB);
    singleClusterSize[0]/=m_width;
    singleClusterSize[1]/=m_height;
    singleClusterSize[2]/=m_depth;
    PFP2::VEC3 minCurrentBB;
    PFP2::VEC3 maxCurrentBB;
    for(unsigned int i=0; i<m_width; ++i)
    {
        PFP2::REAL iDist=singleClusterSize[0]*i;
        minCurrentBB[0]=minBB[0] + iDist;
        maxCurrentBB[0]=minCurrentBB[0] + singleClusterSize[0];
        for(unsigned int j=0; j<m_height; ++j)
        {
            PFP2::REAL jDist=singleClusterSize[1]*j;
            minCurrentBB[1]=minBB[1] + jDist;
            maxCurrentBB[1]=minCurrentBB[1] + singleClusterSize[1];

            for(unsigned int k=0; k<m_depth; ++k)
            {
                PFP2::REAL kDist=singleClusterSize[2]*k;
                minCurrentBB[2]=minBB[2] + kDist;
                maxCurrentBB[2]=minCurrentBB[2] + singleClusterSize[2];

                m_clusters.push_back(FaceCluster(minCurrentBB, maxCurrentBB, i, j, k));
            }
        }
    }
}

FaceClustersAggregation::~FaceClustersAggregation()
{}

void FaceClustersAggregation::addElement(PFP2::MAP& map, Face f,
                                         const VertexAttribute<PFP2::VEC3, PFP2::MAP>& position)
{
    for(FaceCluster& fc : m_clusters)
    {
        if(fc.contains(map, f, position))
        {
            fc.addFace(f);
        }
    }
}

void FaceClustersAggregation::clusterizeMap(PFP2::MAP& map, const VertexAttribute<PFP2::VEC3, PFP2::MAP>& position,
                                            const VertexAttribute<PFP2::VEC3, PFP2::MAP>& normal,
                                            const PFP2::VEC3& N, PFP2::REAL threshold)
{
    for(Face f : allFacesOf(map))
    {
        if(N*normal[f.dart]>=threshold || N*normal[map.phi1(f.dart)]>=threshold || N*normal[map.phi_1(f.dart)]>=threshold)
        {//discard f if every vertice of f disrespects the discrimination
            addElement(map, f, position);
        }
    }
}

PFP2::REAL FaceClustersAggregation::squaredDistancePoint2Cluster(const PFP2::VEC3& p, const FaceCluster& cluster)
{
    //find where otherCluster is in comparasion of clusterOfP
    PFP2::REAL distance2;
    PFP2::VEC3 clusterMin=cluster.bbMin();
    PFP2::VEC3 clusterMax=cluster.bbMax();

    PFP2::REAL dx=std::max(clusterMin[0]-p[0], std::max(PFP2::REAL(0), p[0]-clusterMax[0]));
    PFP2::REAL dy=std::max(clusterMin[1]-p[1], std::max(PFP2::REAL(0), p[1]-clusterMax[1]));
    PFP2::REAL dz=std::max(clusterMin[2]-p[2], std::max(PFP2::REAL(0), p[2]-clusterMax[2]));

    distance2 = (dx*dx + dy*dy + dz*dz);
    return distance2;
}

Face FaceClustersAggregation::findFace(PFP2::MAP &map, const PFP2::VEC3 &p, const VertexAttribute<PFP2::VEC3, PFP2::MAP> &position)
{
    std::priority_queue<distanceEval_t, std::vector<distanceEval_t>, DistanceEval_Compare> pq;
    PFP2::REAL minDist2= std::numeric_limits<PFP2::REAL>::max();
    std::vector<FaceCluster*> markedClusterList;
    Face closestFace;
    FaceCluster* scopedFC;

    pq.push(distanceEval_t(scopedFC=getCluster(p), minDist2));
    pq.top().first->toggleMark();
    markedClusterList.push_back(scopedFC);
    pq.top().first->closestFaceInCluster(map, p, position, closestFace, minDist2);

    unsigned int x=scopedFC->x();
    unsigned int y=scopedFC->y();
    unsigned int z=scopedFC->z();

    while(!pq.empty())
    {
        distanceEval_t pq_top=pq.top();
        x=pq_top.first->x();
        y=pq_top.first->y();
        z=pq_top.first->z();
        pq.pop();
        for(int i=-1; i<2; ++i)
            for(int j=-1; j<2; ++j)
                for(int k=-1; k<2; ++k)
                {
                    PFP2::REAL clusterDistance2;
                    scopedFC=getCluster(x+i, y+j, z+k);
                    if(scopedFC!=NULL && !scopedFC->marked() && (clusterDistance2=squaredDistancePoint2Cluster(p, (*scopedFC)))<minDist2)
                    {
                        scopedFC->toggleMark();
                        markedClusterList.push_back(scopedFC);

                        pq.push(distanceEval_t(scopedFC, clusterDistance2));
                        scopedFC->closestFaceInCluster(map, p, position, closestFace, minDist2);
                    }
                }
    }
    for(FaceCluster *fc : markedClusterList)
    {
        fc->toggleMark();
    }
    return closestFace;
}

FaceCluster* FaceClustersAggregation::getCluster(unsigned int i, unsigned int j, unsigned int k)
{
    if(i>=m_width || j>=m_height || k>=m_depth)
        return NULL;
    else
    {
        FaceCluster* fc=&m_clusters[i*m_height*m_depth+j*m_depth+k];
        return fc;
    }
}

FaceCluster* FaceClustersAggregation::getCluster(const PFP2::VEC3& p)
{
    PFP2::VEC3 bbDimensions=m_clusters[0].bbMax()-m_clusters[0].bbMin();
    PFP2::VEC3 bbToP=p-m_clusters[0].bbMin();
    unsigned int x=std::max(0, std::min((int)(bbToP[0]/bbDimensions[0]), (int)m_width-1));
    unsigned int y=std::max(0, std::min((int)(bbToP[1]/bbDimensions[1]), (int)m_height-1));
    unsigned int z=std::max(0, std::min((int)(bbToP[2]/bbDimensions[2]), (int)m_depth-1));

    return getCluster(x,y,z);
}

//Surface Radiance Plugin

bool Surface_Radiance_Plugin::enable()
{
	//	magic line that init static variables of GenericMap in the plugins
	GenericMap::copyAllStatics(m_schnapps->getStaticPointers());

	m_dockTab = new Surface_Radiance_DockTab(m_schnapps, this);
	m_schnapps->addPluginDockTab(this, m_dockTab, "Surface_Radiance");

	m_importSHAction = new QAction("importSH", this);
	m_schnapps->addMenuAction(this, "Radiance;Import (SH)", m_importSHAction);
	connect(m_importSHAction, SIGNAL(triggered()), this, SLOT(importFromFileDialog_SH()));

	m_importPAction = new QAction("importP", this);
	m_schnapps->addMenuAction(this, "Radiance;Import (P)", m_importPAction);
	connect(m_importPAction, SIGNAL(triggered()), this, SLOT(importFromFileDialog_P()));

	m_computeRadianceDistanceDialog = new Dialog_ComputeRadianceDistance(m_schnapps);

	m_computeRadianceDistanceAction = new QAction("Compute Radiance Distance", this);
	m_schnapps->addMenuAction(this, "Radiance;Compute Distance", m_computeRadianceDistanceAction);
    connect(m_computeRadianceDistanceAction, SIGNAL(triggered()), this, SLOT(openComputeRadianceDistanceDialog()));

	connect(m_computeRadianceDistanceDialog, SIGNAL(accepted()), this, SLOT(computeRadianceDistanceFromDialog()));
	connect(m_computeRadianceDistanceDialog->button_apply, SIGNAL(clicked()), this, SLOT(computeRadianceDistanceFromDialog()));

	connect(m_schnapps, SIGNAL(selectedViewChanged(View*, View*)), this, SLOT(selectedViewChanged(View*, View*)));
	connect(m_schnapps, SIGNAL(selectedMapChanged(MapHandlerGen*, MapHandlerGen*)), this, SLOT(selectedMapChanged(MapHandlerGen*, MapHandlerGen*)));
	connect(m_schnapps, SIGNAL(mapAdded(MapHandlerGen*)), this, SLOT(mapAdded(MapHandlerGen*)));
	connect(m_schnapps, SIGNAL(mapRemoved(MapHandlerGen*)), this, SLOT(mapRemoved(MapHandlerGen*)));

	foreach(MapHandlerGen* map, m_schnapps->getMapSet().values())
		mapAdded(map);

	m_dockTab->updateMapParameters();

	connect(m_schnapps, SIGNAL(schnappsClosing()), this, SLOT(schnappsClosing()));

	return true;
}

void Surface_Radiance_Plugin::disable()
{
	disconnect(m_schnapps, SIGNAL(selectedViewChanged(View*, View*)), this, SLOT(selectedViewChanged(View*, View*)));
	disconnect(m_schnapps, SIGNAL(selectedMapChanged(MapHandlerGen*, MapHandlerGen*)), this, SLOT(selectedMapChanged(MapHandlerGen*, MapHandlerGen*)));
	disconnect(m_schnapps, SIGNAL(mapAdded(MapHandlerGen*)), this, SLOT(mapAdded(MapHandlerGen*)));
	disconnect(m_schnapps, SIGNAL(mapRemoved(MapHandlerGen*)), this, SLOT(mapRemoved(MapHandlerGen*)));

	foreach(MapHandlerGen* map, m_schnapps->getMapSet().values())
		mapRemoved(map);
}

void Surface_Radiance_Plugin::drawMap(View* view, MapHandlerGen* map)
{
	const MapParameters& p = h_mapParameterSet[map];

	if(p.radiancePerVertexShader && p.positionVBO && p.normalVBO)
	{
		p.radiancePerVertexShader->setAttributePosition(p.positionVBO);
		p.radiancePerVertexShader->setAttributeNormal(p.normalVBO);
		p.radiancePerVertexShader->setAttributeRadiance(p.paramVBO, p.radianceTexture, GL_TEXTURE1);

		qglviewer::Vec c = view->getCurrentCamera()->position();
		PFP2::VEC3 camera(c.x, c.y, c.z);
		p.radiancePerVertexShader->setCamera(Geom::Vec3f(camera[0], camera[1], camera[2])); // convert to Vec3f because PFP2 can hold Vec3d !

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		map->draw(p.radiancePerVertexShader, Algo::Render::GL2::TRIANGLES);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	if(p.radiancePerVertexPShader && p.positionVBO && p.normalVBO && p.tangentVBO && p.binormalVBO)
	{
		p.radiancePerVertexPShader->setAttributePosition(p.positionVBO);
		p.radiancePerVertexPShader->setAttributeTangent(p.tangentVBO);
		p.radiancePerVertexPShader->setAttributeNormal(p.normalVBO);
		p.radiancePerVertexPShader->setAttributeBiNormal(p.binormalVBO);
		p.radiancePerVertexPShader->setAttributeRadiance(p.paramVBO, p.radianceTexture, GL_TEXTURE1);

		qglviewer::Vec c = view->getCurrentCamera()->position();
		PFP2::VEC3 camera(c.x, c.y, c.z);
		p.radiancePerVertexPShader->setCamera(Geom::Vec3f(camera[0], camera[1], camera[2])); // convert to Vec3f because PFP2 can hold Vec3d !

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		map->draw(p.radiancePerVertexPShader, Algo::Render::GL2::TRIANGLES);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
}





void Surface_Radiance_Plugin::selectedViewChanged(View *prev, View *cur)
{
	m_dockTab->updateMapParameters();
}

void Surface_Radiance_Plugin::selectedMapChanged(MapHandlerGen *prev, MapHandlerGen *cur)
{
	m_dockTab->updateMapParameters();
	if (cur == NULL)
		m_dockTab->setDisabled(true);
	else
		m_dockTab->setDisabled(false);
}

void Surface_Radiance_Plugin::mapAdded(MapHandlerGen* map)
{
	connect(map, SIGNAL(vboAdded(Utils::VBO*)), this, SLOT(vboAdded(Utils::VBO*)));
	connect(map, SIGNAL(vboRemoved(Utils::VBO*)), this, SLOT(vboRemoved(Utils::VBO*)));
	connect(map, SIGNAL(attributeModified(unsigned int, QString)), this, SLOT(attributeModified(unsigned int, QString)));
}

void Surface_Radiance_Plugin::mapRemoved(MapHandlerGen* map)
{
	disconnect(map, SIGNAL(vboAdded(Utils::VBO*)), this, SLOT(vboAdded(Utils::VBO*)));
	disconnect(map, SIGNAL(vboRemoved(Utils::VBO*)), this, SLOT(vboRemoved(Utils::VBO*)));
	disconnect(map, SIGNAL(attributeModified(unsigned int, QString)), this, SLOT(attributeModified(unsigned int, QString)));
}

void Surface_Radiance_Plugin::schnappsClosing()
{
//	m_computeRadianceDistanceDialog->close();
}

void Surface_Radiance_Plugin::vboAdded(Utils::VBO *vbo)
{
	MapHandlerGen* map = static_cast<MapHandlerGen*>(QObject::sender());

	if(map == m_schnapps->getSelectedMap())
	{
		if(vbo->dataSize() == 3)
		{
			m_dockTab->addPositionVBO(QString::fromStdString(vbo->name()));
			m_dockTab->addNormalVBO(QString::fromStdString(vbo->name()));
			m_dockTab->addTangentVBO(QString::fromStdString(vbo->name()));
			m_dockTab->addBiNormalVBO(QString::fromStdString(vbo->name()));
		}
	}
}

void Surface_Radiance_Plugin::vboRemoved(Utils::VBO *vbo)
{
	MapHandlerGen* map = static_cast<MapHandlerGen*>(QObject::sender());

	if(map == m_schnapps->getSelectedMap())
	{
		if(vbo->dataSize() == 3)
		{
			m_dockTab->removePositionVBO(QString::fromStdString(vbo->name()));
			m_dockTab->removeNormalVBO(QString::fromStdString(vbo->name()));
			m_dockTab->removeTangentVBO(QString::fromStdString(vbo->name()));
			m_dockTab->removeBiNormalVBO(QString::fromStdString(vbo->name()));
		}
	}

	MapParameters& mapParam = h_mapParameterSet[map];
	if(mapParam.positionVBO == vbo)
	{
		mapParam.positionVBO = NULL;
	}
	if(mapParam.normalVBO == vbo)
	{
		mapParam.normalVBO = NULL;
	}
	if(mapParam.tangentVBO == vbo)
	{
		mapParam.tangentVBO = NULL;
	}
	if(mapParam.binormalVBO == vbo)
	{
		mapParam.binormalVBO = NULL;
	}
}

void Surface_Radiance_Plugin::attributeModified(unsigned int orbit, QString nameAttr)
{
	if(orbit == VERTEX)
	{

	}
}

void Surface_Radiance_Plugin::importFromFileDialog_SH()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(m_schnapps, "Import surface with radiance (SH)", m_schnapps->getAppPath(), "Surface with radiance Files (*.ply)");
	QStringList::Iterator it = fileNames.begin();
	while(it != fileNames.end()) {
		importFromFile_SH(*it);
		++it;
	}
}

void Surface_Radiance_Plugin::importFromFileDialog_P()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(m_schnapps, "Import surface with radiance (P)", m_schnapps->getAppPath(), "Surface with radiance Files (*.ply)");
	QStringList::Iterator it = fileNames.begin();
	while(it != fileNames.end()) {
		importFromFile_P(*it);
		++it;
	}
}

void Surface_Radiance_Plugin::openComputeRadianceDistanceDialog()
{
	m_computeRadianceDistanceDialog->show();
}

void Surface_Radiance_Plugin::computeRadianceDistanceFromDialog()
{
	QList<QListWidgetItem*> currentItems1 = m_computeRadianceDistanceDialog->list_maps_1->selectedItems();
	QList<QListWidgetItem*> currentItems2 = m_computeRadianceDistanceDialog->list_maps_2->selectedItems();

	if(!currentItems1.empty() && !currentItems2.empty())
	{
		const QString& mapName1 = currentItems1[0]->text();
		const QString& mapName2 = currentItems2[0]->text();

		QString positionName1 = m_computeRadianceDistanceDialog->combo_positionAttribute_1->currentText();
		QString positionName2 = m_computeRadianceDistanceDialog->combo_positionAttribute_2->currentText();

		QString normalName1 = m_computeRadianceDistanceDialog->combo_normalAttribute_1->currentText();
		QString normalName2 = m_computeRadianceDistanceDialog->combo_normalAttribute_2->currentText();

		QString distanceName1;
		if(m_computeRadianceDistanceDialog->distanceAttributeName_1->text().isEmpty())
			distanceName1 = m_computeRadianceDistanceDialog->combo_distanceAttribute_1->currentText();
		else
			distanceName1 = m_computeRadianceDistanceDialog->distanceAttributeName_1->text();

		QString distanceName2;
		if(m_computeRadianceDistanceDialog->distanceAttributeName_2->text().isEmpty())
			distanceName2 = m_computeRadianceDistanceDialog->combo_distanceAttribute_2->currentText();
		else
			distanceName2 = m_computeRadianceDistanceDialog->distanceAttributeName_2->text();

		// create VBO if asked
		if (m_computeRadianceDistanceDialog->enableVBO->isChecked())
		{
			MapHandlerGen* mhg1 = getSCHNApps()->getMap(mapName1);
			if (mhg1)
				mhg1->createVBO(distanceName1);

			MapHandlerGen* mhg2 = getSCHNApps()->getMap(mapName2);
			if (mhg2)
				mhg2->createVBO(distanceName2);
		}

		computeRadianceDistance(mapName1, positionName1, normalName1, distanceName1, mapName2, positionName2, normalName2, distanceName2);
	}
}





void Surface_Radiance_Plugin::changePositionVBO(const QString& map, const QString& vbo)
{
	MapHandlerGen* m = m_schnapps->getMap(map);
	if(m)
	{
		Utils::VBO* vbuf = m->getVBO(vbo);
		h_mapParameterSet[m].positionVBO = vbuf;
		if(m->isSelectedMap())
			m_dockTab->updateMapParameters();
	}
}

void Surface_Radiance_Plugin::changeNormalVBO(const QString& map, const QString& vbo)
{
	MapHandlerGen* m = m_schnapps->getMap(map);
	if(m)
	{
		Utils::VBO* vbuf = m->getVBO(vbo);
		h_mapParameterSet[m].normalVBO = vbuf;
		if(m->isSelectedMap())
			m_dockTab->updateMapParameters();
	}
}

MapHandlerGen* Surface_Radiance_Plugin::importFromFile_SH(const QString& fileName)
{
	QFileInfo fi(fileName);
	if(fi.exists())
	{
		MapHandlerGen* mhg = m_schnapps->addMap(fi.baseName(), 2);
		if(mhg)
		{
			MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(mhg);
			PFP2::MAP* map = mh->getMap();

			MeshTablesSurface_Radiance importer(*map);
			if (!importer.importPLY<Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3> >(fileName.toStdString()))
			{
				std::cout << "could not import " << fileName.toStdString() << std::endl;
				return NULL;
			}
			CGoGN::Algo::Surface::Import::importMesh<PFP2>(*map, importer);

			// get vertex position attribute
			VertexAttribute<PFP2::VEC3, PFP2::MAP> position = map->getAttribute<PFP2::VEC3, VERTEX, PFP2::MAP>("position") ;
			VertexAttribute<PFP2::VEC3, PFP2::MAP> normal = map->getAttribute<PFP2::VEC3, VERTEX, PFP2::MAP>("normal");
			mh->registerAttribute(position);
			mh->registerAttribute(normal);

			MapParameters& mapParams = h_mapParameterSet[mhg];

			mapParams.nbVertices = Algo::Topo::getNbOrbits<VERTEX>(*map);

            //TER trick
            mapParams.radianceResolution = Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>::get_resolution();
            mapParams.radianceNb_coefs = Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>::get_nb_coefs();

			mapParams.radiance = map->getAttribute<Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>, VERTEX, PFP2::MAP>("radiance") ;
			mapParams.radianceTexture = new Utils::Texture<2, Geom::Vec3f>(GL_FLOAT);
			mapParams.param = map->checkAttribute<Geom::Vec2i, VERTEX, PFP2::MAP>("param");

			// create texture
			unsigned int nbv_nbc = Algo::Topo::getNbOrbits<VERTEX>(*map) * Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>::get_nb_coefs();
			unsigned int size = 1;
			while (size * size < nbv_nbc)
				size <<= 1;

			mapParams.radianceTexture->create(Geom::Vec2i(size, size));

			// fill texture
			unsigned int count = 0;
			foreach_cell<VERTEX>(*map, [&] (Vertex v)
			{
				unsigned int i = count / size;
				unsigned int j = count % size;
				mapParams.param[v] = Geom::Vec2i(i, j) ; // first index for current vertex
				for (int l = 0 ; l <= Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>::get_resolution() ; ++l)
				{
					for (int m = -l ; m <= l ; ++m)
					{
						i = count / size;
						j = count % size;
						(*(mapParams.radianceTexture))(i,j) = mapParams.radiance[v].get_coef(l, m);
						++count;
					}
				}
			}) ;
			// resulting texture : SH00_vx0, SH1-1_vx0, ..., SHlm_vx0, SH00_vx1, SH1-1_vx1, ..., SHlm_vx1, etc.
			// resulting param : param[vxI] points to SH00_vxI
			// the size of the texture is needed to know where to do the divisions and modulos.

			mapParams.radianceTexture->update();

			// uncomment this line to be able to load multiple objects with different SH basis
			// (decimation will be unavailable)
//			map->removeAttribute(mapParams.radiance);

			mapParams.paramVBO = new Utils::VBO();
			mapParams.paramVBO->updateData(mapParams.param);

			mapParams.radiancePerVertexShader = new Utils::ShaderRadiancePerVertex(Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>::get_resolution());
			registerShader(mapParams.radiancePerVertexShader);
		}

        this->pythonRecording("importFromFile_SH", mhg->getName(), fi.baseName());

		return mhg;
	}
	else
		return NULL;
}

MapHandlerGen* Surface_Radiance_Plugin::importFromFile_P(const QString& fileName)
{
	QFileInfo fi(fileName);
	if(fi.exists())
	{
		MapHandlerGen* mhg = m_schnapps->addMap(fi.baseName(), 2);
		if(mhg)
		{
			MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(mhg);
			PFP2::MAP* map = mh->getMap();

			MeshTablesSurface_Radiance_P importer(*map);
			if (!importer.importPLY<Utils::BivariatePolynomials<PFP2::REAL, PFP2::VEC3> >(fileName.toStdString()))
			{
				std::cout << "could not import " << fileName.toStdString() << std::endl;
				return NULL;
			}
			CGoGN::Algo::Surface::Import::importMesh<PFP2>(*map, importer);

            // get vertex position attribute
			VertexAttribute<PFP2::VEC3, PFP2::MAP> position = map->getAttribute<PFP2::VEC3, VERTEX, PFP2::MAP>("position") ;
			VertexAttribute<PFP2::VEC3, PFP2::MAP> tangent = map->getAttribute<PFP2::VEC3, VERTEX, PFP2::MAP>("tangent");
			VertexAttribute<PFP2::VEC3, PFP2::MAP> normal = map->getAttribute<PFP2::VEC3, VERTEX, PFP2::MAP>("normal");
			VertexAttribute<PFP2::VEC3, PFP2::MAP> binormal = map->getAttribute<PFP2::VEC3, VERTEX, PFP2::MAP>("binormal");
			mh->registerAttribute(position);
			mh->registerAttribute(tangent);
			mh->registerAttribute(normal);
			mh->registerAttribute(binormal);

			MapParameters& mapParams = h_mapParameterSet[mhg];

			mapParams.nbVertices = Algo::Topo::getNbOrbits<VERTEX>(*map);

			mapParams.radiance_P = map->getAttribute<Utils::BivariatePolynomials<PFP2::REAL, PFP2::VEC3>, VERTEX, PFP2::MAP>("radiance") ;
			mapParams.radianceTexture = new Utils::Texture<2, Geom::Vec3f>(GL_FLOAT);
			mapParams.param = map->checkAttribute<Geom::Vec2i, VERTEX, PFP2::MAP>("param");

			unsigned int nbCoefs = mapParams.radiance_P[map->begin()].get_nb_coefs();

			// create texture
			unsigned int nbv_nbc = Algo::Topo::getNbOrbits<VERTEX>(*map) * nbCoefs;
			unsigned int size = 1;
			while (size * size < nbv_nbc)
				size <<= 1;

			mapParams.radianceTexture->create(Geom::Vec2i(size, size));

			// fill texture
			unsigned int count = 0;
			foreach_cell<VERTEX>(*map, [&] (Vertex v)
			{
				PFP2::VEC3* coefs = mapParams.radiance_P[v].get_coef_tab();
				unsigned int i = count / size;
				unsigned int j = count % size;
				mapParams.param[v] = Geom::Vec2i(i, j) ; // first index for current vertex
				for (unsigned int l = 0 ; l <= nbCoefs ; ++l)
				{
					i = count / size;
					j = count % size;
					(*(mapParams.radianceTexture))(i,j) = coefs[l];
					++count;
				}
			}) ;
			// resulting texture : PB0_v0, PB1_v0, ..., PB14_v0, PB0_v1, ...
			// resulting param : param[vx] points to PB0_vx
			// the size of the texture is needed to know where to do the divisions and modulos.

			mapParams.radianceTexture->update();

			mapParams.paramVBO = new Utils::VBO();
			mapParams.paramVBO->updateData(mapParams.param);

			mapParams.radiancePerVertexPShader = new Utils::ShaderRadiancePerVertex_P();
			registerShader(mapParams.radiancePerVertexPShader);
		}

        this->pythonRecording("importFromFile_P", mhg->getName(), fi.baseName());

		return mhg;
	}
	else
		return NULL;
}

void Surface_Radiance_Plugin::decimate(const QString& mapName, const QString& positionAttributeName, const QString& normalAttributeName, float decimationGoal, bool halfCollapse, bool exportMeshes)
{
	MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
	if(mh == NULL)
		return;

	VertexAttribute<PFP2::VEC3, PFP2::MAP> position = mh->getAttribute<PFP2::VEC3, VERTEX>(positionAttributeName);
	if(!position.isValid())
		return;

	VertexAttribute<PFP2::VEC3, PFP2::MAP> normal = mh->getAttribute<PFP2::VEC3, VERTEX>(normalAttributeName);
	if(!normal.isValid())
		return;

	PFP2::MAP* map = mh->getMap();

	unsigned int nbVertices = Algo::Topo::getNbOrbits<VERTEX>(*map);

	MapParameters& mapParams = h_mapParameterSet[mh];

	if (halfCollapse)
	{
		if (mapParams.positionApproximator == NULL)
		{
			mapParams.positionApproximator = new Algo::Surface::Decimation::Approximator_HalfCollapse<PFP2, PFP2::VEC3>(*map, position);
		}

		if (mapParams.normalApproximator == NULL)
		{
			mapParams.normalApproximator = new Algo::Surface::Decimation::Approximator_HalfCollapse<PFP2, PFP2::VEC3>(*map, normal);
		}

		if (mapParams.radianceApproximator == NULL)
		{
			mapParams.radianceApproximator = new Algo::Surface::Decimation::Approximator_HalfCollapse<PFP2, Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3> >(*map, mapParams.radiance);
		}

		if (mapParams.selector == NULL)
		{
			mapParams.selector = new HalfEdgeSelector_Radiance<PFP2>(
				*map,
				position,
				normal,
				mapParams.radiance,
				*(Algo::Surface::Decimation::Approximator<PFP2, PFP2::VEC3, DART>*)(mapParams.positionApproximator),
				*(Algo::Surface::Decimation::Approximator<PFP2, PFP2::VEC3, DART>*)(mapParams.normalApproximator),
				*(Algo::Surface::Decimation::Approximator<PFP2, Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>, DART>*)(mapParams.radianceApproximator)
			);
		}
	}
	else
	{
		if (mapParams.positionApproximator == NULL)
		{
			mapParams.positionApproximator = new Algo::Surface::Decimation::Approximator_QEM<PFP2>(*map, position);
		}

		if (mapParams.normalApproximator == NULL)
		{
			mapParams.normalApproximator =
				new Algo::Surface::Decimation::Approximator_InterpolateAlongEdge<PFP2, PFP2::VEC3>(
					*map,
					normal,
					position,
					((Algo::Surface::Decimation::Approximator<PFP2, PFP2::VEC3, EDGE>*)(mapParams.positionApproximator))->getApproximationResultAttribute()
				);
		}

		if (mapParams.radianceApproximator == NULL)
		{
			mapParams.radianceApproximator =
				new Algo::Surface::Decimation::Approximator_InterpolateAlongEdge<PFP2, Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3> >(
					*map,
					mapParams.radiance,
					position,
					((Algo::Surface::Decimation::Approximator<PFP2, PFP2::VEC3, EDGE>*)(mapParams.positionApproximator))->getApproximationResultAttribute()
				);
		}

		if (mapParams.selector == NULL)
		{
			mapParams.selector =
				new EdgeSelector_Radiance<PFP2>(
					*map,
					position,
					normal,
					mapParams.radiance,
					*(Algo::Surface::Decimation::Approximator<PFP2, PFP2::VEC3, EDGE>*)(mapParams.positionApproximator),
					*(Algo::Surface::Decimation::Approximator<PFP2, PFP2::VEC3, EDGE>*)(mapParams.normalApproximator),
					*(Algo::Surface::Decimation::Approximator<PFP2, Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>, EDGE>*)(mapParams.radianceApproximator)
				);

			mapParams.selector =
				new Algo::Surface::Decimation::EdgeSelector_QEM<PFP2>(
					*map,
					position,
					*(Algo::Surface::Decimation::Approximator<PFP2, PFP2::VEC3, EDGE>*)(mapParams.positionApproximator)
				);
		}
	}

	std::vector<Algo::Surface::Decimation::ApproximatorGen<PFP2>*> approximators;
	approximators.push_back(mapParams.positionApproximator);
	approximators.push_back(mapParams.normalApproximator);
	approximators.push_back(mapParams.radianceApproximator);

	exportNbVert.clear();
	if (exportMeshes)
	{
		unsigned int goalNbV = decimationGoal * nbVertices;
		unsigned int curNbV = nbVertices / 2;
		while (curNbV > goalNbV)
		{
			exportNbVert.push_back(curNbV);
			curNbV /= 2;
		}
		exportNbVert.push_back(goalNbV);
		nextExportIndex = 0;
	}

	std::cout << "nb vert -> " << nbVertices << std::endl;
	for (unsigned int v : exportNbVert)
	{
		std::cout << v << std::endl;
	}

	m_currentlyDecimatedMap = mh;
	m_currentDecimationHalf = halfCollapse;
	Algo::Surface::Decimation::decimate<PFP2>(*map, mapParams.selector, approximators, decimationGoal * nbVertices, true, NULL, (void (*)(void*, const void*))(Surface_Radiance_Plugin::checkNbVerticesAndExport), (void*)(this));
	m_currentlyDecimatedMap = NULL;

	mh->notifyConnectivityModification();
	mh->notifyAttributeModification(position);
}

void Surface_Radiance_Plugin::applyTaubinFilter(const QString& mapName,
    const QString& positionAttributeName,
    int iterationsNumber
)
{   
    MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
    if(mh == NULL)
        return;

    VertexAttribute<PFP2::VEC3, PFP2::MAP> position = mh->getAttribute<PFP2::VEC3, VERTEX>(positionAttributeName);
    if(!position.isValid())
        return;


    PFP2::MAP* map = mh->getMap();

    VertexAttribute<PFP2::VEC3, PFP2::MAP> position2=map->addAttribute<PFP2::VEC3, VERTEX, PFP2::MAP>("position2");

    unsigned int nbVertices = Algo::Topo::getNbOrbits<VERTEX>(*map);
    std::cout << "nb vert -> " << nbVertices << std::endl;

    for(int i=0; i<iterationsNumber; ++i)
    {
        Algo::Surface::Filtering::filterTaubin<PFP2>(*map, position, position2);
    }

    map->removeAttribute<PFP2::VEC3, VERTEX, PFP2::MAP>(position2);

    mh->notifyConnectivityModification();
    mh->notifyAttributeModification(position);
}

void Surface_Radiance_Plugin::computeRadianceDistance(const QString& mapName1,
    const QString& positionAttributeName1,
    const QString& normalAttributeName1,
    const QString& distanceAttributeName1,
    const QString& mapName2,
    const QString& positionAttributeName2,
    const QString& normalAttributeName2,
    const QString& distanceAttributeName2,
    const QString& outFile)
{
    clock_t begin = clock();

    std::ofstream ofs;
    if(!outFile.isEmpty())
        ofs.open(outFile.toStdString(), std::ostream::app);

	MapHandler<PFP2>* mh1 = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName1));
	if(mh1 == NULL)
		return;

	MapHandler<PFP2>* mh2 = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName2));
	if(mh2 == NULL)
		return;

	VertexAttribute<PFP2::VEC3, PFP2::MAP> position1 = mh1->getAttribute<PFP2::VEC3, VERTEX>(positionAttributeName1);
	if(!position1.isValid())
		return;

	VertexAttribute<PFP2::VEC3, PFP2::MAP> normal1 = mh1->getAttribute<PFP2::VEC3, VERTEX>(normalAttributeName1);
	if(!normal1.isValid())
		return;

	VertexAttribute<PFP2::VEC3, PFP2::MAP> position2 = mh2->getAttribute<PFP2::VEC3, VERTEX>(positionAttributeName2);
	if(!position2.isValid())
		return;

	VertexAttribute<PFP2::VEC3, PFP2::MAP> normal2 = mh2->getAttribute<PFP2::VEC3, VERTEX>(normalAttributeName2);
	if(!normal2.isValid())
		return;

	VertexAttribute<PFP2::REAL, PFP2::MAP> distance1 = mh1->getAttribute<PFP2::REAL, VERTEX>(distanceAttributeName1);
	if(!distance1.isValid())
		distance1 = mh1->addAttribute<PFP2::REAL, VERTEX>(distanceAttributeName1);

	VertexAttribute<PFP2::REAL, PFP2::MAP> distance2 = mh2->getAttribute<PFP2::REAL, VERTEX>(distanceAttributeName2);
	if(!distance2.isValid())
		distance2 = mh2->addAttribute<PFP2::REAL, VERTEX>(distanceAttributeName2);

	MapParameters& mapParams1 = h_mapParameterSet[mh1];
	MapParameters& mapParams2 = h_mapParameterSet[mh2];

	SphericalFunctionIntegratorCartesian integrator;
	integrator.Init(29);

	PFP2::MAP* map1 = mh1->getMap();
	PFP2::MAP* map2 = mh2->getMap();

	// compute distance between map1 and map2 here
	//   distance from map1 to map2 is stored in map1 vertex attribute distance1
	//   distance from map2 to map1 is stored in map2 vertex attribute distance2

	// for each vertex of map1

    std::vector<std::pair<PFP2::REAL, PFP2::REAL> > errors;

    errors.reserve(map1->getNbCells(VERTEX)); //better allocate everything we'll need immediately

	map2->setExternalThreadsAuthorization(true);

    Geom::BoundingBox<PFP2::VEC3> fusionBB(mh1->getBB().min());
    fusionBB.addPoint(mh1->getBB().max());
    fusionBB.fusion(mh2->getBB());
    FaceClustersAggregation fca(fusionBB, 16, 6, 10);
    fca.clusterizeMap((*map2),position2, normal2, PFP2::VEC3(0,1,0), -0.02);

    foreach_cell<VERTEX>(*map1, [&] (Vertex v)
    {
        if(PFP2::VEC3(0,1,0)*normal1[v]>=-0.02) {
        const PFP2::VEC3& P = position1[v];
        PFP2::VEC3& N = normal1[v];

        // find closest point on map2 (naive)

        Face closestFace;

        //OLD VERSION

        /*
        PFP2::REAL minDist2=std::numeric_limits<PFP2::REAL>::max();
        for (Face f : allFacesOf(*map2))
        {
            PFP2::REAL dist = Algo::Geometry::squaredDistancePoint2Face<PFP2>(*map2, f, position2, P);
            if (dist < minDist2)
            {
                minDist2 = dist;
                closestFace = f;
            }
        }
        */

        //NEW VERSION

        closestFace=fca.findFace((*map2), P, position2);

        //

        double l1, l2, l3;
		Algo::Geometry::closestPointInTriangle<PFP2>(*map2, closestFace, position2, P, l1, l2, l3);

		// compute radiance error

		const PFP2::VEC3& P1 = position2[closestFace.dart];
		const PFP2::VEC3& P2 = position2[map2->phi1(closestFace.dart)];
		const PFP2::VEC3& P3 = position2[map2->phi_1(closestFace.dart)];
		PFP2::VEC3 CP = l1*P1 + l2*P2 + l3*P3;

		const PFP2::VEC3& N1 = normal2[closestFace.dart];
		const PFP2::VEC3& N2 = normal2[map2->phi1(closestFace.dart)];
		const PFP2::VEC3& N3 = normal2[map2->phi_1(closestFace.dart)];
		PFP2::VEC3 CPN = l1*N1 + l2*N2 + l3*N3;

		const Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>& R1 = mapParams2.radiance[closestFace.dart];
		const Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>& R2 = mapParams2.radiance[map2->phi1(closestFace.dart)];
		const Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>& R3 = mapParams2.radiance[map2->phi_1(closestFace.dart)];

        Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3> CPR = R1*l1 + R2*l2 + R3*l3;

		Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3> diffRad(mapParams1.radiance[v]);

        diffRad -= CPR;

        double integral;
        double area;
        integrator.Compute(&integral, &area, SHEvalCartesian_Error, &diffRad, isInHemisphere, N.data());

        PFP2::REAL radError = integral / area;

        distance1[v] = radError;

        // barycentric area=sum of 1/3 of each area involved with the vertice
        PFP2::REAL barycentricArea = Algo::Surface::Geometry::vertexBarycentricArea<PFP2>((*map1), v, position1);

        errors.push_back(std::pair<PFP2::REAL, PFP2::REAL>(radError, barycentricArea));
        }
    }
	);

	map2->setExternalThreadsAuthorization(false);

    std::sort(errors.begin(), errors.end());
    PFP2::REAL Q1 = errors[int(errors.size() / 4)].first;
//	PFP2::REAL Q2 = errors[int(errors.size() / 2)];
    PFP2::REAL Q3 = errors[int(errors.size() * 3 / 4)].first;
	PFP2::REAL IQrange = Q3 - Q1;
	PFP2::REAL lowerBound = Q1 - 1.5*IQrange;
    PFP2::REAL upperBound = Q3 + 1.5*IQrange;

    PFP2::REAL maxDistance = 0;
    PFP2::REAL avgDistance = 0;
    PFP2::REAL avgDistanceN2 = 0;
    PFP2::REAL areaDistance = 0;
    PFP2::REAL areaDistanceN2 = 0;

    PFP2::REAL totalAreaWeights=0;

    for (PFP2::REAL& dist : distance1.iterable())
    {
        if (dist < lowerBound) { dist = lowerBound; }
        if (dist > upperBound) { dist = upperBound; }
    }

    for(unsigned int i=0; i<errors.size(); ++i)
    {
        avgDistance += errors[i].first;
        avgDistanceN2 += errors[i].first*errors[i].first;

        areaDistance += errors[i].first*errors[i].second;
        areaDistanceN2 += errors[i].first*errors[i].first*errors[i].second;

        totalAreaWeights += errors[i].second;
    }

    maxDistance = errors[errors.size()-1].first;

    avgDistance     = errors.size() > 0 ? avgDistance/errors.size() : 0;
    avgDistanceN2   = errors.size() > 0 ? sqrt(avgDistanceN2/errors.size()) : 0;

    areaDistance    = totalAreaWeights > 0 ? areaDistance / totalAreaWeights : 0;
    areaDistanceN2  = totalAreaWeights > 0 ? sqrt(areaDistanceN2 / totalAreaWeights) : 0;

	integrator.Release();

    if(ofs.is_open())
    {
        ofs << map1->getNbCells(VERTEX) << ',' << map2->getNbCells(VERTEX) << ',' << maxDistance << ',';
        ofs << avgDistance << ',' << avgDistanceN2 << ',' << areaDistance << ',' << areaDistanceN2 << ',';
        ofs << mapParams1.radianceResolution << ',' << mapParams2.radianceResolution << std::endl;
    }
    else
    {
        CGoGNout << map1->getNbCells(VERTEX) << ',' << map2->getNbCells(VERTEX) << ',' << maxDistance << ',';
        CGoGNout << avgDistance << ',' << avgDistanceN2 << ',' << areaDistance << ',' << areaDistanceN2 << ',';
        CGoGNout << mapParams1.radianceResolution << ',' << mapParams2.radianceResolution << CGoGNendl;
    }

    ofs.close();

    this->pythonRecording("computeRadianceDistance", "", mapName1, positionAttributeName1, normalAttributeName1, distanceAttributeName1,
                            mapName2, positionAttributeName2, normalAttributeName2, distanceAttributeName2);

    mh1->notifyAttributeModification(distance1);
    mh2->notifyAttributeModification(distance2);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    CGoGNout << "computeRadianceDistance CPU time: " << elapsed_secs << CGoGNendl;
}

bool Surface_Radiance_Plugin::quickRadianceValid(const Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>& radiance)
{
    PFP2::VEC3 rCheck=radiance.get_coef(0,0);
    if(rCheck.hasNan() || !std::isfinite(rCheck[0]))
        return false;
    return true;
}

void Surface_Radiance_Plugin::fixNaNRadiance(const QString& mapName, const QString& positionAttributeName)
{
    MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
    if(mh == NULL)
        return;

    PFP2::MAP* map = mh->getMap();
    MapParameters& mapParams = h_mapParameterSet[mh];

    VertexAttribute<PFP2::VEC3, PFP2::MAP> position = mh->getAttribute<PFP2::VEC3, VERTEX>(positionAttributeName);
    if(!position.isValid())
        return;

    Parallel::foreach_cell<VERTEX>(*map, [&] (Vertex v, unsigned int threadIndex)
    {
        if(!quickRadianceValid(mapParams.radiance[v]))
        {
            CGoGNerr << "Found vertice with infinite radiance coefs at position " << position[v] << CGoGNendl;
            mapParams.radiance[v.dart]=mapParams.radiance[map->phi1(v.dart)];
        }
    });
}

void Surface_Radiance_Plugin::fixDoubledVertices(const QString& mapName, const QString& positionAttributeName, const QString& normalAttributeName)
{
    //Checks every radiances and tries to find a vertice at the same
    MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
    if(mh == NULL)
        return;

    PFP2::MAP* map = mh->getMap();
    MapParameters& mapParams = h_mapParameterSet[mh];

    //for time optimization purposes
    std::vector<Vertex> checkedVertexVector;
    checkedVertexVector.reserve(map->getNbCells(VERTEX));

    VertexAttribute<PFP2::VEC3, PFP2::MAP> position = mh->getAttribute<PFP2::VEC3, VERTEX>(positionAttributeName);
    if(!position.isValid())
        return;

    VertexAttribute<PFP2::VEC3, PFP2::MAP> normal = mh->getAttribute<PFP2::VEC3, VERTEX>(normalAttributeName);
    if(!normal.isValid())
        return;

    foreach_cell<VERTEX>(*map, [&] (Vertex v)
    {
        for(Vertex v2 : checkedVertexVector)
        {
            if(position[v]==position[v2])
            {
                if(mapParams.radiance[v2.dart].get_coef(0,0)[0]!=0)
                    mapParams.radiance[v.dart]=mapParams.radiance[v2.dart];
                else
                    mapParams.radiance[v2.dart]=mapParams.radiance[v.dart];
            }

        }
        checkedVertexVector.push_back(v);
    });
    CGoGNerr << "done with checking errors." << CGoGNendl;
}

void Surface_Radiance_Plugin::fixSize(const QString& mapName, const QString& positionAttributeName, double scale)
{
    //Checks every radiances and tries to find a vertice at the same
    MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
    if(mh == NULL)
        return;

    PFP2::MAP* map = mh->getMap();

    VertexAttribute<PFP2::VEC3, PFP2::MAP> position = mh->getAttribute<PFP2::VEC3, VERTEX>(positionAttributeName);
    if(!position.isValid())
        return;

    foreach_cell<VERTEX>(*map, [&] (Vertex v)
    {
        position[v]*=scale;
    });
}

void Surface_Radiance_Plugin::indiscriminate(const QString& mapName, const QString& positionAttributeName, const QString &normalAttributeName,
                    PFP2::VEC3 N, PFP2::REAL threshold)
{
    MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
    if(mh == NULL)
        return;

    PFP2::MAP* map = mh->getMap();
    MapParameters& mapParams = h_mapParameterSet[mh];

    VertexAttribute<PFP2::VEC3, PFP2::MAP> position = mh->getAttribute<PFP2::VEC3, VERTEX>(positionAttributeName);
    if(!position.isValid())
        return;

    VertexAttribute<PFP2::VEC3, PFP2::MAP> normal = mh->getAttribute<PFP2::VEC3, VERTEX>(normalAttributeName);
    if(!normal.isValid())
        return;

    Parallel::foreach_cell<VERTEX>(*map, [&] (Vertex v, unsigned int threadIndex)
    {
        if(N*normal[v]<threshold)
        {
            int l= mapParams.radianceResolution;
            while(l>0)
            {
                for(int m=-l; m<=l; ++m)
                {
                    mapParams.radiance[v.dart].get_coef(l,m)=PFP2::VEC3(0,0,0);
                }
                --l;
            }
            mapParams.radiance[v.dart].get_coef(0,0)=PFP2::VEC3(2.0, 0, 2.0);
        }
    });

    mh->notifyConnectivityModification();
    mh->notifyAttributeModification(position);
}

void Surface_Radiance_Plugin::checkNbVerticesAndExport(Surface_Radiance_Plugin* p, const unsigned int* nbVertices)
{
	if (!p->exportNbVert.empty())
	{
		MapHandlerGen* mhg = p->currentlyDecimatedMap();
		if (*nbVertices == p->exportNbVert[p->nextExportIndex])
		{
			std::stringstream exportName;
			exportName << p->currentlyDecimatedMap()->getName().toStdString() << "_" << (p->currentDecimationHalf() ? "half_" : "full_") << (*nbVertices) << ".ply";
			std::cout << "export : " << exportName.str() << std::endl;
			p->exportPLY(mhg->getName(), "position", "normal", QString::fromStdString(exportName.str()));
			p->nextExportIndex++;
		}
	}
}

void Surface_Radiance_Plugin::exportPLY(
	const QString& mapName,
	const QString& positionAttributeName,
	const QString& normalAttributeName,
	const QString& filename)
{
	typedef PFP2::MAP MAP;
	typedef PFP2::REAL REAL;
	typedef PFP2::VEC3 VEC3;

#ifdef IN_DEV
    //fixNaNRadiance(mapName, positionAttributeName);
    //fixDoubledVertices(mapName, positionAttributeName, normalAttributeName);
    //indiscriminate(mapName, positionAttributeName, normalAttributeName, PFP2::VEC3(0,1,0), -0.02);
#endif

	MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
	if(mh == NULL)
		return;

	VertexAttribute<VEC3, MAP> position = mh->getAttribute<VEC3, VERTEX>(positionAttributeName);
	if(!position.isValid())
		return;

	VertexAttribute<VEC3, MAP> normal = mh->getAttribute<VEC3, VERTEX>(normalAttributeName);
	if(!normal.isValid())
		return;

    MapParameters& mapParams = h_mapParameterSet[mh];

    VertexAttribute<Utils::SphericalHarmonics<REAL, VEC3>, MAP> radiance = mapParams.radiance;
	if(!radiance.isValid())
		return;

	// open file
	std::ofstream out ;
	out.open(filename.toStdString(), std::ios::out | std::ios::binary) ;

	if (!out.good())
	{
		CGoGNerr << "Unable to open file " << CGoGNendl ;
		return ;
	}

	MAP* map = mh->getMap();

	unsigned int nbDarts = map->getNbDarts() ;
	std::vector<unsigned int> facesSize ;
	std::vector<std::vector<unsigned int> > facesIdx ;
	facesSize.reserve(nbDarts/3) ;
	facesIdx.reserve(nbDarts/3) ;
	std::map<unsigned int, unsigned int> vIndex ;
	unsigned int vCpt = 0 ;
	std::vector<unsigned int> vertices ;
	vertices.reserve(nbDarts/6) ;

	// Go over all faces
	CellMarker<MAP, VERTEX> markV(*map) ;
	TraversorF<MAP> t(*map) ;
	for(Dart d = t.begin(); d != t.end(); d = t.next())
	{
		std::vector<unsigned int> fidx ;
		fidx.reserve(8) ;
		unsigned int degree = 0 ;
		Traversor2FV<MAP> tfv(*map, d) ;
		for(Dart it = tfv.begin(); it != tfv.end(); it = tfv.next())
		{
			++degree ;
			unsigned int vNum = map->getEmbedding<VERTEX>(it) ;
			if(!markV.isMarked(it))
			{
				markV.mark(it) ;
				vIndex[vNum] = vCpt++ ;
				vertices.push_back(vNum) ;
			}
			fidx.push_back(vIndex[vNum]) ;
		}
		facesSize.push_back(degree) ;
		facesIdx.push_back(fidx) ;
	}

	// Start writing the file
	out << "ply" << std::endl ;

	// test endianness
	union
	{
		uint32_t i ;
		char c[4] ;
	} bint = {0x01020304} ;
	if (bint.c[0] == 1) // big endian
		out << "format binary_big_endian 1.0" << std::endl ;
	else
		out << "format binary_little_endian 1.0" << std::endl ;

	out << "comment File generated by the CGoGN library" << std::endl ;
	out << "comment See : http://cgogn.unistra.fr/" << std::endl ;
	out << "comment or contact : cgogn@unistra.fr" << std::endl ;
	// Vertex elements
	out << "element vertex " << vertices.size() << std::endl ;

	std::string nameOfTypePly_REAL(nameOfTypePly(position[0][0])) ;

	out << "property " << nameOfTypePly_REAL << " x" << std::endl ;
	out << "property " << nameOfTypePly_REAL << " y" << std::endl ;
	out << "property " << nameOfTypePly_REAL << " z" << std::endl ;


	out << "property " << nameOfTypePly_REAL << " nx" << std::endl ;
	out << "property " << nameOfTypePly_REAL << " ny" << std::endl ;
	out << "property " << nameOfTypePly_REAL << " nz" << std::endl ;

    int res = mapParams.radianceResolution;
	for (int l = 0 ; l <= res ; ++l)
	{
		for (int m = -l ; m <= l ; ++m)
		{
			out << "property " << nameOfTypePly_REAL << " SHcoef_" << l << "_" << m << "_r" << std::endl ;
			out << "property " << nameOfTypePly_REAL << " SHcoef_" << l << "_" << m << "_g" << std::endl ;
			out << "property " << nameOfTypePly_REAL << " SHcoef_" << l << "_" << m << "_b" << std::endl ;
		}
	}

	// Face element
	out << "element face " << facesSize.size() << std::endl ;
	out << "property list uint8 " << nameOfTypePly(facesIdx[0][0]) << " vertex_indices" << std::endl ;
	out << "end_header" << std::endl ;

	// binary vertices
	for(unsigned int i = 0; i < vertices.size(); ++i)
	{
		const VEC3& p = position[vertices[i]] ;
		out.write((char*)(&(p[0])), sizeof(p)) ;
		const VEC3& n = normal[vertices[i]] ;
		out.write((char*)(&(n[0])), sizeof(n)) ;

		for (int l=0 ; l <= res ; ++l)
		{
			for (int m=-l ; m <= l ; ++m)
			{
				const VEC3& r = radiance[vertices[i]].get_coef(l,m) ;
				out.write((char*)(&(r[0])), sizeof(r)) ;
			}
		}
	}

	// binary faces
	for(unsigned int i = 0; i < facesSize.size(); ++i)
	{
		uint8_t nbe = facesSize[i] ;
		out.write((char*)(&nbe), sizeof(uint8_t)) ;
		out.write((char*)(&(facesIdx[i][0])), facesSize[i] * sizeof(facesIdx[i][0])) ;
	}

	out.close() ;

	this->pythonRecording("exportPLY", "", mapName, positionAttributeName, normalAttributeName, filename);
}

void Surface_Radiance_Plugin::exportPLY_superiorResolution(const QString& mapName,
    const QString& positionAttributeName,
    const QString& normalAttributeName,
    const QString& filename,
    int resolution)
{
#ifdef IN_DEV
    //fixNaNRadiance(mapName, positionAttributeName);
    //fixDoubledVertices(mapName, positionAttributeName, normalAttributeName);
    //indiscriminate(mapName, positionAttributeName, normalAttributeName, PFP2::VEC3(0,1,0), -0.02);
#endif

    typedef PFP2::MAP MAP;
    typedef PFP2::REAL REAL;
    typedef PFP2::VEC3 VEC3;

    MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
    if(mh == NULL)
        return;

    VertexAttribute<VEC3, MAP> position = mh->getAttribute<VEC3, VERTEX>(positionAttributeName);
    if(!position.isValid())
        return;

    VertexAttribute<VEC3, MAP> normal = mh->getAttribute<VEC3, VERTEX>(normalAttributeName);
    if(!normal.isValid())
        return;

    MapParameters& mapParams = h_mapParameterSet[mh];

    if(resolution<=mapParams.radianceResolution)
        return;

    VertexAttribute<Utils::SphericalHarmonics<REAL, VEC3>, MAP> radiance = mapParams.radiance;
    if(!radiance.isValid())
        return;

    // open file
    std::ofstream out ;
    out.open(filename.toStdString(), std::ios::out | std::ios::binary) ;

    if (!out.good())
    {
        CGoGNerr << "Unable to open file " << CGoGNendl ;
        return ;
    }

    MAP* map = mh->getMap();

    unsigned int nbDarts = map->getNbDarts() ;
    std::vector<unsigned int> facesSize ;
    std::vector<std::vector<unsigned int> > facesIdx ;
    facesSize.reserve(nbDarts/3) ;
    facesIdx.reserve(nbDarts/3) ;
    std::map<unsigned int, unsigned int> vIndex ;
    unsigned int vCpt = 0 ;
    std::vector<unsigned int> vertices ;
    vertices.reserve(nbDarts/6) ;

    // Go over all faces
    CellMarker<MAP, VERTEX> markV(*map) ;
    TraversorF<MAP> t(*map) ;
    for(Dart d = t.begin(); d != t.end(); d = t.next())
    {
        std::vector<unsigned int> fidx ;
        fidx.reserve(8) ;
        unsigned int degree = 0 ;
        Traversor2FV<MAP> tfv(*map, d) ;
        for(Dart it = tfv.begin(); it != tfv.end(); it = tfv.next())
        {
            ++degree ;
            unsigned int vNum = map->getEmbedding<VERTEX>(it) ;
            if(!markV.isMarked(it))
            {
                markV.mark(it) ;
                vIndex[vNum] = vCpt++ ;
                vertices.push_back(vNum) ;
            }
            fidx.push_back(vIndex[vNum]) ;
        }
        facesSize.push_back(degree) ;
        facesIdx.push_back(fidx) ;
    }

    // Start writing the file
    out << "ply" << std::endl ;

    // test endianness
    union
    {
        uint32_t i ;
        char c[4] ;
    } bint = {0x01020304} ;
    if (bint.c[0] == 1) // big endian
        out << "format binary_big_endian 1.0" << std::endl ;
    else
        out << "format binary_little_endian 1.0" << std::endl ;

    out << "comment File generated by the CGoGN library" << std::endl ;
    out << "comment See : http://cgogn.unistra.fr/" << std::endl ;
    out << "comment or contact : cgogn@unistra.fr" << std::endl ;
    // Vertex elements
    out << "element vertex " << vertices.size() << std::endl ;

    std::string nameOfTypePly_REAL(nameOfTypePly(position[0][0])) ;

    out << "property " << nameOfTypePly_REAL << " x" << std::endl ;
    out << "property " << nameOfTypePly_REAL << " y" << std::endl ;
    out << "property " << nameOfTypePly_REAL << " z" << std::endl ;


    out << "property " << nameOfTypePly_REAL << " nx" << std::endl ;
    out << "property " << nameOfTypePly_REAL << " ny" << std::endl ;
    out << "property " << nameOfTypePly_REAL << " nz" << std::endl ;

    int res = mapParams.radianceResolution;
    for (int l = 0 ; l <= resolution ; ++l)
    {
        for (int m = -l ; m <= l ; ++m)
        {
            out << "property " << nameOfTypePly_REAL << " SHcoef_" << l << "_" << m << "_r" << std::endl ;
            out << "property " << nameOfTypePly_REAL << " SHcoef_" << l << "_" << m << "_g" << std::endl ;
            out << "property " << nameOfTypePly_REAL << " SHcoef_" << l << "_" << m << "_b" << std::endl ;
        }
    }

    // Face element
    out << "element face " << facesSize.size() << std::endl ;
    out << "property list uint8 " << nameOfTypePly(facesIdx[0][0]) << " vertex_indices" << std::endl ;
    out << "end_header" << std::endl ;

    // binary vertices
    for(unsigned int i = 0; i < vertices.size(); ++i)
    {
        const VEC3& p = position[vertices[i]] ;
        out.write((char*)(&(p[0])), sizeof(p)) ;
        const VEC3& n = normal[vertices[i]] ;
        out.write((char*)(&(n[0])), sizeof(n)) ;

        for (int l=0 ; l <= resolution ; ++l)
        {
            for (int m=-l ; m <= l ; ++m)
            {
                if(l>res)
                {
                    VEC3 r(0,0,0);
                    out.write((char*)(&(r[0])), sizeof(r)) ;
                }
                else
                {
                    const VEC3& r = radiance[vertices[i]].get_coef(l,m) ;
                    out.write((char*)(&(r[0])), sizeof(r)) ;
                }
            }
        }
    }

    // binary faces
    for(unsigned int i = 0; i < facesSize.size(); ++i)
    {
        uint8_t nbe = facesSize[i] ;
        out.write((char*)(&nbe), sizeof(uint8_t)) ;
        out.write((char*)(&(facesIdx[i][0])), facesSize[i] * sizeof(facesIdx[i][0])) ;
    }

    out.close() ;

    this->pythonRecording("exportPLY", "", mapName, positionAttributeName, normalAttributeName, filename);
}

void Surface_Radiance_Plugin::fixSHMap(const QString& mapName,
    const QString& positionAttributeName,
    const QString& normalAttributeName, double scale
)
{
    fixNaNRadiance(mapName, positionAttributeName);
    fixDoubledVertices(mapName, positionAttributeName, normalAttributeName);
    if(scale!=PFP2::REAL(1))
    {
        fixSize(mapName, positionAttributeName, scale);
    }
    this->pythonRecording("fixSHMap", "", mapName, positionAttributeName, normalAttributeName, scale);
}

void Surface_Radiance_Plugin::logRadianceDetails(const QString& mapName, const QString& filename)
{
    //Checks every radiances and tries to find a vertice at the same
    MapHandler<PFP2>* mh = static_cast<MapHandler<PFP2>*>(m_schnapps->getMap(mapName));
    if(mh == NULL)
        return;

    PFP2::MAP* map = mh->getMap();
    MapParameters& mapParams = h_mapParameterSet[mh];

    std::ofstream ofs;
    ofs.open (filename.toStdString(), std::ostream::app);
    assert(ofs.is_open() || !"Couldn't open file in logRadianceDetails");

    ofs << "Map " << mapName.toStdString() << " of radiance level "
        << mapParams.radianceResolution << std::endl;

    foreach_cell<VERTEX>(*map, [&] (Vertex v)
    {
        ofs << mapParams.radiance[v.dart] << std::endl;
    });

    ofs.close();

    this->pythonRecording("logRadianceDetails", "", mapName, filename);
}

#if CGOGN_QT_DESIRED_VERSION == 5
	Q_PLUGIN_METADATA(IID "CGoGN.SCHNapps.Plugin")
#else
	Q_EXPORT_PLUGIN2(Surface_Radiance_Plugin, Surface_Radiance_Plugin)
#endif

} // namespace SCHNApps

} // namespace CGoGN
