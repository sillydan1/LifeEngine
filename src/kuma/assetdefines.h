#ifndef KUMA_ASSETDEFINES
#define KUMA_ASSETDEFINES
#include <string>
// import steps: 
// http://assimp.sourceforge.net/lib_html/postprocess_8h.html
#include <assimp/postprocess.h>

/* default import setting */
#define KUMA_ASSETIMPORTSETTING_DEFAULT aiProcessPreset_TargetRealtime_Fast
#define KUMA_ASSETIMPORTSETTING_LEFTHANDED aiProcess_ConvertToLeftHanded
#define KUMA_ASSETIMPORTSETTING_FAST aiProcessPreset_TargetRealtime_MaxQuality

/* specific post import steps */
#define KUMA_CALCTANGENTSPACE aiProcess_CalcTangentSpace
#define KUMA_JOINIDENTICALVERTICES aiProcess_JoinIdenticalVertices
#define KUMA_MAKELEFTHANDED aiProcess_MakeLeftHanded
#define KUMA_TRIANGULATE aiProcess_Triangulate
#define KUMA_REMOVECOMPONENT aiProcess_RemoveComponent
#define KUMA_GENNORMALS aiProcess_GenNormals
#define KUMA_GENSMOOTHNORMALS aiProcess_GenSmoothNormals
#define KUMA_SPLITLARGEMESHES aiProcess_SplitLargeMeshes
#define KUMA_PRETRANSFORMVERTICES aiProcess_PreTransformVertices
#define KUMA_LIMITBONEWEIGHTS aiProcess_LimitBoneWeights
#define KUMA_VALIDATEDATASTRUCTURE aiProcess_ValidateDataStructure
#define KUMA_IMPROVECACHELOCALITY aiProcess_ImproveCacheLocality
#define KUMA_REMOVEREDUNDANTMATERIALS aiProcess_RemoveRedundantMaterials
#define KUMA_FIXINFACINGNORMALS aiProcess_FixInfacingNormals
#define KUMA_SORTBYPTYPE aiProcess_SortByPType
#define KUMA_FINDDEGENERATES aiProcess_FindDegenerates
#define KUMA_FINDINVALIDDATA aiProcess_FindInvalidData
#define KUMA_GENUVCOORDS aiProcess_GenUVCoords
#define KUMA_TRANSFORMUVCOORDS aiProcess_TransformUVCoords
#define KUMA_FINDINSTANCES aiProcess_FindInstances
#define KUMA_OPTIMIZEMESHES aiProcess_OptimizeMeshes
#define KUMA_OPTIMIZEGRAPH aiProcess_OptimizeGraph
#define KUMA_FLIPUVS aiProcess_FlipUVs
#define KUMA_FLIPWINDINGORDER aiProcess_FlipWindingOrder
#define KUMA_SPLITBYBONECOUNT aiProcess_SplitByBoneCount
#define KUMA_DEBONE aiProcess_Debone

/* Editor Icons (This is technically not KUMA, but it's rendering related) */
#define LIFE_ICON_LIGHT_GENERIC 	0x00
#define LIFE_ICON_LIGHT_SPOT 		0x01
#define LIFE_ICON_LIGHT_DIRECTIONAL 0x02
#define LIFE_ICON_LIGHT_POINT		0x03
typedef unsigned int LifeIconEnum;
#endif // KUMA_ASSETDEFINES
