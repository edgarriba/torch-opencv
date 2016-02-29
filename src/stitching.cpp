#include <stitching.hpp>

extern "C"
struct RectPlusBool detail_overlapRoi(
    struct PointWrapper tl1, struct PointWrapper tl2,
    struct SizeWrapper sz1, struct SizeWrapper sz2)
{
    struct RectPlusBool result;
    cv::Rect roi;
    result.val = detail::overlapRoi(tl1, tl2, sz1, sz2, roi);
    new(&result.rect) RectWrapper(roi);
    return result;
}

extern "C"
struct RectWrapper detail_resultRoi(
	struct PointArray corners,
	struct SizeArray sizes)
{
    return RectWrapper(detail::resultRoi(corners, sizes));
}	

extern "C"
struct RectWrapper detail_resultRoiIntersection(
	struct PointArray corners,
	struct SizeArray sizes)
{
    return RectWrapper(detail::resultRoiIntersection(corners, sizes));
}

extern "C"
struct PointWrapper detail_resultTl(
	struct PointArray corners)
{
    return PointWrapper(detail::resultTl(corners));
}

extern "C"
struct IntArray detail_selectRandomSubset(
	int count, int size)
{
    std::vector<int> subset;
    detail::selectRandomSubset(count, size, subset);
    return IntArray(subset);
}

extern "C"
int detail_stitchingLogLevel()
{
    return detail::stitchingLogLevel();
}	

/****************** Classes ******************/

//CameraParams

extern "C"
struct CameraParamsPtr CameraParams_ctor()
{
    return new cv::detail::CameraParams();
}

extern "C"
struct CameraParamsPtr CameraParams_ctor2(
	struct CameraParamsPtr other)
{
    cv::detail::CameraParams * instant = static_cast<cv::detail::CameraParams *>(other.ptr);
    return new cv::detail::CameraParams(*instant);
}

extern "C"
void CameraParams_dtor(
	struct CameraParamsPtr ptr)
{
    delete static_cast<cv::detail::CameraParams *>(ptr.ptr);
}

struct TensorWrapper CameraParams_K(
	struct CameraParamsPtr ptr)
{
    return TensorWrapper(ptr->K());
}

//TODO need to add const CameraParams& cv::detail::CameraParams::operator=(const CameraParams & other)	

//DisjointSets

extern "C"
struct DisjointSetsPtr DisjointSets_ctor(
	int elem_count)
{
    return new cv::detail::DisjointSets(elem_count);
}

extern "C"
void DisjointSets_dtor(
	struct DisjointSetsPtr ptr)
{
    delete static_cast<cv::detail::DisjointSets *>(ptr.ptr);
}

extern "C"
void DisjointSets_createOneElemSets(
	struct DisjointSetsPtr ptr, int elem_count)
{
    ptr->createOneElemSets(elem_count);
}

extern "C"
int DisjointSets_findSetByElem(
	struct DisjointSetsPtr ptr, int elem)
{
    return ptr->findSetByElem(elem);
}

extern "C"
int DisjointSets_mergeSets(
	struct DisjointSetsPtr ptr,
	int set1, int set2)
{
    return ptr->mergeSets(set1, set2);
}

//Graph

extern "C"
struct GraphPtr Graph_ctor(
	int num_vertices)
{
    return new cv::detail::Graph(num_vertices);
}

extern "C"
void Graph_dtor(
	struct GraphPtr ptr)
{
    delete static_cast<cv::detail::Graph *>(ptr.ptr);
}

extern "C"
void Graph_addEdge(
	struct GraphPtr ptr, int from, int to, float weight)
{
    ptr->addEdge(from, to, weight);
}

extern "C"
void Graph_create(
	struct GraphPtr ptr, int num_vertices)
{
    ptr->create(num_vertices);
}

extern "C"
int Graph_numVertices(
	struct GraphPtr ptr)
{
    return ptr->numVertices();
}

//GraphEdge

extern "C"
struct GraphEdgePtr GraphEdge_ctor(
	int from, int to, float weight)
{
    return new cv::detail::GraphEdge(from, to, weight);
}

extern "C"
void GraphEdge_dtor(
	struct GraphEdgePtr ptr)
{
    delete static_cast<cv::detail::GraphEdge *>(ptr.ptr);
}

//Timelapser

extern "C"
struct TimelapserPtr Timelapser_ctor(
	int type)
{
    return rescueObjectFromPtr(
			cv::detail::Timelapser::createDefault(type));
}

extern "C"
void Timelapser_dtor(
	struct TimelapserPtr ptr)
{
    ptr->~Timelapser();
}

extern "C"
void Timelapser_initialize(
	struct TimelapserPtr ptr, struct PointArray corners,
	struct SizeArray sizes)
{
    ptr->initialize(corners, sizes);
}

extern "C"
void Timelapser_process(
	struct TimelapserPtr ptr, struct TensorWrapper img,
	struct TensorWrapper mask, struct PointWrapper tl)
{
    ptr->process(img.toMat(), mask.toMat(), tl);
}

//TimelapserCrop

extern "C"
void TimelapserCrop_initialize(
	struct TimelapserCropPtr ptr, struct PointArray corners,
	struct SizeArray sizes)
{
    ptr->initialize(corners, sizes);
}

//Features Finding and Images Matching

//MatchesInfo

extern "C"
struct MatchesInfoPtr MatchesInfo_ctor()
{
    return new cv::detail::MatchesInfo();
}

extern "C"
struct MatchesInfoPtr MatchesInfo_ctor2(
        struct MatchesInfoPtr other)
{
    return new cv::detail::MatchesInfo(*static_cast<cv::detail::MatchesInfo *>(other.ptr));
}

extern "C"
void MatchesInfo_dtor(
        struct MatchesInfoPtr ptr)
{
    std::cout<< "d_tor" << std::endl;
    delete static_cast<cv::detail::MatchesInfo *>(ptr.ptr);
}


//****************Features Finding and Images Matching************


//FeaturesFinder

extern "C"
void FeaturesFinder_dtor(
	struct FeaturesFinderPtr ptr)
{
    ptr->~FeaturesFinder();
    delete static_cast<cv::detail::FeaturesFinder *>(ptr.ptr);
}

extern "C"
void FeaturesFinder_collectGarbage(
	struct FeaturesFinderPtr ptr)
{
    ptr->collectGarbage();
}

extern "C"
struct ImageFeaturesPtr FeaturesFinder_call(
        struct FeaturesFinderPtr ptr, struct TensorWrapper image)
{
    detail::ImageFeatures *features = new cv::detail::ImageFeatures();
    ptr->operator()(image.toMat(), *features);
    return ImageFeaturesPtr(features);
}

extern "C"
struct ImageFeaturesPtr FeaturesFinder_call2(
        struct FeaturesFinderPtr ptr, struct TensorWrapper image,
        struct RectArray rois)
{
    detail::ImageFeatures *features = new cv::detail::ImageFeatures();
    ptr->operator()(image.toMat(), *features, rois);
    return ImageFeaturesPtr(features);
}

//OrbFeaturesFinder

extern "C"
struct OrbFeaturesFinderPtr OrbFeaturesFinder_ctor(
        struct SizeWrapper _grid_size, int nfeatures, float scaleFactor, int nlevels)
{
    return new cv::detail::OrbFeaturesFinder(_grid_size, nfeatures, scaleFactor, nlevels);
}

//SurfFeaturesFinder

extern "C"
struct SurfFeaturesFinderPtr SurfFeaturesFinder_ctor(
        double hess_thresh, int num_octaves, int num_layers, int num_octaves_descr, int num_layers_descr)
{
    return new cv::detail::SurfFeaturesFinder(hess_thresh, num_octaves, num_layers, num_octaves_descr, num_layers_descr);
}

//ImageFeatures

extern "C"
struct ImageFeaturesPtr ImageFeatures_ctor()
{
    return new cv::detail::ImageFeatures();
}

extern "C"
struct ImageFeaturesPtr ImageFeatures_dtor(
        struct ImageFeaturesPtr ptr)
{
    delete static_cast<cv::detail::ImageFeatures *>(ptr.ptr);
}

//FeaturesMatcher

//TODO need to do constructor protected

extern "C"
void FeaturesMatcher_dtor(
        struct FeaturesMatcherPtr ptr)
{
    ptr->~FeaturesMatcher();
    delete static_cast<cv::detail::FeaturesMatcher *>(ptr.ptr);
}

extern "C"
void FeaturesMatcher_collectGarbage(
        struct FeaturesMatcherPtr ptr)
{
    ptr->collectGarbage();
}

extern "C"
bool FeaturesMatcher_isThreadSafe(
        struct FeaturesMatcherPtr ptr)
{
    return ptr->isThreadSafe();
}

extern "C"
struct MatchesInfoPtr FeaturesMatcher_call(
        struct FeaturesMatcherPtr ptr, struct ImageFeaturesPtr features1,
        struct ImageFeaturesPtr features2)
{
    cv::detail::MatchesInfo *Mat_inf = new cv::detail::MatchesInfo();
    ptr->operator()(*static_cast<cv::detail::ImageFeatures *>(features1.ptr),
                    *static_cast<cv::detail::ImageFeatures *>(features2.ptr), *Mat_inf);
    return Mat_inf;
}

//BestOf2NearestMatcher

extern "C"
struct BestOf2NearestMatcherPtr BestOf2NearestMatcher_ctor(
        bool try_use_gpu, float match_conf,
        int num_matches_thresh1, int num_matches_thresh2)
{
    return new cv::detail::BestOf2NearestMatcher(try_use_gpu, match_conf, num_matches_thresh1, num_matches_thresh2);
}

extern "C"
void BestOf2NearestMatcher_collectGarbage(
        struct BestOf2NearestMatcherPtr ptr)
{
   ptr->collectGarbage();
}

//BestOf2NearestRangeMatcher

extern "C"
struct BestOf2NearestRangeMatcherPtr BestOf2NearestRangeMatcher_ctor(
        int range_width, bool try_use_gpu, float match_conf,
        int num_matches_thresh1, int num_matches_thresh2)
{
    return new cv::detail::BestOf2NearestRangeMatcher(range_width, try_use_gpu, match_conf,
                                                      num_matches_thresh1, num_matches_thresh2);
}

//**********************Rotation Estimation********************************

extern "C"
struct GraphPtrPlusIntArray detail_findMaxSpanningTree(
        int num_images, struct ClassArray pairwise_matches)
{
    GraphPtrPlusIntArray result;
    cv::detail::Graph *span_tree = new cv::detail::Graph();
    std::vector<int> centers;
    cv::detail::findMaxSpanningTree(num_images, pairwise_matches, *span_tree, centers);
    result.graph = GraphPtr(span_tree);
    result.array = IntArray(centers);
    return result;
}

extern "C"
struct IntArray detail_leaveBiggestComponent(
        struct ClassArray features, struct ClassArray pairwise_matches, float conf_threshold)
{
    std::vector<cv::detail::ImageFeatures> features_vec = features;
    std::vector<cv::detail::MatchesInfo> pairwise_matches_vec = pairwise_matches;

    return IntArray(
               cv::detail::leaveBiggestComponent(
                       features_vec,
                       pairwise_matches_vec,
                       conf_threshold));
}

extern "C"
struct StringWrapper detail_matchesGraphAsString(
        struct StringArray pathes, struct ClassArray pairwise_matches, float conf_threshold)
{
    struct StringWrapper result;
    std::vector<cv::String> pathes_vec = pathes;
    std::vector<cv::detail::MatchesInfo> pairwise_matches_vec = pairwise_matches;
    cv::String retval = cv::detail::matchesGraphAsString(pathes_vec, pairwise_matches_vec, conf_threshold);
    result.str = retval.c_str();
    return result;
}

extern "C"
void detail_waveCorrect(
        struct TensorArray rmats, int kind)
{
    cv::detail::WaveCorrectKind enum_kind;
    if(kind == 0) enum_kind = cv::detail::WAVE_CORRECT_HORIZ;
    else enum_kind = cv::detail::WAVE_CORRECT_VERT;
    std::vector<cv::Mat> rmats_vec = rmats.toMatList();
    cv::detail::waveCorrect(rmats_vec, enum_kind);
}

extern "C"
void Estimator_dtor(
        struct EstimatorPtr ptr)
{
    ptr->~Estimator();
    delete static_cast<cv::detail::Estimator *>(ptr.ptr);
}

extern "C"
struct BoolPlusClassArray Estimator_call(
        struct EstimatorPtr ptr, struct ClassArray features, struct ClassArray 	pairwise_matches)
{
    struct BoolPlusClassArray result;
    std::vector<cv::detail::CameraParams> cameras;
    std::vector<cv::detail::ImageFeatures> features_vec = features;
    std::vector<cv::detail::MatchesInfo> pairwise_matches_vec = pairwise_matches;

    result.val = ptr->operator()(features_vec, pairwise_matches_vec, cameras);
    new(&result.array) ClassArray(cameras);
    return result;
}

extern "C"
struct HomographyBasedEstimatorPtr HomographyBasedEstimator_ctor(
        bool is_focals_estimated)
{
    return new cv::detail::HomographyBasedEstimator(is_focals_estimated);
}

extern "C"
double BundleAdjusterBase_confThresh(
        struct BundleAdjusterBasePtr ptr)
{
    return ptr->confThresh();
}

extern "C"
struct TensorWrapper BundleAdjusterBase_refinementMask(
        struct BundleAdjusterBasePtr ptr)
{
    cv::Mat temp = ptr->refinementMask();
    return TensorWrapper(MatT(temp));
}

extern "C"
void BundleAdjusterBase_setConfThresh(
        struct BundleAdjusterBasePtr ptr, double conf_thresh)
{
    ptr->setConfThresh(conf_thresh);
}

extern "C"
void BundleAdjusterBase_setRefinementMask(
        struct BundleAdjusterBasePtr ptr, struct TensorWrapper mask)
{
    ptr->setRefinementMask(mask.toMat());
}

extern "C"
void BundleAdjusterBase_setTermCriteria(
        struct BundleAdjusterBasePtr ptr, struct TermCriteriaWrapper term_criteria)
{
    cv::TermCriteria term(term_criteria.type, term_criteria.maxCount, term_criteria.epsilon);
    ptr->setTermCriteria(term);
}

extern "C"
struct TermCriteriaWrapper BundleAdjusterBase_termCriteria(
        struct BundleAdjusterBasePtr ptr)
{
    struct TermCriteriaWrapper result;
    cv::TermCriteria term = ptr->termCriteria();
    result.type = term.type;
    result.epsilon = term.epsilon;
    result.maxCount = term.maxCount;
    return result;
}

//BundleAdjusterRay

extern "C"
struct BundleAdjusterRayPtr BundleAdjusterRay_ctor()
{
    return new cv::detail::BundleAdjusterRay();
}

//BundleAdjusterReproj

extern "C"
struct BundleAdjusterReprojPtr BundleAdjusterReproj_ctor()
{
    return new cv::detail::BundleAdjusterReproj();
}


//************************Autocalibration********************


extern "C"
struct TensorPlusBool detail_alibrateRotatingCamera(
        struct TensorArray Hs)
{
    struct TensorPlusBool result;
    cv::Mat K;
    result.val = cv::detail::calibrateRotatingCamera(Hs.toMatList(), K);
    new(&result.tensor) TensorWrapper(MatT(K));
    return result;
}

extern "C"
struct DoubleArray detail_estimateFocal(struct ClassArray features, struct ClassArray pairwise_matches)
{
    std::vector<double> focals;
    std::vector<cv::detail::ImageFeatures> features_vec = features;
    std::vector<cv::detail::MatchesInfo> pairwise_matches_vec = pairwise_matches;
    cv::detail::estimateFocal(features_vec, pairwise_matches_vec, focals);
    return DoubleArray(focals);
}

extern "C"
struct focalsFromHomographyRetval detail_focalsFromHomography(
        struct TensorWrapper H)
{
    struct focalsFromHomographyRetval result;
    double f0, f1;
    bool f0_ok, f1_ok;

    cv::detail::focalsFromHomography(H.toMat(), f0, f1, f0_ok, f1_ok);

    result.f0 = f0;
    result.f1 = f1;
    result.f0_ok = f0_ok;
    result.f1_ok = f1_ok;
    return result;
}


//***********************Images Warping***********************


//ProjectorBase

extern "C"
struct ProjectorBasePtr ProjectorBase_ctor()
{
    return new cv::detail::ProjectorBase;
}

extern "C"
void ProjectorBase_dtor(
        struct ProjectorBasePtr ptr)
{
    delete static_cast<cv::detail::ProjectorBase *>(ptr.ptr);
}

extern "C"
void ProjectorBase_setCameraParams(
        struct ProjectorBasePtr ptr, struct TensorWrapper K,
        struct TensorWrapper R, struct TensorWrapper T)
{
    ptr->setCameraParams(K.toMat(), R.toMat(), T.toMat());
}

////CompressedRectilinearPortraitProjector

extern "C"
struct CompressedRectilinearPortraitProjectorPtr CompressedRectilinearPortraitProjector_ctor()
{
    return new cv::detail::CompressedRectilinearPortraitProjector;
}

extern "C"
void CompressedRectilinearPortraitProjector_dtor(
        struct CompressedRectilinearPortraitProjectorPtr ptr)
{
    delete static_cast<cv::detail::CompressedRectilinearPortraitProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray CompressedRectilinearPortraitProjector_mapBackward(
        struct CompressedRectilinearPortraitProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray CompressedRectilinearPortraitProjector_mapForward(
        struct CompressedRectilinearPortraitProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//CompressedRectilinearProjector

extern "C"
struct CompressedRectilinearProjectorPtr CompressedRectilinearProjector_ctor()
{
    return new cv::detail::CompressedRectilinearProjector;
}

extern "C"
void CompressedRectilinearProjector_dtor(
        struct CompressedRectilinearProjectorPtr ptr)
{
    delete static_cast<cv::detail::CompressedRectilinearProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray CompressedRectilinearProjector_mapBackward(
        struct CompressedRectilinearProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray CompressedRectilinearProjector_mapForward(
        struct CompressedRectilinearProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//CylindricalPortraitProjector

extern "C"
struct CylindricalPortraitProjectorPtr CylindricalPortraitProjector_ctor()
{
    return new cv::detail::CylindricalPortraitProjector;
}

extern "C"
void CylindricalPortraitProjector_dtor(
        struct CylindricalPortraitProjectorPtr ptr)
{
    delete static_cast<cv::detail::CylindricalPortraitProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray CylindricalPortraitProjector_mapBackward(
        struct CylindricalPortraitProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray CylindricalPortraitProjector_mapForward(
        struct CylindricalPortraitProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//CylindricalProjector

extern "C"
struct CylindricalProjectorPtr CylindricalProjector_ctor()
{
    return new cv::detail::CylindricalProjector;
}

extern "C"
void CylindricalProjector_dtor(
        struct CylindricalProjectorPtr ptr)
{
    delete static_cast<cv::detail::CylindricalProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray CylindricalProjector_mapBackward(
        struct CylindricalProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray CylindricalProjector_mapForward(
        struct CylindricalProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//FisheyeProjector

extern "C"
void FisheyeProjector_dtor(
        struct FisheyeProjectorPtr ptr)
{
    delete static_cast<cv::detail::FisheyeProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray FisheyeProjector_mapBackward(
        struct FisheyeProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray FisheyeProjector_mapForward(
        struct FisheyeProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//MercatorProjector

extern "C"
void MercatorProjector_dtor(
        struct MercatorProjectorPtr ptr)
{
    delete static_cast<cv::detail::MercatorProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray MercatorProjector_mapBackward(
        struct MercatorProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray MercatorProjector_mapForward(
        struct MercatorProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//PaniniPortraitProjector

extern "C"
void PaniniPortraitProjector_dtor(
        struct PaniniPortraitProjectorPtr ptr)
{
    delete static_cast<cv::detail::PaniniPortraitProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray PaniniPortraitProjector_mapBackward(
        struct PaniniPortraitProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray PaniniPortraitProjector_mapForward(
        struct PaniniPortraitProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//PaniniProjector

extern "C"
void PaniniProjector_dtor(
        struct PaniniProjectorPtr ptr)
{
    delete static_cast<cv::detail::PaniniProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray PaniniProjector_mapBackward(
        struct PaniniProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray PaniniProjector_mapForward(
        struct PaniniProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//PlanePortraitProjector

extern "C"
void PlanePortraitProjector_dtor(
        struct PlanePortraitProjectorPtr ptr)
{
    delete static_cast<cv::detail::PlanePortraitProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray PlanePortraitProjector_mapBackward(
        struct PlanePortraitProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray PlanePortraitProjector_mapForward(
        struct PlanePortraitProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//PlaneProjector

extern "C"
void PlaneProjector_dtor(
        struct PlaneProjectorPtr ptr)
{
    delete static_cast<cv::detail::PlaneProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray PlaneProjector_mapBackward(
        struct PlaneProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray PlaneProjector_mapForward(
        struct PlaneProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//SphericalPortraitProjector

extern "C"
void SphericalPortraitProjector_dtor(
        struct SphericalPortraitProjectorPtr ptr)
{
    delete static_cast<cv::detail::SphericalPortraitProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray SphericalPortraitProjector_mapBackward(
        struct SphericalPortraitProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray SphericalPortraitProjector_mapForward(
        struct SphericalPortraitProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//SphericalProjector

extern "C"
void SphericalProjector_dtor(
        struct SphericalProjectorPtr ptr)
{
    delete static_cast<cv::detail::SphericalProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray SphericalProjector(
        struct SphericalProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray SphericalProjector_mapForward(
        struct SphericalProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//StereographicProjector

extern "C"
void StereographicProjector_dtor(
        struct StereographicProjectorPtr ptr)
{
    delete static_cast<cv::detail::StereographicProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray StereographicProjector_mapBackward(
        struct StereographicProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray StereographicProjector_mapForward(
        struct StereographicProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//TransverseMercatorProjector

extern "C"
void TransverseMercatorProjector_dtor(
        struct TransverseMercatorProjectorPtr ptr)
{
    delete static_cast<cv::detail::TransverseMercatorProjector *>(ptr.ptr);
}

extern "C"
struct FloatArray TransverseMercatorProjector_mapBackward(
        struct TransverseMercatorProjectorPtr ptr, float u, float v)
{
    std::vector<float> vec(2);
    ptr->mapBackward(u, v, vec[0], vec[0]);
    return FloatArray(vec);
}

extern "C"
struct FloatArray TransverseMercatorProjector_mapForward(
        struct TransverseMercatorProjectorPtr ptr, float x, float y)
{
    std::vector<float> vec(2);
    ptr->mapBackward(x, y, vec[0], vec[0]);
    return FloatArray(vec);
}

//RotationWarper

extern "C"
void RotationWarper_dtor(
        struct RotationWarperPtr ptr)
{
    ptr->~RotationWarper();
    delete static_cast<cv::detail::RotationWarper *>(ptr.ptr);
}

extern "C"
struct TensorArrayPlusRect RotationWarper_buildMaps(
        struct RotationWarperPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarper_getScale(
        struct RotationWarperPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarper_setScale(
        struct RotationWarperPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarper_warp(
        struct RotationWarperPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarper_warpBackward(
        struct RotationWarperPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarper_warpPoint(
        struct RotationWarperPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarper_warpRoi(
        struct RotationWarperPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_CompressedRectilinearPortraitProjector

extern "C"
struct RotationWarperBase_CompressedRectilinearPortraitProjectorPtr RotationWarperBase_CompressedRectilinearPortraitProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::CompressedRectilinearPortraitProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_CompressedRectilinearPortraitProjector_buildMaps(
        struct RotationWarperBase_CompressedRectilinearPortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_CompressedRectilinearPortraitProjector_getScale(
        struct RotationWarperBase_CompressedRectilinearPortraitProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_CompressedRectilinearPortraitProjector_setScale(
        struct RotationWarperBase_CompressedRectilinearPortraitProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_CompressedRectilinearPortraitProjector_warp(
        struct RotationWarperBase_CompressedRectilinearPortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_CompressedRectilinearPortraitProjector_warpBackward(
        struct RotationWarperBase_CompressedRectilinearPortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_CompressedRectilinearPortraitProjector_warpPoint(
        struct RotationWarperBase_CompressedRectilinearPortraitProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_CompressedRectilinearPortraitProjector_warpRoi(
        struct RotationWarperBase_CompressedRectilinearPortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_CompressedRectilinearProjector

extern "C"
struct RotationWarperBase_CompressedRectilinearProjectorPtr RotationWarperBase_CompressedRectilinearProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::CompressedRectilinearProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_CompressedRectilinearProjector_buildMaps(
        struct RotationWarperBase_CompressedRectilinearProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_CompressedRectilinearProjector_getScale(
        struct RotationWarperBase_CompressedRectilinearProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_CompressedRectilinearProjector_setScale(
        struct RotationWarperBase_CompressedRectilinearProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_CompressedRectilinearProjector_warp(
        struct RotationWarperBase_CompressedRectilinearProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_CompressedRectilinearProjector_warpBackward(
        struct RotationWarperBase_CompressedRectilinearProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_CompressedRectilinearProjector_warpPoint(
        struct RotationWarperBase_CompressedRectilinearProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_CompressedRectilinearProjector_warpRoi(
        struct RotationWarperBase_CompressedRectilinearProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}


//RotationWarperBase_CylindricalPortraitProjector

extern "C"
struct RotationWarperBase_CylindricalPortraitProjectorPtr RotationWarperBase_CylindricalPortraitProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::CylindricalPortraitProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_CylindricalPortraitProjector_buildMaps(
        struct RotationWarperBase_CylindricalPortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_CylindricalPortraitProjector_getScale(
        struct RotationWarperBase_CylindricalPortraitProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_CylindricalPortraitProjector_setScale(
        struct RotationWarperBase_CylindricalPortraitProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_CylindricalPortraitProjector_warp(
        struct RotationWarperBase_CylindricalPortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_CylindricalPortraitProjector_warpBackward(
        struct RotationWarperBase_CylindricalPortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_CylindricalPortraitProjector_warpPoint(
        struct RotationWarperBase_CylindricalPortraitProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_CylindricalPortraitProjector_warpRoi(
        struct RotationWarperBase_CylindricalPortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_CylindricalProjector

extern "C"
struct RotationWarperBase_CylindricalProjectorPtr RotationWarperBase_CylindricalProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::CylindricalProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_CylindricalProjector_buildMaps(
        struct RotationWarperBase_CylindricalProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_CylindricalProjector_getScale(
        struct RotationWarperBase_CylindricalProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_CylindricalProjector_setScale(
        struct RotationWarperBase_CylindricalProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_CylindricalProjector_warp(
        struct RotationWarperBase_CylindricalProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_CylindricalProjector_warpBackward(
        struct RotationWarperBase_CylindricalProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_CylindricalProjector_warpPoint(
        struct RotationWarperBase_CylindricalProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_CylindricalProjector_warpRoi(
        struct RotationWarperBase_CylindricalProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_FisheyeProjector

extern "C"
struct RotationWarperBase_FisheyeProjectorPtr RotationWarperBase_FisheyeProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::FisheyeProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_FisheyeProjector_buildMaps(
        struct RotationWarperBase_FisheyeProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_FisheyeProjector_getScale(
        struct RotationWarperBase_FisheyeProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_FisheyeProjector_setScale(
        struct RotationWarperBase_FisheyeProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_FisheyeProjector_warp(
        struct RotationWarperBase_FisheyeProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_FisheyeProjector_warpBackward(
        struct RotationWarperBase_FisheyeProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_FisheyeProjector_warpPoint(
        struct RotationWarperBase_FisheyeProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_FisheyeProjector_warpRoi(
        struct RotationWarperBase_FisheyeProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_MercatorProjector

extern "C"
struct RotationWarperBase_MercatorProjectorPtr RotationWarperBase_MercatorProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::MercatorProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_MercatorProjector_buildMaps(
        struct RotationWarperBase_MercatorProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_MercatorProjector_getScale(
        struct RotationWarperBase_MercatorProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_MercatorProjector_setScale(
        struct RotationWarperBase_MercatorProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_MercatorProjector_warp(
        struct RotationWarperBase_MercatorProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_MercatorProjector_warpBackward(
        struct RotationWarperBase_MercatorProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_MercatorProjector_warpPoint(
        struct RotationWarperBase_MercatorProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_MercatorProjector_warpRoi(
        struct RotationWarperBase_MercatorProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_PaniniPortraitProjector

extern "C"
struct RotationWarperBase_PaniniPortraitProjectorPtr RotationWarperBase_PaniniPortraitProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::PaniniPortraitProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_PaniniPortraitProjector_buildMaps(
        struct RotationWarperBase_PaniniPortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_PaniniPortraitProjector_getScale(
        struct RotationWarperBase_PaniniPortraitProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_PaniniPortraitProjector_setScale(
        struct RotationWarperBase_PaniniPortraitProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_PaniniPortraitProjector_warp(
        struct RotationWarperBase_PaniniPortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_PaniniPortraitProjector_warpBackward(
        struct RotationWarperBase_PaniniPortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_PaniniPortraitProjector_warpPoint(
        struct RotationWarperBase_PaniniPortraitProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_PaniniPortraitProjector_warpRoi(
        struct RotationWarperBase_PaniniPortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_PaniniProjector

extern "C"
struct RotationWarperBase_PaniniProjectorPtr RotationWarperBase_PaniniProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::PaniniProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_PaniniProjector_buildMaps(
        struct RotationWarperBase_PaniniProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_PaniniProjector_getScale(
        struct RotationWarperBase_PaniniProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_PaniniProjector_setScale(
        struct RotationWarperBase_PaniniProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_PaniniProjector_warp(
        struct RotationWarperBase_PaniniProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_PaniniProjector_warpBackward(
        struct RotationWarperBase_PaniniProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_PaniniProjector_warpPoint(
        struct RotationWarperBase_PaniniProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_PaniniProjector_warpRoi(
        struct RotationWarperBase_PaniniProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_PlanePortraitProjector

extern "C"
struct RotationWarperBase_PlanePortraitProjectorPtr RotationWarperBase_PlanePortraitProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::PlanePortraitProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_PlanePortraitProjector_buildMaps(
        struct RotationWarperBase_PlanePortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_PlanePortraitProjector_getScale(
        struct RotationWarperBase_PlanePortraitProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_PlanePortraitProjector_setScale(
        struct RotationWarperBase_PlanePortraitProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_PlanePortraitProjector_warp(
        struct RotationWarperBase_PlanePortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_PlanePortraitProjector_warpBackward(
        struct RotationWarperBase_PlanePortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_PlanePortraitProjector_warpPoint(
        struct RotationWarperBase_PlanePortraitProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_PlanePortraitProjector_warpRoi(
        struct RotationWarperBase_PlanePortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_PlaneProjector

extern "C"
struct RotationWarperBase_PlaneProjectorPtr RotationWarperBase_PlaneProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::PlaneProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_PlaneProjector_buildMaps(
        struct RotationWarperBase_PlaneProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_PlaneProjector_getScale(
        struct RotationWarperBase_PlaneProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_PlaneProjector_setScale(
        struct RotationWarperBase_PlaneProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_PlaneProjector_warp(
        struct RotationWarperBase_PlaneProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_PlaneProjector_warpBackward(
        struct RotationWarperBase_PlaneProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_PlaneProjector_warpPoint(
        struct RotationWarperBase_PlaneProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_PlaneProjector_warpRoi(
        struct RotationWarperBase_PlaneProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_SphericalPortraitProjector

extern "C"
struct RotationWarperBase_SphericalPortraitProjectorPtr RotationWarperBase_SphericalPortraitProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::SphericalPortraitProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_SphericalPortraitProjector_buildMaps(
        struct RotationWarperBase_SphericalPortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_SphericalPortraitProjector_getScale(
        struct RotationWarperBase_SphericalPortraitProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_SphericalPortraitProjector_setScale(
        struct RotationWarperBase_SphericalPortraitProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_SphericalPortraitProjector_warp(
        struct RotationWarperBase_SphericalPortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_SphericalPortraitProjector_warpBackward(
        struct RotationWarperBase_SphericalPortraitProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_SphericalPortraitProjector_warpPoint(
        struct RotationWarperBase_SphericalPortraitProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_SphericalPortraitProjector_warpRoi(
        struct RotationWarperBase_SphericalPortraitProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_SphericalProjector

extern "C"
struct RotationWarperBase_SphericalProjectorPtr RotationWarperBase_SphericalProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::SphericalProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_SphericalProjector_buildMaps(
        struct RotationWarperBase_SphericalProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_SphericalProjector_getScale(
        struct RotationWarperBase_SphericalProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_SphericalProjector_setScale(
        struct RotationWarperBase_SphericalProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_SphericalProjector_warp(
        struct RotationWarperBase_SphericalProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_SphericalProjector_warpBackward(
        struct RotationWarperBase_SphericalProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_SphericalProjector_warpPoint(
        struct RotationWarperBase_SphericalProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_SphericalProjector_warpRoi(
        struct RotationWarperBase_SphericalProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_StereographicProjector

extern "C"
struct RotationWarperBase_StereographicProjectorPtr RotationWarperBase_StereographicProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::StereographicProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_StereographicProjector_buildMaps(
        struct RotationWarperBase_StereographicProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_StereographicProjector_getScale(
        struct RotationWarperBase_StereographicProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_StereographicProjector_setScale(
        struct RotationWarperBase_StereographicProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_StereographicProjector_warp(
        struct RotationWarperBase_StereographicProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_StereographicProjector_warpBackward(
        struct RotationWarperBase_StereographicProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_StereographicProjector_warpPoint(
        struct RotationWarperBase_StereographicProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_StereographicProjector_warpRoi(
        struct RotationWarperBase_StereographicProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//RotationWarperBase_TransverseMercatorProjector

extern "C"
struct RotationWarperBase_TransverseMercatorProjectorPtr RotationWarperBase_TransverseMercatorProjector_ctor()
{
    return new cv::detail::RotationWarperBase<cv::detail::TransverseMercatorProjector>();
}

extern "C"
struct TensorArrayPlusRect RotationWarperBase_TransverseMercatorProjector_buildMaps(
        struct RotationWarperBase_TransverseMercatorProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
float RotationWarperBase_TransverseMercatorProjector_getScale(
        struct RotationWarperBase_TransverseMercatorProjectorPtr ptr)
{
    return ptr->getScale();
}

extern "C"
void RotationWarperBase_TransverseMercatorProjector_setScale(
        struct RotationWarperBase_TransverseMercatorProjectorPtr ptr, float val)
{
    ptr->setScale(val);
}

extern "C"
struct TensorPlusPoint RotationWarperBase_TransverseMercatorProjector_warp(
        struct RotationWarperBase_TransverseMercatorProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorWrapper RotationWarperBase_TransverseMercatorProjector_warpBackward(
        struct RotationWarperBase_TransverseMercatorProjectorPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct SizeWrapper dst_size,
        struct TensorWrapper dst)
{
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    ptr->warpBackward(src.toMat(), K.toMat(), R.toMat(), interp_mode,
                      border_mode, dst_size, dst_mat);
    return TensorWrapper(dst_mat);
}

extern "C"
struct Point2fWrapper RotationWarperBase_TransverseMercatorProjector_warpPoint(
        struct RotationWarperBase_TransverseMercatorProjectorPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper RotationWarperBase_TransverseMercatorProjector_warpRoi(
        struct RotationWarperBase_TransverseMercatorProjectorPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//WarperCreator

extern "C"
void WarperCreator_dtor(
        struct WarperCreatorPtr ptr)
{
    ptr->~WarperCreator();
    delete static_cast<cv::WarperCreator *>(ptr.ptr);
}

extern "C"
struct RotationWarperPtr WarperCreator_create(
        struct WarperCreatorPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//CompressedRectilinearPortraitWarper

extern "C"
struct CompressedRectilinearPortraitWarperPtr CompressedRectilinearPortraitWarper_ctor(
        float A, float B)
{
    return new cv::CompressedRectilinearPortraitWarper(A, B);
}

extern "C"
struct RotationWarperPtr CompressedRectilinearPortraitWarper_create(
        struct CompressedRectilinearPortraitWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//CompressedRectilinearWarper

extern "C"
struct CompressedRectilinearWarperPtr CompressedRectilinearWarper_ctor(
        float A, float B)
{
    return new cv::CompressedRectilinearWarper(A, B);
}

extern "C"
struct RotationWarperPtr CompressedRectilinearWarper_create(
        struct CompressedRectilinearWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//CylindricalWarper

extern "C"
struct CylindricalWarperPtr CylindricalWarper_ctor()
{
    return new cv::CylindricalWarper();
}

extern "C"
struct RotationWarperPtr CylindricalWarper_create(
        struct CylindricalWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//FisheyeWarper

extern "C"
struct FisheyeWarperPtr FisheyeWarper_ctor()
{
    return new cv::FisheyeWarper();
}

extern "C"
struct RotationWarperPtr FisheyeWarper_create(
        struct FisheyeWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//MercatorWarper

extern "C"
struct MercatorWarperPtr MercatorWarper_ctor()
{
    return new cv::MercatorWarper();
}

extern "C"
struct RotationWarperPtr MercatorWarper_create(
        struct MercatorWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//PaniniPortraitWarper

extern "C"
struct PaniniPortraitWarperPtr PaniniPortraitWarper_ctor(
        float A, float B)
{
    return new cv::PaniniPortraitWarper(A, B);
}

extern "C"
struct RotationWarperPtr PaniniPortraitWarper_create(
        struct PaniniPortraitWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//PaniniWarper

extern "C"
struct PaniniWarperPtr PaniniWarper_ctor(
        float A, float B)
{
    return new cv::PaniniWarper(A, B);
}

extern "C"
struct RotationWarperPtr PaniniWarper_create(
        struct PaniniWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//PlaneWarper

extern "C"
struct PlaneWarperPtr PlaneWarper_ctor()
{
    return new cv::PlaneWarper();
}

extern "C"
struct RotationWarperPtr PlaneWarper_create(
        struct PlaneWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//SphericalWarper

extern "C"
struct SphericalWarperPtr SphericalWarper_ctor()
{
    return new cv::SphericalWarper();
}

extern "C"
struct RotationWarperPtr SphericalWarper_create(
        struct SphericalWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//StereographicWarper

extern "C"
struct StereographicWarperPtr StereographicWarper_ctor()
{
    return new cv::StereographicWarper();
}

extern "C"
struct RotationWarperPtr StereographicWarper_create(
        struct StereographicWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//TransverseMercatorWarper

extern "C"
struct TransverseMercatorWarperPtr TransverseMercatorWarper_ctor()
{
    return new cv::TransverseMercatorWarper();
}

extern "C"
struct RotationWarperPtr TransverseMercatorWarper_create(
        struct TransverseMercatorWarperPtr ptr, float scale)
{
    return rescueObjectFromPtr(ptr->create(scale));
}

//detail_CompressedRectilinearPortraitWarper

extern "C"
struct detail_CompressedRectilinearPortraitWarperPtr detail_CompressedRectilinearPortraitWarper_ctor(
        float scale, float A, float B)
{
    return new cv::detail::CompressedRectilinearPortraitWarper(scale, A, B);
}

//detail_CompressedRectilinearWarper

extern "C"
struct detail_CompressedRectilinearWarperPtr detail_CompressedRectilinearWarper_ctor(
        float scale, float A, float B)
{
    return new cv::detail::CompressedRectilinearWarper(scale, A, B);
}

//detail_CylindricalPortraitWarper

extern "C"
struct detail_CylindricalPortraitWarperPtr detail_CylindricalPortraitWarper_ctor(
        float scale)
{
    return new cv::detail::CylindricalPortraitWarper(scale);
}

//detail_CylindricalWarper

extern "C"
struct detail_CylindricalWarperPtr detail_CylindricalWarper_ctor(
        float scale)
{
    return new cv::detail::CylindricalWarper(scale);
}

extern "C"
struct TensorArrayPlusRect detail_CylindricalWarper_buildMaps(
        struct detail_CylindricalWarperPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    TensorArrayPlusRect result;
    std::vector<MatT> vec(2);
    if(!xmap.isNull()) vec[0] = xmap.toMatT();
    if(!ymap.isNull()) vec[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMat(), vec[0], vec[1]);
    new(&result.tensors) TensorArray(vec);
    return result;
}

extern "C"
struct TensorPlusPoint detail_CylindricalWarper_warp(
        struct detail_CylindricalWarperPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct TensorWrapper dst)
{
    TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(), interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

//detail_CylindricalWarperGpu

extern "C"
struct detail_CylindricalWarperGpuPtr detail_CylindricalWarperGpu_ctor(
        float scale)
{
    return new cv::detail::CylindricalWarperGpu(scale);
}

extern "C"
struct TensorArrayPlusRect detail_CylindricalWarperGpu_buildMaps(
        struct detail_CylindricalWarperGpuPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    TensorArrayPlusRect result;
    std::vector<MatT> vec(2);
    if(!xmap.isNull()) vec[0] = xmap.toMatT();
    if(!ymap.isNull()) vec[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMat(), vec[0], vec[1]);
    new(&result.tensors) TensorArray(vec);
    return result;
}

extern "C"
struct TensorPlusPoint detail_CylindricalWarperGpu_warp(
        struct detail_CylindricalWarperGpuPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct TensorWrapper dst)
{
    TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(), interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

//detail_FisheyeWarper

extern "C"
struct detail_FisheyeWarperPtr detail_FisheyeWarper_ctor(
        float scale)
{
    return new cv::detail::FisheyeWarper(scale);
}

//detail_MercatorWarper

extern "C"
struct detail_MercatorWarperPtr detail_MercatorWarper_ctor(
        float scale)
{
    return new cv::detail::MercatorWarper(scale);
}

//detail_PaniniPortraitWarper

extern "C"
struct detail_PaniniPortraitWarperPtr detail_PaniniPortraitWarper_ctor(
        float scale, float A, float B)
{
    return new cv::detail::PaniniPortraitWarper(scale, A, B);
}

//detail_PaniniWarper

extern "C"
struct detail_PaniniWarperPtr detail_PaniniWarper_ctor(
        float scale, float A, float B)
{
    return new cv::detail::PaniniWarper(scale, A, B);
}

//detail_PlanePortraitWarper

extern "C"
struct detail_PlanePortraitWarperPtr detail_PlanePortraitWarper_ctor(
        float scale)
{
    return new cv::detail::PlanePortraitWarper(scale);
}

//detail_PlaneWarper

extern "C"
struct detail_PlaneWarperPtr detail_PlaneWarper_ctor(
        float scale)
{
    return new cv::detail::PlaneWarper(scale);
}

extern "C"
struct TensorArrayPlusRect detail_PlaneWarper_buildMaps2(
        struct detail_PlaneWarperPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    TensorArrayPlusRect result;
    std::vector<MatT> vec(2);
    if(!xmap.isNull()) vec[0] = xmap.toMatT();
    if(!ymap.isNull()) vec[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMat(), vec[0], vec[1]);
    new(&result.tensors) TensorArray(vec);
    return result;
}

extern "C"
struct TensorArrayPlusRect detail_PlaneWarper_buildMaps(
        struct detail_PlaneWarperPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper T, struct TensorWrapper xmap,
        struct TensorWrapper ymap)
{
    struct TensorArrayPlusRect result;
    std::vector<MatT> map_mat(2);
    if(!xmap.isNull()) map_mat[0] = xmap.toMatT();
    if(!ymap.isNull()) map_mat[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMatT(), T.toMat(), map_mat[0], map_mat[1]);
    new(&result.tensors) TensorArray(map_mat);

    return result;
}

extern "C"
struct TensorPlusPoint detail_PlaneWarper_warp(
        struct detail_PlaneWarperPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper T, int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(), T.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct TensorPlusPoint detail_PlaneWarper_warp2(
        struct detail_PlaneWarperPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,
        int interp_mode, int border_mode, struct TensorWrapper dst)
{
    TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(), interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

extern "C"
struct Point2fWrapper detail_PlaneWarper_warpPoint(
        struct detail_PlaneWarperPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R, struct TensorWrapper T)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat(), T.toMat());
}

extern "C"
struct Point2fWrapper detail_PlaneWarper_warpPoint2(
        struct detail_PlaneWarperPtr ptr, struct Point2fWrapper pt,
        struct TensorWrapper K, struct TensorWrapper R)
{
    cv::Point2f pt2 = pt;
    ptr->warpPoint(pt2, K.toMat(), R.toMat());
}

extern "C"
struct RectWrapper detail_PlaneWarper_warpRoi(
        struct detail_PlaneWarperPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R, struct TensorWrapper T)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat(), T.toMat());
}

extern "C"
struct RectWrapper detail_PlaneWarper_warpRoi2(
        struct detail_PlaneWarperPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R)
{
    ptr->warpRoi(src_size, K.toMat(), R.toMat());
}

//detail_SphericalPortraitWarper

extern "C"
struct detail_SphericalPortraitWarperPtr detail_SphericalPortraitWarper_ctor(
        float scale)
{
    return new cv::detail::SphericalPortraitWarper(scale);
}

//detail_SphericalWarper

extern "C"
struct detail_SphericalWarperPtr detail_SphericalWarper_ctor(
        float scale)
{
    return new cv::detail::SphericalWarper(scale);
}

extern "C"
struct TensorArrayPlusRect detail_SphericalWarper_buildMaps(
        struct detail_SphericalWarperPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    TensorArrayPlusRect result;
    std::vector<MatT> vec(2);
    if(!xmap.isNull()) vec[0] = xmap.toMatT();
    if(!ymap.isNull()) vec[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMat(), vec[0], vec[1]);
    new(&result.tensors) TensorArray(vec);
    return result;
}

extern "C"
struct TensorPlusPoint detail_SphericalWarper_warp(
        struct detail_SphericalWarperPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if(!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}

//detail_SphericalWarperGpu

extern "C"
struct detail_SphericalWarperGpuPtr detail_SphericalWarperGpu_ctor(
        float scale)
{
    return new cv::detail::SphericalWarperGpu(scale);
}

extern "C"
struct TensorArrayPlusRect detail_SphericalWarperGpu_buildMaps(
        struct detail_SphericalWarperGpuPtr ptr, struct SizeWrapper src_size,
        struct TensorWrapper K, struct TensorWrapper R,
        struct TensorWrapper xmap, struct TensorWrapper ymap)
{
    TensorArrayPlusRect result;
    std::vector<MatT> vec(2);
    if(!xmap.isNull()) vec[0] = xmap.toMatT();
    if(!ymap.isNull()) vec[1] = ymap.toMatT();

    result.rect = ptr->buildMaps(src_size, K.toMat(), R.toMat(), vec[0], vec[1]);
    new(&result.tensors) TensorArray(vec);
    return result;
}

extern "C"
struct TensorPlusPoint detail_SphericalWarperGpu_warp(
        struct detail_SphericalWarperGpuPtr ptr, struct TensorWrapper src,
        struct TensorWrapper K, struct TensorWrapper R,	int interp_mode,
        int border_mode, struct TensorWrapper dst)
{
    struct TensorPlusPoint result;
    MatT dst_mat;
    if (!dst.isNull()) dst_mat = dst.toMatT();
    result.point = ptr->warp(src.toMat(), K.toMat(), R.toMat(),
                             interp_mode, border_mode, dst_mat);
    new(&result.tensor) TensorWrapper(dst_mat);
    return result;
}




//*************************test***************************
extern "C"
struct StringWrapper test(struct StringArray str){

    std::cout << "__C++__\n";

    std::vector<cv::String> vec = str;

    for(int i = 0; i < vec.size(); i++){
        std::cout << vec[i];
    }
    std::cout << std::endl;

    struct  StringWrapper retval;

    retval.str = static_cast<char *>(malloc(sizeof(char) * 3));
    retval.str = "aa";
    return retval;
 }