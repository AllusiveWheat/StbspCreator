//
// Created by maya on 30.07.26.
//
#include <algorithm>
#include "st_stbsp_exporter.h"

#include "st_material_management.h"
#include "st_settings_controller.h"

#define IALIGN(a,b)  (((a) + ((b)-1)) & ~((b)-1))

uint64_t Hash(const std::string& str)
{
    return 0;
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
        mat.rpakGuid = Hash(name);
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

