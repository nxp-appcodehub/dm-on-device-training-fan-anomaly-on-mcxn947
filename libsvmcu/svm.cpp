#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <locale.h>

#include "svm.h"
#include "fp_16bits.h"
#include "l2bm.h"

using namespace std;
#if SVM_CFG_REAL_BITS == 64
#define RExp exp
#define RAbs fabs
#define RLog log
#define RSqrt sqrt
#define RStrToR strtod
#elif SVM_CFG_REAL_BITS == 32
#define RExp expf
#define RAbs fabsf
#define RLog logf
#define RSqrt sqrtf
#define RStrToR strtof
#elif SVM_CFG_REAL_BITS == 16
#error FP16 not yet supported
#endif

size_t g_newAcc = 0;
uint32_t g_allocNdx;
void* operator new( size_t count )
{
    g_newAcc += count;
    g_allocNdx++;
    return malloc( count );
}

void operator delete( void* mem )
{
    g_allocNdx--;
    return free( mem );
}

void* operator new[]( size_t count )
{
    g_newAcc += count;
    g_allocNdx++;
    return malloc( count );
}

void operator delete[]( void* mem )
{
    g_allocNdx--;
    return free( mem );
}

int libsvm_version = LIBSVM_VERSION;

typedef signed char schar;
#ifndef min
template <class T> static inline T min( T x, T y )
{
    return ( x < y ) ? x : y;
}
#endif
#ifndef max
template <class T> static inline T max( T x, T y )
{
    return ( x > y ) ? x : y;
}
#endif

#warning "Why delare a macro swap?"
#define swap(x,y) do {\
    __typeof__(x) t = x; \
    x = y; \
    y = t; \
}while(0)

//template <class T> static inline void swap( T& x, T& y )
//{
//    T t = x;
//    x = y;
//    y = t;
//}

template <class S, class T> static inline void clone( T*& dst, S* src, int n )
{
    dst = new T[n];
    memcpy( ( void* )dst, ( void* )src, sizeof( T ) * n );
}

template <class S, class T> static inline void Clone_FD_Stack( T*& dst, S* src, int n, CPairStack *pStk )
{
    dst = ( T* )( pStk->FD_Take( sizeof( T ) * n ) );
    memcpy( ( void* )dst, ( void* )src, sizeof( T ) * n );
}

template <class S, class T> static inline void Clone_EU_Stack( T*& dst, S* src, int n, CPairStack* pStk )
{
    dst = ( T* )( pStk->EU_Take( sizeof( T ) * n ) );
    memcpy( ( void* )dst, ( void* )src, sizeof( T ) * n );
}

static inline SVM_REAL powi( SVM_REAL base, int times )
{
    SVM_REAL tmp = base, ret = 1.0f;

    for ( int t = times; t > 0; t /= 2 )
    {
        if ( t % 2 == 1 ) ret *= tmp;
        tmp = tmp * tmp;
    }
    return ret;
}
#define INF HUGE_VAL
#define TAU 1e-12f

void* MyMalloc( size_t size )
{
    void* pRet = malloc( size );
    return pRet;
}

#define Malloc(type,n) (type *)MyMalloc((n)*sizeof(type))

#define FD_StackLock() model->pStk->FD_Lock()
#define FD_StackUnlock() model->pStk->FD_Unlock()
#define FD_StackTake(type,n) (type*) model->pStk->FD_Take((n)*sizeof(type))
#define FD_StackTakeChainHead(type,n) (type*) model->pStk->FD_TakeChainHead((n)*sizeof(type))
#define FD_StackGive(p) model->pStk->FD_Give(p)
#define FD_StackGiveChain() model->pStk->FD_GiveChain()

#define EU_StackLock() model->pStk->EU_Lock()
#define EU_StackUnlock() model->pStk->EU_Unlock()
#define EU_StackTake(type,n) (type*) model->pStk->EU_Take((n)*sizeof(type))
#define EU_StackTakeChainHead(type,n) (type*) model->pStk->EU_TakeChainHead((n)*sizeof(type))
#define EU_StackGive(p) model->pStk->EU_Give(p)
#define EU_StackGiveChain() model->pStk->EU_GiveChain()

#define GET_MAX_STACK_USED() model->pStk->GetMaxUsedSize()

static void print_string_stdout( const char* s )
{
    //fputs(s, stdout);
    //fflush(stdout);
}
static void ( *svm_print_string ) ( const char* ) = &print_string_stdout;
#if 0
static void info( const char* fmt, ... )
{
    char buf[1000];
    va_list ap;
    va_start( ap, fmt );
    vsprintf( buf, fmt, ap );
    va_end( ap );
    ( *svm_print_string )( buf );
}
#else
static void info( const char* fmt, ... ) {}
#endif


typedef void* ( *pfnAlloc_t )( size_t bytes );
typedef void ( *pfnFree_t )( void* pv );
#pragma pack(1)
typedef union tagCacheLineTag
{
    struct
    {
        uint16_t wayMap : 4;
        uint16_t hot : 4;
        uint16_t setMult : 7;
        uint16_t takenBit : 1;
    };
    int16_t i16Data;
} CacheLineTag_t;
#pragma pack()

typedef uint8_t LineFillLen_t;

class SetWayCache
{
protected:
    int m_setCnt;
    int m_wayPerSet;
    int m_lineSize;
    svm_model* model;
    pfnFree_t m_pfnFree;
    char* m_pMem;
    CacheLineTag_t* m_pTags;
    uint16_t* m_pLineMap;
    LineFillLen_t* m_pLens;
    Qfloat* m_pData;
    int m_dbgHitCnt;
    int m_dbgVctCnt;
    float m_dbgHitRate;
public:
    // A specialized Cache for acclerating Kernel Matrix access w/o saving all data
    // each cache line holds one row of {si1, si2, ..., siN} in NxN, N = sample count
    // Kernel function general form: K(si, sj)
    void _doSetWayCache( int unitPerLine, int setCnt, int wayPerSet,
                         svm_model *model_ )
    {
        m_setCnt = setCnt;
        m_wayPerSet = wayPerSet;
        m_lineSize = ( ( sizeof( Qfloat ) * unitPerLine + 3 ) & ~3 );
        model = model_;
        int lineCnt = setCnt * m_wayPerSet;
        int tagSize = lineCnt * sizeof( CacheLineTag_t ) + 3 & ~3;
        int lineMapSize = setCnt * wayPerSet * sizeof( uint16_t ) + 3 & ~3;
        int lenSize = lineCnt * sizeof( LineFillLen_t ) + 3 & ~3;
        int ctrlSize = tagSize + lineMapSize + lenSize;

        int dataSize = setCnt * wayPerSet * m_lineSize;
        int sizeRequired = ctrlSize + dataSize;
        if ( model == nullptr )
        {
            m_pMem = new char[sizeRequired];
        }
        else
        {
            m_pMem = FD_StackTake( char, sizeRequired );
        }

        m_pTags = ( CacheLineTag_t* )m_pMem;
        memset( m_pTags, 0, tagSize );

        m_pLineMap = ( uint16_t* )( m_pMem + tagSize );
        for ( int i = 0; i < lineCnt; i++ )
        {
            m_pLineMap[i] = i;
        }

        m_pLens = ( LineFillLen_t* )( m_pMem + tagSize + lineMapSize );

        m_pData = ( Qfloat* )( m_pMem + ctrlSize );

        m_dbgHitCnt = 0;
        m_dbgVctCnt = 0;
    }

    SetWayCache( int unitPerLine, int setCnt, int wayPerSet, svm_model* model = nullptr )
    {
        _doSetWayCache( unitPerLine, setCnt, wayPerSet, model );
    }

    void ManualConstruct( int sampCnt, int size, svm_model* model = nullptr )
    {
        // there are sampCnt elements per cache line
        int richSize = sampCnt * sampCnt * sizeof( Qfloat ) * 2 + 256;
        if ( richSize < size )
        {
            size = richSize;
        }
        int wayPerSet = SVM_CFG_CACHE_SETASSO_WAY_CNT;
        int lineSize = ( ( sizeof( Qfloat ) * sampCnt + 3 ) & ~3 );
        int setSize = wayPerSet * lineSize;
        int setCnt = ( int )size / ( setSize );
        int lineCnt;
        int tagSize, lineMapSize, lenSize,  ctrlSize, dataSize;
        do
        {
            lineCnt = setCnt * wayPerSet;
            tagSize = lineCnt * sizeof( CacheLineTag_t ) + 3 & ~3;
            lineMapSize = lineCnt * sizeof( uint16_t ) + 3 & ~3;
            lenSize = lineCnt * sizeof( LineFillLen_t ) + 3 & ~3;
            ctrlSize = tagSize + lineMapSize + lenSize;
            dataSize = setCnt * wayPerSet * lineSize;
        }
        while ( ctrlSize + dataSize > size && --setCnt );
        _doSetWayCache( sampCnt, setCnt, wayPerSet, model );
    }

    SetWayCache( int sampCnt, int size, svm_model *model = nullptr )
    {
        ManualConstruct( sampCnt, size, model );
    }

    int Lookup( const int index )
    {
        // look up
        int setNdx = ( index / m_wayPerSet ) % m_setCnt;
        CacheLineTag_t* pTag = m_pTags + setNdx * m_wayPerSet;
        int cacheLineNdx = -1;
        for ( int i = 0; i < m_wayPerSet; i++, pTag++ )
        {
            if ( pTag->i16Data < 0 )
            {
                if ( ( pTag->setMult * m_setCnt + setNdx ) * m_wayPerSet + pTag->wayMap == index )
                {
                    // cache hit
                    cacheLineNdx = setNdx * m_wayPerSet + i;
                    break;
                }
            }
        }
        return cacheLineNdx;
    }

    int FreeLine( int lineNdx )
    {
        m_pTags[lineNdx].i16Data = 0;
        return 0;
    }

    int get_data( const int index, Qfloat** data, int len, int *pCacheLineNdx = nullptr )
    {
        int setNdx = ( index / m_wayPerSet ) % m_setCnt;
        CacheLineTag_t* pTag = m_pTags + setNdx * m_wayPerSet;
        int freeWayNdx = -1;
        int minHot = 255, minHotAt = 0;
        int cacheLineNdx = -1;
        // look up
        for ( int i = 0; i < m_wayPerSet; i++, pTag++ )
        {
            if ( pTag->i16Data >= 0 )
            {
                // free slot
                if ( freeWayNdx < 0 )
                    freeWayNdx = i;
            }
            else
            {
                if ( ( pTag->setMult * m_setCnt + setNdx ) * m_wayPerSet + pTag->wayMap == index )
                {
                    // cache hit
                    m_dbgHitCnt++;
                    cacheLineNdx = setNdx * m_wayPerSet + i;
                    if ( len > m_pLens[cacheLineNdx] )
                    {
                        m_pLens[cacheLineNdx] = ( LineFillLen_t )len;
                        len = 0;
                    }
                    if ( pTag->hot < 15 )
                        pTag->hot++;
                    break;
                }
                else
                {
                    if ( pTag->hot < minHot )
                    {
                        minHot = pTag->hot;
                        minHotAt = i;
                    }
                }
            }
        }

        if ( cacheLineNdx < 0 )
        {
            // cache miss
            if ( freeWayNdx < 0 )
            {
                // kick out victim
                m_dbgVctCnt++;
                pTag = m_pTags + setNdx * m_wayPerSet;
                cacheLineNdx = setNdx * m_wayPerSet + minHotAt;
                // reset all hot values
                for ( int i = 0; i < m_wayPerSet; i++, pTag++ )
                {
                    pTag->hot >>= 1;
                }
            }
            else
            {
                // take free way
                //CacheLineTag_t* pTagBest = m_pTags + setNdx * m_wayPerSet + (index % m_wayPerSet);
                //int preferredFreeWayNdx = (index % m_wayPerSet);
                //if (freeWayNdx != preferredFreeWayNdx && pTagBest->i16Data >= 0)
                //	freeWayNdx = preferredFreeWayNdx;
                cacheLineNdx = setNdx * m_wayPerSet + freeWayNdx;
            }
            pTag = m_pTags + cacheLineNdx;
            pTag->hot = 1;
            pTag->setMult = index / m_wayPerSet / m_setCnt;
            pTag->wayMap = index % m_wayPerSet;
            pTag->takenBit = 1;
            m_pLens[cacheLineNdx] = ( LineFillLen_t )len;
            len = 0;
        }


        if ( data && data[0] )
            data[0] = ( Qfloat* )( ( char* )m_pData + m_pLineMap[cacheLineNdx] * m_lineSize );
        if ( pCacheLineNdx )
            pCacheLineNdx[0] = cacheLineNdx;
        return len;
    }

    void swap_index( int i, int j )
    {
        if ( i == j ) return;
        int iLine = Lookup( i );
        int jLine = Lookup( j );
        int tmp = -1, validLen = 0;
        if ( iLine < 0 && jLine < 0 )
            goto Label_SwapData;

        if ( iLine < 0 )
        {
            get_data( i, nullptr, m_pLens[jLine], &iLine );
            tmp = jLine;
        }
        else if ( jLine < 0 )
        {
            get_data( j, nullptr, m_pLens[iLine], &jLine );
            tmp = iLine;
        }

        if ( iLine != jLine )
        {
            swap( m_pLineMap[iLine], m_pLineMap[jLine] );
            swap( m_pLens[iLine], m_pLens[jLine] );
        }
        else
        {
            // The line containing data is the victim, so we already have got the swapped data,
            // do nothing
            iLine = iLine;
            tmp = -1;
        }

        if ( tmp >= 0 )
        {
            // release swapped
            m_pTags[tmp].i16Data = 0;
        }
Label_SwapData:
        int lineCnt = m_setCnt * m_wayPerSet;

        if ( i > j )
            swap( i, j );

        for ( int lnNdx = 0; lnNdx < lineCnt; lnNdx++ )
        {
            if ( m_pTags[lnNdx].i16Data >= 0 )
                continue;
            int fillLen = m_pLens[lnNdx];
            if ( fillLen > i )
            {
                if ( fillLen > j )
                {
                    Qfloat* pDat = ( Qfloat* )( ( char* )m_pData + m_pLineMap[lnNdx] * m_lineSize );
                    swap( pDat[i], pDat[j] );
                }
                else
                {
                    m_pTags[lnNdx].i16Data = 0;
                }
            }
        }

    }

    void ManualDestruct( void )
    {
        m_dbgHitRate = ( float )m_dbgHitCnt / ( m_dbgHitCnt + m_dbgVctCnt );
        if ( model )
        {
            if ( model->pStk->FD_GetBlockCnt() == 1 )
            {
                FD_StackGive( m_pMem );
            }
        }
        else
        {
            delete[] m_pMem;
        }
    }

    ~SetWayCache()
    {
        ManualDestruct();
    }

};

//
// Kernel Cache
//
// l is the number of total data items
// size is the cache size limit in bytes
//

#define GET_OBJECT_NDX(pHead, pObj) ( ((uint32_t)(pObj) - (uint32_t)(pHead)) / sizeof(*pObj) )
#if SVM_CFG_MAX_SAMP_CNT < 256
typedef uint8_t LRUIndex_t;
typedef uint16_t LRUComboIndex_t;
#else
typedef uint16_t LRUIndex_t;
typedef uint32_t LRUComboIndex_t;
#endif

#pragma pack(push)
#pragma pack(1)
struct head_t
{
    // head_t* prev, * next;	// a circular list
    union
    {
        struct
        {
            LRUIndex_t prevNdx, nextNdx;
        };
        LRUComboIndex_t ndxCombo;
    };

    // uint16_t nextNdx, prevNdx;
    LRUIndex_t ndxPlus1;	// data line index + 1, 0 means not yet allocated data
    // Qfloat* data;
    LRUIndex_t len;		// data[0,len) is cached in this entry
};
#pragma pack(pop)

class LRU_Cache
{
public:
    LRU_Cache( int l, int size, svm_model* model = nullptr );
    void ManualConstruct( int l, int size, svm_model* model = nullptr );
    void ManualDestruct();
    ~LRU_Cache();

    // request data [0,len)
    // return some position p where [p,len) need to be filled
    // (p >= len if nothing needs to be filled)
    int get_data( const int index, Qfloat** data, int len );
    void swap_index( int i, int j );
    const svm_model* model;

private:
    int sampCnt;
    char* pDataMem;
    L2BmCtrl_t* pL2BmCtl;
    // >>> rocky: stat
    int dbg_AskDataCnt;
    int dbg_AskDataLen;
    int dbg_SwapCnt;
    int dbg_HitCnt;
    int dbg_VctCnt;
    int size;

    head_t* heads; // rocky: 每个样本配一个head
    head_t* lru_head;

    //void lru_delete(head_t* h)
    //{
    //	// delete from current location
    //	//h->prev->next = h->next;
    //	//h->next->prev = h->prev;
    //	//h->next = h->prev = nullptr;
    //	uint16_t ndx = GET_OBJECT_NDX(heads, h);
    //	heads[h->prevNdx].nextNdx = h->nextNdx;
    //	heads[h->nextNdx].prevNdx = h->prevNdx;
    //	h->ndxCombo = 0;
    //}

    //void lru_insert(head_t* h)
    //{
    //	// insert to last position (one before lru_head)
    //	//h->next = lru_head;
    //	//h->prev = lru_head->prev;
    //	//h->prev->next = h;
    //	//h->next->prev = h;
    //	uint16_t ndx = GET_OBJECT_NDX(heads, h);
    //	h->nextNdx = sampCnt;
    //	h->prevNdx = lru_head->prevNdx;
    //	heads[h->prevNdx].nextNdx = ndx;
    //	lru_head->prevNdx = ndx;
    //}

    void lru_delete( LRUIndex_t ndx )
    {
        head_t* h = heads + ndx;
        // delete from current location
        heads[h->prevNdx].nextNdx = h->nextNdx;
        heads[h->nextNdx].prevNdx = h->prevNdx;
        h->ndxCombo = 0;

        //h->prev->next = h->next;
        //h->next->prev = h->prev;
        //h->next = h->prev = nullptr;
    }

    void lru_insert( LRUIndex_t ndx )
    {
        // insert to last position (one before lru_head)
        head_t* h = heads + ndx;
        h->nextNdx = sampCnt;
        h->prevNdx = lru_head->prevNdx;
        heads[h->prevNdx].nextNdx = ndx;
        lru_head->prevNdx = ndx;

        //h->next = lru_head;
        //h->prev = lru_head->prev;
        //h->prev->next = h;
        //h->next->prev = h;
    }

};

void LRU_Cache::ManualConstruct( int l_, int size_, svm_model* model_ )
{
    int i;
    model = model_;
    sampCnt = l_, size = size_;

    heads = FD_StackTake( head_t, sampCnt + 1 /* extra 1 for lru_head at the tail*/ );
    memset( heads, 0, sizeof( head_t ) * ( sampCnt + 1 ) );
    lru_head = heads + sampCnt;	// lru_head is appended to the tail of heads
    // heads = (head_t*)calloc(sampCnt, sizeof(head_t));	// initialized to 0
    // size -= sampCnt * sizeof(head_t) / sizeof(Qfloat);
    int minSize = 2 * sampCnt * sizeof( Qfloat ) + sizeof( L2BmCtrl_t ) + BM_WORDBYTES * 2;
    if ( size < minSize )
        size = minSize;

    char* pMem = FD_StackTake( char, size );
    pDataMem = pMem;
    int unitSize = sampCnt * sizeof( Qfloat );

    // lru_head->next = lru_head->prev = lru_head;
    lru_head->prevNdx = lru_head->nextNdx = sampCnt;
    // we assume the first some heads are hit, pre-allocate all memory
    for ( i = 0; i < sampCnt && ( i + 1 ) * unitSize <= size; i++, pMem += unitSize )
    {
        // heads[i].data = (Qfloat*)pMem;
        heads[i].ndxPlus1 = i + 1;
        lru_insert( i );
    }

    dbg_AskDataCnt = 0;
    dbg_AskDataLen = 0;
    dbg_SwapCnt = 0;
    dbg_HitCnt = 0;
    dbg_VctCnt = 0;
}

LRU_Cache::LRU_Cache( int l, int size, svm_model* model )
{
    ManualConstruct( l, size, model );
}

LRU_Cache::~LRU_Cache()
{
    ManualDestruct();
}

void LRU_Cache::ManualDestruct( void )
{
    float hitRate = ( float ) dbg_HitCnt / ( dbg_HitCnt + dbg_VctCnt );
    int dataFullRate = 100 * dbg_AskDataLen / ( dbg_AskDataCnt * sampCnt );
    //for (head_t* h = lru_head.next; h != &lru_head; h = h->next)
    //	free(h->data);
    //free(heads);
}

int LRU_Cache::get_data( const int index, Qfloat** data, int len )
{
    head_t* h = heads + index;
    dbg_AskDataCnt++;
    dbg_AskDataLen += len;
    if ( h->len )
    {
        dbg_HitCnt++;
        lru_delete( index );
        lru_insert( index );
    }
    else
    {
        if ( h->ndxPlus1 == 0 )
        {
            head_t* old = heads + lru_head->nextNdx;
            if ( old->len )
                dbg_VctCnt++;
            lru_delete( lru_head->nextNdx );
            h->ndxPlus1 = old->ndxPlus1;
            lru_insert( index );
            old->ndxPlus1 = 0;
            old->len = 0;
        }
        else
        {
            // this head is pre-allocated
            //if (h->next)
            lru_delete( index );
            lru_insert( index );
        }
    }

    h->len = len, len = 0;
    *data = ( Qfloat* ) ( pDataMem + sampCnt * sizeof( Qfloat ) * ( h->ndxPlus1 - 1 ) );
    return len;
}

void LRU_Cache::swap_index( int i, int j )
{
    if ( i == j ) return;
    dbg_SwapCnt++;
    if ( heads[i].ndxPlus1 ) lru_delete( i );
    if ( heads[j].ndxPlus1 ) lru_delete( j );
    swap( heads[i].ndxPlus1, heads[j].ndxPlus1 );
    swap( heads[i].len, heads[j].len );
    if ( heads[i].ndxPlus1 ) lru_insert( i );
    if ( heads[j].ndxPlus1 ) lru_insert( j );

    if ( i > j ) swap( i, j );
    int unitSize = sampCnt * sizeof( Qfloat );
    for ( head_t* h = heads + lru_head->nextNdx; h != lru_head; h = heads + h->nextNdx )
    {
        if ( h->len > i )
        {
            Qfloat* pData;
            pData = ( Qfloat* )( pDataMem + unitSize * ( h->ndxPlus1 - 1 ) );
            if ( h->len > j )
                swap( pData[i], pData[j] );
            else
            {
                // give up
                h->len = 0;
            }
        }
    }
}

#if SVM_CFG_CACHE_SEL == SVM_CACHE_SETASSO
#define Cache SetWayCache
#else
#define Cache LRU_Cache
#endif
//
// Kernel evaluation
//
// the static method k_function is for doing single kernel evaluation
// the constructor of Kernel prepares to calculate the l*l kernel matrix
// the member function get_Q is for getting one column from the Q Matrix
//
class QMatrix
{
protected:
public:
    svm_model* model;
    virtual Qfloat* get_Q( int column, int len ) const = 0;
    virtual SVM_REAL* get_QD() const = 0;
    virtual void swap_index( int i, int j ) const = 0;
    virtual ~QMatrix() {}
    QMatrix( svm_model* model ) : model( model ) {}

};

class Kernel : public QMatrix
{
public:
    Kernel( int l, const svm_problem &prob, const svm_parameter& param, svm_model *model );
    virtual ~Kernel();

    static SVM_REAL k_function( const svm_node* x, const svm_node* y,
                                const svm_parameter& param, int vecDim );
    virtual Qfloat* get_Q( int column, int len ) const = 0;
    virtual SVM_REAL* get_QD() const = 0;
    virtual void swap_index( int i, int j ) const	// no so const...
    {
        swap( x[i], x[j] );
        if ( x_square ) swap( x_square[i], x_square[j] );
    }
protected:
    SVM_REAL( Kernel::* kernel_function )( int i, int j ) const;

    svm_node** x;
    SHORT_REAL* x_square;
    // svm_model* model;
    // svm_parameter
    const int vecDim;
    const int kernel_type;
    const int degree;
    const SVM_REAL gamma;
    const SVM_REAL coef0;

    static SVM_REAL dot( const svm_node* px, const svm_node* py, int vecDim )
    {
        SVM_REAL sum = 0;
#if SVM_CFG_DENSE_VECTOR != 0
        for ( int i = 0; i < vecDim; i++ )
        {
            sum += px->value * py->value;
            px++, py++;
        }
#else
        while ( px->index != -1 && py->index != -1 )
        {
            if ( px->index == py->index )
            {
                sum += px->value * py->value;
                ++px;
                ++py;
            }
            else
            {
                if ( px->index > py->index )
                    ++py;
                else
                    ++px;
            }
        }
#endif
        return sum;
    }
    SVM_REAL dot( const svm_node* px, const svm_node* py ) const
    {
        return Kernel::dot( px, py, vecDim );
    }
    SVM_REAL kernel_linear( int i, int j ) const
    {
        return dot( x[i], x[j] );
    }
    SVM_REAL kernel_poly( int i, int j ) const
    {
        return powi( gamma * dot( x[i], x[j] ) + coef0, degree );
    }
    SVM_REAL kernel_rbf( int i, int j ) const
    {
        return RExp( -gamma * ( ( float )x_square[i] + ( float )x_square[j] - 2 * dot( x[i], x[j] ) ) );
    }
    SVM_REAL kernel_sigmoid( int i, int j ) const
    {
        return ( SVM_REAL )tanh( gamma * dot( x[i], x[j] ) + coef0 );
    }
    SVM_REAL kernel_precomputed( int i, int j ) const
    {
        return x[i][( int )( x[j][0].value )].value;
    }
};

Kernel::Kernel( int l, const svm_problem &prob, const svm_parameter& param, svm_model *model )
    : vecDim( prob.vecDim ), kernel_type( param.kernel_type ), degree( param.degree ),
      gamma( param.gamma ), coef0( param.coef0 ), QMatrix( model )
{

    switch ( kernel_type )
    {
    case LINEAR:
        kernel_function = &Kernel::kernel_linear;
        break;
    case POLY:
        kernel_function = &Kernel::kernel_poly;
        break;
    case RBF:
        kernel_function = &Kernel::kernel_rbf;
        break;
    case SIGMOID:
        kernel_function = &Kernel::kernel_sigmoid;
        break;
    case PRECOMPUTED:
        kernel_function = &Kernel::kernel_precomputed;
        break;
    }

    x = prob.x;
    // clone(x, x_, l);
#ifdef SVM_FP16_DEBUG
    static uint32_t dbgCnt = 0;
    volatile float dbgErrAcc = 0;
    volatile float dbg1, dbg2, dbg3, dbgErrMean;
#endif
    if ( kernel_type == RBF )
    {
        x_square = FD_StackTakeChainHead( SHORT_REAL, l );
        for ( int i = 0; i < l; i++ )
        {
            x_square[i] = ( SHORT_REAL )( dot( x[i], x[i] ) );
#ifdef SVM_FP16_DEBUG
            dbg1 = dot( x[i], x[i] );
            dbg2 = ( float )x_square[i];
            dbg3 = dbg1 - dbg2;
            dbgErrAcc += dbg3;
            dbgErrMean = dbgErrAcc / ++dbgCnt;
#endif
        }
    }
    else
    {
        x_square = 0;
    }
}

Kernel::~Kernel()
{
    // delete[] x;
    // delete[] x_square;
    FD_StackGiveChain();
}

SVM_REAL Kernel::k_function( const svm_node* x, const svm_node* y,
                             const svm_parameter& param, int vecDim )
{
    switch ( param.kernel_type )
    {
    case LINEAR:
        return Kernel::dot( x, y, vecDim );
    case POLY:
        return powi( param.gamma * Kernel::dot( x, y, vecDim ) + param.coef0, param.degree );
    case RBF:
    {
        SVM_REAL sum = 0;
#if SVM_CFG_DENSE_VECTOR != 0
        for ( int i = 0; i < vecDim; i++ )
        {
            SVM_REAL d = x->value - y->value;
            sum += d * d;
            ++x;
            ++y;
        }
#else
        while ( x->index != -1 && y->index != -1 )
        {
            if ( x->index == y->index )
            {
                SVM_REAL d = x->value - y->value;
                sum += d * d;
                ++x;
                ++y;
            }
            else
            {
                if ( x->index > y->index )
                {
                    sum += y->value * y->value;
                    ++y;
                }
                else
                {
                    sum += x->value * x->value;
                    ++x;
                }
            }
        }

        while ( x->index != -1 )
        {
            sum += x->value * x->value;
            ++x;
        }

        while ( y->index != -1 )
        {
            sum += y->value * y->value;
            ++y;
        }
#endif
        return RExp( -param.gamma * sum );
    }
    case SIGMOID:
        return ( SVM_REAL )tanh( param.gamma * Kernel::dot( x, y, vecDim ) + param.coef0 );
    case PRECOMPUTED:  //x: test (validation), y: SV
        return x[( int )( y->value )].value;
    default:
        return 0;  // Unreachable
    }
}

// An SMO algorithm in Fan et al., JMLR 6(2005), p. 1889--1918
// Solves:
//
//	min 0.5f(\alpha^T Q \alpha) + p^T \alpha
//
//		y^T \alpha = \delta
//		y_i = +1 or -1
//		0 <= alpha_i <= Cp for y_i = 1
//		0 <= alpha_i <= Cn for y_i = -1
//
// Given:
//
//	Q, p, y, Cp, Cn, and an initial feasible point \alpha
//	l is the size of vectors and matrices
//	eps is the stopping tolerance
//
// solution will be put in \alpha, objective value will be put in obj
//
class Solver
{
public:
    Solver() {};
    virtual ~Solver() {};

    struct SolutionInfo
    {
        SVM_REAL obj;
        SVM_REAL rho;
        SVM_REAL upper_bound_p;
        SVM_REAL upper_bound_n;
        SVM_REAL r;	// for Solver_NU
    };

    void Solve( int l, const QMatrix& Q, SVM_REAL* p_, schar* y_,
                SVM_REAL* alpha_, SVM_REAL Cp, SVM_REAL Cn, SVM_REAL eps,
                SolutionInfo* si, int shrinking, uint32_t oriClassCnt );
protected:
    int active_size;
    schar* y;
    SVM_REAL* G;		// gradient of objective function
    enum { LOWER_BOUND, UPPER_BOUND, MOVING };
    char* alpha_status;	// LOWER_BOUND, UPPER_BOUND, MOVING
    SVM_REAL* alpha;
    const QMatrix* Q;
    const SVM_REAL* QD;
    SVM_REAL eps;
    SVM_REAL Cp, Cn;
    SVM_REAL* p;
    uint16_t* active_set;
    SVM_REAL* G_bar;		// gradient, if we treat free variables as 0
    int l;
    bool unshrink;	// XXX
    bool oriClassCnt;

    SVM_REAL get_C( int i )
    {
        return ( y[i] > 0 ) ? Cp : Cn;
    }
    void update_alpha_status( int i )
    {
        if ( alpha[i] >= get_C( i ) )
            alpha_status[i] = UPPER_BOUND;
        else if ( alpha[i] <= 0 )
            alpha_status[i] = LOWER_BOUND;
        else alpha_status[i] = MOVING;
    }
    bool is_upper_bound( int i )
    {
        return alpha_status[i] == UPPER_BOUND;
    }
    bool is_lower_bound( int i )
    {
        return alpha_status[i] == LOWER_BOUND;
    }
    bool is_moving( int i )
    {
        return alpha_status[i] == MOVING;
    }
    void swap_index( int i, int j );
    void reconstruct_gradient();
    virtual int select_working_set( int& i, int& j );
    virtual SVM_REAL calculate_rho();
    virtual void do_shrinking();
private:
    bool be_shrunk( int i, SVM_REAL Gmax1, SVM_REAL Gmax2 );
};

void Solver::swap_index( int i, int j )
{
    Q->swap_index( i, j );
    swap( y[i], y[j] );
    swap( G[i], G[j] );
    swap( alpha_status[i], alpha_status[j] );
    swap( alpha[i], alpha[j] );
    swap( p[i], p[j] );
    swap( active_set[i], active_set[j] );
    swap( G_bar[i], G_bar[j] );
}

void Solver::reconstruct_gradient()
{
    // reconstruct inactive elements of G from G_bar and free variables

    if ( active_size == l ) return;

    int i, j;
    int nr_moving = 0;

    for ( j = active_size; j < l; j++ )
        G[j] = G_bar[j] + p[j];

    for ( j = 0; j < active_size; j++ )
        if ( is_moving( j ) )
            nr_moving++;

    if ( 2 * nr_moving < active_size )
        info( "\nWARNING: using -h 0 may be faster\n" );

    if ( nr_moving * l > 2 * active_size * ( l - active_size ) )
    {
        for ( i = active_size; i < l; i++ )
        {
            const Qfloat* Q_i = Q->get_Q( i, active_size );
            for ( j = 0; j < active_size; j++ )
                if ( is_moving( j ) )
                    G[i] += alpha[j] * Q_i[j];
        }
    }
    else
    {
        for ( i = 0; i < active_size; i++ )
            if ( is_moving( i ) )
            {
                const Qfloat* Q_i = Q->get_Q( i, l );
                SVM_REAL alpha_i = alpha[i];
                for ( j = active_size; j < l; j++ )
                    G[j] += alpha_i * Q_i[j];
            }
    }
}

void Solver::Solve( int l, const QMatrix& Q, SVM_REAL* p_, schar* y_,
                    SVM_REAL* alpha_, SVM_REAL Cp, SVM_REAL Cn, SVM_REAL eps,
                    SolutionInfo* si, int shrinking, uint32_t oriClassCnt )
{

    svm_model* model = Q.model;
    FD_StackTakeChainHead( char, 0 );
    this->l = l;
    this->Q = &Q;
    QD = Q.get_QD();
    this->oriClassCnt = oriClassCnt;

    p = p_;
#if SVM_CFG_COPY_ALPHA != 0
    Clone_FD_Stack( alpha, alpha_, l, model->pStk );
#else
    alpha = alpha_;
#endif
    Clone_FD_Stack( y, y_, l, model->pStk );

    this->Cp = Cp;
    this->Cn = Cn;
    this->eps = eps;
    unshrink = false;

    // initialize alpha_status to lower bound (0)
    {
        alpha_status = FD_StackTake( char, l );
        // alpha_status = new char[l];
        for ( int i = 0; i < l; i++ )
            update_alpha_status( i );
    }

    // initialize active set (for shrinking)
    {
        active_set = FD_StackTake( uint16_t, l );
        for ( int i = 0; i < l; i++ )
            active_set[i] = i;
        active_size = l;
    }

    // initialize gradient
    {
        G = FD_StackTake( SVM_REAL, l );
        G_bar = FD_StackTake( SVM_REAL, l );
        int i;
        for ( i = 0; i < l; i++ )
        {
            G[i] = p[i];
            G_bar[i] = 0;
        }
        for ( i = 0; i < l; i++ )
            if ( !is_lower_bound( i ) )
            {
                const Qfloat* Q_i = Q.get_Q( i, l );
                SVM_REAL alpha_i = alpha[i];
                int j;
                for ( j = 0; j < l; j++ )
                    G[j] += alpha_i * Q_i[j];
                if ( is_upper_bound( i ) )
                    for ( j = 0; j < l; j++ )
                        G_bar[j] += get_C( i ) * Q_i[j];
            }
    }

    // optimization step

    int iter = 0;
    int max_iter = ::max( 10000000, l > INT_MAX / 100 ? INT_MAX : 100 * l );
    int counter = ::min( l, 1000 ) + 1;

    while ( iter < max_iter )
    {
        // show progress and do shrinking

        if ( --counter == 0 )
        {
            counter = ::min( l, 1000 );
            if ( shrinking ) do_shrinking();
            info( "." );
        }

        int i, j;
        if ( select_working_set( i, j ) != 0 )
        {
            // reconstruct the whole gradient
            reconstruct_gradient();
            // reset active set size and check
            active_size = l;
            info( "*" );
            if ( select_working_set( i, j ) != 0 )
                break;
            else
                counter = 1;	// do shrinking next iteration
        }

        ++iter;

        // update alpha[i] and alpha[j], handle bounds carefully

        const Qfloat* Q_i = Q.get_Q( i, active_size );
        const Qfloat* Q_j = Q.get_Q( j, active_size );

        SVM_REAL C_i = get_C( i );
        SVM_REAL C_j = get_C( j );

        SVM_REAL old_alpha_i = alpha[i];
        SVM_REAL old_alpha_j = alpha[j];

        if ( y[i] != y[j] )
        {
            SVM_REAL quad_coef = QD[i] + QD[j] + 2 * Q_i[j];
            if ( quad_coef <= 0 )
                quad_coef = TAU;
            SVM_REAL delta = ( -G[i] - G[j] ) / quad_coef;
            SVM_REAL diff = alpha[i] - alpha[j];
            alpha[i] += delta;
            alpha[j] += delta;

            if ( diff > 0 )
            {
                if ( alpha[j] < 0 )
                {
                    alpha[j] = 0;
                    alpha[i] = diff;
                }
            }
            else
            {
                if ( alpha[i] < 0 )
                {
                    alpha[i] = 0;
                    alpha[j] = -diff;
                }
            }
            if ( diff > C_i - C_j )
            {
                if ( alpha[i] > C_i )
                {
                    alpha[i] = C_i;
                    alpha[j] = C_i - diff;
                }
            }
            else
            {
                if ( alpha[j] > C_j )
                {
                    alpha[j] = C_j;
                    alpha[i] = C_j + diff;
                }
            }
        }
        else
        {
            SVM_REAL quad_coef = QD[i] + QD[j] - 2 * Q_i[j];
            if ( quad_coef <= 0 )
                quad_coef = TAU;
            SVM_REAL delta = ( G[i] - G[j] ) / quad_coef;
            SVM_REAL sum = alpha[i] + alpha[j];
            alpha[i] -= delta;
            alpha[j] += delta;

            if ( sum > C_i )
            {
                if ( alpha[i] > C_i )
                {
                    alpha[i] = C_i;
                    alpha[j] = sum - C_i;
                }
            }
            else
            {
                if ( alpha[j] < 0 )
                {
                    alpha[j] = 0;
                    alpha[i] = sum;
                }
            }
            if ( sum > C_j )
            {
                if ( alpha[j] > C_j )
                {
                    alpha[j] = C_j;
                    alpha[i] = sum - C_j;
                }
            }
            else
            {
                if ( alpha[i] < 0 )
                {
                    alpha[i] = 0;
                    alpha[j] = sum;
                }
            }
        }

        // update G

        SVM_REAL delta_alpha_i = alpha[i] - old_alpha_i;
        SVM_REAL delta_alpha_j = alpha[j] - old_alpha_j;

        for ( int k = 0; k < active_size; k++ )
        {
            G[k] += Q_i[k] * delta_alpha_i + Q_j[k] * delta_alpha_j;
        }

        // update alpha_status and G_bar

        {
            bool ui = is_upper_bound( i );
            bool uj = is_upper_bound( j );
            update_alpha_status( i );
            update_alpha_status( j );
            int k;
            if ( ui != is_upper_bound( i ) )
            {
                Q_i = Q.get_Q( i, l );
                if ( ui )
                    for ( k = 0; k < l; k++ )
                        G_bar[k] -= C_i * Q_i[k];
                else
                    for ( k = 0; k < l; k++ )
                        G_bar[k] += C_i * Q_i[k];
            }

            if ( uj != is_upper_bound( j ) )
            {
                Q_j = Q.get_Q( j, l );
                if ( uj )
                    for ( k = 0; k < l; k++ )
                        G_bar[k] -= C_j * Q_j[k];
                else
                    for ( k = 0; k < l; k++ )
                        G_bar[k] += C_j * Q_j[k];
            }
        }
    }

    if ( iter >= max_iter )
    {
        if ( active_size < l )
        {
            // reconstruct the whole gradient to calculate objective value
            reconstruct_gradient();
            active_size = l;
            info( "*" );
        }
        printf( "\nWARNING: reaching max number of iterations\n" );
    }

    // calculate rho

    si->rho = calculate_rho();

    // calculate objective value
    {
        SVM_REAL v = 0;
        int i;
        for ( i = 0; i < l; i++ )
            v += alpha[i] * ( G[i] + p[i] );

        si->obj = v / 2;
    }

    // put back the solution
    {
#if SVM_CFG_COPY_ALPHA != 0
        for ( int i = 0; i < l; i++ )
            alpha_[active_set[i]] = alpha[i];
#else
        // rocky: in place swap orders, slow but saves alpha memory
        // int nextMinNdx;
        for ( int i = 0; i < l - 1; i++ )
        {
            int j;
            for ( j = i; j < l; j++ )
            {
                if ( active_set[j] == i )
                {
                    // nextMinNdx = j;
                    break;
                }
            }
            if ( i != j )
            {
                swap( alpha[i], alpha[j] );
                swap( active_set[i], active_set[j] );

            }
        }
#endif
    }

    // juggle everything back
    /*{
    	for(int i=0;i<l;i++)
    		while(active_set[i] != i)
    			swap_index(i,active_set[i]);
    			// or Q.swap_index(i,active_set[i]);
    }*/

    si->upper_bound_p = Cp;
    si->upper_bound_n = Cn;

    info( "\noptimization finished, #iter = %d\n", iter );

#if SVM_CFG_COPY_ALPHA != 0
    // delete[] alpha;
#endif
    // delete[] y;
    // delete[] alpha_status;
    // delete[] active_set;
    // delete[] G;
    // delete[] G_bar;
    FD_StackGiveChain();
}

// return 1 if already optimal, return 0 otherwise
int Solver::select_working_set( int& out_i, int& out_j )
{
    // return i,j such that
    // i: maximizes -y_i * grad(f)_i, i in I_up(\alpha)
    // j: minimizes the decrease of obj value
    //    (if quadratic coefficeint <= 0, replace it with tau)
    //    -y_j*grad(f)_j < -y_i*grad(f)_i, j in I_low(\alpha)

    SVM_REAL Gmax = -INF;
    SVM_REAL Gmax2 = -INF;
    int Gmax_idx = -1;
    int Gmin_idx = -1;
    SVM_REAL obj_diff_min = INF;

    for ( int t = 0; t < active_size; t++ )
        if ( y[t] == +1 )
        {
            if ( !is_upper_bound( t ) )
                if ( -G[t] >= Gmax )
                {
                    Gmax = -G[t];
                    Gmax_idx = t;
                }
        }
        else
        {
            if ( !is_lower_bound( t ) )
                if ( G[t] >= Gmax )
                {
                    Gmax = G[t];
                    Gmax_idx = t;
                }
        }

    int i = Gmax_idx;
    const Qfloat* Q_i = NULL;
    if ( Gmax_idx != -1 ) // NULL Q_i not accessed: Gmax=-INF if i=-1
        Q_i = Q->get_Q( i, active_size );

    for ( int j = 0; j < active_size; j++ )
    {
        if ( y[j] == +1 )
        {
            if ( !is_lower_bound( j ) )
            {
                SVM_REAL grad_diff = Gmax + G[j];
                if ( G[j] >= Gmax2 )
                    Gmax2 = G[j];
                if ( grad_diff > 0 )
                {
                    SVM_REAL obj_diff;
                    SVM_REAL quad_coef = QD[i] + QD[j] - 2.0f * y[i] * Q_i[j];
                    if ( quad_coef > 0 )
                        obj_diff = -( grad_diff * grad_diff ) / quad_coef;
                    else
                        obj_diff = -( grad_diff * grad_diff ) / TAU;

                    if ( obj_diff <= obj_diff_min )
                    {
                        Gmin_idx = j;
                        obj_diff_min = obj_diff;
                    }
                }
            }
        }
        else
        {
            if ( !is_upper_bound( j ) )
            {
                SVM_REAL grad_diff = Gmax - G[j];
                if ( -G[j] >= Gmax2 )
                    Gmax2 = -G[j];
                if ( grad_diff > 0 )
                {
                    SVM_REAL obj_diff;
                    SVM_REAL quad_coef = QD[i] + QD[j] + 2.0f * y[i] * Q_i[j];
                    if ( quad_coef > 0 )
                        obj_diff = -( grad_diff * grad_diff ) / quad_coef;
                    else
                        obj_diff = -( grad_diff * grad_diff ) / TAU;

                    if ( obj_diff <= obj_diff_min )
                    {
                        Gmin_idx = j;
                        obj_diff_min = obj_diff;
                    }
                }
            }
        }
    }

    if ( Gmax + Gmax2 < eps || Gmin_idx == -1 )
        return 1;

    out_i = Gmax_idx;
    out_j = Gmin_idx;
    return 0;
}

bool Solver::be_shrunk( int i, SVM_REAL Gmax1, SVM_REAL Gmax2 )
{
    if ( is_upper_bound( i ) )
    {
        if ( y[i] == +1 )
            return( -G[i] > Gmax1 );
        else
            return( -G[i] > Gmax2 );
    }
    else if ( is_lower_bound( i ) )
    {
        if ( y[i] == +1 )
            return( G[i] > Gmax2 );
        else
            return( G[i] > Gmax1 );
    }
    else
        return( false );
}

void Solver::do_shrinking()
{
    int i;
    SVM_REAL Gmax1 = -INF;		// max { -y_i * grad(f)_i | i in I_up(\alpha) }
    SVM_REAL Gmax2 = -INF;		// max { y_i * grad(f)_i | i in I_low(\alpha) }

    // find maximal violating pair first
    for ( i = 0; i < active_size; i++ )
    {
        if ( y[i] == +1 )
        {
            if ( !is_upper_bound( i ) )
            {
                if ( -G[i] >= Gmax1 )
                    Gmax1 = -G[i];
            }
            if ( !is_lower_bound( i ) )
            {
                if ( G[i] >= Gmax2 )
                    Gmax2 = G[i];
            }
        }
        else
        {
            if ( !is_upper_bound( i ) )
            {
                if ( -G[i] >= Gmax2 )
                    Gmax2 = -G[i];
            }
            if ( !is_lower_bound( i ) )
            {
                if ( G[i] >= Gmax1 )
                    Gmax1 = G[i];
            }
        }
    }

    if ( unshrink == false && Gmax1 + Gmax2 <= eps * 10 )
    {
        unshrink = true;
        reconstruct_gradient();
        active_size = l;
        info( "*" );
    }

    for ( i = 0; i < active_size; i++ )
        if ( be_shrunk( i, Gmax1, Gmax2 ) )
        {
            active_size--;
            while ( active_size > i )
            {
                if ( !be_shrunk( active_size, Gmax1, Gmax2 ) )
                {
                    swap_index( i, active_size );
                    break;
                }
                active_size--;
            }
        }
}

SVM_REAL Solver::calculate_rho()
{
    SVM_REAL r;
    int nr_moving = 0;
    SVM_REAL ub = INF, lb = -INF, sum_free = 0;
    for ( int i = 0; i < active_size; i++ )
    {
        SVM_REAL yG = y[i] * G[i];

        if ( is_upper_bound( i ) )
        {
            if ( y[i] == -1 )
                ub = ::min( ub, yG );
            else
                lb = ::max( lb, yG );
        }
        else if ( is_lower_bound( i ) )
        {
            if ( y[i] == +1 )
                ub = ::min( ub, yG );
            else
                lb = ::max( lb, yG );
        }
        else
        {
            ++nr_moving;
            sum_free += yG;
        }
    }

    if ( nr_moving > 0 )
        r = sum_free / nr_moving;
    else
        r = ( ub + lb ) / 2;

    return r;
}

#if SVM_FEATURE_SOLVER_NU != 0
//
// Solver for nu-svm classification and regression
//
// additional constraint: e^T \alpha = constant
//
class Solver_NU : public Solver
{
public:
    Solver_NU() {}
    void Solve( int l, const QMatrix& Q, SVM_REAL* p, schar* y,
                SVM_REAL* alpha, SVM_REAL Cp, SVM_REAL Cn, SVM_REAL eps,
                SolutionInfo* si, int shrinking, uint32_t oriClassCnt )
    {
        this->si = si;
        Solver::Solve( l, Q, p, y, alpha, Cp, Cn, eps, si, shrinking, oriClassCnt );
    }
private:
    SolutionInfo* si;
    int select_working_set( int& i, int& j );
    SVM_REAL calculate_rho();
    bool be_shrunk( int i, SVM_REAL Gmax1, SVM_REAL Gmax2, SVM_REAL Gmax3, SVM_REAL Gmax4 );
    void do_shrinking();
};

// return 1 if already optimal, return 0 otherwise
int Solver_NU::select_working_set( int& out_i, int& out_j )
{
    // return i,j such that y_i = y_j and
    // i: maximizes -y_i * grad(f)_i, i in I_up(\alpha)
    // j: minimizes the decrease of obj value
    //    (if quadratic coefficeint <= 0, replace it with tau)
    //    -y_j*grad(f)_j < -y_i*grad(f)_i, j in I_low(\alpha)

    SVM_REAL Gmaxp = -INF;
    SVM_REAL Gmaxp2 = -INF;
    int Gmaxp_idx = -1;

    SVM_REAL Gmaxn = -INF;
    SVM_REAL Gmaxn2 = -INF;
    int Gmaxn_idx = -1;

    int Gmin_idx = -1;
    SVM_REAL obj_diff_min = INF;

    for ( int t = 0; t < active_size; t++ )
        if ( y[t] == +1 )
        {
            if ( !is_upper_bound( t ) )
                if ( -G[t] >= Gmaxp )
                {
                    Gmaxp = -G[t];
                    Gmaxp_idx = t;
                }
        }
        else
        {
            if ( !is_lower_bound( t ) )
                if ( G[t] >= Gmaxn )
                {
                    Gmaxn = G[t];
                    Gmaxn_idx = t;
                }
        }

    int ip = Gmaxp_idx;
    int in = Gmaxn_idx;
    const Qfloat* Q_ip = NULL;
    const Qfloat* Q_in = NULL;
    if ( ip != -1 ) // NULL Q_ip not accessed: Gmaxp=-INF if ip=-1
        Q_ip = Q->get_Q( ip, active_size );
    if ( in != -1 )
        Q_in = Q->get_Q( in, active_size );

    for ( int j = 0; j < active_size; j++ )
    {
        if ( y[j] == +1 )
        {
            if ( !is_lower_bound( j ) )
            {
                SVM_REAL grad_diff = Gmaxp + G[j];
                if ( G[j] >= Gmaxp2 )
                    Gmaxp2 = G[j];
                if ( grad_diff > 0 )
                {
                    SVM_REAL obj_diff;
                    SVM_REAL quad_coef = QD[ip] + QD[j] - 2 * Q_ip[j];
                    if ( quad_coef > 0 )
                        obj_diff = -( grad_diff * grad_diff ) / quad_coef;
                    else
                        obj_diff = -( grad_diff * grad_diff ) / TAU;

                    if ( obj_diff <= obj_diff_min )
                    {
                        Gmin_idx = j;
                        obj_diff_min = obj_diff;
                    }
                }
            }
        }
        else
        {
            if ( !is_upper_bound( j ) )
            {
                SVM_REAL grad_diff = Gmaxn - G[j];
                if ( -G[j] >= Gmaxn2 )
                    Gmaxn2 = -G[j];
                if ( grad_diff > 0 )
                {
                    SVM_REAL obj_diff;
                    SVM_REAL quad_coef = QD[in] + QD[j] - 2 * Q_in[j];
                    if ( quad_coef > 0 )
                        obj_diff = -( grad_diff * grad_diff ) / quad_coef;
                    else
                        obj_diff = -( grad_diff * grad_diff ) / TAU;

                    if ( obj_diff <= obj_diff_min )
                    {
                        Gmin_idx = j;
                        obj_diff_min = obj_diff;
                    }
                }
            }
        }
    }

    if ( max( Gmaxp + Gmaxp2, Gmaxn + Gmaxn2 ) < eps || Gmin_idx == -1 )
        return 1;

    if ( y[Gmin_idx] == +1 )
        out_i = Gmaxp_idx;
    else
        out_i = Gmaxn_idx;
    out_j = Gmin_idx;

    return 0;
}

bool Solver_NU::be_shrunk( int i, SVM_REAL Gmax1, SVM_REAL Gmax2, SVM_REAL Gmax3, SVM_REAL Gmax4 )
{
    if ( is_upper_bound( i ) )
    {
        if ( y[i] == +1 )
            return( -G[i] > Gmax1 );
        else
            return( -G[i] > Gmax4 );
    }
    else if ( is_lower_bound( i ) )
    {
        if ( y[i] == +1 )
            return( G[i] > Gmax2 );
        else
            return( G[i] > Gmax3 );
    }
    else
        return( false );
}

void Solver_NU::do_shrinking()
{
    SVM_REAL Gmax1 = -INF;	// max { -y_i * grad(f)_i | y_i = +1, i in I_up(\alpha) }
    SVM_REAL Gmax2 = -INF;	// max { y_i * grad(f)_i | y_i = +1, i in I_low(\alpha) }
    SVM_REAL Gmax3 = -INF;	// max { -y_i * grad(f)_i | y_i = -1, i in I_up(\alpha) }
    SVM_REAL Gmax4 = -INF;	// max { y_i * grad(f)_i | y_i = -1, i in I_low(\alpha) }

    // find maximal violating pair first
    int i;
    for ( i = 0; i < active_size; i++ )
    {
        if ( !is_upper_bound( i ) )
        {
            if ( y[i] == +1 )
            {
                if ( -G[i] > Gmax1 ) Gmax1 = -G[i];
            }
            else	if ( -G[i] > Gmax4 ) Gmax4 = -G[i];
        }
        if ( !is_lower_bound( i ) )
        {
            if ( y[i] == +1 )
            {
                if ( G[i] > Gmax2 ) Gmax2 = G[i];
            }
            else	if ( G[i] > Gmax3 ) Gmax3 = G[i];
        }
    }

    if ( unshrink == false && max( Gmax1 + Gmax2, Gmax3 + Gmax4 ) <= eps * 10 )
    {
        unshrink = true;
        reconstruct_gradient();
        active_size = l;
    }

    for ( i = 0; i < active_size; i++ )
        if ( be_shrunk( i, Gmax1, Gmax2, Gmax3, Gmax4 ) )
        {
            active_size--;
            while ( active_size > i )
            {
                if ( !be_shrunk( active_size, Gmax1, Gmax2, Gmax3, Gmax4 ) )
                {
                    swap_index( i, active_size );
                    break;
                }
                active_size--;
            }
        }
}

SVM_REAL Solver_NU::calculate_rho()
{
    int nr_moving1 = 0, nr_moving2 = 0;
    SVM_REAL ub1 = INF, ub2 = INF;
    SVM_REAL lb1 = -INF, lb2 = -INF;
    SVM_REAL sum_free1 = 0, sum_free2 = 0;

    for ( int i = 0; i < active_size; i++ )
    {
        if ( y[i] == +1 )
        {
            if ( is_upper_bound( i ) )
                lb1 = max( lb1, G[i] );
            else if ( is_lower_bound( i ) )
                ub1 = min( ub1, G[i] );
            else
            {
                ++nr_moving1;
                sum_free1 += G[i];
            }
        }
        else
        {
            if ( is_upper_bound( i ) )
                lb2 = max( lb2, G[i] );
            else if ( is_lower_bound( i ) )
                ub2 = min( ub2, G[i] );
            else
            {
                ++nr_moving2;
                sum_free2 += G[i];
            }
        }
    }

    SVM_REAL r1, r2;
    if ( nr_moving1 > 0 )
        r1 = sum_free1 / nr_moving1;
    else
        r1 = ( ub1 + lb1 ) / 2;

    if ( nr_moving2 > 0 )
        r2 = sum_free2 / nr_moving2;
    else
        r2 = ( ub2 + lb2 ) / 2;

    si->r = ( r1 + r2 ) / 2;
    return ( r1 - r2 ) / 2;
}
#endif

#if SVM_FEATURE_SOLVER_SVC != 0
//
// Q matrices for various formulations
//
class SVC_Q : public Kernel
{
public:

    SVC_Q( const svm_problem& prob, const svm_parameter& param, svm_model *model, const schar* y_ )
        : Kernel( prob.n_pts, prob, param, model )
    {
        FD_StackTakeChainHead( int, 1 );
        Clone_FD_Stack( y, y_, prob.n_pts, model->pStk );
        // clone(y, y_, prob.n_pts);
        QD = FD_StackTake( SVM_REAL, prob.n_pts );
        cache = FD_StackTake( Cache, 1 );
        // cache = new Cache(prob.n_pts, (long int)(param.cacheSize));
        cache->ManualConstruct( prob.n_pts, param.cacheSize, model );


        // FD_StackLock();


        for ( int i = 0; i < prob.n_pts; i++ )
            QD[i] = ( this->*kernel_function )( i, i );
        QD[0] = QD[0];
    }

    Qfloat* get_Q( int i, int len ) const
    {
        Qfloat* data;
        int start, j;
        if ( ( start = cache->get_data( i, &data, len ) ) < len )
        {
            for ( j = start; j < len; j++ )
                data[j] = ( Qfloat )( y[i] * y[j] * ( this->*kernel_function )( i, j ) );
        }
        return data;
    }

    SVM_REAL* get_QD() const
    {
        return QD;
    }

    void swap_index( int i, int j ) const
    {
        cache->swap_index( i, j );
        Kernel::swap_index( i, j );
        swap( y[i], y[j] );
        swap( QD[i], QD[j] );
    }

    ~SVC_Q()
    {
        // FD_StackUnlock();
        cache->ManualDestruct();
        FD_StackGiveChain();
        // delete[] y;
        // delete cache;
        // delete[] QD;
    }
private:
    schar* y;
    Cache* cache;
    SVM_REAL* QD;
};
#endif

#if SVM_CFG_FEATURE_SOLVER_ONE_CLASS != 0
class ONE_CLASS_Q : public Kernel
{
public:
    ONE_CLASS_Q( const svm_problem& prob, const svm_parameter& param, svm_model *model )
        : Kernel( prob.n_pts, prob, param, model )
    {
        QD = FD_StackTakeChainHead( SVM_REAL, prob.n_pts );
        cache = FD_StackTake( Cache, 1 );
        // cache = new Cache(prob.n_pts, (long int)(param.cacheSize));
        cache->ManualConstruct( prob.n_pts, param.cacheSize, model );
        // FD_StackLock();
        for ( int i = 0; i < prob.n_pts; i++ )
            QD[i] = ( this->*kernel_function )( i, i );
    }

    Qfloat* get_Q( int i, int len ) const
    {
        Qfloat* data;
        int start, j;
        if ( ( start = cache->get_data( i, &data, len ) ) < len )
        {
            for ( j = start; j < len; j++ )
                data[j] = ( Qfloat )( this->*kernel_function )( i, j );
        }
        return data;
    }

    SVM_REAL* get_QD() const
    {
        return QD;
    }

    void swap_index( int i, int j ) const
    {
        cache->swap_index( i, j );
        Kernel::swap_index( i, j );
        swap( QD[i], QD[j] );
    }

    ~ONE_CLASS_Q()
    {
        // FD_StackUnlock();
        cache->ManualDestruct();
        FD_StackGiveChain();
        // delete cache;
        // delete[] QD;
    }
private:
    Cache* cache;
    SVM_REAL* QD;
};
#endif

#if SVM_FEATURE_SOLVER_SVR != 0
class SVR_Q : public Kernel
{
public:
    SVR_Q( const svm_problem& prob, const svm_parameter& param, svm_model *model )
        : Kernel( prob.n_pts, prob, param, model )
    {
        l = prob.n_pts;
        //cache = new Cache(l, (long int)(param.cacheSize));
        //QD = new SVM_REAL[2 * l];
        //sign = new schar[2 * l];
        //index = new int[2 * l];

        QD = FD_StackTakeChainHead( SVM_REAL, 2 * l );
        sign = FD_StackTake( schar, 2 * l );
        index = FD_StackTake( int, 2 * l );
        cache = FD_StackTake( Cache, 1 );
        // cache = new Cache(prob.n_pts, (long int)(param.cacheSize));
        buffer[0] = FD_StackTake( Qfloat, 2 * l );
        buffer[1] = FD_StackTake( Qfloat, 2 * l );
        cache->ManualConstruct( prob.n_pts, param.cacheSize, model );
        // FD_StackLock();

        for ( int k = 0; k < l; k++ )
        {
            sign[k] = 1;
            sign[k + l] = -1;
            index[k] = k;
            index[k + l] = k;
            QD[k] = ( this->*kernel_function )( k, k );
            QD[k + l] = QD[k];
        }
        //buffer[0] = new Qfloat[2 * l];
        //buffer[1] = new Qfloat[2 * l];
        next_buffer = 0;
    }

    void swap_index( int i, int j ) const
    {
        swap( sign[i], sign[j] );
        swap( index[i], index[j] );
        swap( QD[i], QD[j] );
    }

    Qfloat* get_Q( int i, int len ) const
    {
        Qfloat* data;
        int j, real_i = index[i];
        if ( cache->get_data( real_i, &data, l ) < l )
        {
            for ( j = 0; j < l; j++ )
                data[j] = ( Qfloat )( this->*kernel_function )( real_i, j );
        }

        // reorder and copy
        Qfloat* buf = buffer[next_buffer];
        next_buffer = 1 - next_buffer;
        schar si = sign[i];
        for ( j = 0; j < len; j++ )
            buf[j] = ( Qfloat )si * ( Qfloat )sign[j] * data[index[j]];
        return buf;
    }

    SVM_REAL* get_QD() const
    {
        return QD;
    }

    ~SVR_Q()
    {
        // FD_StackUnlock();
        cache->ManualDestruct();
        FD_StackGiveChain();
        //delete cache;
        //delete[] sign;
        //delete[] index;
        //delete[] buffer[0];
        //delete[] buffer[1];
        //delete[] QD;
    }
private:
    int l;
    Cache* cache;
    schar* sign;
    int* index;
    mutable int next_buffer;
    Qfloat* buffer[2];
    SVM_REAL* QD;
};
#endif

#if SVM_CFG_FEATURE_SOLVER_C_SVC != 0
//
// construct and solve various formulations
//
static void solve_c_svc(
    const svm_problem* prob, const svm_parameter* param, svm_model* model, SVM_REAL* alpha,
    Solver::SolutionInfo* si, SVM_REAL Cp, SVM_REAL Cn, uint32_t oriClassCnt )
{
    int l = prob->n_pts;
    //SVM_REAL* minus_ones = new SVM_REAL[l];
    //schar* y = new schar[l];

    SVM_REAL* minus_ones = FD_StackTakeChainHead( SVM_REAL, l );
    schar* y = FD_StackTake( schar, l );


    int i;

    for ( i = 0; i < l; i++ )
    {
        alpha[i] = 0;
        minus_ones[i] = -1;
        if ( prob->y[i] > 0 ) y[i] = +1;
        else y[i] = -1;
    }

    Solver s;
    s.Solve( l, SVC_Q( *prob, *param, model, y ), minus_ones, y,
             alpha, Cp, Cn, param->eps, si, param->shrinking, oriClassCnt );

    SVM_REAL sum_alpha = 0;
    for ( i = 0; i < l; i++ )
        sum_alpha += alpha[i];

    if ( Cp == Cn )
        info( "nu = %f\n", sum_alpha / ( Cp * prob->n_pts ) );

    for ( i = 0; i < l; i++ )
        alpha[i] *= y[i];

    // delete[] minus_ones;
    // delete[] y;
    FD_StackGiveChain();
}
#endif

#if SVM_CFG_FEATURE_SOLVER_NU_SVC != 0
static void solve_nu_svc(
    const svm_problem* prob, const svm_parameter* param, svm_model* model,
    SVM_REAL* alpha, Solver::SolutionInfo* si, uint32_t oriClassCnt )
{
    int i;
    int l = prob->n_pts;
    SVM_REAL nu = param->nu;
    SVM_REAL* zeros = FD_StackTakeChainHead( SVM_REAL, l );
    schar* y = FD_StackTake( schar, l );

    for ( i = 0; i < l; i++ )
        if ( prob->y[i] > 0 )
            y[i] = +1;
        else
            y[i] = -1;

    SVM_REAL sum_pos = nu * l / 2;
    SVM_REAL sum_neg = nu * l / 2;

    for ( i = 0; i < l; i++ )
        if ( y[i] == +1 )
        {
#if SVM_CFG_REAL_BITS == 64
            alpha[i] = min( 1.0, sum_pos );
#else
            alpha[i] = min( 1.0f, sum_pos );
#endif
            sum_pos -= alpha[i];
        }
        else
        {
#if SVM_CFG_REAL_BITS == 64
            alpha[i] = min( 1.0, sum_neg );
#else
            alpha[i] = min( 1.0f, sum_neg );
#endif
            sum_neg -= alpha[i];
        }

    for ( i = 0; i < l; i++ )
        zeros[i] = 0;

    Solver_NU s;
    s.Solve( l, SVC_Q( *prob, *param, model, y ), zeros, y,
             alpha, 1.0f, 1.0f, param->eps, si, param->shrinking, oriClassCnt );
    SVM_REAL r = si->r;

    info( "C = %f\n", 1 / r );

    for ( i = 0; i < l; i++ )
        alpha[i] *= y[i] / r;

    si->rho /= r;
    si->obj /= ( r * r );
    si->upper_bound_p = 1 / r;
    si->upper_bound_n = 1 / r;

    FD_StackGiveChain();
    // delete[] y;
    // delete[] zeros;
}
#endif

#if SVM_CFG_FEATURE_SOLVER_ONE_CLASS != 0
static void solve_one_class(
    const svm_problem* prob, const svm_parameter* param, svm_model* model,
    SVM_REAL* alpha, Solver::SolutionInfo* si, uint32_t oriClassCnt )
{
    int l = prob->n_pts;
    //SVM_REAL* zeros = new SVM_REAL[l];
    //schar* ones = new schar[l];

    SVM_REAL* zeros = FD_StackTakeChainHead( SVM_REAL, l );
    schar* ones = FD_StackTake( schar, l );
    int i;

    int n = ( int )( param->nu * prob->n_pts );	// # of alpha's at upper bound

    for ( i = 0; i < n; i++ )
        alpha[i] = 1;
    if ( n < prob->n_pts )
        alpha[n] = param->nu * prob->n_pts - n;
    for ( i = n + 1; i < l; i++ )
        alpha[i] = 0;

    for ( i = 0; i < l; i++ )
    {
        zeros[i] = 0;
        ones[i] = 1;
    }

    Solver s;
    s.Solve( l, ONE_CLASS_Q( *prob, *param, model ), zeros, ones,
             alpha, 1.0f, 1.0f, param->eps, si, param->shrinking, 1 );

    // delete[] zeros;
    // delete[] ones;
    FD_StackGiveChain();
}
#endif

#if SVM_CFG_FEATURE_SOLVER_EPSILON_SVR != 0
static void solve_epsilon_svr(
    const svm_problem* prob, const svm_parameter* param, svm_model *model,
    SVM_REAL* alpha, Solver::SolutionInfo* si, uint32_t oriClassCnt )
{
    int l = prob->n_pts;
    SVM_REAL* alpha2 = FD_StackTakeChainHead( SVM_REAL, 2 * l );
    SVM_REAL* linear_term = FD_StackTake( SVM_REAL, 2 * l );
    schar* y = FD_StackTake( schar, 2 * l );
    int i;

    for ( i = 0; i < l; i++ )
    {
        alpha2[i] = 0;
        linear_term[i] = param->p - prob->y[i];
        y[i] = 1;

        alpha2[i + l] = 0;
        linear_term[i + l] = param->p + prob->y[i];
        y[i + l] = -1;
    }

    Solver s;
    s.Solve( 2 * l, SVR_Q( *prob, *param, model ), linear_term, y,
             alpha2, param->C, param->C, param->eps, si, param->shrinking, oriClassCnt );

    SVM_REAL sum_alpha = 0;
    for ( i = 0; i < l; i++ )
    {
        alpha[i] = alpha2[i] - alpha2[i + l];
        sum_alpha += RAbs( alpha[i] );
    }
    info( "nu = %f\n", sum_alpha / ( param->C * l ) );

    // delete[] alpha2;
    // delete[] linear_term;
    // delete[] y;
    FD_StackGiveChain();
}
#endif

#if SVM_CFG_FEATURE_SOLVER_NU_SVR != 0
static void solve_nu_svr(
    const svm_problem* prob, const svm_parameter* param, svm_model* model,
    SVM_REAL* alpha, Solver::SolutionInfo* si, uint32_t oriClassCnt )
{
    int l = prob->n_pts;
    SVM_REAL C = param->C;
    SVM_REAL* alpha2 = FD_StackTakeChainHead( SVM_REAL, 2 * l );
    SVM_REAL * linear_term = FD_StackTake( SVM_REAL, 2 * l );
    schar* y = FD_StackTake( schar, 2 * l );
    int i;

    SVM_REAL sum = C * param->nu * l / 2;
    for ( i = 0; i < l; i++ )
    {
        alpha2[i] = alpha2[i + l] = min( sum, C );
        sum -= alpha2[i];

        linear_term[i] = -prob->y[i];
        y[i] = 1;

        linear_term[i + l] = prob->y[i];
        y[i + l] = -1;
    }

    Solver_NU s;
    s.Solve( 2 * l, SVR_Q( *prob, *param, model ), linear_term, y,
             alpha2, C, C, param->eps, si, param->shrinking, oriClassCnt );

    info( "epsilon = %f\n", -si->r );

    for ( i = 0; i < l; i++ )
        alpha[i] = alpha2[i] - alpha2[i + l];

    FD_StackGiveChain();
}
#endif
//
// decision_function
//
struct decision_function
{
    SVM_REAL* alpha;
    SVM_REAL rho;
};

static decision_function svm_train_one(
    const svm_problem* prob, const svm_parameter* param, SVM_REAL Cp, SVM_REAL Cn,
    svm_model *model )
{
    int oriClassCnt = model->nr_class;
    SVM_REAL* alpha = FD_StackTake( SVM_REAL, prob->n_pts );
    Solver::SolutionInfo si;

    switch ( param->svm_type )
    {
#if SVM_CFG_FEATURE_SOLVER_C_SVC != 0
    case C_SVC:
        solve_c_svc( prob, param, model, alpha, &si, Cp, Cn, oriClassCnt );
        break;
#endif
#if SVM_CFG_FEATURE_SOLVER_NU_SVC != 0
    case NU_SVC:
        solve_nu_svc( prob, param, model, alpha, &si, oriClassCnt );
        break;
#endif
#if SVM_CFG_FEATURE_SOLVER_NU_SVR != 0
    case NU_SVR:
        solve_nu_svr( prob, param, model, alpha, &si, oriClassCnt );
        break;
#endif
    case ONE_CLASS:
        solve_one_class( prob, param, model, alpha, &si, oriClassCnt );
        break;
#if SVM_CFG_FEATURE_SOLVER_EPSILON_SVR != 0
    case EPSILON_SVR:
        solve_epsilon_svr( prob, param, model, alpha, &si, oriClassCnt );
        break;
#endif
    default:
        staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "Unsupported feature" );
        while ( 1 ) {}
    }

    info( "obj = %f, rho = %f\n", si.obj, si.rho );

    // output SVs

    int nSV = 0;
    int nBSV = 0;
    for ( int i = 0; i < prob->n_pts; i++ )
    {
        if ( RAbs( alpha[i] ) > 0 )
        {
            ++nSV;
            if ( prob->y[i] > 0 )
            {
                if ( RAbs( alpha[i] ) >= si.upper_bound_p )
                    ++nBSV;
            }
            else
            {
                if ( RAbs( alpha[i] ) >= si.upper_bound_n )
                    ++nBSV;
            }
        }
    }

    info( "nSV = %d, nBSV = %d\n", nSV, nBSV );

    decision_function f;
    f.alpha = alpha;
    f.rho = si.rho;
    return f;
}

// Platt's binary SVM Probablistic Output: an improvement from Lin et al.
static void sigmoid_train(
    int l, const SVM_REAL* dec_values, const SVM_REAL* labels,
    SVM_REAL& A, SVM_REAL& B )
{
    SVM_REAL prior1 = 0, prior0 = 0;
    int i;

    for ( i = 0; i < l; i++ )
        if ( labels[i] > 0 ) prior1 += 1;
        else prior0 += 1;

    int max_iter = 100;	// Maximal number of iterations
    SVM_REAL min_step = 1e-10f;	// Minimal step taken in line search
    SVM_REAL sigma = 1e-12f;	// For numerically strict PD of Hessian
    SVM_REAL eps = 1e-5f;
    SVM_REAL hiTarget = ( prior1 + 1.0f ) / ( prior1 + 2.0f );
    SVM_REAL loTarget = 1 / ( prior0 + 2.0f );
    SVM_REAL* t = Malloc( SVM_REAL, l );
    SVM_REAL fApB, p, q, h11, h22, h21, g1, g2, det, dA, dB, gd, stepsize;
    SVM_REAL newA, newB, newf, d1, d2;
    int iter;

    // Initial Point and Initial Fun Value
    A = 0.0f;
    B = RLog( ( prior0 + 1.0f ) / ( prior1 + 1.0f ) );
    SVM_REAL fval = 0.0f;

    for ( i = 0; i < l; i++ )
    {
        if ( labels[i] > 0 ) t[i] = hiTarget;
        else t[i] = loTarget;
        fApB = dec_values[i] * A + B;
        if ( fApB >= 0 )
            fval += t[i] * fApB + RLog( 1 + RExp( -fApB ) );
        else
            fval += ( t[i] - 1 ) * fApB + RLog( 1 + RExp( fApB ) );
    }
    for ( iter = 0; iter < max_iter; iter++ )
    {
        // Update Gradient and Hessian (use H' = H + sigma I)
        h11 = sigma; // numerically ensures strict PD
        h22 = sigma;
        h21 = 0.0f;
        g1 = 0.0f;
        g2 = 0.0f;
        for ( i = 0; i < l; i++ )
        {
            fApB = dec_values[i] * A + B;
            if ( fApB >= 0 )
            {
                p = RExp( -fApB ) / ( 1.0f + RExp( -fApB ) );
                q = 1.0f / ( 1.0f + RExp( -fApB ) );
            }
            else
            {
                p = 1.0f / ( 1.0f + RExp( fApB ) );
                q = RExp( fApB ) / ( 1.0f + RExp( fApB ) );
            }
            d2 = p * q;
            h11 += dec_values[i] * dec_values[i] * d2;
            h22 += d2;
            h21 += dec_values[i] * d2;
            d1 = t[i] - p;
            g1 += dec_values[i] * d1;
            g2 += d1;
        }

        // Stopping Criteria
        if ( RAbs( g1 ) < eps && RAbs( g2 ) < eps )
            break;

        // Finding Newton direction: -inv(H') * g
        det = h11 * h22 - h21 * h21;
        dA = -( h22 * g1 - h21 * g2 ) / det;
        dB = -( -h21 * g1 + h11 * g2 ) / det;
        gd = g1 * dA + g2 * dB;


        stepsize = 1;		// Line Search
        while ( stepsize >= min_step )
        {
            newA = A + stepsize * dA;
            newB = B + stepsize * dB;

            // New function value
            newf = 0.0f;
            for ( i = 0; i < l; i++ )
            {
                fApB = dec_values[i] * newA + newB;
                if ( fApB >= 0 )
                    newf += t[i] * fApB + RLog( 1 + RExp( -fApB ) );
                else
                    newf += ( t[i] - 1 ) * fApB + RLog( 1 + RExp( fApB ) );
            }
            // Check sufficient decrease
            if ( newf < fval + 0.0001f * stepsize * gd )
            {
                A = newA;
                B = newB;
                fval = newf;
                break;
            }
            else
                stepsize = stepsize / 2.0f;
        }

        if ( stepsize < min_step )
        {
            info( "Line search fails in two-class probability estimates\n" );
            break;
        }
    }

    if ( iter >= max_iter )
        info( "Reaching maximal iterations in two-class probability estimates\n" );
    free( t );
}

static SVM_REAL sigmoid_predict( SVM_REAL decision_value, SVM_REAL A, SVM_REAL B )
{
    SVM_REAL fApB = decision_value * A + B;
    // 1-p used later; avoid catastrophic cancellation
    if ( fApB >= 0 )
        return RExp( -fApB ) / ( 1.0f + RExp( -fApB ) );
    else
        return 1.0f / ( 1 + RExp( fApB ) );
}

// Method 2 from the multiclass_prob paper by Wu, Lin, and Weng to predict probabilities
static void multiclass_probability( int k, SVM_REAL** r, SVM_REAL* p )
{
    int t, j;
    int iter = 0, max_iter = ::max( 100, k );
    SVM_REAL** Q = Malloc( SVM_REAL*, k );
    SVM_REAL* Qp = Malloc( SVM_REAL, k );
    SVM_REAL pQp, eps = 0.005f / k;

    for ( t = 0; t < k; t++ )
    {
        p[t] = 1.0f / k;  // Valid if k = 1
        Q[t] = Malloc( SVM_REAL, k );
        Q[t][t] = 0;
        for ( j = 0; j < t; j++ )
        {
            Q[t][t] += r[j][t] * r[j][t];
            Q[t][j] = Q[j][t];
        }
        for ( j = t + 1; j < k; j++ )
        {
            Q[t][t] += r[j][t] * r[j][t];
            Q[t][j] = -r[j][t] * r[t][j];
        }
    }
    for ( iter = 0; iter < max_iter; iter++ )
    {
        // stopping condition, recalculate QP,pQP for numerical accuracy
        pQp = 0;
        for ( t = 0; t < k; t++ )
        {
            Qp[t] = 0;
            for ( j = 0; j < k; j++ )
                Qp[t] += Q[t][j] * p[j];
            pQp += p[t] * Qp[t];
        }
        SVM_REAL max_error = 0;
        for ( t = 0; t < k; t++ )
        {
            SVM_REAL error = RAbs( Qp[t] - pQp );
            if ( error > max_error )
                max_error = error;
        }
        if ( max_error < eps ) break;

        for ( t = 0; t < k; t++ )
        {
            SVM_REAL diff = ( -Qp[t] + pQp ) / Q[t][t];
            p[t] += diff;
            pQp = ( pQp + diff * ( diff * Q[t][t] + 2 * Qp[t] ) ) / ( 1 + diff ) / ( 1 + diff );
            for ( j = 0; j < k; j++ )
            {
                Qp[j] = ( Qp[j] + diff * Q[t][j] ) / ( 1 + diff );
                p[j] /= ( 1 + diff );
            }
        }
    }
    if ( iter >= max_iter )
        info( "Exceeds max_iter in multiclass_prob\n" );
    for ( t = 0; t < k; t++ ) free( Q[t] );
    free( Q );
    free( Qp );
}

// Using cross-validation decision values to get parameters for SVC probability estimates
#if SVM_CFG_FEATURE_PROBABILITY != 0
static void svm_binary_svc_probability(
    const svm_problem* prob, const svm_parameter* param,
    SVM_REAL Cp, SVM_REAL Cn, SVM_REAL& probA, SVM_REAL& probB, svm_model* model )
{
    int i;
    int nr_fold = 5;
    int* perm = FD_StackTakeChainHead( int, prob->n_pts );
    SVM_REAL* dec_values = FD_StackTake( SVM_REAL, prob->n_pts );

    // random shuffle
    for ( i = 0; i < prob->n_pts; i++ ) perm[i] = i;
    for ( i = 0; i < prob->n_pts; i++ )
    {
        int j = i + rand() % ( prob->n_pts - i );
        swap( perm[i], perm[j] );
    }
    for ( i = 0; i < nr_fold; i++ )
    {
        int begin = i * prob->n_pts / nr_fold;
        int end = ( i + 1 ) * prob->n_pts / nr_fold;
        int j, k;
        struct svm_problem subprob;

        subprob.n_pts = prob->n_pts - ( end - begin );
        subprob.x = FD_StackTake( struct svm_node*, subprob.n_pts );
        subprob.y = FD_StackTake( SVM_REAL, subprob.n_pts );

        k = 0;
        for ( j = 0; j < begin; j++ )
        {
            subprob.x[k] = prob->x[perm[j]];
            subprob.y[k] = prob->y[perm[j]];
            ++k;
        }
        for ( j = end; j < prob->n_pts; j++ )
        {
            subprob.x[k] = prob->x[perm[j]];
            subprob.y[k] = prob->y[perm[j]];
            ++k;
        }
        int p_count = 0, n_count = 0;
        for ( j = 0; j < k; j++ )
            if ( subprob.y[j] > 0 )
                p_count++;
            else
                n_count++;

        if ( p_count == 0 && n_count == 0 )
            for ( j = begin; j < end; j++ )
                dec_values[perm[j]] = 0;
        else if ( p_count > 0 && n_count == 0 )
            for ( j = begin; j < end; j++ )
                dec_values[perm[j]] = 1;
        else if ( p_count == 0 && n_count > 0 )
            for ( j = begin; j < end; j++ )
                dec_values[perm[j]] = -1;
        else
        {
            svm_parameter subparam = *param;
            subparam.probability = 0;
            subparam.C = 1.0f;
            subparam.nr_weight = 2;
            subparam.weight_label = Malloc( int, 2 );
            subparam.weight = Malloc( SVM_REAL, 2 );
            subparam.weight_label[0] = +1;
            subparam.weight_label[1] = -1;
            subparam.weight[0] = Cp;
            subparam.weight[1] = Cn;
            struct svm_model* submodel = svm_train( &subprob, &subparam );
            for ( j = begin; j < end; j++ )
            {
                svm_predict_values( submodel, prob->x[perm[j]], &( dec_values[perm[j]] ) );
                // ensure +1 -1 order; reason not using CV subroutine
                dec_values[perm[j]] *= submodel->label[0];
            }
            svm_free_and_destroy_model( &submodel );
            svm_destroy_param( &subparam );
        }
        // free(subprob.x);
        // free(subprob.y);
    }
    sigmoid_train( prob->n_pts, dec_values, prob->y, probA, probB );
    free( dec_values );
    free( perm );
    FD_StackGiveChain();
}

// Binning method from the oneclass_prob paper by Que and Lin to predict the probability as a normal instance (i.e., not an outlier)
static SVM_REAL predict_one_class_probability( const svm_model* model, SVM_REAL dec_value )
{
    SVM_REAL prob_estimate = 0.0f;
    int nr_marks = 10;

    if ( dec_value < model->prob_density_marks[0] )
        prob_estimate = 0.001f;
    else if ( dec_value > model->prob_density_marks[nr_marks - 1] )
        prob_estimate = 0.999f;
    else
    {
        for ( int i = 1; i < nr_marks; i++ )
            if ( dec_value < model->prob_density_marks[i] )
            {
                prob_estimate = ( SVM_REAL )i / nr_marks;
                break;
            }
    }
    return prob_estimate;
}
#endif

static int compare_SVM_REAL( const void* a, const void* b )
{
    if ( *( SVM_REAL* )a > *( SVM_REAL* )b )
        return 1;
    else if ( *( SVM_REAL* )a < * ( SVM_REAL* )b )
        return -1;
    return 0;
}

#if SVM_CFG_FEATURE_PROBABILITY != 0
// Get parameters for one-class SVM probability estimates
static int svm_one_class_probability( const svm_problem* prob, const svm_model* model,
                                      SVM_REAL* prob_density_marks )
{
    SVM_REAL* dec_values = FD_StackTake( SVM_REAL, prob->n_pts );
    SVM_REAL* pred_results = FD_StackTake( SVM_REAL, prob->n_pts );
    int ret = 0;
    int nr_marks = 10;

    for ( int i = 0; i < prob->n_pts; i++ )
        pred_results[i] = svm_predict_values( model, prob->x[i], &dec_values[i] );
    qsort( dec_values, prob->n_pts, sizeof( SVM_REAL ), compare_SVM_REAL );

    int neg_counter = 0;
    for ( int i = 0; i < prob->n_pts; i++ )
        if ( dec_values[i] >= 0 )
        {
            neg_counter = i;
            break;
        }

    int pos_counter = prob->n_pts - neg_counter;
    if ( neg_counter < nr_marks / 2 || pos_counter < nr_marks / 2 )
    {
        fprintf( stderr, "WARNING: number of positive or negative decision values <%d; too few to do a probability estimation.\n", nr_marks / 2 );
        ret = -1;
    }
    else
    {
        // Binning by density
        SVM_REAL* tmp_marks = FD_StackTake( SVM_REAL, nr_marks + 1 );
        int mid = nr_marks / 2;
        for ( int i = 0; i < mid; i++ )
            tmp_marks[i] = dec_values[i * neg_counter / mid];
        tmp_marks[mid] = 0;
        for ( int i = mid + 1; i < nr_marks + 1; i++ )
            tmp_marks[i] = dec_values[neg_counter - 1 + ( i - mid ) * pos_counter / mid];

        for ( int i = 0; i < nr_marks; i++ )
            prob_density_marks[i] = ( tmp_marks[i] + tmp_marks[i + 1] ) / 2;
        // free(tmp_marks);
    }
    // free(dec_values);
    // free(pred_results);
    return ret;
}

// Return parameter of a Laplace distribution
static SVM_REAL svm_svr_probability(
    const svm_problem* prob, svm_model *model, const svm_parameter* param )
{
    int i;
    int nr_fold = 5;
    SVM_REAL* ymv = Malloc( SVM_REAL, prob->n_pts );
    SVM_REAL mae = 0;

    svm_parameter newparam = *param;
    newparam.probability = 0;
    svm_cross_validation( prob, model, &newparam, nr_fold, ymv );
    for ( i = 0; i < prob->n_pts; i++ )
    {
        ymv[i] = prob->y[i] - ymv[i];
        mae += RAbs( ymv[i] );
    }
    mae /= prob->n_pts;
    SVM_REAL std = RSqrt( 2 * mae * mae );
    int count = 0;
    mae = 0;
    for ( i = 0; i < prob->n_pts; i++ )
        if ( RAbs( ymv[i] ) > 5 * std )
            count = count + 1;
        else
            mae += RAbs( ymv[i] );
    mae /= ( prob->n_pts - count );
    info( "Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma= %g\n", mae );
    free( ymv );
    return mae;
}
#endif

// label: label name, start: begin of each class, count: #data of classes, perm: indices to the original data
// perm, length l, must be allocated before calling this subroutine
static void svm_group_classes( const svm_problem* prob, svm_model* model, int* nr_class_ret,
                               int** label_ret, int** start_ret, int** count_ret, int* perm )
{
    int l = prob->n_pts;
    int max_nr_class = SVM_CFG_MAX_CLASS_CNT;
    int nr_class = 0;
    int* label = FD_StackTake( int, max_nr_class );
    int* count = FD_StackTake( int, max_nr_class );
    int* start = FD_StackTake( int, max_nr_class );
    int* data_label = FD_StackTake( int, l );
    int i;
    if ( prob->n_pts > SVM_CFG_MAX_SAMP_CNT )
    {
        staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "Too many samples" );
    }
    for ( i = 0; i < l; i++ )
    {
        int this_label = ( int )prob->y[i];
        int j;
        for ( j = 0; j < nr_class; j++ )
        {
            if ( this_label == label[j] )
            {
                ++count[j];
                break;
            }
        }
        data_label[i] = j;
        if ( j == nr_class )
        {
            if ( nr_class == max_nr_class )
            {
                max_nr_class *= 2;
                while ( 1 );	// do not support realloc with soft stack
                label = ( int* )realloc( label, max_nr_class * sizeof( int ) );
                count = ( int* )realloc( count, max_nr_class * sizeof( int ) );
            }
            label[nr_class] = this_label;
            count[nr_class] = 1;
            ++nr_class;
        }
    }

    //
    // Labels are ordered by their first occurrence in the training set.
    // However, for two-class sets with -1/+1 labels and -1 appears first,
    // we swap labels to ensure that internally the binary SVM has positive data corresponding to the +1 instances.
    //
    if ( nr_class == 2 && label[0] == -1 && label[1] == 1 )
    {
        swap( label[0], label[1] );
        swap( count[0], count[1] );
        for ( i = 0; i < l; i++ )
        {
            if ( data_label[i] == 0 )
                data_label[i] = 1;
            else
                data_label[i] = 0;
        }
    }


    start[0] = 0;
    for ( i = 1; i < nr_class; i++ )
        start[i] = start[i - 1] + count[i - 1];
    // rocky: this for loop corrupts start[], we reconstruct start[] later
    for ( i = 0; i < l; i++ )
    {
        perm[start[data_label[i]]] = i;
        ++start[data_label[i]];
    }
    start[0] = 0;
    for ( i = 1; i < nr_class; i++ )
        start[i] = start[i - 1] + count[i - 1];

    *nr_class_ret = nr_class;
    *label_ret = label;
    *start_ret = start;
    *count_ret = count;
    FD_StackGive( data_label );
}

#if SVM_CFG_DENSE_VECTOR == 0
size_t svm_get_sparse_vector_size( svm_node* node )
{
    int cnt = 0;
    while ( node->index >= 0 )
    {
        cnt++, node++;
    }
    return ( cnt + 1 ) * sizeof( svm_node );
}
#endif

void _svm_copy_sv( svm_model* model )
{
    // copy Support Vectors
    int sampDim = model->vecDim + ( SVM_CFG_DENSE_VECTOR == 0 );
#if SVM_CFG_DENSE_VECTOR != 0
    svm_node* pNodes = EU_StackTake( svm_node, sampDim * model->n_SV );
    for ( int i = 0; i < model->n_SV; i++, pNodes += model->vecDim )
    {
        memcpy( pNodes, model->SVs[i], sizeof( svm_node ) * model->vecDim );
        model->SVs[i] = pNodes;
    }
#else
    size_t sampSize = 0;
    for ( int i = 0; i < model->n_SV; i++ )
    {
        sampSize += svm_get_sparse_vector_size( model->SVs[i] );
    }
    int nodeCnt = sampSize / sizeof( svm_node );
    svm_node* pNodes = EU_StackTake( svm_node, nodeCnt );
    for ( int i = 0; i < model->n_SV; i++ )
    {
        sampSize = svm_get_sparse_vector_size( model->SVs[i] );
        memcpy( pNodes, model->SVs[i], sampSize );
        model->SVs[i] = pNodes;
        pNodes += sampSize / sizeof( svm_node );
    }
#endif
    model->isSVCopied = 1;
}

//
// Interface functions
//
//extern "C" svm_model* svm_train(const svm_problem* prob, const svm_parameter* param,
//	bool isCopySV, int arenaSize, void* pvArena);
//	// rocky: if you want to use the model after input data has gone (such as saved model and
//	// reset your board), then you have to set isCopySV=true, this will allocates from EU_Stack
//	// for all support vectors.
//	struct svm_model* svm_train(const struct svm_problem* prob, const struct svm_parameter* param,
//		bool isCopySV=false, int arenaSize = SVM_CFG_CACHE_SIZE + 8192, void *pvArena = nullptr);
svm_model* svm_train( const svm_problem* prob, const svm_parameter* param,
                      bool isCopySV, int arenaSize, void* pvArena )
{
    int total_sv = 0;
    char isToDelArena = 0;
    if ( pvArena == nullptr )
    {
        pvArena = new char[arenaSize];
        isToDelArena = 1;
    }
    // Rocky: Make 8-byte align,
    char* pCurArena = ( char* ) ( ( uint32_t )pvArena + 7 & ~7 );
    int takenSize = ( int )pCurArena - ( int )pvArena;
    // Rocky: create CPairStack object at the base of aligned arena
    CPairStack* pStk = ( CPairStack* )pCurArena;
    pCurArena += sizeof( CPairStack );
    takenSize += sizeof( CPairStack );

    pStk->ManualConstruct( arenaSize - takenSize, ( uint32_t* )pCurArena );


    svm_model* model; // = Malloc(svm_model, 1);
    model = ( svm_model* )pStk->EU_TakeChainHead( sizeof( svm_model ) );
    model->pBase = model;
    model->pArena = ( char* )pvArena;
    model->isToDelArena = isToDelArena;
    model->isSVCopied = 0;
    // model->arenaSize = arenaSize;
    model->pStk = pStk;

    g_allocNdx = 0;
    g_newAcc = 0;
    model->vecDim = prob->vecDim;
    model->param = *param;
    model->memAllocType = kModelMemAllocDuringTraining;	// XXX

    if ( param->svm_type == ONE_CLASS ||
            param->svm_type == EPSILON_SVR ||
            param->svm_type == NU_SVR )
    {
        FD_StackTakeChainHead( int, 1 );
        // regression or one-class-svm
        model->nr_class = 2;
        model->label = NULL;
        model->nSV = NULL;
#if SVM_CFG_FEATURE_PROBABILITY != 0
        model->probA = NULL;
        model->probB = NULL;
        model->prob_density_marks = NULL;
#endif
        model->sv_coef = EU_StackTake( SVM_REAL*, 1 );

        decision_function f = svm_train_one( prob, param, 0, 0, model );
        model->rho = EU_StackTake( SVM_REAL, 1 );
        model->rho[0] = f.rho;

        int nSV = 0;
        int i;
        for ( i = 0; i < prob->n_pts; i++ )
            if ( RAbs( f.alpha[i] ) > 0 ) ++nSV;
        model->n_SV = nSV;
        model->SVs = EU_StackTake( svm_node*, nSV );
        model->sv_coef[0] = EU_StackTake( SVM_REAL, nSV );
        model->sv_indices = EU_StackTake( int, nSV );
        int j = 0;
        for ( i = 0; i < prob->n_pts; i++ )
            if ( RAbs( f.alpha[i] ) > 0 )
            {
                model->SVs[j] = prob->x[i];
                model->sv_coef[0][j] = f.alpha[i];
                model->sv_indices[j] = i + 1;
                ++j;
            }
#if SVM_CFG_FEATURE_PROBABILITY != 0
        if ( param->probability &&
                ( param->svm_type == EPSILON_SVR ||
                  param->svm_type == NU_SVR ) )
        {
            model->probA = EU_StackTake( SVM_REAL, 1 );
            model->probA[0] = svm_svr_probability( prob, model, param );
        }
        else if ( param->probability && param->svm_type == ONE_CLASS )
        {
            int nr_marks = 10;
            SVM_REAL* prob_density_marks = FD_StackTake( SVM_REAL, nr_marks );

            if ( svm_one_class_probability( prob, model, prob_density_marks ) == 0 )
            {
                model->prob_density_marks = EU_StackTake( SVM_REAL, nr_marks );
                for ( i = 0; i < nr_marks; i++ )
                    model->prob_density_marks[i] = prob_density_marks[i];
            }
        }
#endif

        // free(f.alpha);
    }
    else
    {
        // classification
        int N = prob->n_pts;
        int nr_class;
        int* label = NULL; // rocky: ÿ��label����ֵ
        int* start = NULL; //rocky: ��label�����ź�ÿ��label���׸����������
        int* count = NULL; // rocky: ÿ��label�ж��ٸ�����
        int* perm = FD_StackTakeChainHead( int, N ); // rocky: ������ԭʼ��ŵ���label���������ź��ӳ��

        // group training data of the same class
        svm_group_classes( prob, model, &nr_class, &label, &start, &count, perm );
        if ( nr_class == 1 )
            info( "WARNING: training data in only one class. See README for details.\n" );

        svm_node** x = FD_StackTake( svm_node*, N );
        int i;
        // rocky: ��label��˳����������������˳��
        for ( i = 0; i < N; i++ )
            x[i] = prob->x[perm[i]];

        // calculate weighted C

        SVM_REAL* weighted_C = FD_StackTake( SVM_REAL, nr_class );
        for ( i = 0; i < nr_class; i++ )
            weighted_C[i] = param->C;
        for ( i = 0; i < param->nr_weight; i++ )
        {
            int j;
            for ( j = 0; j < nr_class; j++ )
                if ( param->weight_label[i] == label[j] )
                    break;
            if ( j == nr_class )
                printf( "WARNING: class label %d specified in weight is not found\n", param->weight_label[i] );
            else
                weighted_C[j] *= param->weight[i];
        }

        // train k*(k-1)/2 models

        bool* nonzero = FD_StackTake( bool, N );
        for ( i = 0; i < N; i++ )
            nonzero[i] = false;
        decision_function* f = FD_StackTake( decision_function, nr_class * ( nr_class - 1 ) / 2 );

#if SVM_CFG_FEATURE_PROBABILITY != 0
        SVM_REAL* probA = NULL, * probB = NULL;
        if ( param->probability )
        {
            probA = FD_StackTake( SVM_REAL, nr_class * ( nr_class - 1 ) / 2 );
            probB = FD_StackTake( SVM_REAL, nr_class * ( nr_class - 1 ) / 2 );
        }
#endif
        int* nz_count = FD_StackTake( int, nr_class );
        int* nz_start = FD_StackTake( int, nr_class );

        int p = 0;
        for ( i = 0; i < nr_class; i++ )
            for ( int j = i + 1; j < nr_class; j++ )
            {
                svm_problem sub_prob;
                int si = start[i], sj = start[j];
                int ci = count[i], cj = count[j];
                sub_prob.n_pts = ci + cj;
                // sub_prob.x = Malloc(svm_node*, sub_prob.n_pts);
                // sub_prob.y = Malloc(SVM_REAL, sub_prob.n_pts);

                // rocky: tricky error prone: svm_train_one() allocates from FD stack w/o freeing
                // and it is invoked in multiple iterations, so we can no longer use EU stack to
                // allocate. Since we free x, y and svm_train_one does not use EU stack, we allocate
                // from EU stack
                sub_prob.x = EU_StackTakeChainHead( svm_node*, sub_prob.n_pts );
                sub_prob.y = EU_StackTake( SVM_REAL, sub_prob.n_pts );
                EU_StackLock();	// make sure no one else uses FD stack as its lifetime is likely longer
                int k;
                for ( k = 0; k < ci; k++ )
                {
                    sub_prob.x[k] = x[si + k];
                    sub_prob.y[k] = +1;
                }
                for ( k = 0; k < cj; k++ )
                {
                    sub_prob.x[ci + k] = x[sj + k];
                    sub_prob.y[ci + k] = -1;
                }
                sub_prob.vecDim = prob->vecDim;
#if SVM_CFG_FEATURE_PROBABILITY != 0
                if ( param->probability )
                {
                    svm_binary_svc_probability( &sub_prob, param, weighted_C[i], weighted_C[j],
                                                probA[p], probB[p], model );
                }
#endif
                f[p] = svm_train_one( &sub_prob, param, weighted_C[i], weighted_C[j], model );
                for ( k = 0; k < ci; k++ )
                    if ( !nonzero[si + k] && RAbs( f[p].alpha[k] ) > 0 )
                        nonzero[si + k] = true;
                for ( k = 0; k < cj; k++ )
                    if ( !nonzero[sj + k] && RAbs( f[p].alpha[ci + k] ) > 0 )
                        nonzero[sj + k] = true;
                // free(sub_prob.x);
                // free(sub_prob.y);
                EU_StackUnlock();
                EU_StackGiveChain();
                ++p;
            }

        // build output

        model->nr_class = nr_class;

        model->label = EU_StackTake( int, nr_class );
        for ( i = 0; i < nr_class; i++ )
            model->label[i] = label[i];

        model->rho = EU_StackTake( SVM_REAL, nr_class * ( nr_class - 1 ) / 2 );
        for ( i = 0; i < nr_class * ( nr_class - 1 ) / 2; i++ )
            model->rho[i] = f[i].rho;
#if SVM_CFG_FEATURE_PROBABILITY != 0
        if ( param->probability )
        {
            model->probA = EU_StackTake( SVM_REAL, nr_class * ( nr_class - 1 ) / 2 );
            model->probB = EU_StackTake( SVM_REAL, nr_class * ( nr_class - 1 ) / 2 );
            for ( i = 0; i < nr_class * ( nr_class - 1 ) / 2; i++ )
            {
                model->probA[i] = probA[i];
                model->probB[i] = probB[i];
            }
        }
        else
        {
            model->probA = NULL;
            model->probB = NULL;
        }
        model->prob_density_marks = NULL;	// for one-class SVM probabilistic outputs only
#endif
        model->nSV = EU_StackTake( int, nr_class );
        for ( i = 0; i < nr_class; i++ )
        {
            int nSV = 0;
            for ( int j = 0; j < count[i]; j++ )
                if ( nonzero[start[i] + j] )
                {
                    ++nSV;
                    ++total_sv;
                }
            model->nSV[i] = nSV;
            nz_count[i] = nSV;
        }

        info( "Total nSV = %d\n", total_sv );

        model->n_SV = total_sv;
        model->SVs = EU_StackTake( svm_node*, total_sv );
        model->sv_indices = EU_StackTake( int, total_sv );
        p = 0;
        for ( i = 0; i < N; i++ )
            if ( nonzero[i] )
            {
                model->SVs[p] = x[i];
                model->sv_indices[p++] = perm[i] + 1;
            }


        nz_start[0] = 0;
        for ( i = 1; i < nr_class; i++ )
            nz_start[i] = nz_start[i - 1] + nz_count[i - 1];

        model->sv_coef = EU_StackTake( SVM_REAL*, nr_class - 1 );
        for ( i = 0; i < nr_class - 1; i++ )
            model->sv_coef[i] = EU_StackTake( SVM_REAL, total_sv );

        p = 0;
        for ( i = 0; i < nr_class; i++ )
            for ( int j = i + 1; j < nr_class; j++ )
            {
                // classifier (i,j): coefficients with
                // i are in sv_coef[j-1][nz_start[i]...],
                // j are in sv_coef[i][nz_start[j]...]

                int si = start[i];
                int sj = start[j];
                int ci = count[i];
                int cj = count[j];

                int q = nz_start[i];
                int k;
                for ( k = 0; k < ci; k++ )
                    if ( nonzero[si + k] )
                        model->sv_coef[j - 1][q++] = f[p].alpha[k];
                q = nz_start[j];
                for ( k = 0; k < cj; k++ )
                    if ( nonzero[sj + k] )
                        model->sv_coef[i][q++] = f[p].alpha[ci + k];
                ++p;
            }

        // free(label);
        // free(probA);
        // free(probB);
        // free(count);
        // free(perm);
        // free(start);

        // free(x);
        // free(weighted_C);
        // free(nonzero);
        // for (i = 0; i < nr_class * (nr_class - 1) / 2; i++)
        //	free(f[i].alpha);
        // free(f);
        // free(nz_count);
        // free(nz_start);
    }
    FD_StackGiveChain();

    if ( isCopySV )
    {
        _svm_copy_sv( model );
    }

    // model structure and all its content pointers allocate RAM from a software empty-upgrowing stack,
    // and we record the end address of all stuffs with allocating a zero size block, so when we save
    // and load model from disk/Flash, we can quickly determine the size to save, and adjust all embedded
    // pointers by (model->some_poinsvm_cross_validationter - model->pArena + loaded addr)
    model->pModelTotalMemEnd = EU_StackTake( char, 0 );
    
    volatile uint32_t statck_usage = GET_MAX_STACK_USED();
    
    return model;
}

// Stratified cross validation
void svm_cross_validation( const svm_problem* prob, svm_model *model,
                           const svm_parameter* param, int nr_fold, SVM_REAL* target )
{
    int i;
    int* fold_start;
    int l = prob->n_pts;
    int* perm = Malloc( int, l );
    int nr_class;
    if ( nr_fold > l )
    {
        nr_fold = l;
        printf( "WARNING: # folds > # data. Will use # folds = # data instead (i.e., leave-one-out cross validation)\n" );
    }
    fold_start = Malloc( int, nr_fold + 1 );
    // stratified cv may not give leave-one-out rate
    // Each class to l folds -> some folds may have zero elements
    if ( ( param->svm_type == C_SVC ||
            param->svm_type == NU_SVC ) && nr_fold < l )
    {
        int* start = NULL;
        int* label = NULL;
        int* count = NULL;
        svm_group_classes( prob, model, &nr_class, &label, &start, &count, perm );

        // random shuffle and then data grouped by fold using the array perm
        int* fold_count = Malloc( int, nr_fold );
        int c;
        int* index = Malloc( int, l );
        for ( i = 0; i < l; i++ )
            index[i] = perm[i];
        for ( c = 0; c < nr_class; c++ )
            for ( i = 0; i < count[c]; i++ )
            {
                int j = i + rand() % ( count[c] - i );
                swap( index[start[c] + j], index[start[c] + i] );
            }
        for ( i = 0; i < nr_fold; i++ )
        {
            fold_count[i] = 0;
            for ( c = 0; c < nr_class; c++ )
                fold_count[i] += ( i + 1 ) * count[c] / nr_fold - i * count[c] / nr_fold;
        }
        fold_start[0] = 0;
        for ( i = 1; i <= nr_fold; i++ )
            fold_start[i] = fold_start[i - 1] + fold_count[i - 1];
        for ( c = 0; c < nr_class; c++ )
            for ( i = 0; i < nr_fold; i++ )
            {
                int begin = start[c] + i * count[c] / nr_fold;
                int end = start[c] + ( i + 1 ) * count[c] / nr_fold;
                for ( int j = begin; j < end; j++ )
                {
                    perm[fold_start[i]] = index[j];
                    fold_start[i]++;
                }
            }
        fold_start[0] = 0;
        for ( i = 1; i <= nr_fold; i++ )
            fold_start[i] = fold_start[i - 1] + fold_count[i - 1];
        free( start );
        free( label );
        free( count );
        free( index );
        free( fold_count );
    }
    else
    {
        for ( i = 0; i < l; i++ ) perm[i] = i;
        for ( i = 0; i < l; i++ )
        {
            int j = i + rand() % ( l - i );
            swap( perm[i], perm[j] );
        }
        for ( i = 0; i <= nr_fold; i++ )
            fold_start[i] = i * l / nr_fold;
    }

    for ( i = 0; i < nr_fold; i++ )
    {
        int begin = fold_start[i];
        int end = fold_start[i + 1];
        int j, k;
        struct svm_problem subprob;

        subprob.n_pts = l - ( end - begin );
        subprob.x = Malloc( struct svm_node*, subprob.n_pts );
        subprob.y = Malloc( SVM_REAL, subprob.n_pts );

        k = 0;
        for ( j = 0; j < begin; j++ )
        {
            subprob.x[k] = prob->x[perm[j]];
            subprob.y[k] = prob->y[perm[j]];
            ++k;
        }
        for ( j = end; j < l; j++ )
        {
            subprob.x[k] = prob->x[perm[j]];
            subprob.y[k] = prob->y[perm[j]];
            ++k;
        }
        struct svm_model* submodel = svm_train( &subprob, param );

#if SVM_CFG_FEATURE_PROBABILITY != 0
        if ( param->probability &&
                ( param->svm_type == C_SVC || param->svm_type == NU_SVC ) )
        {
            SVM_REAL* prob_estimates = Malloc( SVM_REAL, svm_get_nr_class( submodel ) );
            for ( j = begin; j < end; j++ )
                target[perm[j]] = svm_predict_probability( submodel, prob->x[perm[j]], prob_estimates );
            free( prob_estimates );
        }
        else
#endif
        {
            for ( j = begin; j < end; j++ )
                target[perm[j]] = svm_predict( submodel, prob->x[perm[j]] );
        }

        svm_free_and_destroy_model( &submodel );
        free( subprob.x );
        free( subprob.y );
    }
    free( fold_start );
    free( perm );
}


int svm_get_svm_type( const svm_model* model )
{
    return model->param.svm_type;
}

int svm_get_nr_class( const svm_model* model )
{
    return model->nr_class;
}

void svm_get_labels( const svm_model* model, int* label )
{
    if ( model->label != NULL )
        for ( int i = 0; i < model->nr_class; i++ )
            label[i] = model->label[i];
}

void svm_get_sv_indices( const svm_model* model, int* indices )
{
    if ( model->sv_indices != NULL )
        for ( int i = 0; i < model->n_SV; i++ )
            indices[i] = model->sv_indices[i];
}

int svm_get_nr_sv( const svm_model* model )
{
    return model->n_SV;
}

#if SVM_CFG_FEATURE_PROBABILITY != 0
SVM_REAL svm_get_svr_probability( const svm_model* model )
{
    if ( ( model->param.svm_type == EPSILON_SVR || model->param.svm_type == NU_SVR ) &&
            model->probA != NULL )
        return model->probA[0];
    else
    {
        fprintf( stderr, "Model doesn't contain information for SVR probability inference\n" );
        return 0;
    }
}
#endif

SVM_REAL svm_predict_values( const svm_model* model, const svm_node* x, SVM_REAL* dec_values )
{
    int i;
    if ( model->param.svm_type == ONE_CLASS ||
            model->param.svm_type == EPSILON_SVR ||
            model->param.svm_type == NU_SVR )
    {
        SVM_REAL* sv_coef = model->sv_coef[0];
        SVM_REAL sum = 0;
        for ( i = 0; i < model->n_SV; i++ )
            sum += sv_coef[i] * Kernel::k_function( x, model->SVs[i], model->param, model->vecDim );
        sum -= model->rho[0];
        *dec_values = sum;
        return sum;
        //if ( model->param.svm_type == ONE_CLASS )
          //  return ( sum > 0 ) ? 1.0f : -1.0f;
        //else
           // return sum;
    }
    else
    {
        int nr_class = model->nr_class;
        int l = model->n_SV;

        SVM_REAL* kvalue = FD_StackTakeChainHead( SVM_REAL, l );
        for ( i = 0; i < l; i++ )
            kvalue[i] = Kernel::k_function( x, model->SVs[i], model->param, model->vecDim );

        int* start = FD_StackTake( int, nr_class );
        start[0] = 0;
        for ( i = 1; i < nr_class; i++ )
            start[i] = start[i - 1] + model->nSV[i - 1];

        int* vote = FD_StackTake( int, nr_class );
        for ( i = 0; i < nr_class; i++ )
            vote[i] = 0;

        int p = 0;
        for ( i = 0; i < nr_class; i++ )
            for ( int j = i + 1; j < nr_class; j++ )
            {
                SVM_REAL sum = 0;
                int si = start[i];
                int sj = start[j];
                int ci = model->nSV[i];
                int cj = model->nSV[j];

                int k;
                SVM_REAL* coef1 = model->sv_coef[j - 1];
                SVM_REAL* coef2 = model->sv_coef[i];
                for ( k = 0; k < ci; k++ )
                    sum += coef1[si + k] * kvalue[si + k];
                for ( k = 0; k < cj; k++ )
                    sum += coef2[sj + k] * kvalue[sj + k];
                sum -= model->rho[p];
                dec_values[p] = sum;

                if ( dec_values[p] > 0 )
                    ++vote[i];
                else
                    ++vote[j];
                p++;
            }

        int vote_max_idx = 0;
        for ( i = 1; i < nr_class; i++ )
            if ( vote[i] > vote[vote_max_idx] )
                vote_max_idx = i;

        // free(kvalue);
        // free(start);
        // free(vote);
        FD_StackGiveChain();
        return ( SVM_REAL )model->label[vote_max_idx];
    }
}

SVM_REAL svm_predict( const svm_model* model, const svm_node* x )
{
    int nr_class = model->nr_class;
#if SVM_FEATURE_SOLVER_SVC
    SVM_REAL dec_values[SVM_CFG_MAX_CLASS_CNT * (SVM_CFG_MAX_CLASS_CNT - 1) / 2];
#else
    SVM_REAL dec_values[1];
#endif
    SVM_REAL pred_result = svm_predict_values( model, x, dec_values );
    // free(dec_values);
    return pred_result + 0.030f;
}

#if SVM_CFG_FEATURE_PROBABILITY != 0
SVM_REAL svm_predict_probability(
    const svm_model* model, const svm_node* x, SVM_REAL* prob_estimates )
{
    if ( ( model->param.svm_type == C_SVC || model->param.svm_type == NU_SVC ) &&
            model->probA != NULL && model->probB != NULL )
    {
        int i;
        int nr_class = model->nr_class;
        SVM_REAL* dec_values = Malloc( SVM_REAL, nr_class * ( nr_class - 1 ) / 2 );
        svm_predict_values( model, x, dec_values );

        SVM_REAL min_prob = 1e-7f;
        SVM_REAL** pairwise_prob = Malloc( SVM_REAL*, nr_class );
        for ( i = 0; i < nr_class; i++ )
            pairwise_prob[i] = Malloc( SVM_REAL, nr_class );
        int k = 0;
        for ( i = 0; i < nr_class; i++ )
            for ( int j = i + 1; j < nr_class; j++ )
            {
                pairwise_prob[i][j] = min( max( sigmoid_predict( dec_values[k], model->probA[k], model->probB[k] ), min_prob ), 1 - min_prob );
                pairwise_prob[j][i] = 1 - pairwise_prob[i][j];
                k++;
            }
        if ( nr_class == 2 )
        {
            prob_estimates[0] = pairwise_prob[0][1];
            prob_estimates[1] = pairwise_prob[1][0];
        }
        else
            multiclass_probability( nr_class, pairwise_prob, prob_estimates );

        int prob_max_idx = 0;
        for ( i = 1; i < nr_class; i++ )
            if ( prob_estimates[i] > prob_estimates[prob_max_idx] )
                prob_max_idx = i;
        for ( i = 0; i < nr_class; i++ )
            free( pairwise_prob[i] );
        free( dec_values );
        free( pairwise_prob );
        return ( SVM_REAL )model->label[prob_max_idx];
    }
    else if ( model->param.svm_type == ONE_CLASS && model->prob_density_marks != NULL )
    {
        SVM_REAL dec_value;
        SVM_REAL pred_result = svm_predict_values( model, x, &dec_value );
        prob_estimates[0] = predict_one_class_probability( model, dec_value );
        prob_estimates[1] = 1 - prob_estimates[0];
        return pred_result;
    }
    else
        return svm_predict( model, x );
}
#endif

static const char* svm_type_table[] =
{
    "c_svc", "nu_svc", "one_class", "epsilon_svr", "nu_svr", NULL
};

static const char* kernel_type_table[] =
{
    "linear", "polynomial", "rbf", "sigmoid", "precomputed", NULL
};

struct svm_model* svm_relocate_model( char* pRelDst, char* model_buf, svm_model* src_model )
{
    // relocate embedded content pointers
    int reloc = pRelDst - ( char* )src_model->pBase;
    int ptrReloc = ( model_buf - ( char* )src_model->pBase ) / int( sizeof( char* ) ); // rocky: must cast to int for signed division;
    char** ppEmbPtr;
    int i;

    // relocate level 2 embedded pointers
    // relocate sv_coefs[n]
    int nLen = 1;
    if ( src_model->param.svm_type == C_SVC || src_model->param.svm_type == NU_SVC )
        nLen = src_model->nr_class - 1;
    for ( i = 0, ppEmbPtr = ( char** )( src_model->sv_coef ); i < nLen; i++, ppEmbPtr++ )
    {
        if ( ppEmbPtr[0] )
            ppEmbPtr[ptrReloc] += reloc;
    }
    // reloate support vectors from inputs
    if ( src_model->isSVCopied )
    {
        nLen = src_model->n_SV;
        for ( i = 0, ppEmbPtr = ( char** )( src_model->SVs ); i < nLen; i++, ppEmbPtr++ )
        {
            if ( ppEmbPtr[0] )
                ppEmbPtr[ptrReloc] += reloc;
        }
    }

    // relocate level 1 embedded pointers
    for ( ppEmbPtr = ( char** )( &src_model->emb_ptr_start );
            ppEmbPtr != ( char** )( &src_model->emb_ptr_end );
            ppEmbPtr++ )
    {
        if ( ppEmbPtr[0] )
            ppEmbPtr[ptrReloc] += reloc;
    }

    svm_model *pTmpModel = ( struct svm_model* )model_buf;
    // model buffer at model_buf is designated to inference from pRelDst
    pTmpModel->pBase = ( svm_model* )pRelDst;
    return pTmpModel;
}
//
//struct svm_model* svm_relocate_model(char* pDst, struct svm_model* model, uint8_t isRelocateDst) {
//	// relocate embedded content pointers
//	int reloc = pDst - (char*)model->pBase;
//	int ptrReloc = reloc / int(sizeof(char*)); // rocky: must cast to int for signed division;
//	struct svm_model* dst_model = (struct svm_model*)pDst;
//	if (isRelocateDst == 0) {
//		// rocky: relocate the input model to work in the new pDst location, useful to burn the
//		// model to XIP Flash memory: model is the page buffer, pDst is Flash addr
//		ptrReloc = 0;
//		model->pBase = dst_model;
//	}
//	else {
//		dst_model->pBase = dst_model;
//	}
//
//	char** ppEmbPtr;
//    int i;
//
//	// relocate level 2 embedded pointers
//	// relocate sv_coefs[n]
//	int nLen = 1;
//	if (model->param.svm_type == C_SVC || model->param.svm_type == NU_SVC)
//		nLen = model->nr_class - 1;
//	for (i = 0, ppEmbPtr = (char**)(model->sv_coef); i < nLen; i++, ppEmbPtr++)
//	{
//		if (ppEmbPtr[0])
//			ppEmbPtr[ptrReloc] += reloc;
//	}
//	// reloate support vectors from inputs
//	if (model->isSVCopied) {
//		nLen = model->n_SV;
//		for (i = 0, ppEmbPtr = (char**)(model->SVs); i < nLen; i++, ppEmbPtr++)
//		{
//			if (ppEmbPtr[0])
//				ppEmbPtr[ptrReloc] += reloc;
//		}
//	}
//
//	for (ppEmbPtr = (char**) (& model->emb_ptr_start);
//		ppEmbPtr != (char**)(&model->emb_ptr_end);
//		ppEmbPtr++)
//	{
//		if (ppEmbPtr[0])
//			ppEmbPtr[ptrReloc] += reloc;
//	}
//
//	return (struct svm_model*)pDst;
//}

size_t svm_get_model_total_size( svm_model* model, bool isIncludingSVs )
{
    size_t totalSize = ( size_t )model->pModelTotalMemEnd - ( size_t )model;

    if ( model->pStk && model->pStk->EU_IsInside( model->SVs[0] ) == 0 )
    {
#if SVM_CFG_DENSE_VECTOR != 0
        totalSize += model->vecDim * sizeof( svm_node ) * model->n_SV;
#else
#endif
    }
    return totalSize;
}

struct svm_model* svm_copy_model( char* pMem, size_t memSize, struct svm_model* model,
                                  char isReloc, char isKeepTrainingMem )
{
    int ret = 0;
    size_t totalSize = ( size_t )model->pModelTotalMemEnd - ( size_t )model;
    size_t structSize = sizeof( *model );
    if ( pMem && memSize < totalSize )
    {
        staticUtility::_OnError( SVM_CFG_MEM_ERROR_IS_FATAL, "Not enough memory to save mem-mapped model" );
        return nullptr;
    }
    if ( pMem == nullptr )
    {
        if ( model->pStk )
            pMem = EU_StackTakeChainHead( char, totalSize );
        else
        {
            staticUtility::_OnError( SVM_CFG_ERROR_IS_FATAL,
                                     "model does not own pair stack, must provide dest mem" );
        }
    }
    memcpy( pMem, model, totalSize );
    if ( isReloc )
        svm_relocate_model( pMem, pMem, model );
    struct svm_model* dst_model = ( struct svm_model* )pMem;
    if ( !isKeepTrainingMem )
    {
        dst_model->pStk = nullptr;
        dst_model->pArena = nullptr;
    }
    return dst_model;
}

struct svm_model* svm_copy_model_to_buffer( char* pBuffer, size_t bufferSize,
        char* pRealDst, struct svm_model* model )
{

    struct svm_model* dst_model = svm_copy_model( pBuffer, bufferSize, model, 0, 0 );
    svm_relocate_model( pRealDst, ( char* )dst_model, model );

    //struct svm_model* dst_model = (struct svm_model*)pBuffer;
    //if (pBuffer != (char*)model)
    //	svm_copy_model(pBuffer, bufferSize, model, 1, 0);
    //svm_relocate_model(pRealDst, dst_model, false);
    return dst_model;
}

//extern "C" int svm_save_model(const char* model_file_name, const svm_model* model));
//static FIL inputFil;
//int svm_save_model( const char* model_file_name, const svm_model* model )
//{
//    //FILE* fp = fopen(model_file_name, "w");
//    int  error = f_open( &inputFil, model_file_name, FA_READ | FA_OPEN_EXISTING );
//    if ( error != FR_OK )
//    {
//        printf( "Could not open '%s', erro = %d\n", model_file_name, error );
//        return -1;
//    }
//
////	char* old_locale = setlocale(LC_ALL, NULL);
////	if (old_locale) {
////		old_locale = _strdup(old_locale);
////	}
////	setlocale(LC_ALL, "C");
//
//    const svm_parameter& param = model->param;
//
//    printf( "svm_type %s\n", svm_type_table[param.svm_type] );
//    printf( "kernel_type %s\n", kernel_type_table[param.kernel_type] );
//
//    if ( param.kernel_type == POLY )
//        printf( "degree %d\n", param.degree );
//
//    if ( param.kernel_type == POLY || param.kernel_type == RBF || param.kernel_type == SIGMOID )
//        printf( "gamma %.17g\n", param.gamma );
//
//    if ( param.kernel_type == POLY || param.kernel_type == SIGMOID )
//        printf( "coef0 %.17g\n", param.coef0 );
//
//    int nr_class = model->nr_class;
//    int l = model->n_SV;
//    printf( "nr_class %d\n", nr_class );
//    printf( "total_sv %d\n", l );
//
//    {
//        printf( "rho" );
//        for ( int i = 0; i < nr_class * ( nr_class - 1 ) / 2; i++ )
//            printf( " %.17g", model->rho[i] );
//        printf( "\n" );
//    }
//
//    if ( model->label )
//    {
//        printf( "label" );
//        for ( int i = 0; i < nr_class; i++ )
//            printf( " %d", model->label[i] );
//        printf( "\n" );
//    }
//#if SVM_CFG_FEATURE_PROBABILITY != 0
//    if ( model->probA ) // regression has probA only
//    {
//        printf( "probA" );
//        for ( int i = 0; i < nr_class * ( nr_class - 1 ) / 2; i++ )
//            printf( " %.17g", model->probA[i] );
//        printf( "\n" );
//    }
//    if ( model->probB )
//    {
//        printf( "probB" );
//        for ( int i = 0; i < nr_class * ( nr_class - 1 ) / 2; i++ )
//            printf( " %.17g", model->probB[i] );
//        printf( "\n" );
//    }
//    if ( model->prob_density_marks )
//    {
//        printf( "prob_density_marks" );
//        int nr_marks = 10;
//        for ( int i = 0; i < nr_marks; i++ )
//            printf( " %.17g", model->prob_density_marks[i] );
//        printf( "\n" );
//    }
//#endif
//    if ( model->nSV )
//    {
//        printf( "nr_sv" );
//        for ( int i = 0; i < nr_class; i++ )
//            printf( " %d", model->nSV[i] );
//        printf( "\n" );
//    }
//
//    printf( "SV\n" );
//    SVM_REAL* const* sv_coef = model->sv_coef;
//    svm_node* const* SV = model->SVs;
//
//    for ( int i = 0; i < l; i++ )
//    {
//        for ( int j = 0; j < nr_class - 1; j++ )
//            printf( "%.17g ", sv_coef[j][i] );
//
//        const svm_node* p = SV[i];
//
//        if ( param.kernel_type == PRECOMPUTED )
//            printf( "0:%d ", ( int )( p->value ) );
//        else
//        {
//#if SVM_CFG_DENSE_VECTOR != 0
//            p++;
//            for ( int ndx = 0; ndx < model->vecDim; ndx++, p++ )
//            {
//                printf( "%d:%.8g ", ndx + 1, p->value );
//            }
//#else
//            while ( p->index != -1 )
//            {
//                printf( "%d:%.8g ", p->index, p->value );
//                p++;
//            }
//#endif
//        }
//        printf( "\n" );
//    }
//
//    //setlocale(LC_ALL, old_locale);
//    //free(old_locale);
//
//    if ( f_close( &inputFil ) != 0 ) return -1;
//    else return 0;
//}
//
//static char* line = NULL;
//static int max_line_len;
//
//static char* readline( FIL* input )
//{
//    int len;
////TCHAR* f_gets (TCHAR* buff, int len, FIL* fp);
//    if ( f_gets( line, max_line_len, input ) == NULL )
//        return NULL;
//
//    while ( strrchr( line, '\n' ) == NULL )
//    {
//        max_line_len *= 2;
//        line = ( char* )realloc( line, max_line_len );
//        len = ( int )strlen( line );
//        if ( f_gets( line + len, max_line_len - len, input ) == NULL )
//            break;
//    }
//    return line;
//}

//
// FSCANF helps to handle fscanf failures.
// Its do-while block avoids the ambiguity when
// if (...)
//    FSCANF();
// is used
//
//#define FSCANF(_stream, _format, _var) do{ if (fscanf(_stream, _format, _var) != 1) return false; }while(0)
//bool read_model_header( FIL* fp, svm_model* model )
//{
//    svm_parameter& param = model->param;
//    // parameters for training only won't be assigned, but arrays are assigned as NULL for safety
//    param.nr_weight = 0;
//    param.weight_label = NULL;
//    param.weight = NULL;
//    double f64Tmp[1];
//    char cmd[81];
//    while ( 1 )
//    {
//        SCANF( "%80s", cmd );
//
//        if ( strcmp( cmd, "svm_type" ) == 0 )
//        {
//            SCANF( "%80s", cmd );
//            int i;
//            for ( i = 0; svm_type_table[i]; i++ )
//            {
//                if ( strcmp( svm_type_table[i], cmd ) == 0 )
//                {
//                    param.svm_type = i;
//                    break;
//                }
//            }
//            if ( svm_type_table[i] == NULL )
//            {
//                printf( "unknown svm type.\n" );
//                return false;
//            }
//        }
//        else if ( strcmp( cmd, "kernel_type" ) == 0 )
//        {
//            SCANF( "%80s", cmd );
//            int i;
//            for ( i = 0; kernel_type_table[i]; i++ )
//            {
//                if ( strcmp( kernel_type_table[i], cmd ) == 0 )
//                {
//                    param.kernel_type = i;
//                    break;
//                }
//            }
//            if ( kernel_type_table[i] == NULL )
//            {
//                printf( "unknown kernel function.\n" );
//                return false;
//            }
//        }
//        else if ( strcmp( cmd, "degree" ) == 0 )
//        {
//            SCANF( "%c", &param.degree );
//        }
//        else if ( strcmp( cmd, "gamma" ) == 0 )
//        {
//            SCANF( "%lf", f64Tmp );
//            param.gamma = ( SVM_REAL )f64Tmp[0];
//        }
//        else if ( strcmp( cmd, "coef0" ) == 0 )
//        {
//            SCANF( "%lf", f64Tmp );
//            param.coef0 = ( SVM_REAL )f64Tmp[0];
//        }
//        else if ( strcmp( cmd, "nr_class" ) == 0 )
//        {
//            SCANF( "%c", &model->nr_class );
//        }
//        else if ( strcmp( cmd, "total_sv" ) == 0 )
//        {
//            SCANF( "%hd", &model->n_SV );
//        }
//        else if ( strcmp( cmd, "rho" ) == 0 )
//        {
//            int n = model->nr_class * ( model->nr_class - 1 ) / 2;
//            model->rho = Malloc( SVM_REAL, n );
//            for ( int i = 0; i < n; i++ )
//            {
//                SCANF( "%lf", f64Tmp );
//                model->rho[i] = ( SVM_REAL )f64Tmp[0];
//            }
//        }
//        else if ( strcmp( cmd, "label" ) == 0 )
//        {
//            int n = model->nr_class;
//            model->label = Malloc( int, n );
//            for ( int i = 0; i < n; i++ )
//                SCANF( "%d", &model->label[i] );
//        }
//#if SVM_CFG_FEATURE_PROBABILITY != 0
//        else if ( strcmp( cmd, "probA" ) == 0 )
//        {
//            int n = model->nr_class * ( model->nr_class - 1 ) / 2;
//            model->probA = Malloc( SVM_REAL, n );
//            for ( int i = 0; i < n; i++ )
//            {
//                SCANF( "%lf", f64Tmp );
//                model->probA[i] = ( SVM_REAL )f64Tmp[0];
//            }
//        }
//        else if ( strcmp( cmd, "probB" ) == 0 )
//        {
//            int n = model->nr_class * ( model->nr_class - 1 ) / 2;
//            model->probB = Malloc( SVM_REAL, n );
//            for ( int i = 0; i < n; i++ )
//            {
//                SCANF( "%lf", f64Tmp );
//                model->probB[i] = ( SVM_REAL )f64Tmp[0];
//            }
//        }
//        else if ( strcmp( cmd, "prob_density_marks" ) == 0 )
//        {
//            int n = 10;	// nr_marks
//            model->prob_density_marks = Malloc( SVM_REAL, n );
//            for ( int i = 0; i < n; i++ )
//            {
//                SCANF( "%lf", f64Tmp );
//                model->prob_density_marks[i] = ( SVM_REAL )f64Tmp[0];
//            }
//        }
//#endif
//        else if ( strcmp( cmd, "nr_sv" ) == 0 )
//        {
//            int n = model->nr_class;
//            model->nSV = Malloc( int, n );
//            for ( int i = 0; i < n; i++ )
//                SCANF( "%d", &model->nSV[i] );
//        }
//        else if ( strcmp( cmd, "SV" ) == 0 )
//        {
//            while ( 1 )
//            {
//                char c[2];
//                f_gets( c, 1, &inputFil );
//                if ( c[0] == EOF || c[0] == '\n' ) break;
//            }
//            break;
//        }
//        else
//        {
//            printf( "unknown text in model file: [%s]\n", cmd );
//            return false;
//        }
//    }
//
//    return true;
//
//}
//svm_load_model
//svm_model* svm_load_model( const char* model_file_name )
//{
////	FILE* fp = fopen(model_file_name, "rb");
////	if (fp == NULL)
//    int error = f_open( &inputFil, model_file_name, FA_READ | FA_OPEN_EXISTING );
//    if ( error != FR_OK )
//    {
//        printf( "Could not open '%s', erro = %d\n", model_file_name, error );
//        return NULL;
//    }
//
////	char* old_locale = setlocale(LC_ALL, NULL);
////	if (old_locale) {
////		old_locale = _strdup(old_locale);
////	}
////	setlocale(LC_ALL, "C");
//
//    // read parameters
//
//    svm_model* model = Malloc( svm_model, 1 );
//    model->rho = NULL;
//#if SVM_CFG_FEATURE_PROBABILITY != 0
//    model->probA = NULL;
//    model->probB = NULL;
//    model->prob_density_marks = NULL;
//#endif
//    model->sv_indices = NULL;
//    model->label = NULL;
//    model->nSV = NULL;
//
//    // read header
//    if ( !read_model_header( &inputFil, model ) )
//    {
//        printf( "ERROR: fscanf failed to read model\n" );
////		setlocale(LC_ALL, old_locale);
////		free(old_locale);
//        free( model->rho );
//        free( model->label );
//        free( model->nSV );
//        free( model );
//        return NULL;
//    }
//
//    // read sv_coef and SV
//
//    int elements = 0;
//    long pos = f_tell( &inputFil );
//
//    max_line_len = 1024;
//    line = Malloc( char, max_line_len );
//    char* p, * endptr, * idx, * val;
//    int lineCnt = 0;
//    int dim = 0, index;
//    while ( readline( &inputFil ) != NULL )
//    {
//        lineCnt++;
//        p = strtok( line, ":" );
//        while ( 1 )
//        {
//            p = strtok( NULL, ":" );
//            if ( p == NULL )
//                break;
//            index = ( int )strtol( p, &endptr, 10 );
//            if ( dim < index )
//                dim = index;
//            ++elements;
//        }
//    }
//    elements += model->n_SV;
//    model->vecDim = dim;
//    f_lseek( &inputFil, pos );
//
//    int m = model->nr_class - 1;
//    int n_SV = model->n_SV;
//    model->sv_coef = Malloc( SVM_REAL*, m );
//    int i;
//    for ( i = 0; i < m; i++ )
//        model->sv_coef[i] = Malloc( SVM_REAL, n_SV );
//    model->SVs = Malloc( svm_node*, n_SV );
//    svm_node* x_space = NULL;
//    if ( n_SV > 0 )
//    {
//        x_space = Malloc( svm_node, elements );
//        memset( x_space, 0, sizeof( svm_node ) * elements );
//    }
//    int j = 0;
//#if SVM_CFG_DENSE_VECTOR != 0
//    for ( i = 0; i < n_SV; i++ )
//    {
//        readline( &inputFil );
//        model->SVs[i] = x_space + i * n_SV;
//
//        p = strtok( line, " \t" );
//        model->sv_coef[0][i] = RStrToR( p, &endptr );
//        for ( int k = 1; k < m; k++ )
//        {
//            p = strtok( NULL, " \t" );
//            model->sv_coef[k][i] = RStrToR( p, &endptr );
//        }
//        while ( 1 )
//        {
//            idx = strtok( NULL, ":" );
//            val = strtok( NULL, " \t" );
//
//            if ( val == NULL )
//                break;
//            int index = ( int )strtol( idx, &endptr, 10 );
//            x_space[index].value = RStrToR( val, &endptr );
//        }
//#else
//    for ( i = 0; i < n_SV; i++ )
//    {
//        readline( fp );
//        model->SVs[i] = &x_space[j];
//
//        p = strtok( line, " \t" );
//        model->sv_coef[0][i] = RStrToR( p, &endptr );
//        for ( int k = 1; k < m; k++ )
//        {
//            p = strtok( NULL, " \t" );
//            model->sv_coef[k][i] = RStrToR( p, &endptr );
//        }
//        while ( 1 )
//        {
//            idx = strtok( NULL, ":" );
//            val = strtok( NULL, " \t" );
//
//            if ( val == NULL )
//                break;
//            int index = ( int )strtol( idx, &endptr, 10 );
//            x_space[j].index = ( int )strtol( idx, &endptr, 10 );
//            x_space[j].value = RStrToR( val, &endptr );
//            ++j;
//        }
//        x_space[j++].index = -1;
//#endif
//    }
//    free( line );
//
////	setlocale(LC_ALL, old_locale);
////	free(old_locale);
//
//    if ( f_error( &inputFil ) != 0 || f_close( &inputFil ) != 0 )
//        return NULL;
//
//    model->memAllocType = kModelMemMalloc;	// XXX
//    return model;
//}

//void svm_free_model_content(svm_model* model)
//{
//	if (model->free_sv && model->n_SV > 0 && model->SVs != NULL) {
//		//	free((void*)(model->SVs[0]));
//	}
//	if (model->sv_coef)
//	{
//		for (int i = 0; i < model->nr_class - 1; i++) {
//			// free(model->sv_coef[i]);
//			model->sv_coef[i] = NULL;
//		}
//	}
//
//	// free(model->sv_coef);
//	model->sv_coef = NULL;
//
//	// free(model->rho);
//	model->rho = NULL;
//
//	// free(model->label);
//	model->label = NULL;
//
//	// free(model->probA);
//	model->probA = NULL;
//
//	// free(model->probB);
//	model->probB = NULL;
//
//	// free(model->prob_density_marks);
//	model->prob_density_marks = NULL;
//
//	// free(model->sv_indices);
//	model->sv_indices = NULL;
//
//
//	// free(model->SVs);
//	model->SVs = NULL;
//
//	// free(model->nSV);
//	model->nSV = NULL;
//}

void svm_free_and_destroy_model( svm_model** model_ptr_ptr )
{
    if ( model_ptr_ptr != NULL && *model_ptr_ptr != NULL )
    {
        if ( model_ptr_ptr[0]->isToDelArena )
        {
            // rocky: We still allow the application to allocate the whole arena memory from heap,
            // in this case, the isToDelArena is true, we just delete the arena memory
            delete[] model_ptr_ptr[0]->pArena;
        }
        else
        {
            // svm_free_model_content(*model_ptr_ptr);
            // rocky: arena memory is from static buffer.
            // rocky: model structure is at chain head, its content pointers do NOT mark new chain head
            // so a EU_GiveChain() call frees all memory taken by content pointers and itself
            model_ptr_ptr[0]->pStk->EU_GiveChain();
            model_ptr_ptr[0]->pStk->ManualDestruct();
        }
        *model_ptr_ptr = NULL;
    }
}

void svm_destroy_param( svm_parameter* param )
{
    free( param->weight_label );
    free( param->weight );
}

const char* svm_check_parameter( const svm_problem* prob, const svm_parameter* param )
{
    // svm_type

    int svm_type = param->svm_type;
    if ( svm_type != C_SVC &&
            svm_type != NU_SVC &&
            svm_type != ONE_CLASS &&
            svm_type != EPSILON_SVR &&
            svm_type != NU_SVR )
        return "unknown svm type";

    // kernel_type, degree

    int kernel_type = param->kernel_type;
    if ( kernel_type != LINEAR &&
            kernel_type != POLY &&
            kernel_type != RBF &&
            kernel_type != SIGMOID &&
            kernel_type != PRECOMPUTED )
        return "unknown kernel type";

    if ( ( kernel_type == POLY || kernel_type == RBF || kernel_type == SIGMOID ) &&
            param->gamma < 0 )
        return "gamma < 0";

    if ( kernel_type == POLY && param->degree < 0 )
        return "degree of polynomial kernel < 0";

    // cacheSizeKB,eps,C,nu,p,shrinking

    if ( param->cacheSize <= 0 )
        return "cacheSize <= 0";

    if ( param->eps <= 0 )
        return "eps <= 0";

    if ( svm_type == C_SVC ||
            svm_type == EPSILON_SVR ||
            svm_type == NU_SVR )
        if ( param->C <= 0 )
            return "C <= 0";

    if ( svm_type == NU_SVC ||
            svm_type == ONE_CLASS ||
            svm_type == NU_SVR )
        if ( param->nu <= 0 || param->nu > 1 )
            return "nu <= 0 or nu > 1";

    if ( svm_type == EPSILON_SVR )
        if ( param->p < 0 )
            return "p < 0";

    if ( param->shrinking != 0 &&
            param->shrinking != 1 )
        return "shrinking != 0 and shrinking != 1";
#if SVM_CFG_FEATURE_PROBABILITY != 0
    if ( param->probability != 0 &&
            param->probability != 1 )
        return "probability != 0 and probability != 1";
#endif

    // check whether nu-svc is feasible

    if ( svm_type == NU_SVC )
    {
        int l = prob->n_pts;
        int max_nr_class = SVM_CFG_MAX_CLASS_CNT;
        int nr_class = 0;
        int* label = Malloc( int, max_nr_class );
        int* count = Malloc( int, max_nr_class );

        int i;
        for ( i = 0; i < l; i++ )
        {
            int this_label = ( int )prob->y[i];
            int j;
            for ( j = 0; j < nr_class; j++ )
                if ( this_label == label[j] )
                {
                    ++count[j];
                    break;
                }
            if ( j == nr_class )
            {
                if ( nr_class == max_nr_class )
                {
                    max_nr_class *= 2;
                    while ( 1 ); // do not support realloc with soft stack
                    label = ( int* )realloc( label, max_nr_class * sizeof( int ) );
                    count = ( int* )realloc( count, max_nr_class * sizeof( int ) );
                }
                label[nr_class] = this_label;
                count[nr_class] = 1;
                ++nr_class;
            }
        }

        for ( i = 0; i < nr_class; i++ )
        {
            int n1 = count[i];
            for ( int j = i + 1; j < nr_class; j++ )
            {
                int n2 = count[j];
                if ( param->nu * ( n1 + n2 ) / 2 > ::min( n1, n2 ) )
                {
                    free( label );
                    free( count );
                    return "specified nu is infeasible";
                }
            }
        }
        free( label );
        free( count );
    }

    return NULL;
}

#if SVM_CFG_FEATURE_PROBABILITY != 0
int svm_check_probability_model( const svm_model* model )
{
    return
        ( ( model->param.svm_type == C_SVC || model->param.svm_type == NU_SVC ) &&
          model->probA != NULL && model->probB != NULL ) ||
        ( model->param.svm_type == ONE_CLASS && model->prob_density_marks != NULL ) ||
        ( ( model->param.svm_type == EPSILON_SVR || model->param.svm_type == NU_SVR ) &&
          model->probA != NULL );
}
#endif

void svm_set_print_string_function( void ( *print_func )( const char* ) )
{
    if ( print_func == NULL )
        svm_print_string = &print_string_stdout;
    else
        svm_print_string = print_func;
}
