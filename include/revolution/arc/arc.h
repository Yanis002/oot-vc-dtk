#ifndef REVOSDK_ARC_H
#define REVOSDK_ARC_H

#include "dolphin/types.h"
#include "revolution/cnt/cnt.h"

// based on WiiCore
typedef enum ARCType {
    RX_ARCHIVE_FILE, // The entry is a file. Accessed via rxArchiveFile with rxArchiveFileOpen/rxArchiveFileOpenLow
    RX_ARCHIVE_FOLDER // The entry is a folder.
} ARCType;

typedef struct ARCNode {
    /* 0x00 */ u16 type;
    /* 0x02 */ u16 name_offset;
    /* 0x04 */ u32 data_offset;
    /* 0x08 */ size_t size;
} ARCNode; // size = 0xC

typedef struct ARCHeader {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 rootnode_offset;
    /* 0x08 */ u32 header_size;
    /* 0x0C */ u32 data_offset;
    /* 0x10 */ char pad[0x8];
} ARCHeader; // size = 0x18

typedef struct ARCHandle {
    /* 0x000 */ ARCHeader* hdr;
    /* 0x004 */ ARCNode* nodes;
    /* 0x008 */ void* unk8;
    /* 0x00C */ s32 node_cnt;
    /* 0x010 */ char* string_table;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 data_offset;
    /* 0x01C */ s32 unk1C;
} ARCHandle; // size = 0x20

typedef struct ARCFile {
    ARCHandle* parent; //!< 00 The archive the file is a part of.
    u32 offset; //!< 04 Offset into the archive's file data buffer.
    u32 size; //!< 08 Size of the file.
} ARCFile;

typedef struct ARCEntry {
    ARCHandle* parent; //!< 00 The archive the file is a part of.
    u32 path; //!< 04 Special ID corresponding to a file path.
              //!< Accepted by rxArchiveFileOpenLow.
    ARCType node_type; //!< The entry type.
    const char* name; //!< 0C Name of this entry in specific.
                      //!< (Not an absolute path)
} ARCEntry;

typedef struct ARCDir {
    ARCHandle* parent; //!< 00 The archive the file is a part of.
    u32 path_begin; //!< 04 The lower bound of the iterator range.
    u32 path_it; //!< 08 The current position of the iterator.
    u32 path_end; //!< 0C The upper bound of the iterator range.
} ARCDir;

bool ARCInitHandle(void*, ARCHandle*);
bool ARCOpen(const ARCHandle*, const char*, ARCFile*);
bool ARCFastOpen(const ARCHandle*, s32, ARCFile*);
s32 ARCConvertPathToEntrynum(ARCHandle*, const char*);
void* ARCGetStartAddrInMem(const ARCFile*);
s32 ARCGetStartOffset(const ARCFile*);
u32 ARCGetLength(const ARCFile*);
bool ARCClose(ARCFile*);
bool ARCChangeDir(ARCHandle*, const char*);
bool ARCOpenDir(const ARCHandle*, const char*, ARCDir*);
bool ARCReadDir(ARCDir*, ARCEntry*);
bool ARCCloseDir(ARCDir*);
bool ARCGetFile(ARCHandle*, char* filename, CNTFileInfo* cntFileInfo);

#endif
