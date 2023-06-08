
#define avamposto_bianco 0x00007e7e7e000000
#define avamposto_nero 0x0000007e7e7e0000

#define bitbordo_due(x,y) (((uint64) 1)<<(x))|(((uint64) 1)<<(y))
static const uint64 trappola_torre[64] =
{ 0, bitbordo_due (A1, A2), bitbordo_due (A1, A2) | bitbordo_due (B1, B2), 0,
  0, bitbordo_due (H1, H2) | bitbordo_due (G1, G2), bitbordo_due (H1, H2), 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, bitbordo_due (A8, A7), bitbordo_due (A8, A7) | bitbordo_due (B8, B7), 0,
  0, bitbordo_due (H8, H7) | bitbordo_due (G8, G7), bitbordo_due (H8, H7), 0
};

#define puntos(x,y) (( (x) << 16) + (y))

#define pedone_mobilita_non puntos(3, 10)
#define mobilita_donna(Y) puntos(2, 2) * POPCNT(Y)
#define mobilita_alfiere(Y, Z) puntos (5, 5) * POPCNT( Y & Z)
#define mobilita_torre(Y) puntos(2, 3) * POPCNT(Y)
#define mobilita_cavallo(Y, Z) puntos (6, 8) * POPCNT(Y & Z)

#define xray_alfiere0 puntos(0, 0)
#define xray_alfiere_mio_pedone puntos(3, 5)
#define xray_alfiere_mio_cavallo puntos(3, 5)
#define xray_alfiere_mio_re puntos(3, 5)
#define xray_alfiere_mio_alfiere puntos(0, 0)
#define xray_alfiere_mio_torre puntos(3, 5)
#define xray_alfiere_mio_donna puntos(0, 0)

#define xray_alfiere_tuo_pedone puntos(2, 5)
#define xray_alfiere_tuo_cavallo puntos(2, 5)
#define xray_alfiere_tuo_re puntos(0, 0)
#define xray_alfiere_tuo_alfiere puntos(0, 0)
#define xray_alfiere_tuo_torre puntos(15, 25)
#define xray_alfiere_tuo_donna puntos(10, 20)

#define xray_torre0 puntos(0, 0)
#define xray_torre_mio_pedone puntos(0, 0)
#define xray_torre_mio_cavallo puntos(3, 5)
#define xray_torre_mio_re puntos(3, 5)
#define xray_torre_mio_alfiere puntos(3, 5)
#define xray_torre_mio_torre puntos(0, 0)
#define xray_torre_mio_donna puntos(0, 0)

#define xray_torre_tuo_pedone puntos(2, 5)
#define xray_torre_tuo_cavallo puntos(2, 5)
#define xray_torre_tuo_re puntos(0, 0)
#define xray_torre_tuo_alfiere puntos(2, 5)
#define xray_torre_tuo_torre puntos(0, 0)
#define xray_torre_tuo_donna puntos(10, 20)

static const uint32 bianco_alfiere_xray[16] =
{ xray_alfiere0, xray_alfiere_mio_pedone,
  xray_alfiere_mio_cavallo, xray_alfiere_mio_re,
  xray_alfiere_mio_alfiere, xray_alfiere_mio_alfiere,
  xray_alfiere_mio_torre, xray_alfiere_mio_donna,
  xray_alfiere0, xray_alfiere_tuo_pedone,
  xray_alfiere_tuo_cavallo, xray_alfiere_tuo_re,
  xray_alfiere_tuo_alfiere, xray_alfiere_tuo_alfiere,
  xray_alfiere_tuo_torre, xray_alfiere_tuo_donna };

static const uint32 nero_alfiere_xray[16] =
{ xray_alfiere0, xray_alfiere_tuo_pedone,
  xray_alfiere_tuo_cavallo, xray_alfiere_tuo_re,
  xray_alfiere_tuo_alfiere, xray_alfiere_tuo_alfiere,
  xray_alfiere_tuo_torre, xray_alfiere_tuo_donna,
  xray_alfiere0, xray_alfiere_mio_pedone,
  xray_alfiere_mio_cavallo, xray_alfiere_mio_re,
  xray_alfiere_mio_alfiere, xray_alfiere_mio_alfiere,
  xray_alfiere_mio_torre, xray_alfiere_mio_donna };

static const uint32 bianco_torre_xray[16] =
{ xray_torre0, xray_torre_mio_pedone,
  xray_torre_mio_cavallo, xray_torre_mio_re,
  xray_torre_mio_alfiere, xray_torre_mio_alfiere,
  xray_torre_mio_torre, xray_torre_mio_donna,
  xray_torre0, xray_torre_tuo_pedone,
  xray_torre_tuo_cavallo, xray_torre_tuo_re,
  xray_torre_tuo_alfiere, xray_torre_tuo_alfiere,
  xray_torre_tuo_torre, xray_torre_tuo_donna };

static const uint32 nero_torre_xray[16] =
{ xray_torre0, xray_torre_tuo_pedone,
  xray_torre_tuo_cavallo, xray_torre_tuo_re,
  xray_torre_tuo_alfiere, xray_torre_tuo_alfiere,
  xray_torre_tuo_torre, xray_torre_tuo_donna,
  xray_torre0, xray_torre_mio_pedone,
  xray_torre_mio_cavallo, xray_torre_mio_re,
  xray_torre_mio_alfiere, xray_torre_mio_alfiere,
  xray_torre_mio_torre, xray_torre_mio_donna };

#define xray_donna_diag0 puntos(0, 0)
#define xray_donna_diag_mio_pedone puntos(1, 2)
#define xray_donna_diag_mio_cavallo puntos(2, 4)
#define xray_donna_diag_mio_re puntos(2, 4)
#define xray_donna_diag_mio_alfiere puntos(0, 0)
#define xray_donna_diag_mio_torre puntos(2, 4)
#define xray_donna_diag_mio_donna puntos(0, 0)

#define xray_donna_diag_tuo_pedone puntos(0, 0)
#define xray_donna_diag_tuo_cavallo puntos(2, 5)
#define xray_donna_diag_tuo_re puntos(0, 0)
#define xray_donna_diag_tuo_alfiere puntos(0, 0)
#define xray_donna_diag_tuo_torre puntos(2, 5)
#define xray_donna_diag_tuo_donna puntos(0, 0)

#define xray_donna_orto0 puntos(0, 0)
#define xray_donna_orto_mio_pedone puntos(0, 0)
#define xray_donna_orto_mio_cavallo puntos(2, 4)
#define xray_donna_orto_mio_re puntos(2, 4)
#define xray_donna_orto_mio_alfiere puntos(2, 4)
#define xray_donna_orto_mio_torre puntos(0, 0)
#define xray_donna_orto_mio_donna puntos(0, 0)

#define xray_donna_orto_tuo_pedone puntos(0, 0)
#define xray_donna_orto_tuo_cavallo puntos(2, 5)
#define xray_donna_orto_tuo_re puntos(0, 0)
#define xray_donna_orto_tuo_alfiere puntos(2, 5)
#define xray_donna_orto_tuo_torre puntos(0, 0)
#define xray_donna_orto_tuo_donna puntos(0, 0)

static const uint32 bianco_donna_diag_xray[16] =
{ xray_donna_diag0, xray_donna_diag_mio_pedone,
  xray_donna_diag_mio_cavallo, xray_donna_diag_mio_re,
  xray_donna_diag_mio_alfiere, xray_donna_diag_mio_alfiere,
  xray_donna_diag_mio_torre, xray_donna_diag_mio_donna,
  xray_donna_diag0, xray_donna_diag_tuo_pedone,
  xray_donna_diag_tuo_cavallo, xray_donna_diag_tuo_re,
  xray_donna_diag_tuo_alfiere, xray_donna_diag_tuo_alfiere,
  xray_donna_diag_tuo_torre, xray_donna_diag_tuo_donna };

static const uint32 nero_donna_diag_xray[16] =
{ xray_donna_diag0, xray_donna_diag_tuo_pedone,
  xray_donna_diag_tuo_cavallo, xray_donna_diag_tuo_re,
  xray_donna_diag_tuo_alfiere, xray_donna_diag_tuo_alfiere,
  xray_donna_diag_tuo_torre, xray_donna_diag_tuo_donna,
  xray_donna_diag0, xray_donna_diag_mio_pedone,
  xray_donna_diag_mio_cavallo, xray_donna_diag_mio_re,
  xray_donna_diag_mio_alfiere, xray_donna_diag_mio_alfiere,
  xray_donna_diag_mio_torre, xray_donna_diag_mio_donna };

static const uint32 bianco_donna_orto_xray[16] =
{ xray_donna_orto0, xray_donna_orto_mio_pedone,
  xray_donna_orto_mio_cavallo, xray_donna_orto_mio_re,
  xray_donna_orto_mio_alfiere, xray_donna_orto_mio_alfiere,
  xray_donna_orto_mio_torre, xray_donna_orto_mio_donna,
  xray_donna_orto0, xray_donna_orto_tuo_pedone,
  xray_donna_orto_tuo_cavallo, xray_donna_orto_tuo_re,
  xray_donna_orto_tuo_alfiere, xray_donna_orto_tuo_alfiere,
  xray_donna_orto_tuo_torre, xray_donna_orto_tuo_donna };

static const uint32 nero_donna_orto_xray[16] =
{ xray_donna_orto0, xray_donna_orto_tuo_pedone,
  xray_donna_orto_tuo_cavallo, xray_donna_orto_tuo_re,
  xray_donna_orto_tuo_alfiere, xray_donna_orto_tuo_alfiere,
  xray_donna_orto_tuo_torre, xray_donna_orto_tuo_donna,
  xray_donna_orto0, xray_donna_orto_mio_pedone,
  xray_donna_orto_mio_cavallo, xray_donna_orto_mio_re,
  xray_donna_orto_mio_alfiere, xray_donna_orto_mio_alfiere,
  xray_donna_orto_mio_torre, xray_donna_orto_mio_donna };

static const uint8 alfiere_trappola_quadretto[64] =
{ 0x00,  C2,  0x00, 0x00, 0x00, 0x00,  F2,  0x00,
   B3,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  G3,
   B4,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  G4,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   B5,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  G5,
   B6,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  G6,
  0x00,  C7,  0x00, 0x00, 0x00, 0x00,  F7,  0x00 };

#define alfiere_trappola_valu puntos(40, 40)
#define alfiere_trappola_guardato_valu puntos(40, 40)
static const uint8 buono_trappola_alfiere_quadretto[64] =
{ 0x00,  D1,  0x00, 0x00, 0x00, 0x00,  E1,  0x00,
   C2,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  F2,
   C3,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  F3,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   C6,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  F6,
   C7,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  F7,
  0x00,  D8,  0x00, 0x00, 0x00, 0x00,  E8,  0x00 };

static const uint32 pedone_libero_mio_ok[8] =
  { 0, 0, 0, puntos (0, 0), puntos (0, 0), puntos (3, 5), puntos (5, 10), 0 };
static const uint32 pedone_libero_tuo_ok[8] =
  { 0, 0, 0, puntos (0, 0), puntos (5, 10), puntos (15, 30), puntos (25, 50) };
static const uint32 pedone_libero_mobilita[8] =
  { 0, 0, 0, puntos (1, 2), puntos (2, 3), puntos (3, 5), puntos (5, 10), 0 };
static const uint32 pedone_libero_libero[8] =
  { 0, 0, 0, puntos (0, 0), puntos (5, 10), puntos (10, 20), puntos (20, 40) };

#define donna_guardato puntos(5, 2)
#define torre_guardato puntos(3, 1)
#define alfiere_guardato puntos(2, 1)
#define cavallo_guardato puntos(4, 2)

#define donna_settima_doppia puntos(10, 15)
#define torre_settima_re_pedone_doppia puntos(10, 20)
#define attacco_molteplice puntos(15, 25)
#define donna_settima puntos(5, 25)
#define re_attacco_nonguardato_pedone puntos(0, 5)

#define pedone_attacco_donna puntos(8, 12)
#define torre_attacco_donna puntos(5, 5)
#define alfiere_attacco_donna_torre puntos(7, 10)
#define cavallo_attacco_donna_torre puntos(7, 10)
#define pedone_attacco_torre puntos(7, 10)
#define pedone_attacco_cavallo puntos(5, 7)
#define pedone_attacco_alfiere puntos(5, 7)
#define alfiere_attacco_cavallo puntos(5, 5)
#define cavallo_attacco_alfiere puntos(5, 5)
#define donna_attacco puntos(4, 4)
#define torre_attacco_minore puntos(4, 5)
#define torre_attacco_pedone puntos(2, 3)
#define cavallo_attacco_pedone puntos(3, 4)
#define alfiere_attacco_pedone puntos(3, 4)

#define torre_aperto_mezzo puntos(3, 6)
#if 1
#define torre_aperto puntos(20, 10)
#define torre_aperto_minore_fisso puntos(10, 0)
#define torre_aperto_minore puntos(15, 5)
#else
int torre_apertoA[9] =
{ puntos (20, 10), puntos (18, 9), puntos (16, 8), puntos (14, 7),
  puntos (12, 6), puntos (10, 5), puntos (8, 4), puntos (6, 3), puntos (4, 2) };

int torre_aperto_minore_fissoA[9] =
{ puntos (10, 0), puntos (9, 0), puntos (8, 0), puntos (7, 0), puntos (6, 0),
  puntos (5, 0), puntos (4, 0), puntos (3, 0), puntos (2, 0) };
int torre_aperto_minoreA[9] =
{ puntos (15, 5), puntos (13, 4), puntos (11, 4), puntos (10, 3),
  puntos (8, 3), puntos (6, 2), puntos (5, 2), puntos (3, 1), puntos (2, 0) };

#define torre_aperto\
 ( torre_apertoA[PEDONE_VALU->aperto_colonna_conteggio] + puntos(5, 3) )
#define torre_aperto_minore_fisso\
 ( torre_aperto_minore_fissoA[PEDONE_VALU->aperto_colonna_conteggio]\
   + puntos(3, 0) )
#define torre_aperto_minore\
 ( torre_aperto_minoreA[PEDONE_VALU->aperto_colonna_conteggio] + puntos(4, 2) )
#endif

#define torre_mezzo_pedone puntos(5, 5)
#define torre_mezzo_re puntos(15, 0)
#define torre_ottava_re puntos(5, 10)
#define torre_settima_re_pedone puntos(10, 30)
#define torra_sessa_re_pedone puntos(5, 15)

#define avamposto_alfiere puntos(1, 2)
#define avamposto_alfiere_guardato puntos(3, 4)
#define avamposto_torre puntos(1, 2)
#define avamposto_torre_guardato puntos(3, 4)

#define avamposto_cavallo puntos(2, 3)
#define avamposto_cavallo_pedone puntos(2, 3)
#define avamposto_cavallo_guardato puntos(5, 5)
#define avamposto_cavallo_quinto puntos(2, 2)
#define avamposto_cavallo_DE puntos(3, 3)

static const uint32 re_sicura_moltiplicatore[16] =
  { 0, 1, 4, 9, 16, 25, 36, 49, 50, 50, 50, 50, 50, 50, 50, 50 };
#define COLPO(x,y) ( (x) << 16) + (y)
#define COLPO_pedone COLPO(1, 0)
#define COLPO_donna COLPO(1, 40)
#define COLPO_torre COLPO(1, 25)
#define COLPO_cavallo COLPO(1, 15)
#define COLPO_alfiere COLPO(1, 15)
#define COLPO_re COLPO(0, 0)
#define re_sicura_dividendo 8

static const uint32 traversa_finale_donna[8] =
{ 0, 0, 0, puntos (5, 5), puntos (10, 10), puntos (20, 20), puntos (40, 40), 0 };

#if 0
#define finale_torre_settima puntos(20, 50)
#define finale_torre_sessa puntos(0, 15)
#define finale_donna_settima puntos(0, 10)
#else
#define finale_torre_settima puntos(100, 100)
#define finale_torre_sessa puntos(25, 25)
#define finale_donna_settima puntos(10, 10)
#endif

static const uint64 crampo_colonna[8] =
  { COLONNAb, 0, 0, 0, 0, 0, 0, COLONNAg };
