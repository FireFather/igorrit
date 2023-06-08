#include "RobboLito.h"
#include "mfa.h"

static INLINE void
annulla_bianco_arrocco (tipo_posizione *POSIZIONE, int ai)
{
  if (ai == G1)
    {
      bitbordo_occupato_bianco ^= F1H1;
      bitbordo_bianco_torre ^= F1H1;
      POSIZIONE->qu[F1] = 0;
      POSIZIONE->qu[H1] = conto_torre_bianco;
      POSIZIONE->occupato_nero_bianco ^= F1H1;
      POSIZIONE->occupato_sinistro90 ^= F1H1_sinistra90;
      POSIZIONE->occupato_sinistro45 ^= F1H1_sinistra45;
      POSIZIONE->occupato_retto45 ^= F1H1_retto45;
    }
  else if (ai == C1)
    {
      bitbordo_occupato_bianco ^= A1D1;
      bitbordo_bianco_torre ^= A1D1;
      POSIZIONE->qu[D1] = 0;
      POSIZIONE->qu[A1] = conto_torre_bianco;
      POSIZIONE->occupato_nero_bianco ^= A1D1;
      POSIZIONE->occupato_sinistro90 ^= A1D1_sinistra90;
      POSIZIONE->occupato_sinistro45 ^= A1D1_sinistra45;
      POSIZIONE->occupato_retto45 ^= A1D1_retto45;
    }
}

static INLINE void
annulla_nero_arrocco (tipo_posizione *POSIZIONE, int ai)
{
  if (ai == G8)
    {
      bitbordo_occupato_nero ^= F8H8;
      bitbordo_nero_torre ^= F8H8;
      POSIZIONE->qu[F8] = 0;
      POSIZIONE->qu[H8] = conto_torre_nero;
      POSIZIONE->occupato_nero_bianco ^= F8H8;
      POSIZIONE->occupato_sinistro90 ^= F8H8_sinistra90;
      POSIZIONE->occupato_sinistro45 ^= F8H8_sinistra45;
      POSIZIONE->occupato_retto45 ^= F8H8_retto45;
    }
  else if (ai == C8)
    {
      bitbordo_occupato_nero ^= A8D8;
      bitbordo_nero_torre ^= A8D8;
      POSIZIONE->qu[D8] = 0;
      POSIZIONE->qu[A8] = conto_torre_nero;
      POSIZIONE->occupato_nero_bianco ^= A8D8;
      POSIZIONE->occupato_sinistro90 ^= A8D8_sinistra90;
      POSIZIONE->occupato_sinistro45 ^= A8D8_sinistra45;
      POSIZIONE->occupato_retto45 ^= A8D8_retto45;
    }
}

void
annullare_bianco (tipo_posizione *POSIZIONE, uint32 mossa)
{
  int di, ai, pezzo, cattura, z;
  uint64 cel;
  di = DI (mossa);
  ai = AI (mossa);
  pezzo = POSIZIONE->qu[ai];
  POSIZIONE->bianco_en_mossa ^= 1;
  POSIZIONE->height--;
  if (mossa_promozione (mossa))
    {
      POSIZIONE->bitbordo[pezzo] &= quadretto_libero[ai];
      pezzo = conto_pedone_bianco;
    }
  POSIZIONE->qu[di] = pezzo;
  POSIZIONE->qu[ai] = DINAMICO->cattura;
  if (pezzo == conto_re_bianco)
    POSIZIONE->bianco_re_quadretto = di;
  cel = quadretto_fisso[di];
  bitbordo_occupato_bianco |= cel;
  POSIZIONE->bitbordo[pezzo] |= cel;
  occupato_fisso (cel, di);
  cel = quadretto_libero[ai];
  bitbordo_occupato_bianco &= cel;
  POSIZIONE->bitbordo[pezzo] &= cel;
  cattura = DINAMICO->cattura;
  if (cattura)
    {
      cel = ~cel;
      bitbordo_occupato_nero |= cel;
      POSIZIONE->bitbordo[cattura] |= cel;
    }
  else
    {
      occupato_libero (cel, ai);
      if (mossa_arrocco (mossa))
	annulla_bianco_arrocco (POSIZIONE, ai);
      else if (mossa_en_passant (mossa))
	{
	  z = ai ^ 8;
	  POSIZIONE->qu[z] = conto_pedone_nero;
	  cel = quadretto_fisso[z];
	  bitbordo_occupato_nero |= cel;
	  bitbordo_nero_pedone |= cel;
	  occupato_fisso (cel, z);
	}
    }
  DINAMICO--;
  mucchio_altezzo--;
}

void
annullare_nero (tipo_posizione *POSIZIONE, uint32 mossa)
{
  int di, ai, pezzo, cattura, z;
  uint64 cel;
  di = DI (mossa);
  ai = AI (mossa);
  pezzo = POSIZIONE->qu[ai];
  POSIZIONE->bianco_en_mossa ^= 1;
  POSIZIONE->height--;
  if (mossa_promozione (mossa))
    {
      POSIZIONE->bitbordo[pezzo] &= quadretto_libero[ai];
      pezzo = conto_pedone_nero;
    }
  POSIZIONE->qu[di] = pezzo;
  POSIZIONE->qu[ai] = DINAMICO->cattura;
  if (pezzo == conto_re_nero)
    POSIZIONE->nero_re_quadretto = di;
  cel = quadretto_fisso[di];
  bitbordo_occupato_nero |= cel;
  POSIZIONE->bitbordo[pezzo] |= cel;
  occupato_fisso (cel, di);
  cel = quadretto_libero[ai];
  bitbordo_occupato_nero &= cel;
  POSIZIONE->bitbordo[pezzo] &= cel;
  cattura = DINAMICO->cattura;
  if (cattura)
    {
      cel = ~cel;
      bitbordo_occupato_bianco |= cel;
      POSIZIONE->bitbordo[cattura] |= cel;
    }
  else
    {
      occupato_libero (cel, ai);
      if (mossa_arrocco (mossa))
	annulla_nero_arrocco (POSIZIONE, ai);
      else if (mossa_en_passant (mossa))
	{
	  z = ai ^ 8;
	  POSIZIONE->qu[z] = conto_pedone_bianco;
	  cel = quadretto_fisso[z];
	  bitbordo_occupato_bianco |= cel;
	  bitbordo_bianco_pedone |= cel;
	  occupato_fisso (cel, z);
	}
    }
  DINAMICO--;
  mucchio_altezzo--;
}

void
annullare (tipo_posizione *POSIZIONE, uint32 mossa)
{
  if (!POSIZIONE->bianco_en_mossa)
    annullare_bianco (POSIZIONE, mossa);
  else
    annullare_nero (POSIZIONE, mossa);
}
