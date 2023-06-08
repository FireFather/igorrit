#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable : 4244)
// conversion from 'int' to 'short', possible loss of data
#pragma warning(disable : 4018)
// signed/unsigned mismatch
#pragma warning(disable : 4996)
// 'sscanf': This function or variable may be unsafe
#pragma warning(disable : 4334)
// '<<' : result of 32-bit shift implicitly converted to 64 bits
#pragma warning(disable : 4098)
// 'void' function returning a value
#pragma warning(disable : 4761)
// integral size mismatch in argument; conversion supplied


#define VERO 1
#define FALSO 0

#define WINDOWS
#define YUSUF_MULTICORE
#define EPONYM "Igorrit"
#define HAS_PREFETCH
#define MERSENNE_RAND
//#define NO_INPUT_PIPES
//#define CON_ROBBO_BUILD
//#define X64_RELEASE



#define MULTI_GUADAGNO
#define MULTI_STORIA

#define MAX_CPUS 8 /* vary for increase */

#ifdef WINDOWS
#include <windows.h>
#define sint8 __int8
#define sint16 __int16
#define sint32 __int32
#define sint64 __int64
#define uint8 unsigned __int8
#define uint16 unsigned __int16
#define uint32 unsigned __int32
#define uint64 unsigned __int64
#define INLINE /* */
#define atoll _atoi64
#define POSIX_MEMALIGN(a, b, c) a = malloc(c)
#ifdef X64_RELEASE
#define VERSION "0.086v8_x64"
#include "win64bits.h"
#else
#define VERSION "0.086v8_w32"
#include "win32bits.h"
#endif
#define __builtin_prefetch(x, y, z) _mm_prefetch((char*)x, z);
#define MUTEX_TYPE CRITICAL_SECTION
#define COND_TYPE HANDLE
#define LOCK(x) EnterCriticalSection(x)
#define UNLOCK(x) LeaveCriticalSection(x)
#define WAIT_CON_LOCK(x, y) WaitForSingleObject (x, INFINITE)
#define SIGNAL_CON_LOCK(x, y) { /* LOCK (y); */ SetEvent (x); /* UNLOCK (y); */ }
#define LOCK_INIT(x) InitializeCriticalSection(x)
#define COND_INIT(x, y) (x) = CreateEvent (0, FALSO, FALSO, 0)
#define PTHREAD_CREATE(N, b, thr, d)\
  CreateThread (NULL, 0, thr, (LPVOID) (d), 0, (LPDWORD)N);
DWORD PTHREAD[MAX_CPUS];
//#define IVAN_THREAD(A) DWORD WINAPI ivan_thread (LPVOID A)
#else
#define sint8 signed char
#define sint16 signed short int
#define sint32 int
#define sint64 long long int
#define uint8 unsigned char
#define uint16 unsigned short int
#define uint32 unsigned int
#define uint64 unsigned long long int
#define INLINE inline
#define POSIX_MEMALIGN(a, b, c) posix_memalign ((void*) &a, b, c)
#define __builtin_prefetch
#include "bits.h"
#include <pthread.h>
#define MUTEX_TYPE pthread_mutex_t
#define COND_TYPE pthread_cond_t
#define LOCK(x) pthread_mutex_lock (x)
#define UNLOCK(x) pthread_mutex_unlock (x)
#define WAIT(x, y) pthread_cond_wait (x, y)
#define SIGNAL(x) pthread_cond_signal (x)
#define WAIT_CON_LOCK(x, y) { LOCK (y); WAIT (x, y); UNLOCK (y); }
#define SIGNAL_CON_LOCK(x, y) { LOCK (y); SIGNAL (x); UNLOCK (y); }
#define LOCK_INIT(x) pthread_mutex_init ((x), NULL)
#define COND_INIT(x, y)\
 { pthread_cond_init (&(x), NULL); pthread_mutex_init (&(y), NULL); }
#define PTHREAD_CREATE(N, b, thr, d) pthread_create (N, NULL, thr, (void*) (d))
pthread_t PTHREAD[MAX_CPUS];
#define IVAN_THREAD(A) void* ivan_thread (void* A)
#endif

MUTEX_TYPE SMP_IVAN[1];
MUTEX_TYPE PTHREAD_COND_MUTEX[MAX_CPUS];
MUTEX_TYPE WAKEUP_LOCK[MAX_CPUS];
COND_TYPE WAIT_EVENT[MAX_CPUS];
COND_TYPE WAKEUP[MAX_CPUS];
COND_TYPE THREAD_WAITS[MAX_CPUS];
COND_TYPE THREAD_RUNS[MAX_CPUS];

#define logico uint8


typedef struct
{
  uint64 volatile ZobristPedone;
  uint8 bianco_pedone_linea_conteggio,
    nero_pedone_linea_conteggio, aperto_colonna_conteggio;
  logico locked;
  uint32 bianco_re_periglio, nero_re_periglio;
  uint8 bianco_pedone_chiaro, bianco_pedone_scuro, nero_pedone_chiaro,
    nero_pedone_scuro, bianco_pedone_libero_linea, nero_pedone_libero_linea,
    bianco_patta_numero, nero_patta_numero;
  uint32 puntos;
} tipo_pedone_valu; /* 32 bytes */
tipo_pedone_valu *pedone_zobrist;

#define CHECK_HALT() { if (POSIZIONE->halt) { RETURN (0); } }

#define pietra_bassa 1
#define pietra_alta 2
#define pietra_tagliata 4
#define pietra_totale 8
#define pietra_precisa 16

#define zobrist_tagliato(tr) ((tr->pietras) & pietra_tagliata)
#define zobrist_totale(tr) ((tr->pietras) & pietra_totale)
#define zobrist_preciso(tr) ((tr)->pietras & pietra_precisa)
#define zobrist_valu_alta(tr) (tr->valu_alta)
#define zobrist_valu_bassa(tr) (tr->valu_bassa)

#define COLONNA(s) ((s) & 7)
#define TRAVERSA(s) ((s) >> 3)
#define DI(s) (((s) >> 6) & 077)
#define AI(s) ((s) & 077)

/* YUSUF */
#ifdef YUSUF_MULTICORE
logico TITANIC_MODE;
#endif
uint64 CPUS_SIZE;
uint64 NODE_CHECK;

typedef struct {uint64 h1, h2;} YUSUF128;
#if 0 /* compiler */
#define HYATT_HASH() \
  asm volatile ("movq (%0),%%r15\n" "movq %%r15,(%1)\n" \
		"movq 0x8(%0),%%r15\n" "movq %%r15,0x8(%1)\n" \
		/* "xorq %%r15,(%1)\n" "xorq %%r15,(%1)\n" */ \
		"movq %1,%0\n" \
		: : "r" (tr), "r" (H) : "%r15");
#define UPDATE_ANNI() (elenco_zobrist + (k + i))->anni = ANNI;
#if 0
#define HYATT_HASH() \
  H->h1 = ((uint64 *) tr)[0]; \
  H->h2 = ((uint64 *) tr)[1]; \
  H->h1 ^= H->h2; \
  H->h1 ^= H->h2; \
  tr = (tipo_zobrist *) (H);
#endif
#else /* non funzione ! */
#define HYATT_HASH() /* */
#define UPDATE_ANNI() tr->anni = ANNI;
#endif

#define chiamata_valu_piena(m)\
 valutazione (POSIZIONE, -0x7fff0000, 0x7fff0000, m)
#define chiamata_valu_pigra(B, A, p, m)\
 valutazione (POSIZIONE, (B) - (p), (A) + (p), m)

#define bianco_in_scacco (DINAMICO->attacco_nero & bitbordo_bianco_re)
#define nero_in_scacco (DINAMICO->attacco_bianco & bitbordo_nero_re)
#define POSIZIONE1 (POSIZIONE0 + 1)
#define mossa_bianco_in_scacco (POSIZIONE1->bianco_re_scacco)
#define mossa_nero_in_scacco (POSIZIONE1->nero_re_scacco)

#ifdef MULTI_GUADAGNO
sint16 massimo_posizionale_guadagno[MAX_CPUS][0x10][010000];
#define MAX_POS_GAIN(pez, mos) massimo_posizionale_guadagno[POSIZIONE->cpu][pez][mos]
#else
sint16 massimo_posizionale_guadagno[0x10][010000]; /* multicore ? */
#define MAX_POS_GAIN(pez, mos) massimo_posizionale_guadagno[pez][mos]
#endif

logico bordo_legittimo, PARTITA_NUOVA;

#define ripetizione_verificare \
 CHECK_HALT(); \
 if (DINAMICO->mossa50 >= 100) RETURN(0); \
 for (i = 4; i <= DINAMICO->mossa50 && i <= mucchio_altezzo; i += 2) \
   if (MUCCHIO[mucchio_altezzo - i] == DINAMICO->Zobrist) RETURN(0);

#ifdef CON_ROBBO_BUILD /* in the yet */
#define occupato5_verificare /* */
#else
#define occupato5_verificare /* */
#endif

typedef struct
{
  uint32 zobrist;
  uint8 pietras, anni, abisso_alto, abisso_basso;
  sint16 valu_bassa, valu_alta;
  uint16 mossa, _2;
} tipo_zobrist;

tipo_zobrist *elenco_zobrist;
uint64 elenco_celare, ANNI;

typedef struct
{
  uint64 zobrist;
  sint32 valu;
  uint16 mossa;
  uint8 abisso, anni;
} tipo_zobrist_pv;
tipo_zobrist_pv elenco_zobrist_pv[0x10000]; /* multicore ? */
#define elenco_celare_pv 0xfffc

#if 0
#define elenco_pedone_numero (1 << 20) /* 33% lesser access when 20 over 16 */
#else
int elenco_pedone_numero; /* uci option ahora */
#endif

typedef struct
{
  uint32 mossa;
} tipo_lista_mossa;

typedef struct
{
  /* 0x00-0x0f */ uint64 Zobrist, ZobristPedone;
  /* 0x10-0x1b */ uint32 materiale, statico, _7;
  /* 0x1c-0x1f */  uint8 arrocco, mossa50, en_passant, cattura;
  /* 0x20-0x3f */ uint64 attacco_bianco, attacco_nero, bianco_xray, nero_xray;
  /* 0x40-0x47 */ sint32 valu, valu_posizionale;
  /* 0x48-0x51 */ uint16 _5, _6, uccisore_uno, uccisore_due, mossa;
  /* 0x52-0x57 */  uint8 _0, _3, _4, pigro, pietras_salvas, pietras;
  /* 0x58-0x7f */ uint64 bianco_re_scacco, nero_re_scacco, _1, _2, _8;
} tipo_dinamico;

typedef struct
{
  int fase, celare, cb;
  uint32 mossa_tras, mossa, exclude;
  uint64 targa;
  tipo_lista_mossa LISTA[256];
  uint32 cattura_brutta[64];
} tipo_prossimo;

#if 0
#define HEIGHT(x) ((x) - (dinamico_inizio + 1))
#else
#define HEIGHT(x) ((x)->height)
#endif

typedef struct
{
  int alpha;
  int beta;
  int abisso;
  int node_type;
  int value;
  uint32 mossa;
  uint32 mossa_buona;
  uint32 childs;
  tipo_prossimo* MOVE_PICK;
  logico tot;
  logico aktiv;
  MUTEX_TYPE splock[1];
}
SPLITPUNKT;

#define MAX_SP 16
SPLITPUNKT ROOT_SP[MAX_SP];
volatile int SMP_FREE;
#define NODE_TYPE_PV 1
#define NODE_TYPE_ALL 2
#define NODE_TYPE_CUT 3
#include <setjmp.h>

struct TP
{
  uint8 qu[64];
  uint64 bitbordo[16];
  uint64 occupato_nero_bianco, occupato_sinistro90, occupato_sinistro45, occupato_retto45;
  uint8 xray_bianco_lista[64], xray_nero_lista[64];
  uint8 bianco_en_mossa, bianco_re_quadretto, nero_re_quadretto, height;
  /* 64 + 128 + 32 + 128 + 4 = 356 */
  tipo_dinamico *DIN, *din_iniz;
  uint64 MUCCHIO[1024];
  uint64 mucchio_altezzo, cpu, nodes;
  logico halt, used;
  MUTEX_TYPE padlock[1];
  int child_count;
  struct TP *parent, *children[MAX_CPUS];
  SPLITPUNKT *splitpunkt;
};

typedef struct TP tipo_posizione;
#define RP_PER_CPU 8
tipo_posizione ROOT_POSIZIONE[MAX_CPUS][RP_PER_CPU];

tipo_posizione ROOT_POSIZIONE0[1];
tipo_posizione NULL_PARENT[1];

#define abisso_massimo 1024
/* SMP rotogravure */
#define DINAMICO (POSIZIONE->DIN)
#define dinamico_inizio (POSIZIONE->din_iniz)
#define MUCCHIO (POSIZIONE->MUCCHIO)
#define mucchio_altezzo (POSIZIONE->mucchio_altezzo)
#define xray_bianco_lista (POSIZIONE->xray_bianco_lista)
#define xray_nero_lista (POSIZIONE->xray_nero_lista)


#define ep_targa (DINAMICO->en_passant)

#define arrocco_bianco_oo (DINAMICO->arrocco & 0x1)
#define arrocco_bianco_ooo (DINAMICO->arrocco & 0x2)
#define arrocco_nero_oo (DINAMICO->arrocco & 0x4)
#define arrocco_nero_ooo (DINAMICO->arrocco & 0x8)

typedef struct
{
  sint16 valu;
  uint8 segno;
  uint8 pietras;
} tipo_materiale;
tipo_materiale MATERIALE[419904]; /* SMP read */

typedef enum
{ conto_occupato_bianco, conto_pedone_bianco, conto_cavallo_bianco,
  conto_re_bianco, conto_chiaro_bianco, conto_scuro_bianco,
  conto_torre_bianco, conto_donna_bianca,
  conto_occupato_nero, conto_pedone_nero, conto_cavallo_nero, conto_re_nero,
  conto_chiaro_nero, conto_scuro_nero, conto_torre_nero, conto_donna_nera
} conto_pezzos;
typedef enum
{ TRAS, cattura_gen, mossa_cattura, UCCISORE_UNO, UCCISORE_DUE,
  mossa_ordinaria, cattura_brutta,
  TRAS2, cattura_gen2, mossa_cattura2, scacco_zitto, elusione, TRAS3,
  cattura_gen3, mossa_cattura3, scacco_zitto3, guadagno, FASE_0
} conto_fases;
typedef enum
{ A1, B1, C1, D1, E1, F1, G1, H1, A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3, A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8
} conto_quadrettos;
typedef enum
{ T1, T2, T3, T4, T5, T6, T7, T8 } conto_traversas;
typedef enum
{ CA, CB, CC, CD, CE, CF, CG, CH } conto_colonnas;

sint32 statico_valu[0x10][0100]; /* SMP read */

#ifdef MULTI_STORIA
uint16 storia_valu[MAX_CPUS][0x10][0100]; /* SMP cashline ? */
#define STORIA_VALU(P, M) storia_valu[P->cpu][P->qu[DI (M)]][AI (M)]
#else
uint16 storia_valu[0x10][0100];
#define STORIA_VALU(P, M) storia_valu[P->qu[DI (M)]][AI (M)]
#endif

char STRING1[64], STRING2[64], STRING3[64], STRING4[64];

#define bitbordo_bianco_re POSIZIONE->bitbordo[conto_re_bianco]
#define bitbordo_bianca_donna POSIZIONE->bitbordo[conto_donna_bianca]
#define bitbordo_bianco_torre POSIZIONE->bitbordo[conto_torre_bianco]
#define bitbordo_bianco_chiaro POSIZIONE->bitbordo[conto_chiaro_bianco]
#define bitbordo_bianco_scuro POSIZIONE->bitbordo[conto_scuro_bianco]
#define bitbordo_bianco_alfiere (bitbordo_bianco_chiaro|bitbordo_bianco_scuro)
#define bitbordo_bianco_cavallo POSIZIONE->bitbordo[conto_cavallo_bianco]
#define bitbordo_bianco_pedone POSIZIONE->bitbordo[conto_pedone_bianco]
#define bitbordo_occupato_bianco POSIZIONE->bitbordo[conto_occupato_bianco]
#define bitbordo_nero_re POSIZIONE->bitbordo[conto_re_nero]
#define bitbordo_nera_donna POSIZIONE->bitbordo[conto_donna_nera]
#define bitbordo_nero_torre POSIZIONE->bitbordo[conto_torre_nero]
#define bitbordo_nero_chiaro POSIZIONE->bitbordo[conto_chiaro_nero]
#define bitbordo_nero_scuro POSIZIONE->bitbordo[conto_scuro_nero]
#define bitbordo_nero_alfiere (bitbordo_nero_chiaro|bitbordo_nero_scuro)
#define bitbordo_nero_cavallo POSIZIONE->bitbordo[conto_cavallo_nero]
#define bitbordo_nero_pedone POSIZIONE->bitbordo[conto_pedone_nero]
#define bitbordo_occupato_nero POSIZIONE->bitbordo[conto_occupato_nero]

#define turno_sinistro45 linea_turno[direzione_h1a8]
#define turno_retto45 linea_turno[direzione_a1h8]
#define turno_normale linea_turno[direzione_orizzontale]
#define turno_sinistro90 linea_turno[direzione_verticale]
#define attaco_sinistro45 linea_obscura[direzione_h1a8]
#define attaco_retto45 linea_obscura[direzione_a1h8]
#define attaco_normale linea_obscura[direzione_orizzontale]
#define attaco_sinistro90 linea_obscura[direzione_verticale]

#define ATTACO_h1a8(di)\
  attaco_sinistro45\
  [di][(POSIZIONE->occupato_sinistro45 >> turno_sinistro45[di]) & 077]
#define ATTACO_a1h8(di)\
  attaco_retto45\
  [di][(POSIZIONE->occupato_retto45 >> turno_retto45[di]) & 077]
#define ATTACO_traversa(di)\
  attaco_normale\
  [di][(POSIZIONE->occupato_nero_bianco >> turno_normale[di]) & 077]
#define ATTACO_colonna(di)\
  attaco_sinistro90\
  [di][(POSIZIONE->occupato_sinistro90 >> turno_sinistro90[di]) & 077]

#define MASSIMO(x, y) (( (x) >= (y)) ? (x) : (y))
#define MINIMO(x, y) (( (x) <= (y)) ? (x) : (y))
#define ASSOLUTO(x) (( (x) >= 0) ? (x) : -(x))
#define colonna_distacco(x, y) (ASSOLUTO(COLONNA(x) - COLONNA(y)))
#define traversa_distacco(x, y) (ASSOLUTO(TRAVERSA(x) - TRAVERSA(y)))

#define attaco_alfiere(di) (ATTACO_a1h8(di) | ATTACO_h1a8(di))
#define attaco_torre(di) (ATTACO_traversa(di) | ATTACO_colonna(di))
#define attaco_donna(di) (attaco_torre(di) | attaco_alfiere(di))

#define pietra_en_passant 030000
#define pietra_arrocco 010000
#define pietra_celare 070000
#define pietra_promozione_donna 070000
#define pietra_promozione_torre 060000
#define pietra_promozione_alfiere 050000
#define pietra_promozione_cavallo 040000
#define mossa_en_passant(x) (((x) & pietra_celare) == pietra_en_passant)
#define mossa_promozione(x) (((x) & pietra_celare) >= pietra_promozione_cavallo)
#define mossa_arrocco(x) (((x) & pietra_celare) == pietra_arrocco)
#define mossa_storia(x) (((x) & 060000) == 0)

#define direzione_h1a8 0
#define direzione_a1h8 1
#define direzione_orizzontale 2
#define direzione_verticale 3
#define direzione_malato 37
#define VALU_MATTO 30000
#define VALU_INFINITO 32750
#define MOSSA_NON 0

#include "pila.h"
#include "funzione.h"
#include "comune.h"
