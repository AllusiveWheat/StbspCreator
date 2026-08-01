//
// Created by maya on 30.07.26.
//
#include <algorithm>
#include "st_stbsp_exporter.h"

#include "st_material_management.h"
#include "st_settings_controller.h"

#define IALIGN(a,b)  (((a) + ((b)-1)) & ~((b)-1))


static inline unsigned char BitScanReverseCompat(unsigned long* index, unsigned long mask)
{
#if defined(_MSC_VER)
    return _BitScanReverse(index, mask);
#else
    if (mask == 0)
        return 0;

    *index = static_cast<unsigned long>(31u - __builtin_clz(mask));
    return 1;
#endif
}


static uint64_t Pak_StringToGuidAligned(const char* string)
{
    uint64_t         v1; // r9
    int               i; // r11d
    uint32_t         v4; // edi
    int              v5; // ebp
    int              v6; // r10d
    uint32_t         v7; // ecx
    uint32_t         v8; // edx
    uint32_t         v9; // eax
    uint32_t        v10; // r8d
    int64_t         v11; // r10
    uint64_t        v12; // r8
    int             v13; // eax
    int             v15; // ecx

    v1 = 0ull;
    for (i = 0; ; i += 4)
    {
        v4 = ~*(uint32_t*)string & (*(uint32_t*)string - 0x1010101) & 0x80808080;
        v5 = v4 ^ (v4 - 1);
        v6 = v5 & *(uint32_t*)string ^ 0x5C5C5C5C;
        v7 = ~v6 & (v6 - 0x1010101) & 0x80808080;
        v8 = v7 & -(int32_t)v7;
        if (v7 != v8)
        {
            v9 = 0xFF000000;
            do
            {
                v10 = v9;
                if ((v9 & v6) == 0)
                    v8 |= v9 & 0x80808080;
                v9 >>= 8;
            } while (v10 >= 0x100);
        }
        v11 = 0x633D5F1 * v1;
        v12 = ((long long)0xFB8C4D96501 * (uint64_t)(((v5 & *(uint32_t*)string) - 45 * (v8 >> 7)) & 0xDFDFDFDF)) >> 24;
        if (v4)
            break;
        string += 4;
        v1 = ((v11 + v12) >> 61) ^ (v11 + v12);
    }
    v13 = -1;
    if (BitScanReverseCompat((unsigned long*)&v15, v5))
        v13 = v15;
    return v12 + v11 - (long long)0xAE502812AA7333 * (uint32_t)(i + v13 / 8);
}

static uint64_t Pak_StringToGuidUnaligned(const char* string)
{
    uint64_t        v1; // rbx
    uint64_t        v2; // r10
    int              i; // esi
    int             v4; // edx
    uint32_t        v5; // edi
    int             v6; // ebp
    int             v7; // edx
    uint32_t        v8; // ecx
    uint32_t        v9; // r8d
    uint32_t       v10; // eax
    uint32_t       v11; // r9d
    int64_t        v12; // r9
    uint64_t       v13; // r8
    int            v14; // eax
    int            v16; // ecx

    v1 = 0ull;
    v2 = (uint64_t)(string + 3);
    for (i = 0; ; i += 4)
    {
        if ((v2 ^ (v2 - 3)) >= 0x1000)
        {
            v4 = *(uint8_t*)(v2 - 3);
            if ((uint8_t)v4)
            {
                v4 = *(uint16_t*)(v2 - 3);
                if (*(uint8_t*)(v2 - 2))
                {
                    v4 |= *(uint8_t*)(v2 - 1) << 16;
                    if (*(uint8_t*)(v2 - 1))
                        v4 |= *(uint8_t*)v2 << 24;
                }
            }
        }
        else
        {
            v4 = *(uint32_t*)(v2 - 3);
        }
        v5 = ~v4 & (v4 - 0x1010101) & 0x80808080;
        v6 = v5 ^ (v5 - 1);
        v7 = v6 & v4;
        v8 = ~(v7 ^ 0x5C5C5C5C) & ((v7 ^ 0x5C5C5C5C) - 0x1010101) & 0x80808080;
        v9 = v8 & -(int32_t)v8;
        if (v8 != v9)
        {
            v10 = 0xFF000000;
            do
            {
                v11 = v10;
                if ((v10 & (v7 ^ 0x5C5C5C5C)) == 0)
                    v9 |= v10 & 0x80808080;
                v10 >>= 8;
            } while (v11 >= 0x100);
        }
        v12 = 0x633D5F1 * v1;
        v13 = ((long long)0xFB8C4D96501 * (uint64_t)((v7 - 45 * (v9 >> 7)) & 0xDFDFDFDF)) >> 24;
        if (v5)
            break;
        v2 += (long long)4;
        v1 = ((v12 + v13) >> 61) ^ (v12 + v13);
    }
    v14 = -1;
    if (BitScanReverseCompat((unsigned long*)&v16, v6))
        v14 = v16;
    return v13 + v12 - (long long)0xAE502812AA7333 * (uint32_t)(i + v14 / 8);
}

uint64_t calculateRpakHash(const char* str)
{
    return ((uintptr_t)str & 3)
        ? Pak_StringToGuidUnaligned(str)
        : Pak_StringToGuidAligned(str);
}

uint64_t Hash(const std::string& str)
{
	const char* cstr = str.c_str();
	return calculateRpakHash(cstr);
}

namespace st
{
    uint32_t StringTable::AddString(const std::string& str)
    {
        if (strings.contains(str))
            return strings[str];
        uint32_t offset = currentStringOffset;
        strings[str] = offset;
        currentStringOffset += str.length() + 1;
        return offset;
    }

    void StringTable::ExportToFile(std::ofstream& file)
    {
        std::vector<char> out;
        out.resize(currentStringOffset);
        for (auto&[str,offset] : strings)
        {
            memcpy(&out[offset],str.c_str(),str.length());
            out[offset+str.length()] = '\0';
        }
        file.write(out.data(),out.size());
    }

    void DataWriter::ExportToFile(std::ofstream& file)
    {
        file.write(data.data(),data.size());
    }

    uint32_t StbspExporter::AddRpakMaterial(const std::string& name)
    {
        auto& mat = materials.emplace_back();
        mat.nameOffset = materialNames.AddString(name);
		auto lowerName = name;
		for (auto& c : lowerName) c = std::tolower(c);
        if (lowerName.starts_with("materials/world"))
        {
            mat.rpakGuid = Hash(name+ "_wld.rpak");
        }
        else if(lowerName.starts_with("materials/models"))
        {
           mat.rpakGuid = Hash(name + "_skn.rpak");
        }
        else {
            mat.rpakGuid = Hash(name + ".rpak");
        }
        mat.vtfStart = vtfNameIndices.size();
        mat.vtfEnd = mat.vtfStart;
        return materials.size() - 1;
    }

    uint32_t StbspExporter::AddVmtMaterial(const std::string& name,std::vector<std::string>& textures)
    {
        auto& mat = materials.emplace_back();
        mat.nameOffset = materialNames.AddString(name);
        mat.rpakGuid = 0;
        mat.vtfStart = vtfNameIndices.size();
        for (auto& str : textures)
        {
            vtfNameIndices.push_back(vtfNames.AddString(str));
        }
        mat.vtfEnd = vtfNameIndices.size();
        return materials.size() - 1;
    }

    void StbspExporter::AddResidentPage(int xMin,int yMin,int xMax,int yMax, std::vector<std::vector<uint32_t>>& histograms)
    {
        struct MaterialSorting
        {
            uint32_t materialIndex;
            uint32_t bin;
            uint32_t count;
        };
        std::vector<std::vector<MaterialSorting>> materialSorts;
        for (auto& histogram : histograms)
        {
            auto& data = materialSorts.emplace_back();
            if (!histogram.size())
                continue;
            for (int i = 0; i < StMaterialManager::getManager().getMaterialCount(); ++i)
            {
                for (int bin = 0; bin < 16; ++bin)
                {
                    auto& sort = data.emplace_back();
                    sort.materialIndex = i;
                    sort.bin = bin;
                    sort.count = histogram[i*16+bin];
                }
            }
            std::sort(data.begin(), data.end(),[](const MaterialSorting& s1,const MaterialSorting& s2)
            {
                return s1.count > s2.count;
            });

        }
        float cvg = std::numeric_limits<float>::min();
        for (auto& data: materialSorts)
        {
            if (!data.size())
                continue;
            cvg = std::max(cvg,(float)data[0].count/65535.f);
        }



        auto& page = residentPages.emplace_back();
        page.minCellX = xMin;
        page.minCellY = yMin;
        page.maxCellX = xMax;
        page.maxCellY = yMax;
        page.coverageScale = cvg;
        page.dataOffset = pageData.Size();
        for (auto& data: materialSorts)
        {
            std::vector<FilePageData> page;
            for (auto& mat:data)
            {
                if (mat.count == 0)
                    break;


                auto& p = page.emplace_back();
                p.matIndex = mat.materialIndex;
                p.bin = mat.bin;
                p.cvg = std::round(mat.count/cvg);
                if (page.size()==512)
                    break;
            }
            uint16_t pageSize = page.size();
            pageData.Write<uint16_t>(pageSize);
            for (auto& p:page)
            {
                pageData.Write<FilePageData>(p);
            }

        }
        page.dataSize = pageData.Size() - page.dataOffset;
    }

    void StbspExporter::SetCellGrid(int xMin_, int yMin_, int xMax_, int yMax_)
    {
        xMin = xMin_;
        yMin = yMin_;
        xMax = xMax_;
        yMax = yMax_;
    }

    void StbspExporter::FinishFile(const fs::path& exportPath)
    {
        FileHeader hdr;
        hdr.magic = 0xCB00CBB5;
        hdr.majorVer = 8;
        hdr.minorVer = 0;
        hdr.minCellX = xMin;
        hdr.minCellY = yMin;
        hdr.maxCellX = xMax;
        hdr.maxCellY = yMax;
        hdr.cellsPerPageSide = 4;
        hdr.cellSizeX = StSettingsManager::getManager().cellSize;
        hdr.cellSizeY = StSettingsManager::getManager().cellSize;
        hdr.lumps[LUMP_MATERIAL_NAMES].offset = sizeof(hdr);
        hdr.lumps[LUMP_MATERIAL_NAMES].size = materialNames.Size();
        hdr.lumps[LUMP_MATERIAL_INFO].offset = IALIGN(sizeof(hdr) + materialNames.Size(),4);
        hdr.lumps[LUMP_MATERIAL_INFO].size = materials.size();
        hdr.lumps[LUMP_VTF_NAMES].offset = hdr.lumps[LUMP_MATERIAL_INFO].offset + materials.size()*sizeof(FileMaterial);
        hdr.lumps[LUMP_VTF_NAMES].size = (vtfNames.Size()+3)/4;
        hdr.lumps[LUMP_VTF_INDICES].offset = hdr.lumps[LUMP_VTF_NAMES].offset + hdr.lumps[LUMP_VTF_NAMES].size * 4;
        hdr.lumps[LUMP_VTF_INDICES].size = vtfNameIndices.size();
        hdr.lumps[LUMP_RESIDENT_PAGES].offset = IALIGN(hdr.lumps[LUMP_VTF_INDICES].offset + vtfNameIndices.size() * sizeof(uint16_t),4);
        hdr.lumps[LUMP_RESIDENT_PAGES].size = residentPages.size();
        hdr.lumps[LUMP_PAGE_DATA].offset = hdr.lumps[LUMP_RESIDENT_PAGES].offset + residentPages.size() * sizeof(FileResidentPage);
        hdr.lumps[LUMP_PAGE_DATA].size = pageData.Size();
        std::ofstream out{exportPath,std::ios::binary};
        out.write(reinterpret_cast<char*>(&hdr),sizeof(hdr));
        materialNames.ExportToFile(out);
        while (out.tellp()<hdr.lumps[LUMP_MATERIAL_INFO].offset)
            out.put(0);
        out.write(reinterpret_cast<char*>(materials.data()),materials.size()*sizeof(FileMaterial));
        vtfNames.ExportToFile(out);
        while (out.tellp()<hdr.lumps[LUMP_VTF_INDICES].offset)
            out.put(0);
        out.write(reinterpret_cast<char*>(vtfNameIndices.data()),vtfNameIndices.size()*sizeof(uint16_t));
        while (out.tellp()<hdr.lumps[LUMP_RESIDENT_PAGES].offset)
            out.put(0);
        out.write(reinterpret_cast<char*>(residentPages.data()),residentPages.size()*sizeof(FileResidentPage));
        pageData.ExportToFile(out);
        out.close();
    }
}

