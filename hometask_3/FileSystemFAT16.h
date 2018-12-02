#pragma once
#include <cstring>
#include <linux/types.h>
#include <list>
#include <string>
#include <vector>

#define OK 0
#define CANNOT_OPEN_IMAGE -1

struct fatBootSector {
    __u8       ignored[3];     /* Boot strap short or near jump */
    __u8       systemId[8];    /* Name - can be used to special case
                                              partition manager volumes */
    __u8       sectorSize[2];  /* bytes per logical sector */
    __u8       secPerClus;     /* sectors/cluster */
    __le16     reserved;       /* reserved sectors */
    __u8       fats;           /* number of FATs */
    __u8       dirEntries[2];  /* root directory entries */
    __u8       sectors[2];     /* number of sectors */
    __u8       media;          /* media code */
    __le16     fatLength;      /* sectors/FAT */
    __le16     secsTrack;      /* sectors per track */
    __le16     heads;          /* number of heads */
    __le32     hidden;         /* hidden sectors (unused) */
    __le32     totalSect;      /* number of sectors (if sectors == 0) */

    /*  Extended BPB Fields for FAT16 */
    __u8       driveNumber;    /* Physical drive number */
    __u8       state;          /* undocumented, but used
                                               for mount state. */
    __u8       signature;      /* extended boot signature */
    __u8       volId[4];       /* volume ID */
    __u8       volLabel[11];   /* volume label */
    __u8       fsType[8];      /* file system type */
};

struct fatDirEntry {
    __u8        name[11];       /* name and extension */
    __u8        attr;           /* attribute bits */
    __u8    lcase;          /* Case for base and extension */
    __u8        ctime_cs;       /* Creation time, centiseconds (0-199) */
    __le16      ctime;          /* Creation time */
    __le16      cdate;          /* Creation date */
    __le16      adate;          /* Last access date */
    __le16      starthi;        /* High 16 bits of cluster in FAT32 */
    __le16      time,date,start;/* time, date and first cluster */
    __le32      size;           /* file size (in bytes) */
};

class FAT16DirEntry {
public:
    FAT16DirEntry(fatDirEntry& entry);

private:
    bool isDirectory;
    std::string Name;
    int Size;
    int StartCluster;
    std::vector<long> Sectors;
    friend class FAT16FileSystem;
    friend class FAT16Root;
    friend std::ostream& operator<<(std::ostream& os,
                                    const FAT16DirEntry& dirEntry);
};

class FAT16Root {
public:
    FAT16Root() = default;
    int addEntry(FAT16DirEntry entry);
    std::vector<FAT16DirEntry> entries;

private:
    friend class FAT16FileSystem;
};

class FAT16FileSystem {
public:
    FAT16FileSystem() = default;
    int openImage(std::string& fName);
    int listDir();
    int catFile(std::string& fName);

private:
    int ClusterSize;
    int SectorSize;
    int RootEntriesCapacity;
    int FatSize;
    int FatStart;
    int RootStart;
    int DataStart;

    friend class FAT16Root;
    FAT16Root root;

protected:
    FILE *fd = nullptr;
};
