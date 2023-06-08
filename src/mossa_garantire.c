#ifndef BUILD_mossa_garantire
#define BUILD_mossa_garantire
#include "RobboLito.h"

#include "mossa_garantire.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

logico
mio_garantire (tipo_posizione *POSIZIONE, uint32 mossa)
{
  int di, ai, pezzo, cattura;
  uint64 cel;
  ai = AI (mossa);
  cel = quadretto_fisso[ai];
  di = DI (mossa);
  pezzo = POSIZIONE->qu[di];
  if (pezzo == 0)
    return FALSO;
  if (tuo_pezzo (pezzo))
    return FALSO;
  cattura = POSIZIONE->qu[ai];
  if (cattura && mio_pezzo (cattura))
    return FALSO;
  if (cattura == conto_tuo_re)
    return FALSO;
  if (pezzo == conto_mio_pedone)
    {
      if (TRAVERSA_OTTAVA (ai) && !mossa_promozione (mossa))
	return FALSO;
      if (mossa_en_passant (mossa) && ai == DINAMICO->en_passant &&
	  (di == indietro_sinistro (ai) || di == indietro_retto (ai)))
	return VERO;
      if (di == indietro_sinistro (ai) || di == indietro_retto (ai))
	{
	  if (quadretto_fisso[ai] & tuo_occupato)
	    return VERO;
	  return FALSO;
	}
      if (di == indietro (ai))
	{
	  if ((quadretto_fisso[ai] & POSIZIONE->occupato_nero_bianco) == 0)
	    return VERO;
	  return FALSO;
	}
      if (di != indietro_due (ai) || TRAVERSA (di) != numero_traversa2)
	return FALSO;
      if (POSIZIONE->occupato_nero_bianco & quadretto_fisso[avanti (di)])
	return FALSO;
      return VERO;
    }
  if (pezzo == conto_mio_cavallo)
    {
      if (attaco_cavallo[di] & cel)
	return VERO;
      return FALSO;
    }
  if (pezzo == conto_mio_chiaro || pezzo == conto_mio_scuro)
    {
      if (attaco_alfiere (di) & cel)
	return VERO;
      return FALSO;
    }
  if (mossa_arrocco (mossa))
    {
      if (ai == BIANCO_G1)
	{
	  if (!arrocco_oo || POSIZIONE->occupato_nero_bianco & BIANCO_F1G1
	      || tuo_attaco & BIANCO_F1G1)
	    return FALSO;
	  return VERO;
	}
      if (ai == BIANCO_C1)
	{
	  if (!arrocco_ooo || POSIZIONE->occupato_nero_bianco & BIANCO_B1C1D1
	      || tuo_attaco & BIANCO_C1D1)
	    return FALSO;
	  return VERO;
	}
    }
  if (pezzo == conto_mio_torre)
    {
      if (attaco_torre (di) & cel)
	return VERO;
      return FALSO;
    }
  if (pezzo == conto_mio_donna)
    {
      if (attaco_donna (di) & cel)
	return VERO;
      return FALSO;
    }
  if (pezzo == conto_mio_re)
    {
      if (attaco_re[di] & cel && (quadretto_fisso[ai] & tuo_attaco) == 0)
	return VERO;
      return FALSO;
    }
  return FALSO;
}
