const static uint8 sinistro90[64] =
{ 7, 15, 23, 31, 39, 47, 55, 63,
  6, 14, 22, 30, 38, 46, 54, 62,
  5, 13, 21, 29, 37, 45, 53, 61,
  4, 12, 20, 28, 36, 44, 52, 60,
  3, 11, 19, 27, 35, 43, 51, 59,
  2, 10, 18, 26, 34, 42, 50, 58,
  1, 9, 17, 25, 33, 41, 49, 57,
  0, 8, 16, 24, 32, 40, 48, 56
};

const static uint8 sinistro45[64] =
{ 0, 2, 5, 9, 14, 20, 27, 35,
  1, 4, 8, 13, 19, 26, 34, 42,
  3, 7, 12, 18, 25, 33, 41, 48,
  6, 11, 17, 24, 32, 40, 47, 53,
  10, 16, 23, 31, 39, 46, 52, 57,
  15, 22, 30, 38, 45, 51, 56, 60,
  21, 29, 37, 44, 50, 55, 59, 62,
  28, 36, 43, 49, 54, 58, 61, 63
};

const static uint8 retto45[64] =
{ 28, 21, 15, 10, 6, 3, 1, 0,
  36, 29, 22, 16, 11, 7, 4, 2,
  43, 37, 30, 23, 17, 12, 8, 5,
  49, 44, 38, 31, 24, 18, 13, 9,
  54, 50, 45, 39, 32, 25, 19, 14,
  58, 55, 51, 46, 40, 33, 26, 20,
  61, 59, 56, 52, 47, 41, 34, 27,
  63, 62, 60, 57, 53, 48, 42, 35
};

uint64 attaco_cavallo[64], attaco_re[64], attaco_pedone_bianco[64],
  attaco_pedone_nero[64], linea_obscura[4][64][64];
int linea_turno[4][64];

static const uint64 pila_traversa[8] =
{ 0x00000000000000ff, 0x000000000000ff00,
  0x0000000000ff0000, 0x00000000ff000000,
  0x000000ff00000000, 0x0000ff0000000000,
  0x00ff000000000000, 0xff00000000000000
};

#define TRAVERSA1 0x00000000000000ff
#define TRAVERSA2 0x000000000000ff00
#define TRAVERSA3 0x0000000000ff0000
#define TRAVERSA4 0x00000000ff000000
#define TRAVERSA5 0x000000ff00000000
#define TRAVERSA6 0x0000ff0000000000
#define TRAVERSA7 0x00ff000000000000
#define TRAVERSA8 0xff00000000000000

static const uint64 pila_colonna[8] =
{ 0x0101010101010101, 0x0202020202020202,
  0x0404040404040404, 0x0808080808080808,
  0x1010101010101010, 0x2020202020202020,
  0x4040404040404040, 0x8080808080808080
};

#define COLONNAa 0x0101010101010101
#define COLONNAb 0x0202020202020202
#define COLONNAc 0x0404040404040404
#define COLONNAd 0x0808080808080808
#define COLONNAe 0x1010101010101010
#define COLONNAf 0x2020202020202020
#define COLONNAg 0x4040404040404040
#define COLONNAh 0x8080808080808080

uint64 quadretto_fisso[64], quadretto_libero[64],
  non_diagonale[64], non_ortogonale[64];
uint64 ortogonale[64], diagonale[64], ortodiagonale[64];

static const uint64 traversa26nonA = 0x0000fefefefefe00;
static const uint64 traversa26nonAB = 0x0000fcfcfcfcfc00;
static const uint64 traversa26 = 0x0000ffffffffff00;
static const uint64 traversa26nonH = 0x00007f7f7f7f7f00;
static const uint64 traversa26nonGH = 0x00003f3f3f3f3f00;
static const uint64 traversa37nonA = 0x00fefefefefe0000;
static const uint64 traversa37nonAB = 0x00fcfcfcfcfc0000;
static const uint64 traversa37 = 0x00ffffffffffff0000;
static const uint64 traversa37nonGH = 0x003f3f3f3f3f0000;
static const uint64 traversa37nonH = 0x007f7f7f7f7f0000;
static const uint64 CHIARO = 0x55aa55aa55aa55aa;
static const uint64 SCURO = 0xaa55aa55aa55aa55;
static const uint64 traversa2nonA = 0x000000000000fe00;
static const uint64 traversa2nonH = 0x0000000000007f00;
static const uint64 traversa7nonA = 0x00fe000000000000;
static const uint64 traversa7nonH = 0x007f000000000000;
static const uint64 nonA = 0xfefefefefefefefe;
static const uint64 nonH = 0x7f7f7f7f7f7f7f7f;

uint64 colonna_aperta_bianca[64], colonna_aperta_nera[64],
  pedone_libero_bianco[64], pedone_libero_nero[64];
uint64 pedone_protetto_bianco[64], pedone_protetto_nero[64];
uint64 pila_isolato2_nero[64], pila_isolato2_bianco[64], pedone_connesso[64];
uint64 anteriore_bianco[8], non_anteriore_bianco[8], anteriore_nero[8],
  non_anteriore_nero[8], colonna_isolata[8];
uint64 colonnas_sinistras[8], colonnas_rettas[8];

static const uint64 F1G1 = 0x0000000000000060;
static const uint64 C1D1 = 0x000000000000000c;
static const uint64 B1C1D1 = 0x000000000000000e;
static const uint64 F8G8 = 0x6000000000000000;
static const uint64 C8D8 = 0x0c00000000000000;
static const uint64 B8C8D8 = 0x0e00000000000000;

uint64 DOPPIO[64];

uint64 ovest2[64], est2[64], ovest1[64], est1[64],
  adiacente[64], diagonale_lungo[64];

uint64 quadrante_bianco_mossa_bianca[64], quadrante_nero_mossa_bianca[64];
uint64 quadrante_bianco_mossa_nera[64], quadrante_nero_mossa_nera[64];
uint64 bianco_guido[64], nero_guido[64];

uint64 INTERPOSIZIONE[64][64], EVASIONE[64][64];
uint64 nord_ovest[64], sud_ovest[64], nord_est[64], sud_est[64];
uint64 sinistro90_reset[64], sinistro45_reset[64], retto45_reset[64],
  sinistro90_fisso[64], sinistro45_fisso[64], retto45_fisso[64];
sint8 indice_linea[64][64];
uint64 zobrist_oo[16], ZOBRIST[16][64], zobrist_ep[8], zobrist_mossa_bianca;
