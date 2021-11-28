#ifndef BUILD_mossa_prossimo
#define BUILD_mossa_prossimo
#include "RobboLito.h"
#include <signal.h>

#include "mossa_prossimo.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

#define RETURN(x) return(x)
uint32
mio_prossimo (tipo_posizione *POSIZIONE, tipo_prossimo * PROSSIMO)
{
  tipo_lista_mossa *p, *q, *lista;
  uint32 mossa, TEMP;
  switch (PROSSIMO->fase)
    {
    case TRAS:
      PROSSIMO->fase = cattura_gen;
      if (PROSSIMO->mossa_tras && mio_garantire (POSIZIONE, PROSSIMO->mossa_tras))
	RETURN (PROSSIMO->mossa_tras);
    case cattura_gen:
      PROSSIMO->fase = mossa_cattura;
      PROSSIMO->mossa = 0;
      mio_cattura (POSIZIONE, PROSSIMO->LISTA, tuo_occupato);
    case mossa_cattura:
      while (1)
	{
	  p = PROSSIMO->LISTA + PROSSIMO->mossa;
	  mossa = p->mossa;
	  if (!mossa)
	    break;
	  q = p + 1;
	  PROSSIMO->mossa++;
	  while (q->mossa)
	    {
	      if (mossa < q->mossa)
		{
		  TEMP = q->mossa;
		  q->mossa = mossa;
		  mossa = TEMP;
		}
	      q++;
	    }
	  if ((mossa & 0x7fff) == PROSSIMO->mossa_tras)
	    continue;
	  if (!SEE_facile (mossa) && !mio_SEE (POSIZIONE, mossa))
	    PROSSIMO->cattura_brutta[PROSSIMO->cb++] = mossa;
	  else
	    break;
	}
      if (mossa)
	RETURN (mossa);
      PROSSIMO->fase = UCCISORE_UNO;
      mossa = DINAMICO->uccisore_uno;
      if (mossa && mossa != PROSSIMO->mossa_tras && POSIZIONE->qu[AI (mossa)] == 0
	  && mio_garantire (POSIZIONE, mossa))
	RETURN (mossa);
    case UCCISORE_UNO:
      PROSSIMO->fase = UCCISORE_DUE;
      mossa = DINAMICO->uccisore_due;
      if (mossa && mossa != PROSSIMO->mossa_tras && POSIZIONE->qu[AI (mossa)] == 0
	  && mio_garantire (POSIZIONE, mossa))
	RETURN (mossa);
    case UCCISORE_DUE:
      PROSSIMO->fase = mossa_ordinaria;
      PROSSIMO->mossa = 0;
      lista = mio_ordinaria (POSIZIONE, PROSSIMO->LISTA);
      sorta_ordinaria (PROSSIMO->LISTA, lista, PROSSIMO->mossa_tras,
		       DINAMICO->uccisore_uno, DINAMICO->uccisore_due);
    case mossa_ordinaria:
      mossa = (PROSSIMO->LISTA + PROSSIMO->mossa)->mossa;
      PROSSIMO->mossa++;
      if (mossa)
	RETURN (mossa);
      PROSSIMO->fase = cattura_brutta;
      PROSSIMO->cattura_brutta[PROSSIMO->cb] = 0;
      PROSSIMO->mossa = 0;
    case cattura_brutta:
      mossa = PROSSIMO->cattura_brutta[PROSSIMO->mossa++];
      RETURN (mossa);
      /* */
    case TRAS2:
      PROSSIMO->fase = cattura_gen2;
      if (PROSSIMO->mossa_tras && mio_garantire (POSIZIONE, PROSSIMO->mossa_tras))
	RETURN (PROSSIMO->mossa_tras);
    case cattura_gen2:
      PROSSIMO->fase = mossa_cattura2;
      PROSSIMO->mossa = 0;
      mio_cattura (POSIZIONE, PROSSIMO->LISTA, PROSSIMO->targa);
    case mossa_cattura2:
      while (1)
	{
	  p = PROSSIMO->LISTA + PROSSIMO->mossa;
	  mossa = p->mossa;
	  if (!mossa)
	    break;
	  q = p + 1;
	  PROSSIMO->mossa++;
	  while (q->mossa)
	    {
	      if (mossa < q->mossa)
		{
		  TEMP = q->mossa;
		  q->mossa = mossa;
		  mossa = TEMP;
		}
	      q++;
	    }
	  if ((mossa & 0x7fff) == PROSSIMO->mossa_tras)
	    continue;
	  else
	    break;
	}
      if (mossa)
	RETURN (mossa);
      PROSSIMO->mossa = 0;
      PROSSIMO->fase = scacco_zitto;
      mio_scacco_zitto (POSIZIONE, PROSSIMO->LISTA, PROSSIMO->targa);
    case scacco_zitto:
      mossa = (PROSSIMO->LISTA + PROSSIMO->mossa)->mossa;
      PROSSIMO->mossa++;
      RETURN (mossa);
      /* */
    case elusione:
      mossa = (PROSSIMO->LISTA + PROSSIMO->mossa)->mossa;
      PROSSIMO->mossa++;
      RETURN (mossa);
      /* */
    case TRAS3:
      PROSSIMO->fase = cattura_gen3;
      if (PROSSIMO->mossa_tras && mio_garantire (POSIZIONE, PROSSIMO->mossa_tras))
	RETURN (PROSSIMO->mossa_tras);
    case cattura_gen3:
      PROSSIMO->fase = mossa_cattura3;
      PROSSIMO->mossa = 0;
      mio_cattura (POSIZIONE, PROSSIMO->LISTA, tuo_occupato);
    case mossa_cattura3:
      while (1)
	{
	  p = PROSSIMO->LISTA + PROSSIMO->mossa;
	  mossa = p->mossa;
	  if (!mossa)
	    break;
	  q = p + 1;
	  PROSSIMO->mossa++;
	  while (q->mossa)
	    {
	      if (mossa < q->mossa)
		{
		  TEMP = q->mossa;
		  q->mossa = mossa;
		  mossa = TEMP;
		}
	      q++;
	    }
	  if ((mossa & 0x7fff) == PROSSIMO->mossa_tras)
	    continue;
	  else
	    break;
	}
      if (mossa)
	RETURN (mossa);
      PROSSIMO->mossa = 0;
      PROSSIMO->fase = scacco_zitto3;
      mio_scacco_zitto (POSIZIONE, PROSSIMO->LISTA, tuo_occupato);
    case scacco_zitto3:
      mossa = (PROSSIMO->LISTA + PROSSIMO->mossa)->mossa;
      PROSSIMO->mossa++;
      if (mossa)
	RETURN (mossa);
      PROSSIMO->mossa = 0;
      PROSSIMO->fase = guadagno;
      mio_guadagno (POSIZIONE, PROSSIMO->LISTA, PROSSIMO->celare);
    case guadagno:
      mossa = (PROSSIMO->LISTA + PROSSIMO->mossa)->mossa;
      PROSSIMO->mossa++;
      RETURN (mossa);
    case FASE_0:
      RETURN (0);
    }
  return 0;
}
