#include <algorithm>
#include <iostream>
#include "FileSystemFAT16.h"

FAT16DirEntry::FAT16DirEntry(fatDirEntry &entry) {
    if (entry.attr & 0x10)
        isDirectory = true;
    else
        isDirectory = false;

    char name[9] = {};
    strncpy(name, (char*) entry.name, 8);
    char type[4] = {};
    strncpy(type, (char*) entry.name + 8, 3);

    Name.append(name);
    Name.erase(std::remove_if(Name.begin(), Name.end(), isspace), Name.end());
    if (!isDirectory){
        Name.append(".");
        Name.append(type);
    }
    Name.erase(std::remove_if(Name.begin(), Name.end(), isspace), Name.end());
    StartCluster = entry.start;
    Size         = entry.size;
}

std::ostream &operator<<(std::ostream &os, const FAT16DirEntry &dirEntry) {
    if(dirEntry.Size == -1)
        return os;
    std::cout << dirEntry.Name;
    if(!dirEntry.isDirectory)
        std::cout << "\t\t" << dirEntry.Size;
    std::cout << "\t\t" << dirEntry.StartCluster;
    std::cout << std::endl;
    return os;
}

int FAT16FileSystem::catFile(std::string& fName) {
    auto it = std::find_if(root.entries.begin(), root.entries.end(),
                           [fName](FAT16DirEntry dirEntry) {
        return fName == dirEntry.Name;
    });
    if (it == root.entries.end())
        return OK;

    std::vector<int> out_clusters;
    int cluster = it->StartCluster;
    while((0x0002 <= cluster) && (cluster <= 0xFFEF)) {
        out_clusters.emplace_back(cluster);
        std::cout << std::hex << cluster << std::endl;
        int newCluster = 0;
        fseek(fd, FatStart + 2*sizeof(char)*cluster, SEEK_SET);
        fread(&newCluster, 2*sizeof(char), 1, fd);
        cluster = newCluster;
    }

    for (auto& out_cluster : out_clusters)
        std::cout << out_cluster << std::endl;
    return OK;
}

int FAT16FileSystem::openImage(std::string& fName) {
    std::cout << "Opening FAT16 image " << fName << std::endl;
    fd = fopen(fName.c_str(), "r");
    if(fd == NULL) {
        throw std::runtime_error("Cannot open image " + fName);
        return CANNOT_OPEN_IMAGE;
    }
    std::cout << "OK." << std::endl;
    fatBootSector sector = {};
    fread(&sector, sizeof(sector), 1, fd);
    char sysid[32] = {};
    strncpy(sysid, (char *)sector.systemId, 8);
    SectorSize          = sector.sectorSize[0] + (sector.sectorSize[1] << 8);
    ClusterSize         = sector.secPerClus*SectorSize;
    RootEntriesCapacity = sector.dirEntries[0] + (sector.dirEntries[1] << 8) ;
    FatSize   = SectorSize * sector.fatLength;
    FatStart  = SectorSize * sector.reserved;
    RootStart = FatStart + FatSize * sector.fats;
    DataStart = RootStart + RootEntriesCapacity * 32;

    fseek(fd, RootStart, SEEK_SET);

    for (int i = 0; i < RootEntriesCapacity; i++) {
        fatDirEntry entry;
        fread(&entry, sizeof(entry), 1, fd);

        if (entry.name[0] == 0x00)
            break;

        if (entry.name[0] == 0xE5)
            continue;

        root.addEntry(FAT16DirEntry(entry));
    }

    return OK;
}

int FAT16FileSystem::listDir() {
    for(auto& dirEntry : root.entries)
        std::cout << dirEntry;
    return OK;
}

int FAT16Root::addEntry(FAT16DirEntry entry) {
    entries.emplace_back(entry);
    return OK;
}
