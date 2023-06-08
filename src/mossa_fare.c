#include "RobboLito.h"
#include "mfa.h"
#include "valore.i"

typedef enum
{ ooK = 1, ooQ = 2, ook = 4, ooq = 8 } pila_oo;
typedef enum
{ KQkq = ooK | ooQ | ook | ooq,
  Qkq = ooQ | ook | ooq, Kkq = ooK | ook | ooq, kq = ook | ooq,
  KQk = ooK | ooQ | ook, KQ = ooK | ooQ, KQq = ooK | ooQ | ooq
} pila_KQkq;

static const uint64 oo_arrocco[64] =
  { Kkq, KQkq, KQkq, KQkq, kq, KQkq, KQkq, Qkq,
    KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq,
    KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq,
    KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq,
    KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq,
    KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq,
    KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq, KQkq,
    KQk, KQkq, KQkq, KQkq, KQ, KQkq, KQkq, KQq
};

const static uint64 en_passant_bianco[8] =
  { bitbordo_due (B4, B4), bitbordo_due (A4, C4),
    bitbordo_due (B4, D4), bitbordo_due (C4, E4),
    bitbordo_due (D4, F4), bitbordo_due (E4, G4),
    bitbordo_due (F4, H4), bitbordo_due (G4, G4) };
const static uint64 en_passant_nero[8] =
  { bitbordo_due (B5, B5), bitbordo_due (A5, C5),
    bitbordo_due (B5, D5), bitbordo_due (C5, E5),
    bitbordo_due (D5, F5), bitbordo_due (E5, G5),
    bitbordo_due (F5, H5), bitbordo_due (G5, G5) };

static INLINE void
fare_bianco_arrocco (tipo_posizione *POSIZIONE, int ai)
{
  if (ai == G1)
    {
      bitbordo_occupato_bianco ^= F1H1;
      bitbordo_bianco_torre ^= F1H1;
      POSIZIONE->occupato_nero_bianco ^= F1H1;
      POSIZIONE->occupato_sinistro90 ^= F1H1_sinistra90;
      POSIZIONE->occupato_sinistro45 ^= F1H1_sinistra45;
      POSIZIONE->occupato_retto45 ^= F1H1_retto45;
      DINAMICO->statico +=
	statico_valu[conto_torre_bianco][F1] -
	statico_valu[conto_torre_bianco][H1];
      DINAMICO->Zobrist ^=
	ZOBRIST[conto_torre_bianco][F1] ^ ZOBRIST[conto_torre_bianco][H1];
      POSIZIONE->qu[H1] = 0;
      POSIZIONE->qu[F1] = conto_torre_bianco;
    }
  else if (ai == C1)
    {
      bitbordo_occupato_bianco ^= A1D1;
      bitbordo_bianco_torre ^= A1D1;
      POSIZIONE->occupato_nero_bianco ^= A1D1;
      POSIZIONE->occupato_sinistro90 ^= A1D1_sinistra90;
      POSIZIONE->occupato_sinistro45 ^= A1D1_sinistra45;
      POSIZIONE->occupato_retto45 ^= A1D1_retto45;
      DINAMICO->statico +=
	statico_valu[conto_torre_bianco][D1] -
	statico_valu[conto_torre_bianco][A1];
      DINAMICO->Zobrist ^=
	ZOBRIST[conto_torre_bianco][A1] ^ ZOBRIST[conto_torre_bianco][D1];
      POSIZIONE->qu[A1] = 0;
      POSIZIONE->qu[D1] = conto_torre_bianco;
    }
}

static INLINE void
fare_nero_arrocco (tipo_posizione *POSIZIONE, int ai)
{
  if (ai == G8)
    {
      bitbordo_occupato_nero ^= F8H8;
      bitbordo_nero_torre ^= F8H8;
      POSIZIONE->occupato_nero_bianco ^= F8H8;
      POSIZIONE->occupato_sinistro90 ^= F8H8_sinistra90;
      POSIZIONE->occupato_sinistro45 ^= F8H8_sinistra45;
      POSIZIONE->occupato_retto45 ^= F8H8_retto45;
      DINAMICO->statico +=
	statico_valu[conto_torre_nero][F8] -
	statico_valu[conto_torre_nero][H8];
      DINAMICO->Zobrist ^=
	ZOBRIST[conto_torre_nero][F8] ^ ZOBRIST[conto_torre_nero][H8];
      POSIZIONE->qu[H8] = 0;
      POSIZIONE->qu[F8] = conto_torre_nero;
    }
  else if (ai == C8)
    {
      bitbordo_occupato_nero ^= A8D8;
      bitbordo_nero_torre ^= A8D8;
      POSIZIONE->occupato_nero_bianco ^= A8D8;
      POSIZIONE->occupato_sinistro90 ^= A8D8_sinistra90;
      POSIZIONE->occupato_sinistro45 ^= A8D8_sinistra45;
      POSIZIONE->occupato_retto45 ^= A8D8_retto45;
      DINAMICO->statico +=
	statico_valu[conto_torre_nero][D8] -
	statico_valu[conto_torre_nero][A8];
      DINAMICO->Zobrist ^=
	ZOBRIST[conto_torre_nero][A8] ^ ZOBRIST[conto_torre_nero][D8];
      POSIZIONE->qu[A8] = 0;
      POSIZIONE->qu[D8] = conto_torre_nero;
    }
}

const static uint8 promozione_bianco[8] =
  { 0, 0, 0, 0,
    conto_cavallo_bianco, conto_chiaro_bianco, conto_torre_bianco, conto_donna_bianca };

void
fare_bianco (tipo_posizione *POSIZIONE, uint32 mossa)
{
  int di, ai, pezzo, pietra, cattura, z;
  uint64 cel;
  POSIZIONE->nodes++;
  if (POSIZIONE->cpu == 0)
    {
      NODE_CHECK++;
      if ((NODE_CHECK & 4095) == 0)
	risolvere_termine (POSIZIONE, 0);
    }
  memcpy (DINAMICO + 1, DINAMICO, 32);
  di = DI (mossa);
  ai = AI (mossa);
  pezzo = POSIZIONE->qu[di];
  DINAMICO++;
  DINAMICO->mossa50++;
  DINAMICO->mossa = mossa;
  pietra = oo_arrocco[di] & oo_arrocco[ai] & DINAMICO->arrocco;
  DINAMICO->Zobrist ^= zobrist_oo[DINAMICO->arrocco ^ pietra];
  DINAMICO->ZobristPedone ^= zobrist_oo[DINAMICO->arrocco ^ pietra];
  DINAMICO->arrocco = pietra;
  if (DINAMICO->en_passant)
    {
      DINAMICO->Zobrist ^= zobrist_ep[DINAMICO->en_passant & 7];
      DINAMICO->en_passant = 0;
    }
  POSIZIONE->qu[di] = 0;
  cel = quadretto_libero[di];
  bitbordo_occupato_bianco &= cel;
  POSIZIONE->bitbordo[pezzo] &= cel;
  occupato_libero (cel, di);
  DINAMICO->statico += statico_valu[pezzo][ai] - statico_valu[pezzo][di];
  cel = ZOBRIST[pezzo][di] ^ ZOBRIST[pezzo][ai];
  cattura = POSIZIONE->qu[ai];
  DINAMICO->cattura = cattura;
  DINAMICO->Zobrist ^= cel;
  if (pezzo == conto_pedone_bianco)
    DINAMICO->ZobristPedone ^= cel;
  POSIZIONE->bianco_en_mossa ^= 1;
  POSIZIONE->height++;
  DINAMICO->Zobrist ^= zobrist_mossa_bianca;
  if (pezzo == conto_re_bianco)
    {
      DINAMICO->ZobristPedone ^= cel;
      POSIZIONE->bianco_re_quadretto = ai;
    }
  if (cattura)
    {
      cel = quadretto_libero[ai];
      bitbordo_occupato_nero &= cel;
      POSIZIONE->bitbordo[cattura] &= cel;
      DINAMICO->materiale -= valu_materiale[cattura];
      DINAMICO->statico -= statico_valu[cattura][ai];
      if (cattura == conto_pedone_nero)
	DINAMICO->ZobristPedone ^= ZOBRIST[cattura][ai];
      DINAMICO->Zobrist ^= ZOBRIST[cattura][ai];
      DINAMICO->mossa50 = 0;
    }
  else
    {
      cel = quadretto_fisso[ai];
      occupato_fisso (cel, ai);
      if (mossa_arrocco (mossa))
	{
	  DINAMICO->mossa50 = 0;
	  fare_bianco_arrocco (POSIZIONE, ai);
	}
    }
  POSIZIONE->qu[ai] = pezzo;
  bitbordo_occupato_bianco |= quadretto_fisso[ai];
  POSIZIONE->bitbordo[pezzo] |= quadretto_fisso[ai];
  if (pezzo == conto_pedone_bianco)
    {
      DINAMICO->mossa50 = 0;
      if (mossa_en_passant (mossa))
	{
	  z = ai ^ 8;
	  cel = quadretto_libero[z];
	  bitbordo_occupato_nero &= cel;
	  bitbordo_nero_pedone &= cel;
	  occupato_libero (cel, z);
	  DINAMICO->materiale -= valu_materiale[conto_pedone_nero];
	  DINAMICO->statico -= statico_valu[conto_pedone_nero][z];
	  DINAMICO->Zobrist ^= ZOBRIST[conto_pedone_nero][z];
	  DINAMICO->ZobristPedone ^= ZOBRIST[conto_pedone_nero][z];
	  POSIZIONE->qu[z] = 0;
	}
      else if (mossa_promozione (mossa))
	{
	  pezzo = promozione_bianco[(mossa & pietra_celare) >> 12];
	  if (pezzo == conto_chiaro_bianco && quadretto_fisso[ai] & SCURO)
	    pezzo = conto_scuro_bianco;
	  POSIZIONE->qu[ai] = pezzo;
	  if (POSIZIONE->bitbordo[pezzo])
	    DINAMICO->materiale |= 0x80000000;
	  bitbordo_bianco_pedone &= quadretto_libero[ai];
	  POSIZIONE->bitbordo[pezzo] |= quadretto_fisso[ai];
	  DINAMICO->materiale +=
	    valu_materiale[pezzo] - valu_materiale[conto_pedone_bianco];
	  DINAMICO->statico +=
	    statico_valu[pezzo][ai] - statico_valu[conto_pedone_bianco][ai];
	  DINAMICO->Zobrist ^=
	    ZOBRIST[pezzo][ai] ^ ZOBRIST[conto_pedone_bianco][ai];
	  DINAMICO->ZobristPedone ^= ZOBRIST[conto_pedone_bianco][ai];
	}
      else if ((ai ^ di) == 16)
	{
	  if (en_passant_bianco[ai & 7] & bitbordo_nero_pedone)
	    {
	      z = (di + ai) >> 1;
	      DINAMICO->en_passant = z;
	      DINAMICO->Zobrist ^= zobrist_ep[z & 7];
	    }
	}
    }
  MUCCHIO[++mucchio_altezzo] = DINAMICO->Zobrist;
}

const static uint8 promozione_nero[8] =
  { 0, 0, 0, 0,
    conto_cavallo_nero, conto_chiaro_nero, conto_torre_nero, conto_donna_nera };

void
fare_nero (tipo_posizione *POSIZIONE, uint32 mossa)
{
  int di, ai, pezzo, pietra, cattura, z;
  uint64 cel;
  POSIZIONE->nodes++;
  memcpy (DINAMICO + 1, DINAMICO, 32);
  di = DI (mossa);
  ai = AI (mossa);
  pezzo = POSIZIONE->qu[di];
  DINAMICO++;
  DINAMICO->mossa50++;
  DINAMICO->mossa = mossa;
  pietra = oo_arrocco[di] & oo_arrocco[ai] & DINAMICO->arrocco;
  DINAMICO->Zobrist ^= zobrist_oo[DINAMICO->arrocco ^ pietra];
  DINAMICO->ZobristPedone ^= zobrist_oo[DINAMICO->arrocco ^ pietra];
  DINAMICO->arrocco = pietra;
  if (DINAMICO->en_passant)
    {
      DINAMICO->Zobrist ^= zobrist_ep[DINAMICO->en_passant & 7];
      DINAMICO->en_passant = 0;
    }
  POSIZIONE->qu[di] = 0;
  cel = quadretto_libero[di];
  bitbordo_occupato_nero &= cel;
  POSIZIONE->bitbordo[pezzo] &= cel;
  occupato_libero (cel, di);
  DINAMICO->statico += statico_valu[pezzo][ai] - statico_valu[pezzo][di];
  cel = ZOBRIST[pezzo][di] ^ ZOBRIST[pezzo][ai];
  cattura = POSIZIONE->qu[ai];
  DINAMICO->cattura = cattura;
  DINAMICO->Zobrist ^= cel;
  if (pezzo == conto_pedone_nero)
    DINAMICO->ZobristPedone ^= cel;
  POSIZIONE->bianco_en_mossa ^= 1;
  POSIZIONE->height++;
  DINAMICO->Zobrist ^= zobrist_mossa_bianca;
  if (pezzo == conto_re_nero)
    {
      DINAMICO->ZobristPedone ^= cel;
      POSIZIONE->nero_re_quadretto = ai;
    }
  if (cattura)
    {
      cel = quadretto_libero[ai];
      bitbordo_occupato_bianco &= cel;
      POSIZIONE->bitbordo[cattura] &= cel;
      DINAMICO->materiale -= valu_materiale[cattura];
      DINAMICO->statico -= statico_valu[cattura][ai];
      if (cattura == conto_pedone_bianco)
	DINAMICO->ZobristPedone ^= ZOBRIST[cattura][ai];
      DINAMICO->Zobrist ^= ZOBRIST[cattura][ai];
      DINAMICO->mossa50 = 0;
    }
  else
    {
      cel = quadretto_fisso[ai];
      occupato_fisso (cel, ai);
      if (mossa_arrocco (mossa))
	{
	  DINAMICO->mossa50 = 0;
	  fare_nero_arrocco (POSIZIONE, ai);
	}
    }
  POSIZIONE->qu[ai] = pezzo;
  bitbordo_occupato_nero |= quadretto_fisso[ai];
  POSIZIONE->bitbordo[pezzo] |= quadretto_fisso[ai];
  if (pezzo == conto_pedone_nero)
    {
      DINAMICO->mossa50 = 0;
      if (mossa_en_passant (mossa))
	{
	  z = ai ^ 8;
	  cel = quadretto_libero[z];
	  bitbordo_occupato_bianco &= cel;
	  bitbordo_bianco_pedone &= cel;
	  occupato_libero (cel, z);
	  DINAMICO->materiale -= valu_materiale[conto_pedone_bianco];
	  DINAMICO->statico -= statico_valu[conto_pedone_bianco][z];
	  DINAMICO->Zobrist ^= ZOBRIST[conto_pedone_bianco][z];
	  DINAMICO->ZobristPedone ^= ZOBRIST[conto_pedone_bianco][z];
	  POSIZIONE->qu[z] = 0;
	}
      else if (mossa_promozione (mossa))
	{
	  pezzo = promozione_nero[(mossa & pietra_celare) >> 12];
	  if (pezzo == conto_chiaro_nero && quadretto_fisso[ai] & SCURO)
	    pezzo = conto_scuro_nero;
	  POSIZIONE->qu[ai] = pezzo;
	  if (POSIZIONE->bitbordo[pezzo])
	    DINAMICO->materiale |= 0x80000000;
	  POSIZIONE->bitbordo[conto_pedone_nero] &= quadretto_libero[ai];
	  POSIZIONE->bitbordo[pezzo] |= quadretto_fisso[ai];
	  DINAMICO->materiale +=
	    valu_materiale[pezzo] - valu_materiale[conto_pedone_nero];
	  DINAMICO->statico +=
	    statico_valu[pezzo][ai] - statico_valu[conto_pedone_nero][ai];
	  DINAMICO->Zobrist ^=
	    ZOBRIST[pezzo][ai] ^ ZOBRIST[conto_pedone_nero][ai];
	  DINAMICO->ZobristPedone ^= ZOBRIST[conto_pedone_nero][ai];
	}
      else if ((ai ^ di) == 16)
	{
	  if (en_passant_nero[ai & 7] & bitbordo_bianco_pedone)
	    {
	      z = (di + ai) >> 1;
	      DINAMICO->en_passant = z;
	      DINAMICO->Zobrist ^= zobrist_ep[z & 7];
	    }
	}
    }
  MUCCHIO[++mucchio_altezzo] = DINAMICO->Zobrist;
}

void
fare (tipo_posizione *POSIZIONE, uint32 mossa)
{
  if (POSIZIONE->bianco_en_mossa)
    {
      if (NODE_CHECK & 4095)
	NODE_CHECK--;
      POSIZIONE->nodes--;
      fare_bianco (POSIZIONE, mossa);
    }
  else
    {
      POSIZIONE->nodes--;
      fare_nero (POSIZIONE, mossa);
    }
}
