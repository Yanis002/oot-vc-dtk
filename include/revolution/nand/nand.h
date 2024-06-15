#ifndef REVOSDK_NAND_H
#define REVOSDK_NAND_H

#include "dolphin/types.h"

struct NANDCommandBlock;
struct NANDFileInfo;

typedef void (* NANDAsyncCallback)(s32, struct NANDCommandBlock *);
typedef void (* NANDCBAsyncCallback)(s32, struct NANDCB_UNK *, s32);

struct NANDCB_UNK
{
	char UNK_0x0[0x8];
	s32 WORD_0x8;
	NANDCBAsyncCallback CALLBACK_0xC;
	s32 WORD_0x10;
	char UNK_0x14[0x152];
	char BYTE_0x166;
};

struct NANDCommandBlock
{
	char UNK_0x0[0x144];
	struct NANDCB_UNK * PTR_0x144;
};

struct NANDFileInfo
{
	s32 WORD_0x0;
	s32 WORD_0x4;
	
	volatile char BUF_0x8[0x40];
	
	volatile char BUF_0x48[0x40];
	
	char BYTE_0x88;
	char BYTE_0x89;
	char BYTE_0x8a;
};

bool NANDOpen(const char *, struct NANDFileInfo *, u8);
s32 NANDGetLength(struct NANDFileInfo *, u32 *);
void NANDClose(struct NANDFileInfo *);
s32 NANDRead(struct NANDFileInfo *, void *, u32);
s32 NANDWrite(struct NANDFileInfo *, const void *, u32);
s32 NANDReadAsync(struct NANDFileInfo *, void *, u32, NANDAsyncCallback, void *);
s32 NANDWriteAsync(struct NANDFileInfo *, const void *, u32, NANDAsyncCallback, void *);

s32 NANDSeek(struct NANDFileInfo *, u32, s32);

void NANDGetCurrentDir(char *);

#endif