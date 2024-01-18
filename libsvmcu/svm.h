#ifndef _LIBSVM_H_
#define _LIBSVM_H_
#include <stdint.h>

#define LIBSVM_VERSION 330

#ifndef SVM_CFG_FEATURE_SOLVER_C_SVC
#define SVM_CFG_FEATURE_SOLVER_C_SVC	0
#endif

#ifndef SVM_CFG_FEATURE_SOLVER_NU_SVC
#define SVM_CFG_FEATURE_SOLVER_NU_SVC	0
#endif

#ifndef SVM_CFG_FEATURE_SOLVER_NU_SVR
#define SVM_CFG_FEATURE_SOLVER_NU_SVR	0
#endif

#ifndef SVM_CFG_FEATURE_SOLVER_EPSILON_SVR
#define SVM_CFG_FEATURE_SOLVER_EPSILON_SVR	0
#endif

#ifndef SVM_CFG_FEATURE_SOLVER_ONE_CLASS
#define SVM_CFG_FEATURE_SOLVER_ONE_CLASS	1
#endif

#ifndef SVM_CFG_FEATURE_PROBABILITY
#define SVM_CFG_FEATURE_PROBABILITY			0
#endif


#ifndef SVM_CFG_DENSE_VECTOR
#define SVM_CFG_DENSE_VECTOR	1
#endif



extern int libsvm_version;

#ifndef SVM_CFG_ERROR_IS_FATAL
#define SVM_CFG_ERROR_IS_FATAL 1
#endif

#ifndef SVM_CFG_REAL_BITS
#define SVM_CFG_REAL_BITS   32
#endif

#ifndef SVM_CFG_COPY_ALPHA
#define SVM_CFG_COPY_ALPHA 0
#endif

#ifndef SVM_CFG_MAX_CLASS_CNT
#define SVM_CFG_MAX_CLASS_CNT	(10)
#endif

#if SVM_CFG_REAL_BITS == 64
#define SVM_REAL double
#elif SVM_CFG_REAL_BITS == 32
#define SVM_REAL float
#elif SVM_CFG_REAL_BITS == 16
#error FP16 not yet supported
#endif

#ifndef SVM_CFG_FP16_NO_CHECK
#define SVM_CFG_FP16_NO_CHECK	1	// do not check for NaN, Inf
#endif

#ifndef SVM_CFG_16BITS_FP_LEVEL
// Level 0: No 16 bit FP
// Level 1:
#define SVM_CFG_16BITS_FP_LEVEL   0
#endif

#define SVM_FP16_FORMAT_IEEE		0	// 1.5.10, slow but accurate
#define SVM_FP16_FORMAT_BF16		1	// 1.8.7 , fast but rough
#ifndef SVM_CFG_FP16_FORMAT
#define SVM_CFG_FP16_FORMAT		SVM_FP16_FORMAT_IEEE
// #define SVM_CFG_FP16_FORMAT		SVM_FP16_FORMAT_BF16
#endif


#if SVM_CFG_16BITS_FP_LEVEL > 0
#include "fp_16bits.h"
#if SVM_CFG_FP16_FORMAT == SVM_CFG_FP16_FORMAT_IEEE
#define SHORT_REAL	CFP16 // CBFloat16
#else
#define SHORT_REAL	CBFloat16
#endif
#else
#define SHORT_REAL	float
#endif


#ifndef SVM_CACHE_REAL
typedef float Qfloat;
#define SVM_CACHE_REAL	Qfloat
#endif

// Cache for kernel matrix
#define SVM_CACHE_LRU		1	// Least-Recent-Used ring-list based cache
#define SVM_CACHE_SETASSO	2	// Set associative cache, similar to CPU cache
#ifndef SVM_CFG_CACHE_SEL
#define SVM_CFG_CACHE_SEL	SVM_CACHE_LRU
// #define SVM_CFG_CACHE_SEL	SVM_CACHE_SETASSO
#endif

#ifndef SVM_CFG_CACHE_SETASSO_WAY_CNT
#define SVM_CFG_CACHE_SETASSO_WAY_CNT 4
#endif

#if SVM_CFG_CACHE_SETASSO_WAY_CNT	>= 16
#warning "max way per set is 16"
#undef SVM_CFG_CACHE_SETASSO_WAY_CNT
#define SVM_CFG_CACHE_SETASSO_WAY_CNT 16
#endif


#ifndef SVM_CFG_CACHE_SIZE
#define SVM_CFG_CACHE_SIZE (2 * 1024)
#endif

#ifndef SVM_CFG_MEM_ERROR_IS_FATAL
#define SVM_CFG_MEM_ERROR_IS_FATAL	1
#endif

#ifndef SVM_CFG_MAX_SAMP_CNT
#define SVM_CFG_MAX_SAMP_CNT	255 // 255
#endif

#ifndef SVM_CFG_PROF
#define SVM_CFG_PROF	1
#endif


// calculated by SVM_CFG_FEATURE_XXX macros
#define SVM_FEATURE_SOLVER_NU	(SVM_CFG_FEATURE_SOLVER_NU_SVC | SVM_CFG_FEATURE_SOLVER_NU_SVR)
#define SVM_FEATURE_SOLVER_SVC	(SVM_CFG_FEATURE_SOLVER_NU_SVC | SVM_CFG_FEATURE_SOLVER_C_SVC)
#define SVM_FEATURE_SOLVER_SVR  (SVM_CFG_FEATURE_SOLVER_NU_SVR | SVM_CFG_FEATURE_SOLVER_EPSILON_SVR)

typedef union tagPairStackSizeWord
{
    uint32_t u32Val;
    int32_t i32Val;
    struct
    {
        uint32_t b2LSBs : 2;
        uint32_t c4Size : 29;
        uint32_t isChainHead : 1;
    } sizeBits;
} PairStackSizeWord_t;


class staticUtility
{
public:
    static int _OnError( int isFatal, const char* reason = "generic" )
    {
        if ( isFatal )
        {
#ifdef _MSC_VER
            printf( "Fatal error/ pair stack: %s !\r\n", reason );
#endif
            volatile int i;
            for ( i = 0; i < 100; )
            {
                i = i;
            }
        }
        return -1;
    }
};

/*

Layout of FD Stack (Full-Downward stack, same as Cortex-M's h/w stack) and
EU Stack (Empty-Upward stack), they grow toward each other

|  *OVERFLOW AREA*     |  <-- FD Stack Bottom (m_pBuf + m_c4Size)
------------------------
|                      |        |
~      ......          ~        |
------------------------        v
|                      |
~         data n-1     ~
|                      |
------------------------
|      size word n-1   |
------------------------
|                      |
|        data n        |
~                      ~
|                      |
------------------------
|      size word n     |  <-- m_FDSP (FD Stack Top)
------------------------
|                      |
|                      |
|       public         |
~     free space       ~
~                      ~
|                      |
|                      |  <-- m_EUSP  (EU Stack Top)
------------------------
|      size word 2     |
------------------------
|                      |
~         data 2       ~
|                      |
------------------------
|      size word 1     |
------------------------
|                      |
|        data 1        |       ^
~                      ~       |
|                      |       |
------------------------  <-- EU Stack Bottom (m_pBuf)
|   UNDERFLOW AREA     |
*/
class CPairStack
{
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

    void* _FD_Take( uint32_t byteSize, uint8_t isChainHead )
    {
        uint32_t c4AlcSize = ( byteSize + 3 ) / 4 + 1 /* size value*/;
        uint32_t* newFDSP = m_FDSP - c4AlcSize;
        if ( m_FDLockCnt )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_Take while LOCKED" );
            return nullptr;
        }
        int freeSize = ( newFDSP - m_EUSP ) * 4;
        if ( freeSize < m_dbgMinFreeSize )
            m_dbgMinFreeSize = freeSize;
        if ( newFDSP <= m_EUSP )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_Take OVERFLOW" );
            return nullptr;
        }
        PairStackSizeWord_t sizeWord;
        sizeWord.sizeBits.isChainHead = ( isChainHead != 0 );
        sizeWord.sizeBits.c4Size = c4AlcSize;
        sizeWord.sizeBits.b2LSBs = 0;
        m_FDSP = newFDSP;
        m_FDSP[0] = sizeWord.u32Val;
        m_dbgBlkCnt++;
        return m_FDSP + 1 /* skip the size word*/;
    }

    void* _EU_Take( uint32_t byteSize, uint8_t isChainHead )
    {
        uint32_t c4AlcSize = ( byteSize + 3 ) / 4 + 1 /* size value*/;
        uint32_t* newEUSP = m_EUSP + c4AlcSize;
        if ( m_EULockCnt )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_Take while LOCKED" );
            return nullptr;
        }
        int freeSize = ( m_FDSP - newEUSP ) * 4;
        if ( freeSize < m_dbgMinFreeSize )
            m_dbgMinFreeSize = freeSize;
        if ( newEUSP >= m_FDSP )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_Take OVERFLOW" );
            return nullptr;
        }
        PairStackSizeWord_t sizeWord;
        sizeWord.sizeBits.isChainHead = ( isChainHead != 0 );
        sizeWord.sizeBits.c4Size = c4AlcSize;
        sizeWord.sizeBits.b2LSBs = 0;
        m_EUSP = newEUSP;
        m_EUSP[-1] = sizeWord.u32Val;
        m_dbgEUBlkCnt++;
        return m_EUSP - c4AlcSize; /* skip the size word*/
    }


    int _DoFD_Give( void )
    {
        PairStackSizeWord_t sizeWord;
        if ( m_FDLockCnt )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_Give while LOCKED" );
            return -1;
        }
        if ( m_FDSP >= m_pBuf + m_c4Size - 1 )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_Give not matched" );
            return -1;
        }

        sizeWord.u32Val = m_FDSP[0];
        m_FDSP += sizeWord.sizeBits.c4Size;
        m_dbgBlkCnt--;
        return ( sizeWord.sizeBits.c4Size - 1 ) * 4;
    }

    int _DoEU_Give( void )
    {
        PairStackSizeWord_t sizeWord;
        if ( m_EULockCnt )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_Give while LOCKED" );
            return -1;
        }
        if ( m_EUSP <= m_pBuf )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_Give not matched" );
            return -1;
        }

        sizeWord.u32Val = m_EUSP[-1];
        m_EUSP -= sizeWord.sizeBits.c4Size;
        m_dbgEUBlkCnt--;
        return ( sizeWord.sizeBits.c4Size - 1 ) * 4;
    }

    int _FD_GiveChains( int chainCnt )
    {
        PairStackSizeWord_t sizeWord;
        uint32_t c4FD_Gived = 0;
        if ( m_FDLockCnt )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_GiveChains while LOCKED" );
            goto Cleanup;
        }
        for ( int chn = 0; chn < chainCnt; chn++ )
        {
            do
            {
                if ( m_FDSP >= m_pBuf + m_c4Size - 1 )
                {
                    staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_GiveChains not matched" );
                    goto Cleanup;
                }
                sizeWord.u32Val = m_FDSP[0];
                m_FDSP += sizeWord.sizeBits.c4Size;
                c4FD_Gived += sizeWord.sizeBits.c4Size - 1;
                m_dbgBlkCnt--;
            }
            while ( sizeWord.i32Val >= 0 );
        }
Cleanup:
        return c4FD_Gived;
    }

    int _EU_GiveChains( int chainCnt )
    {
        PairStackSizeWord_t sizeWord;
        uint32_t c4FD_Gived = 0;
        if ( m_EULockCnt )
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_GiveChains while LOCKED" );
            goto Cleanup;
        }
        for ( int chn = 0; chn < chainCnt; chn++ )
        {
            do
            {
                if ( m_EUSP <= m_pBuf )
                {
                    staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_GiveChains not matched" );
                    goto Cleanup;
                }
                sizeWord.u32Val = m_EUSP[-1];
                m_EUSP -= sizeWord.sizeBits.c4Size;
                c4FD_Gived += sizeWord.sizeBits.c4Size - 1;
                m_dbgEUBlkCnt--;
            }
            while ( sizeWord.i32Val >= 0 );
        }
Cleanup:
        return c4FD_Gived;
    }

public:
    void ManualConstruct( uint32_t byteSize, uint32_t* pBuf )
    {
        m_c4Size = byteSize / 4;
        m_FDSP = pBuf + m_c4Size;
        m_EUSP = pBuf;
        m_pBuf = pBuf;
        m_FDLockCnt = 0;
        m_EULockCnt = 0;
        m_dbgBlkCnt = 0;
        m_dbgEUBlkCnt = 0;
        m_dbgMinFreeSize = byteSize;
    }

    void ManualDestruct()
    {
    }

    CPairStack( uint32_t byteSize, uint32_t* pBuf )
    {
        ManualConstruct( byteSize, pBuf );
    }

    ~CPairStack()
    {
        m_dbgMinFreeSize = m_dbgMinFreeSize;
#ifdef _MSC_VER
        printf( "min pairstack free size = %d\n", m_dbgMinFreeSize );
#endif
    }

    int FD_GetBlockCnt( void )
    {
        return m_dbgBlkCnt;
    }

    int EU_GetBlockCnt( void )
    {
        return m_dbgEUBlkCnt;
    }

    int FD_Lock( void )
    {
        int oldLocked = m_FDLockCnt;
        m_FDLockCnt++;
        return oldLocked;
    }

    int FD_Unlock( void )
    {
        int oldLocked = m_FDLockCnt;
        if ( m_FDLockCnt )
        {
            --m_FDLockCnt;
        }
        else
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_Unlock mismatched" );
            return -1;
        }
        return oldLocked;
    }

    void* FD_TakeChainHead( size_t byteSize )
    {
        return _FD_Take( byteSize, 1 );
    }

    void* FD_Take( size_t byteSize )
    {
        return _FD_Take( byteSize, 0 );
    }

    int FD_Give( void* p = nullptr )
    {
        if ( p && m_FDSP + 1 != ( uint32_t* )p )
        {
            // we must FD_Give from stack top
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_Give wrong data pointer" );
            return -1;
        }
        return _DoFD_Give();
    }

    bool FD_IsAtStackTop( void* p )
    {
        if ( p && m_FDSP + 1 == ( uint32_t* )p )
            return true;
        return false;
    }

    int ForceFD_Give( void )
    {
        return _DoFD_Give();
    }

    int FD_GiveChain( void* p = nullptr )
    {
        if ( p && m_FDSP + 1 != ( uint32_t* )p )
        {
            // we must FD_Give from stack top
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "FD_GiveChain wrong data pointer" );
            return -1;
        }
        return _FD_GiveChains( 1 );
    }

    int EU_Lock( void )
    {
        int oldLocked = m_EULockCnt;
        m_EULockCnt++;
        return oldLocked;
    }

    int EU_Unlock( void )
    {
        int oldLocked = m_EULockCnt;
        if ( m_EULockCnt )
        {
            --m_EULockCnt;
        }
        else
        {
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_Unlocked mismatched" );
            return -1;
        }
        return oldLocked;
    }

    void* EU_TakeChainHead( size_t byteSize )
    {
        return _EU_Take( byteSize, 1 );
    }

    void* EU_Take( size_t byteSize )
    {
        return _EU_Take( byteSize, 0 );
    }

    bool EU_IsAtStackTop( void* p )
    {
        PairStackSizeWord_t szWord;
        szWord.u32Val = m_EUSP[-1];
        if ( p && m_EUSP - szWord.sizeBits.c4Size == ( uint32_t* )p )
            return true;
        return false;
    }

    int EU_Give( void* p = nullptr )
    {
        PairStackSizeWord_t szWord;
        szWord.u32Val = m_EUSP[-1];
        if ( p && m_EUSP - szWord.sizeBits.c4Size != ( uint32_t* )p )
        {
            // we must FD_Give from stack top
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_Give wrong data pointer" );
            return -1;
        }
        return _DoEU_Give();
    }

    int ForceEU_Give( void )
    {
        return _DoEU_Give();
    }

    int EU_GiveChain( void* p = nullptr )
    {
        PairStackSizeWord_t szWord;
        szWord.u32Val = m_EUSP[-1];
        if ( p && m_EUSP - szWord.sizeBits.c4Size != ( uint32_t* )p )
        {
            // we must FD_Give from stack top
            staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "EU_GiveChain wrong data pointer" );
            return -1;
        }
        return _EU_GiveChains( 1 );
    }

    bool FD_IsInside( void* pBlk )
    {
        if ( pBlk < m_FDSP + 1 || pBlk >= ( m_pBuf + m_c4Size ) )
        {
            return false;
        }
        return true;
    }

    bool EU_IsInside( void* pBlk )
    {
        if ( pBlk >= m_EUSP || pBlk < m_pBuf )
        {
            return false;
        }
        return true;
    }

    uint32_t WhereIs( void* pBlk )
    {
        if ( EU_IsInside( pBlk ) )
            return 1;
        if ( FD_IsInside( pBlk ) )
            return 2;
        return 0;
    }
    
    int GetMaxUsedSize(void) {
		return m_c4Size * 4 - m_dbgMinFreeSize;
	}

};



struct svm_node
{
#if SVM_CFG_DENSE_VECTOR == 0
    int index;
#endif
    SVM_REAL value;
};
//struct svm_problem   svm_problem_t;
//struct svm_parameter svm_parameter_t;
struct svm_problem
{
    int n_pts;
    int vecDim;
    SVM_REAL* y;
    struct svm_node** x;
};

enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR };	/* svm_type */
enum { LINEAR, POLY, RBF, SIGMOID, PRECOMPUTED }; /* kernel_type */

struct svm_parameter
{
    uint8_t svm_type;
    uint8_t kernel_type;
    int8_t degree;	/* for poly */
    uint8_t shrinking;	/* use the shrinking heuristics */
#if SVM_CFG_FEATURE_PROBABILITY != 0
    uint8_t probability; /* do probability estimates */
#endif
    uint16_t nr_weight;		/* weights of "C" param of every class, for training C_SVC */
    int* weight_label;	/* for training C_SVC */
    SVM_REAL* weight;		/* for C_SVC */

    SVM_REAL gamma;	/* for poly/rbf/sigmoid */
    SVM_REAL coef0;	/* for poly/sigmoid */

    /* these are for training only */
    int cacheSize; /* in byte */
    SVM_REAL eps;	/* stopping criteria */
    SVM_REAL C;	/* for C_SVC, EPSILON_SVR and NU_SVR */


    SVM_REAL nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
    SVM_REAL p;	/* for EPSILON_SVR */
};

struct svm_parameter_64b
{
    int svm_type;
    int kernel_type;
    int degree;	/* for poly */
    int shrinking;	/* use the shrinking heuristics */
    int probability; /* do probability estimates */
    int nr_weight;		/* for C_SVC */
    int* weight_label;	/* for C_SVC */
    double gamma;	/* for poly/rbf/sigmoid */
    double coef0;	/* for poly/sigmoid */
    /* these are for training only */
    double cacheSizeKB; /* in KB */
    double eps;	/* stopping criteria */
    double C;	/* for C_SVC, EPSILON_SVR and NU_SVR */
    double* weight;		/* for C_SVC */
    double nu;	/* for NU_SVC, ONE_CLASS, and NU_SVR */
    double p;	/* for EPSILON_SVR */

};

//
// svm_model
//
enum svm_model_mem_type
{
    kModelMemAllocDuringTraining = 0,
    kModelMemMapped = 1,	// model is already trained, and saved to RAM or Flash
    kModelMemMalloc = 2, // model is loaded from file system and malloc its content pointers
};
#define SVM_MODEL_MAGIC (0x53564d00) // "SVM"
struct svm_model
{
    uint32_t model_magic;
    uint32_t model_tag;
    uint8_t isToDelArena;
    uint8_t memAllocType;		/* 1 if svm_model is loaded from flash or file system*/
    uint8_t nr_class;		/* number of classes, = 2 in regression/one class svm */
    uint8_t isSVCopied;
    uint16_t vecDim;	// rocky: add vecDim to avoid using sparse vectors
    short n_SV;			/* total #SV */
    struct svm_parameter param;	/* parameter */
    // these pointers are not content pointers
    char* pArena;	// records pair stack object, does not make sense for loaded models
    CPairStack* pStk;	// records pair stack object, does not make sense for loaded models
    // rockyS: base addr of this struct, useful for transmit models with network: After you
    // received the model data, call svm_relocate_model to the buffer you hold the model data.
    const struct svm_model* pBase;
    // all content pointers go below, easier to load from flash and relocate content pointers
    char* emb_ptr_start;
    struct svm_node** SVs;		/* SVs (SV[l]) */
    SVM_REAL** sv_coef;	/* coefficients for SVs in decision functions (sv_coef[k-1][l]) */
    SVM_REAL* rho;		/* constants in decision functions (rho[k*(k-1)/2]) */
#if SVM_CFG_FEATURE_PROBABILITY != 0
    SVM_REAL* probA;		/* pariwise probability information */
    SVM_REAL* probB;
    SVM_REAL* prob_density_marks;	/* probability information for ONE_CLASS */
#endif
    int* sv_indices;        /* sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data] to indicate SVs in the training set */

    /* for classification only */
    int* label;		/* label of each class (label[k]) */
    int* nSV;		/* number of SVs for each class (nSV[k]) */
    /* nSV[0] + nSV[1] + ... + nSV[k-1] = l */
    /* XXX */
    // this structure and all content pointers allocate RAM from a software empty-upgrowing stack,
    // and we record the end address of all stuffs with allocating a zero size block
    char* pModelTotalMemEnd;

    char* emb_ptr_end;
};

// rocky: if you want to use the model after input data has gone (such as saved model and
// reset your board), then you have to set isCopySV=true, this will allocates from EU_Stack
// for all support vectors.
struct svm_model* svm_train( const struct svm_problem* prob, const struct svm_parameter* param,
                             bool isCopySV = false, int arenaSize = SVM_CFG_CACHE_SIZE + 8192, void *pvArena = nullptr );

void svm_cross_validation( const struct svm_problem* prob, svm_model *model,
                           const struct svm_parameter* param, int nr_fold, SVM_REAL* target );

int svm_save_model( const char* model_file_name, const struct svm_model* model );

// rocky: if pMem is nullptr, then allocate from EU_Stack from model->pStk

struct svm_model* svm_copy_model( char* pMem, size_t memSize, struct svm_model* model,
                                  char isReloc, char isKeepTrainingMem );

// copy model to pBuffer, but model is intended to run from pRealDst, usually in Flash or memory in a
// remote device that receives the model from network.
struct svm_model* svm_copy_model_to_buffer( char* pBuffer, size_t bufferSize,
        char* pRealDst, struct svm_model* model );

//// rocky: relocate embedded pointers of a model to started from pDst.
//// isRelocateDst = 0 : Relocate source model, useful for prepairing model saving to Flash, pDst->Flash area
//// isRelocateDst = 1 : Relocate dest model by pDst, seldom called directly, use svm_copy_model instead.
//struct svm_model* svm_relocate_model(char* pDst, struct svm_model* model, uint8_t isRelocateDst);

// model_buf contains the memcpy of src_model data, and is designated to inference from pRelocDst
struct svm_model* svm_relocate_model( char* pRelocDst, char* model_buf, svm_model* src_model );

#define svm_model_total_size(model) ( (size_t)model->pModelTotalMemEnd - (size_t)model)

struct svm_model* svm_load_model( const char* model_file_name );

int svm_get_svm_type( const struct svm_model* model );

int svm_get_nr_class( const struct svm_model* model );

void svm_get_labels( const struct svm_model* model, int* label );

void svm_get_sv_indices( const struct svm_model* model, int* sv_indices );

int svm_get_nr_sv( const struct svm_model* model );

SVM_REAL svm_get_svr_probability( const struct svm_model* model );

SVM_REAL svm_predict_values( const struct svm_model* model,
                             const struct svm_node* x, SVM_REAL* dec_values );

SVM_REAL svm_predict( const struct svm_model* model, const struct svm_node* x );

SVM_REAL svm_predict_probability( const struct svm_model* model,
                                  const struct svm_node* x, SVM_REAL* prob_estimates );

void svm_free_model_content( struct svm_model* model_ptr );

void svm_free_and_destroy_model( struct svm_model** model_ptr_ptr );

void svm_destroy_param( struct svm_parameter* param );

const char* svm_check_parameter( const struct svm_problem* prob,
                                 const struct svm_parameter* param );

int svm_check_probability_model( const struct svm_model* model );

void svm_set_print_string_function( void ( *print_func )( const char* ) );


#endif /* _LIBSVM_H */

