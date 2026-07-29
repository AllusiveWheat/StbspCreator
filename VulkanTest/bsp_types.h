#pragma once 

#define GLM_FORCE_SSE2
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

typedef glm::vec2 Vector2;
typedef glm::vec3 Vector;
typedef glm::vec4 Vector4;

typedef glm::vec3 Vector3;

typedef unsigned char BYTE;
typedef unsigned short WORD;
#ifndef _MSC_VER
typedef unsigned int DWORD;
#endif
enum LumpIds {
    ENTITIES = 0x0000,
    BRUSH_PLANES = 0x0001,
    TEXTURE_DATA = 0x0002,
    VERTICES = 0x0003,
    LIGHTPROBE_PARENT_INFOS = 0x0004,
    SHADOW_ENVIRONMENTS = 0x0005,
    LIGHTPROBE_BSP_NODES = 0x0006,
    LIGHTPROBE_BSP_REF_IDS = 0x0007, // indexes ? (Mod_LoadLightProbeBSPRefIdxs)
    UNUSED_8 = 0x0008,
    UNUSED_9 = 0x0009,
    UNUSED_10 = 0x000A,
    UNUSED_11 = 0x000B,
    UNUSED_12 = 0x000C,
    UNUSED_13 = 0x000D,
    MODELS = 0x000E,
    UNUSED_15 = 0x000F,
    UNUSED_16 = 0x0010,
    UNUSED_17 = 0x0011,
    UNUSED_18 = 0x0012,
    UNUSED_19 = 0x0013,
    UNUSED_20 = 0x0014,
    UNUSED_21 = 0x0015,
    UNUSED_22 = 0x0016,
    UNUSED_23 = 0x0017,
    ENTITY_PARTITIONS = 0x0018,
    UNUSED_25 = 0x0019,
    UNUSED_26 = 0x001A,
    UNUSED_27 = 0x001B,
    UNUSED_28 = 0x001C,
    PHYSICS_COLLIDE = 0x001D,
    VERTEX_NORMALS = 0x001E,
    UNUSED_31 = 0x001F,
    UNUSED_32 = 0x0020,
    UNUSED_33 = 0x0021,
    UNUSED_34 = 0x0022,
    GAME_LUMP = 0x0023,
    LEAF_WATER_DATA = 0x0024,
    UNUSED_37 = 0x0025,
    UNUSED_38 = 0x0026,
    UNUSED_39 = 0x0027,
    PAKFILE = 0x0028,  // zip file, contains cubemaps
    UNUSED_41 = 0x0029,
    CUBEMAPS = 0x002A,
    TEXTURE_DATA_STRING_DATA = 0x002B,
    TEXTURE_DATA_STRING_TABLE = 0x002C,
    UNUSED_45 = 0x002D,
    UNUSED_46 = 0x002E,
    UNUSED_47 = 0x002F,
    UNUSED_48 = 0x0030,
    UNUSED_49 = 0x0031,
    UNUSED_50 = 0x0032,
    UNUSED_51 = 0x0033,
    UNUSED_52 = 0x0034,
    UNUSED_53 = 0x0035,
    WORLD_LIGHTS = 0x0036,  // versions 1 - 3 supported(0 might cause a crash, idk)
    WORLD_LIGHT_PARENT_INFOS = 0x0037,
    UNUSED_56 = 0x0038,
    UNUSED_57 = 0x0039,
    UNUSED_58 = 0x003A,
    UNUSED_59 = 0x003B,
    UNUSED_60 = 0x003C,
    UNUSED_61 = 0x003D,
    UNUSED_62 = 0x003E,
    UNUSED_63 = 0x003F,
    UNUSED_64 = 0x0040,
    UNUSED_65 = 0x0041,
    TRICOLL_TRIANGLES = 0x0042,
    UNUSED_67 = 0x0043,
    TRICOLL_NODES = 0x0044,
    TRICOLL_HEADERS = 0x0045,
    UNUSED_70 = 0x0046,
    VERTEX_UNLIT = 0x0047,        // VERTEX_RESERVED_0
    VERTEX_LIT_FLAT = 0x0048,     // VERTEX_RESERVED_1
    VERTEX_LIT_BUMP = 0x0049,     // VERTEX_RESERVED_2
    VERTEX_UNLIT_TS = 0x004A,     // VERTEX_RESERVED_3
    VERTEX_BLINN_PHONG = 0x004B,  // VERTEX_RESERVED_4
    VERTEX_RESERVED_5 = 0x004C,
    VERTEX_RESERVED_6 = 0x004D,
    VERTEX_RESERVED_7 = 0x004E,
    MESH_INDICES = 0x004F,
    MESHES = 0x0050,
    MESH_BOUNDS = 0x0051,
    MATERIAL_SORTS = 0x0052,
    LIGHTMAP_HEADERS = 0x0053,
    UNUSED_84 = 0x0054,
    CM_GRID = 0x0055,
    CM_GRID_CELLS = 0x0056,
    CM_GEO_SETS = 0x0057,
    CM_GEO_SET_BOUNDS = 0x0058,
    CM_PRIMITIVES = 0x0059,
    CM_PRIMITIVE_BOUNDS = 0x005A,
    CM_UNIQUE_CONTENTS = 0x005B,
    CM_BRUSHES = 0x005C,
    CM_BRUSH_SIDE_PLANE_OFFSETS = 0x005D,
    CM_BRUSH_SIDE_PROPERTIES = 0x005E,
    CM_BRUSH_SIDE_TEXTURE_VECTORS = 0x005F,
    TRICOLL_BEVEL_STARTS = 0x0060,
    TRICOLL_BEVEL_INDICES = 0x0061,
    LIGHTMAP_DATA_SKY = 0x0062,
    CSM_AABB_NODES = 0x0063,
    CSM_OBJ_REFERENCES = 0x0064,
    LIGHTPROBES = 0x0065,
    STATIC_PROP_LIGHTPROBE_INDICES = 0x0066,
    LIGHTPROBE_TREE = 0x0067,
    LIGHTPROBE_REFERENCES = 0x0068,
    LIGHTMAP_DATA_REAL_TIME_LIGHTS = 0x0069,
    CELL_BSP_NODES = 0x006A,
    CELLS = 0x006B,
    PORTALS = 0x006C,
    PORTAL_VERTICES = 0x006D,
    PORTAL_EDGES = 0x006E,
    PORTAL_VERTEX_EDGES = 0x006F,
    PORTAL_VERTEX_REFERENCES = 0x0070,
    PORTAL_EDGE_REFERENCES = 0x0071,
    PORTAL_EDGE_INTERSECT_AT_EDGE = 0x0072,
    PORTAL_EDGE_INTERSECT_AT_VERTEX = 0x0073,
    PORTAL_EDGE_INTERSECT_HEADER = 0x0074,
    OCCLUSION_MESH_VERTICES = 0x0075,
    OCCLUSION_MESH_INDICES = 0x0076,
    CELL_AABB_NODES = 0x0077,
    OBJ_REFERENCES = 0x0078,
    OBJ_REFERENCE_BOUNDS = 0x0079,
    LIGHTMAP_DATA_RTL_PAGE = 0x007A,
    LEVEL_INFO = 0x007B,
    SHADOW_MESH_OPAQUE_VERTICES = 0x007C,
    SHADOW_MESH_ALPHA_VERTICES = 0x007D,
    SHADOW_MESH_INDICES = 0x007E,
    SHADOW_MESHES = 0x007F
};

enum Contents {

    // r1/scripts/vscripts/_consts.nut:1159
    EMPTY = 0x00,
    SOLID = 0x01,
    WINDOW = 0x02,  // bulletproof glass etc. (transparent but solid)
    AUX = 0x04,  // unused ?
    GRATE = 0x08,  // allows bullets & vis
    SLIME = 0x10,
    WATER = 0x20,
    WINDOW_NO_COLLIDE = 0x40,
    ISOPAQUE = 0x80,  // blocks AI Line Of Sight, may be non - solid
    TEST_FOG_VOLUME = 0x100,  // cannot be seen through, but may be non - solid
    UNUSED_1 = 0x200,
    BLOCK_LIGHT = 0x400,
    TEAM_1 = 0x800,
    TEAM_2 = 0x1000,
    IGNORE_NODRAW_OPAQUE = 0x2000,  // ignore opaque if Surface.NO_DRAW
    MOVEABLE = 0x4000,
    PLAYER_CLIP = 0x10000,  // blocks human players
    MONSTER_CLIP = 0x20000,
    BRUSH_PAINT = 0x40000,
    BLOCK_LOS = 0x80000,  // block AI line of sight
    NO_CLIMB = 0x100000,
    TITAN_CLIP = 0x200000,  // blocks titan players
    BULLET_CLIP = 0x400000,
    UNUSED_5 = 0x800000,
    ORIGIN = 0x1000000,  // removed before bsping an entity
    MONSTER = 0x2000000, // should never be on a brush, only in game
    DEBRIS = 0x4000000,
    DETAIL = 0x8000000, // brushes to be added after vis leafs
    TRANSLUCENT = 0x10000000,  // auto set if any surface has trans
    LADDER = 0x20000000,
    HITBOX = 0x40000000  // use accurate hitboxes on trace
};

enum class MeshFlags {
    // source.Surface (source.TextureInfo rolled into titanfall.TextureData?)
    SKY_2D = 0x0002,  // TODO: test overriding sky with this in-game
    SKY = 0x0004,
    WARP = 0x0008,  // Quake water surface ?
    TRANSLUCENT = 0x0010,  // decals & atmo ?
    // titanfall.Mesh.flags
    VERTEX_LIT_FLAT = 0x000,     // VERTEX_RESERVED_1
    VERTEX_LIT_BUMP = 0x200,     // VERTEX_RESERVED_2
    VERTEX_UNLIT = 0x400,        // VERTEX_RESERVED_0
    VERTEX_UNLIT_TS = 0x600,     // VERTEX_RESERVED_3
    // VERTEX_BLINN_PHONG = 0x ? ? ? # VERTEX_RESERVED_4
    SKIP = 0x20000,  // 0x200 in valve.source.Surface(<< 8 ? )
    TRIGGER = 0x40000,  // guessing
    // masks
    MASK_VERTEX = 0x600,
};
struct LumpHeader {
	uint32_t offset;
	uint32_t length;
	uint32_t version;
	uint32_t fourCC;
};

struct CMGrid
{
    float cellSize;
    int cellOrg[2];
    int cellCount[2];
    int straddleGroupCount;
    int basePlaneOffset;
};


struct BspHeader {
	uint32_t   magic;
	uint32_t   version;
	uint32_t   revision;
	uint32_t   _127;
	LumpHeader lumps[128];
};

struct VertexUnlit {
	int vertexIndex;
	int normalIndex;
	glm::vec2 albedoUv;
	uint32_t color;
};

struct VertexLitFlat {
	uint32_t vertexIndex;
	uint32_t normalIndex;
	glm::vec2 albedoUv;
	uint32_t color;
	float lightMapUv[2];
	float lightMapXy[2];
};

struct VertexLitBump {
	int vertexIndex;
	int normalIndex;
	glm::vec2 albedoUv;
	uint32_t color;
	float lightMapUv[2];
	float lightMapXy[2];
	int tangent[2];
};

struct VertexUnlitTS{
	int vertexIndex;
	int normalIndex;
	glm::vec2 albedoUv;
	uint32_t color;
	uint32_t unk[2];
};

struct VertexBlinnPhong {
	int vertexIndex;
	int normalIndex;
	uint32_t color;
	float uv[4];
	float tangent[16];
};

struct BspMesh{

	unsigned int first_mesh_index;
	unsigned short num_triangles;
	unsigned short first_vertex;
	unsigned short num_vertices;
	unsigned short vertex_type;
	BYTE styles[4];
	short luxel_origin[2];
	BYTE luxel_offset_max[2];
	unsigned short material_sort;
	unsigned int meshFlags;

};

struct MaterialSort {
	short textureData;
	short lightMapHeader;
	short cubemap;
	short lastVertex;
	int vertexOffset;
};

struct TextureData {
	float reflectivity[3];
	int nameStringId;
	int width;
	int height;
	int view_width;
	int view_height;
	int flags;
};

struct StaticProp
{
	Vector3 m_Origin;
	Vector3 m_Angles;
	float scale;
	uint16_t modelIndex;
	BYTE m_Solid;
	BYTE m_flags;
	WORD skin;
	WORD word_22;
	float forced_fade_scale;
	Vector3 m_LightingOrigin;
	uint8_t m_DiffuseModulation_r;
	uint8_t m_DiffuseModulation_g;
	uint8_t m_DiffuseModulation_b;
	uint8_t m_DiffuseModulation_a;
	int unk;
	DWORD collision_flags_remove;
};

struct Tricoll_Header
{
	int16_t flags;
	int16_t texInfoFlags;
	int16_t texData;
	int16_t vetCount;
	uint16_t trisCount;
	uint16_t bevelIndicesCount;
	int32_t vertStart;
	uint32_t trisStart;
	uint32_t nodeStart;
	uint32_t bevelIndicesStart;
	Vector3 origin;
	float scale;
};


static_assert(sizeof(Tricoll_Header)==0x2C);
static_assert(offsetof(Tricoll_Header,flags)==0x0);
static_assert(offsetof(Tricoll_Header,texInfoFlags)==0x2);
static_assert(offsetof(Tricoll_Header,texData)==0x4);
static_assert(offsetof(Tricoll_Header,vetCount)==0x6);
static_assert(offsetof(Tricoll_Header,trisCount)==0x8);
static_assert(offsetof(Tricoll_Header,bevelIndicesCount)==0xA);
static_assert(offsetof(Tricoll_Header,vertStart)==0xC);
static_assert(offsetof(Tricoll_Header,trisStart)==0x10);
static_assert(offsetof(Tricoll_Header,nodeStart)==0x14);
static_assert(offsetof(Tricoll_Header,bevelIndicesStart)==0x18);
static_assert(offsetof(Tricoll_Header,origin)==0x1C);
static_assert(offsetof(Tricoll_Header,scale)==0x28);




struct GridCell
{
	uint16_t geoSetStart;
	uint16_t geoSetCount;

};
static_assert(sizeof(GridCell)==4);
struct GeoSet{
	uint16_t straddleGroup;
	uint16_t primCount;
	unsigned int primStart;
};
static_assert(sizeof(GeoSet)==8);
struct GeoSetBounds {
	short origin[3];
	short cos;
	short extends[3];
	short sin;
};
static_assert(sizeof(GeoSetBounds)==16);

enum class PrimitiveType {
    Brush = 0,
    Ticoll = 2,
    Prop = 3,
};

struct Brush {
    Vector3 origin;
    char num_non_axial_do_discard;
    char num_plane_offsets;
    short index;
    Vector3 extends;
    int brush_side_offset;
};

static_assert(sizeof(Brush)== 0x20);
static_assert(offsetof(Brush,origin)== 0);
static_assert(offsetof(Brush,num_non_axial_do_discard)== 0xC);
static_assert(offsetof(Brush,num_plane_offsets)== 0xD);
static_assert(offsetof(Brush,index)== 0xE);
static_assert(offsetof(Brush,extends)== 0x10);
static_assert(offsetof(Brush,brush_side_offset)== 0x1C);
