#pragma once
#include <stdint.h>
#include <stddef.h>
#ifdef _MSC_VER
#include <stdio.h>
#endif
#ifndef PS_CFG_MEM_ERROR_IS_FATAL
#define PS_CFG_MEM_ERROR_IS_FATAL	1
#endif

class staticUtility {
public:
	static int _OnError(int isFatal, const char* reason = "generic") {
		if (isFatal)
		{
#ifdef _MSC_VER
			printf("Fatal error/ pair stack: %s !\r\n", reason);
#endif
			volatile int i;
			for (i = 0; i < 100; ) {
				i = i;
			}
		}
		return -1;
	}
};


typedef union tagPairStackSizeWord {
	uint32_t u32Val;
	int32_t i32Val;
	struct {
		uint32_t b2LSBs : 2;
		uint32_t c4Size : 29;
		uint32_t isChainHead : 1;
	}sizeBits;
}PairStackSizeWord_t;

class CPairStack {
protected:
	// Downard full stack, with stack bottom saves the size of current block
	uint32_t* m_pBuf;
	uint32_t m_c4Size;	// size of current block, including size word
	uint32_t* m_FDSP;	// full downard SP (Like Cortex-M)
	uint32_t* m_EUSP; // empty upward SP
	int m_EULockCnt;
	int m_FDLockCnt;
	int m_dbgBlkCnt;
	int m_dbgEUBlkCnt;
	int m_dbgMinFreeSize;

	void* _FD_Take(uint32_t byteSize, uint8_t isChainHead);
	void* _EU_Take(uint32_t byteSize, uint8_t isChainHead);
	int _DoFD_Give(void);
	int _DoEU_Give(void);
	int _FD_GiveChains(int chainCnt);
	int _EU_GiveChains(int chainCnt);

public:
	void ManualConstruct(uint32_t byteSize, uint32_t* pBuf);

	void ManualDestruct();

	CPairStack(uint32_t byteSize, uint32_t* pBuf);

	~CPairStack();

	void* GetAllFreeSpace(int* pFreeSize);

	int FD_GetBlockCnt(void);

	int EU_GetBlockCnt(void);

	int FD_Lock(void);

	int FD_Unlock(void);

	void* FD_TakeChainHead(size_t byteSize);

	void* FD_Take(size_t byteSize);

	int FD_Give(void* p = nullptr);

	bool FD_IsAtStackTop(void* p);

	int ForceFD_Give(void);

	int FD_GiveChain(void* p = nullptr);

	int EU_Lock(void);

	int EU_Unlock(void);

	void* EU_TakeChainHead(size_t byteSize);

	void* EU_Take(size_t byteSize);

	bool EU_IsAtStackTop(void* p);

	int EU_Give(void* p = nullptr);

	int ForceEU_Give(void);

	int EU_GiveChain(void* p = nullptr);

	bool FD_IsInside(void* pBlk);

	bool EU_IsInside(void* pBlk);

	uint32_t WhereIs(void* pBlk);

    int GetMaxUsedSize(void) {
		return m_c4Size * 4 - m_dbgMinFreeSize;
	}

};
