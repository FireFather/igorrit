#ifndef BUILD_nodo_superiore
#define BUILD_nodo_superiore
#include "RobboLito.h"
#define RETURN(x) return(x)

#include "sorvegliare.h"
tipo_lista_mossa RADICE_LISTA_MOSSA[256];
#include "nodo_superiore.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

int
mio_nodo_superiore (tipo_posizione *POSIZIONE, int BASSO, int ALTO, int abisso)
{
  int CNT, basso_orig, valu_migliore, cnt,
    scacco_mossa, abisso_nuovo, v, TEMPvalu;
  tipo_lista_mossa *p, *q;
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  uint32 mossa;
  int ESTENSIONE;
  int ai;

  if (ALTO > VALU_MATTO)
    ALTO = VALU_MATTO;
  if (BASSO < -VALU_MATTO)
    BASSO = -VALU_MATTO;
  if (abisso > 8 && abisso <= 70)
  TRASMISSIONE ("info depth %d\n", abisso / 2);
  CNT = 0;
  for (p = RADICE_LISTA_MOSSA; p->mossa; p++)
    {
      CNT++;
      p->mossa &= 0x7fff;
    }
  basso_orig = BASSO;
  p = RADICE_LISTA_MOSSA;
  v = valu_migliore = -VALU_INFINITO;
  cnt = 0; /* SMP ? */
  while ((mossa = p->mossa))
    {
      FARE (POSIZIONE, mossa);
      chiamata_valu_piena (mossa);
      scacco_mossa = (mossa_scacco != 0);
#if 0
      abisso_nuovo = abisso - (2 - scacco_mossa); /* estenione ? */
#else
      ESTENSIONE =0;
      ai = AI (mossa);
      if (POSIZIONE1->cattura || scacco_mossa ||
	  pedone_libero_mossa (ai, TRAVERSA_QUARTA (ai)))
	ESTENSIONE = 1;
      abisso_nuovo = abisso - 2 + ESTENSIONE;
#endif
      if ((abisso >= 24 && (orologio () - OROLOGIO) > 4000000) || ANALISI)
	TRASMISSIONE ("info currmove %s currmovenumber %d\n",
		      mossa_notazione (mossa, STRING1),
		      (p - RADICE_LISTA_MOSSA) + 1);
      if (valu_migliore == -VALU_INFINITO || abisso <= 2)
	v = -tuo_nodo_pv (POSIZIONE, -ALTO, -BASSO, abisso_nuovo, scacco_mossa);
      else
	{
#if 0
	  if (scacco_mossa)
	    {
	      if (abisso_nuovo <= 7)
		v = -tuo_cerca_mezzo_scacco (POSIZIONE, -BASSO, abisso_nuovo);
	      else
		v = -tuo_nodo_tagliato_scacco (POSIZIONE, -BASSO, abisso_nuovo);
	    }
	  else
	    {
	      if (abisso_nuovo <= 7)
		v = -tuo_cerca_mezzo (POSIZIONE, -BASSO, abisso_nuovo);
	      else
		v = -tuo_nodo_tagliato (POSIZIONE, -BASSO, abisso_nuovo);
	    }
#else
	  if (abisso_nuovo <= 7)
	    {
	      if (scacco_mossa)
		v = -tuo_cerca_mezzo_scacco (POSIZIONE, -BASSO, abisso_nuovo);
	      else
		v = -tuo_cerca_mezzo (POSIZIONE, -BASSO, abisso_nuovo);
	    }
	  else
	    {
#if 1
	      if (abisso_nuovo >= 16 && ANALISI)
		{
		  int an = abisso_nuovo - 12;
		  v = VALU_INFINITO;
		  while (an <= abisso_nuovo && v > BASSO)
		    {
		      v = -tuo_nodo_pv (POSIZIONE, -BASSO - 1, -BASSO, an, scacco_mossa);
		      an += 4;
		    }
		  if (an > abisso_nuovo)
		    {
		      MOSSA_BATTAGLIA = 1;
		      FACILE = 0;
		      goto DEC;
		    }
		}
#endif
	      if (scacco_mossa)
		v = -tuo_nodo_tagliato_scacco (POSIZIONE, -BASSO, abisso_nuovo);
	      else
		v = -tuo_nodo_tagliato (POSIZIONE, -BASSO, abisso_nuovo);
	    }

#endif
	  if (v > BASSO)
	    {
	      MOSSA_BATTAGLIA = 1;
	      FACILE = 0;
	    }
	  if (v > BASSO)
	    v = -tuo_nodo_pv (POSIZIONE, -BASSO - 1, -BASSO, abisso_nuovo, scacco_mossa);
	DEC:
	  if (v > BASSO)
	    v = -tuo_nodo_pv (POSIZIONE, -ALTO, -BASSO, abisso_nuovo, scacco_mossa);
	  if (v <= BASSO)
	    v = BASSO;
	}
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      if (v <= BASSO)
	TEMPvalu = basso_orig;
      else
	TEMPvalu = v;
      p->mossa |= (TEMPvalu + 0x8000) << 16;
      if (v > valu_migliore)
	{
	  valu_migliore = v;
	  if (valu_migliore == -VALU_INFINITO || v > BASSO)
	    {
	      radice_mossa = mossa;
	      radice_valu = v;
	      radice_abisso = abisso;
		  if ((abisso > 12 && abisso <= 80) || (abisso == 100) || (abisso == 120) || (abisso == 140)
			  || (abisso == 160) || (abisso == 180) || ANALISI)
		  {
			if (v > BASSO && v < ALTO)
		        informazione (POSIZIONE, orologio () - OROLOGIO, basso_orig, v, ALTO);
			else if (v < ALTO)
				informazione (POSIZIONE, orologio () - OROLOGIO, basso_orig, BASSO, ALTO);
			else if (v > BASSO)
				informazione (POSIZIONE, orologio () - OROLOGIO, basso_orig, ALTO, BASSO);
		  }
	      if (v >= radice_precedente - 25)
		MOSSA_BRUTTA = 0;
	      else
		{
		  MOSSA_BRUTTA = 1;
		  FACILE = 0;
		}
	    }
	}
      if (v <= BASSO)
	{
	  if (cnt == 0)
	    {
	      MOSSA_BRUTTA = 1;
	      FACILE = 0;
	    }
	}
      else
	BASSO = v;
      cnt++;
      if (v < ALTO)
	{
	  p++;
	  continue;
	}
      break;
    }
  for (p = RADICE_LISTA_MOSSA + (CNT - 1); p >= RADICE_LISTA_MOSSA; p--)
    {
      mossa = p->mossa;
      for (q = p + 1; q < RADICE_LISTA_MOSSA + CNT; q++)
	{
	  if ((mossa & 0xffff0000) < (q->mossa & 0xffff0000))
	    (q - 1)->mossa = q->mossa;
	  else
	    break;
	}
      q--;
      q->mossa = mossa;
    }
  radice_abisso = abisso;
  return valu_migliore;
}
