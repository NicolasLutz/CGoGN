#ifndef _SURFACE_RADIANCE_PLUGIN_H_
#define _SURFACE_RADIANCE_PLUGIN_H_

#include "plugin_interaction.h"
#include "surface_radiance_dockTab.h"
#include "dialog_computeRadianceDistance.h"

#include "Utils/sphericalHarmonics.h"
#include "Utils/bivariatePolynomials.h"
#include "Utils/Shaders/shaderRadiancePerVertex.h"
#include "Utils/Shaders/shaderRadiancePerVertex_P.h"
#include "Utils/drawer.h"

#include "Algo/Decimation/decimation.h"
#include "Algo/Geometry/intersection.h"
#include "Algo/Filtering/taubin.h"

namespace CGoGN
{

namespace SCHNApps
{

struct MapParameters
{
	MapParameters() :
		positionVBO(NULL),
		tangentVBO(NULL),
		normalVBO(NULL),
		binormalVBO(NULL),
		radiancePerVertexShader(NULL),
		radiancePerVertexPShader(NULL),
		radianceTexture(NULL),
		paramVBO(NULL),
		positionApproximator(NULL),
		normalApproximator(NULL),
		radianceApproximator(NULL),
		selector(NULL)
	{}

	unsigned int nbVertices;

	Utils::VBO* positionVBO;
	Utils::VBO* tangentVBO;
	Utils::VBO* normalVBO;
	Utils::VBO* binormalVBO;

	CGoGN::Utils::ShaderRadiancePerVertex* radiancePerVertexShader;
	CGoGN::Utils::ShaderRadiancePerVertex_P* radiancePerVertexPShader;

    //This is new~
    int radianceResolution;
    int radianceNb_coefs;

	VertexAttribute<Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>, PFP2::MAP> radiance;
	VertexAttribute<Utils::BivariatePolynomials<PFP2::REAL, PFP2::VEC3>, PFP2::MAP> radiance_P;

	Utils::Texture<2, Geom::Vec3f>* radianceTexture;
	VertexAttribute<Geom::Vec2i, PFP2::MAP> param;
	Utils::VBO* paramVBO;

//	DartAttribute<PFP2::REAL, PFP2::MAP> halfedgeError;

	Algo::Surface::Decimation::ApproximatorGen<PFP2>* positionApproximator;
	Algo::Surface::Decimation::ApproximatorGen<PFP2>* normalApproximator;
	Algo::Surface::Decimation::ApproximatorGen<PFP2>* radianceApproximator;

	Algo::Surface::Decimation::Selector<PFP2>* selector;
};

class FaceCluster
{
public:

    FaceCluster();
    FaceCluster(const PFP2::VEC3& bbMin, const PFP2::VEC3& bbMax, unsigned int x, unsigned int y, unsigned int z);
    ~FaceCluster();

    //Checks if p is in cluster
    bool contains(const PFP2::VEC3 &p) {return m_bb.contains(p);}
    bool contains(const PFP2::VEC3 &p1, const PFP2::VEC3 &p2) {return m_bb.contains(p1, p2);}
    bool contains(PFP2::MAP& map, Face f, const VertexAttribute<PFP2::VEC3, PFP2::MAP>& position);

    //Adds f in cluster without asking questions
    void addFace(Face f) {m_cluster.push_back(f);}

    //find the closest triangular face from p in the cluster fc and returns false if there were no faces in the cluster.
    //it fills face with the found face and distance2 with the squared distance found, if the original distance2 was bigger.
    bool closestFaceInCluster(PFP2::MAP& map, const PFP2::VEC3& p, const VertexAttribute<PFP2::VEC3, PFP2::MAP>& position,
                              Face& face, PFP2::REAL& distance2) const;

    size_t size() const {return m_cluster.size();}
    const PFP2::VEC3& bbMin() const {return m_bb.min();}
    const PFP2::VEC3& bbMax() const {return m_bb.max();}

    typedef std::vector<Face>::iterator iterator;
    typedef std::vector<Face>::const_iterator const_iterator;

    iterator begin() {return m_cluster.begin();}
    const_iterator begin() const {return m_cluster.begin();}

    iterator end() {return m_cluster.end();}
    const_iterator end() const {return m_cluster.end();}

    //mark for algorithms
    bool marked() const {return m_mark;}
    void mark(bool b) {m_mark=b;}
    void toggleMark() {m_mark=!m_mark;}

    //index
    unsigned int x() const {return m_x;}
    unsigned int y() const {return m_y;}
    unsigned int z() const {return m_z;}

private:
    std::vector<Face>               m_cluster;
    Geom::BoundingBox<PFP2::VEC3>   m_bb;
    bool                            m_mark;
    unsigned int m_x, m_y, m_z;
};

class FaceClustersAggregation
{
public:

    FaceClustersAggregation(const Geom::BoundingBox<PFP2::VEC3>& boundingBox,
                            size_t width, size_t height, size_t depth);
    ~FaceClustersAggregation();

    //expands the aggregation by adding fc in it
    void addCluster(const FaceCluster& fc) {m_clusters.push_back(fc);}

    //adds f in every clusters it intersects
    void addElement(PFP2::MAP& map, Face f,
                    const VertexAttribute<PFP2::VEC3, PFP2::MAP>& position);

    //Clusterize the whole map
    void clusterizeMap(PFP2::MAP& map, const VertexAttribute<PFP2::VEC3, PFP2::MAP>& position,
                       const VertexAttribute<PFP2::VEC3, PFP2::MAP>& normal,
                       const PFP2::VEC3& N, PFP2::REAL threshold);

    //find the distance from p to the other Cluster, provided the cluster p belongs to.
    PFP2::REAL squaredDistancePoint2Cluster(const PFP2::VEC3& p, const FaceCluster& cluster);

    //find the closest triangular face from p on map considering vertex attribute position
    Face findFace(PFP2::MAP& map, const PFP2::VEC3& p, const VertexAttribute<PFP2::VEC3, PFP2::MAP>& position);

    //get the cluster at the current indexes
    FaceCluster* getCluster(unsigned int i, unsigned int j, unsigned int k);

    //get the cluster p is at, and the closest if p is outside of the clusterized area
    FaceCluster* getCluster(const PFP2::VEC3& p);

    size_t getNbFaces() const {return m_nbFaces;}

    typedef std::vector<FaceCluster>::iterator iterator;
    typedef std::vector<FaceCluster>::const_iterator const_iterator;

    iterator begin() {return m_clusters.begin();}
    const_iterator begin() const {return m_clusters.begin();}

    iterator end() {return m_clusters.end();}
    const_iterator end() const {return m_clusters.end();}

private:

    typedef std::pair<FaceCluster*, PFP2::REAL> distanceEval_t;
    class DistanceEval_Compare
    {
    public:
        DistanceEval_Compare(){}
        bool operator()(const distanceEval_t& object, const distanceEval_t& other){return object.second<other.second;}
    };
    std::vector<FaceCluster>        m_clusters;
    size_t                          m_nbFaces;
    size_t                          m_width;
    size_t                          m_height;
    size_t                          m_depth;
};

class Surface_Radiance_Plugin : public PluginInteraction
{
	Q_OBJECT
	Q_INTERFACES(CGoGN::SCHNApps::Plugin)
#if CGOGN_QT_DESIRED_VERSION == 5
	Q_PLUGIN_METADATA(IID "CGoGN.SCHNapps.Plugin")
#endif

	friend class Surface_Radiance_DockTab;

public:
	Surface_Radiance_Plugin()
	{}

	~Surface_Radiance_Plugin()
	{}

	virtual bool enable();
	virtual void disable();

	virtual void draw(View *view) {}
	virtual void drawMap(View* view, MapHandlerGen* map);

	virtual void keyPress(View* view, QKeyEvent* event) {}
	virtual void keyRelease(View* view, QKeyEvent* event) {}
	virtual void mousePress(View* view, QMouseEvent* event) {}
	virtual void mouseRelease(View* view, QMouseEvent* event) {}
	virtual void mouseMove(View* view, QMouseEvent* event) {}
	virtual void wheelEvent(View* view, QWheelEvent* event) {}

	virtual void viewLinked(View *view) {}
	virtual void viewUnlinked(View *view) {}

private slots:
	// slots called from SCHNApps signals
	void selectedViewChanged(View* prev, View* cur);
	void selectedMapChanged(MapHandlerGen* prev, MapHandlerGen* cur);
	void mapAdded(MapHandlerGen* map);
	void mapRemoved(MapHandlerGen* map);
	void schnappsClosing();

	// slots called from MapHandler signals
	void vboAdded(Utils::VBO* vbo);
	void vboRemoved(Utils::VBO* vbo);
    void attributeModified(unsigned int orbit, QString nameAttr);

	void importFromFileDialog_SH();
	void importFromFileDialog_P();

	void openComputeRadianceDistanceDialog();
    void computeRadianceDistanceFromDialog();

public slots:
	// slots for Python calls
	void changePositionVBO(const QString& map, const QString& vbo);
	void changeNormalVBO(const QString& map, const QString& vbo);
	MapHandlerGen* importFromFile_SH(const QString& fileName);
	MapHandlerGen* importFromFile_P(const QString& fileName);
	void decimate(
		const QString& mapName,
		const QString& positionAttributeName,
		const QString& normalAttributeName,
		float decimationGoal,
		bool halfCollapse = false,
		bool exportMeshes = false
	);
    void applyTaubinFilter(const QString& mapName,
        const QString& positionAttributeName,
        int iterationsNumber
    );
	void computeRadianceDistance(const QString& mapName1,
		const QString& positionAttributeName1,
		const QString& normalAttributeName1,
		const QString& distanceAttributeName1,
		const QString& mapName2,
		const QString& positionAttributeName2,
		const QString& normalAttributeName2,
        const QString& distanceAttributeName2,
        const QString& outFile = QString()
	);
	void exportPLY(
		const QString& mapName,
		const QString& positionAttributeName,
		const QString& normalAttributeName,
		const QString& filename
	);
    void exportPLY_superiorResolution(
        const QString& mapName,
        const QString& positionAttributeName,
        const QString& normalAttributeName,
        const QString& filename,
        int resolution
    );
    void fixSHMap(const QString& mapName,
        const QString& positionAttributeName,
        const QString& normalAttributeName
    , double scale=1);
    void logRadianceDetails(
        const QString& mapName,
        const QString& filename
    );

protected:
	MapHandlerGen* currentlyDecimatedMap() { return m_currentlyDecimatedMap; }
	bool currentDecimationHalf() { return m_currentDecimationHalf; }
	static void checkNbVerticesAndExport(Surface_Radiance_Plugin* p, const unsigned int* nbVertices);
    bool quickRadianceValid(const Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>& radiance);

    void fixNaNRadiance(const QString& mapName, const QString& positionAttributeName);
    void fixDoubledVertices(const QString& mapName, const QString& positionAttributeName, const QString &normalAttributeName);
    void fixSize(const QString& mapName, const QString& positionAttributeName, double scale);

    //colors in ugly magenta vertices that have a normal that has a scalar product with N inferior to threshold
    void indiscriminate(const QString& mapName, const QString& positionAttributeName, const QString &normalAttributeName,
                        PFP2::VEC3 N, PFP2::REAL threshold);

	Surface_Radiance_DockTab* m_dockTab;

	Dialog_ComputeRadianceDistance* m_computeRadianceDistanceDialog;
	QAction* m_computeRadianceDistanceAction;

	QHash<MapHandlerGen*, MapParameters> h_mapParameterSet;

	MapHandlerGen* m_currentlyDecimatedMap;
	bool m_currentDecimationHalf;
	std::vector<unsigned int> exportNbVert;
	unsigned int nextExportIndex;

	QAction* m_importSHAction;
    QAction* m_importPAction;

	static bool isInHemisphere(double x, double y, double z, void* u)
	{ // true iff [x,y,z] and u have the same direction
		PFP2::REAL* n = (PFP2::REAL*)(u);
		return x*n[0] + y*n[1] + z*n[2] >= 0.0;
	}

	static double SHEvalCartesian_Error(double x, double y, double z, void* u)
	{
		Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>& e = *(Utils::SphericalHarmonics<PFP2::REAL, PFP2::VEC3>*)(u);
		PFP2::VEC3 c = e.evaluate_at(x, y, z, 0);
		return c.norm2();
	}
};

} // namespace SCHNApps

} // namespace CGoGN

#endif // _SURFACE_RADIANCE_PLUGIN_H_
