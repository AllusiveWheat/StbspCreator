#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <fstream>
#include "st_bsp_loader.h"

namespace fs = std::filesystem;

namespace st
{

    struct StringTable
    {
        uint32_t AddString(const std::string& str);
        void ExportToFile(std::ofstream& file);
        size_t Size() { return currentStringOffset; }
        uint32_t currentStringOffset = 0;
        std::unordered_map<std::string,uint32_t> strings;

    };

    struct DataWriter
    {
        std::vector<char> data;
        template<typename T> void Write(T& w)
        {
            const char* a = reinterpret_cast<char*>(&w);
            for (size_t i = 0; i < sizeof(T); ++i)
            {
                data.push_back(a[i]);
            }
        }
        size_t Size() { return data.size(); }
        void ExportToFile(std::ofstream& file);
    };

    class StbspExporter
    {
        enum LUMPS // lump types??
        {
            LUMP_MATERIAL_NAMES,
            LUMP_MATERIAL_INFO,
            LUMP_VTF_NAMES,
            LUMP_VTF_INDICES,
            LUMP_RESIDENT_PAGES,
            LUMP_PAGE_DATA
        };


        struct FileLumpOffsetPair
        {
            uint64_t offset;
            uint64_t size;
        };
        struct FileHeader
        {
            uint32_t magic;
            uint16_t majorVer;
            uint16_t minorVer;
            int minCellX;
            int minCellY;
            int maxCellX;
            int maxCellY;
            int cellsPerPageSide;
            float cellSizeX;
            float cellSizeY;
            char gap_24[132];
            FileLumpOffsetPair lumps[6];
            char unk2[128];
        };

        struct FileMaterial
        {
            uint32_t nameOffset;
            char pad[4];
            uint64_t rpakGuid;
            uint32_t vtfStart;
            uint32_t vtfEnd;
        };

        struct FileResidentPage
        {
            uint64_t dataOffset;
            int dataSize;
            float coverageScale;
            short minCellX;
            short minCellY;
            short maxCellX;
            short maxCellY;
        };


        struct FilePageData
        {
            uint16_t matIndex : 10;
            uint16_t bin : 6;
            uint16_t cvg;
        };
    public:
        uint32_t AddRpakMaterial(const std::string& name);
        uint32_t AddVmtMaterial(const std::string& name,std::vector<std::string>& vtfName);
        void AddResidentPage(int xMin,int yMin,int xMax,int yMax, std::vector<std::vector<uint32_t>>& histogram);
        void SetCellGrid(int xMin,int yMin,int xMax,int yMax);
        void FinishFile(const fs::path& exportPath);
    private:
        StringTable materialNames;
        std::vector<FileMaterial> materials;
        StringTable vtfNames;
        std::vector<uint32_t> vtfNameIndices;
        std::vector<FileResidentPage> residentPages;
        int xMin;
        int yMin;
        int xMax;
        int yMax;
        DataWriter pageData;
    };
}
