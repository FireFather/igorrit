#ifndef SMP_search
#define SMP_search
#include "RobboLito.h"
#endif

#ifdef YUSUF_MULTICORE
#ifndef BUILD_IVAN_SMP_PV
#define BUILD_IVAN_SMP_PV

static INLINE void SMP_BAD_HISTORY
(tipo_posizione* POS, uint32 m, SPLITPUNKT* sp)
{
  if ((POS->DIN + 1)->cattura == 0 && mossa_storia (m))
    {
      int sv = STORIA_VALU (POS, m);
      if (POS->DIN->valu > sp->alpha - 50)
	  STORIA_VALU (POS, m) = sv - ((sv * sp->abisso) >> 8);
    }
}

#include "SMP_search.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

void mio_smp_nodo_pv (tipo_posizione* POSIZIONE)
{
  int v;
  int alpha;
  int beta;
  int m;
  tipo_prossimo* PROSSIMO;
  SPLITPUNKT* sp;
  int ESTENSIONE;
  int ai;
  int abisso_nuovo;

  sp = POSIZIONE->splitpunkt;
  while (VERO)
    {
      LOCK (sp->splock);
      beta = sp->beta;
      alpha = sp->alpha;
      if (sp->tot)
	{
	  UNLOCK (sp->splock);
	  return;
	}
      PROSSIMO = sp->MOVE_PICK;
      m = mio_prossimo (POSIZIONE, PROSSIMO);      
      if (!m)
	{
	  PROSSIMO->fase = FASE_0;
	  UNLOCK (sp->splock);
	  return;
	}
      UNLOCK (sp->splock);
      FARE (POSIZIONE, m);
      valutazione (POSIZIONE, -0x7fff0000, 0x7fff0000, m);
      if (mio_re_scacco)
        {
          ANNULLA (POSIZIONE, m);
          continue;
        }
      ESTENSIONE = 0;
      ai = AI (m);
      if (pedone_libero_mossa (ai, TRAVERSA_SESTA (ai)))
	ESTENSIONE = 2;
      else
	{
	  if (POSIZIONE->DIN->cattura != 0 || tuo_re_scacco != 0)
	    ESTENSIONE = 1;
	  else if (pedone_libero_mossa (ai, TRAVERSA_QUARTA (ai)))
	    ESTENSIONE = 1;
	}
      /* LMR ? */
      abisso_nuovo = sp->abisso - 2 + ESTENSIONE;
      if (tuo_re_scacco)
	v = -tuo_nodo_tagliato_scacco (POSIZIONE, -alpha, abisso_nuovo);
      else
	v = -tuo_nodo_tagliato (POSIZIONE, -alpha, abisso_nuovo);
      if (v <= alpha)
	{
	  ANNULLA (POSIZIONE, m);
	  if (POSIZIONE->halt)
	    return;
	  SMP_BAD_HISTORY (POSIZIONE, m, sp);
	  continue;
	}
      if (!sp->tot && !POSIZIONE->halt)
	{
	  logico b = (tuo_re_scacco != 0);
	  v = -tuo_nodo_pv (POSIZIONE, -beta, -alpha, abisso_nuovo, b);
	  ANNULLA (POSIZIONE, m);
	  if (POSIZIONE->halt)
	    return;
	  if (v > alpha)
	    {
	      LOCK (sp->splock);
	      if (v > sp->alpha)
		{
		  sp->alpha = v;
		  sp->value = v;
		  sp->mossa_buona = m;
		}
	      UNLOCK (sp->splock);
	      Zobrist_basso (POSIZIONE->DIN->Zobrist, m, sp->abisso, v);
	    }
	}
      else
	ANNULLA (POSIZIONE, m);
      if (POSIZIONE->halt)
	return;
      if (v >= beta)
	{
	  ivan_fail_high (sp, POSIZIONE, m);
	  return;
	}
    }
}

void mio_smp_nodo_all (tipo_posizione* POSIZIONE)
{
  int v;
  int m;
  tipo_prossimo* PROSSIMO;
  SPLITPUNKT* sp;
  int scout, abisso, ph, c;
  sp = POSIZIONE->splitpunkt;
  scout = sp->beta;
  abisso = sp->abisso;

  while (VERO)
    {
      LOCK (sp->splock);
      if (sp->tot)
	{
	  UNLOCK (sp->splock);
	  return;
	}
      PROSSIMO = sp->MOVE_PICK;
      m = mio_prossimo (POSIZIONE, PROSSIMO) & 0x7fff;
      ph = PROSSIMO->fase;
      c = PROSSIMO->mossa;
      if (!m)
	{
	  PROSSIMO->fase = FASE_0;
	  UNLOCK (sp->splock);
	  return;
	}
      UNLOCK (sp->splock);
      if (m == PROSSIMO->exclude)
	continue;
      FARE (POSIZIONE, m);
      valutazione (POSIZIONE, -0x7fff0000, 0x7fff0000, m); /* usa LAZY ? */
      if (mio_re_scacco)
        {
          ANNULLA (POSIZIONE, m);
          continue;
        }
      m &= 0x7fff;
      if (tuo_re_scacco)
	{
	  v = -tuo_nodo_tagliato_scacco (POSIZIONE, 1 - scout, abisso - 1);
	}
      else
	{
	  int ai = AI (m);
	  int ESTENSIONE = 0;
	  if (pedone_libero_mossa (ai, TRAVERSA_SESTA (ai)))
	    ESTENSIONE = 1;
	  if (ph == mossa_ordinaria && !ESTENSIONE)
	    {
	      int RIDUZIONE = 2 + BSR (2 + c);
	      int nuovo_abisso = MASSIMO (8, abisso - RIDUZIONE);
	      v = -tuo_nodo_tagliato (POSIZIONE, 1 - scout, nuovo_abisso);
	      if (v < scout)
		goto I;
	    }	  
	  v = -tuo_nodo_tagliato (POSIZIONE, 1 - scout, abisso - 2 + ESTENSIONE);
	}
    I:
      ANNULLA (POSIZIONE, m);
      if (POSIZIONE->halt)
	return;
      if (v >= scout)
	{
	  ivan_fail_high (sp, POSIZIONE, m);
	  return;
	}
      SMP_BAD_HISTORY (POSIZIONE, m, sp);
    }
}

void mio_smp_nodo_cut (tipo_posizione* POSIZIONE)
{
  int v;
  int m;
  tipo_prossimo* PROSSIMO;
  SPLITPUNKT* sp;
  int scout, abisso, ph, c;
  sp = POSIZIONE->splitpunkt;
  scout = sp->beta;
  abisso = sp->abisso;

  while (VERO)
    {
      LOCK (sp->splock);
      if (sp->tot)
	{
	  UNLOCK (sp->splock);
	  return;
	}
      PROSSIMO = sp->MOVE_PICK;
      m = mio_prossimo (POSIZIONE, PROSSIMO);      
      ph = PROSSIMO->fase;
      c = PROSSIMO->mossa;
      if (!m)
	{
	  PROSSIMO->fase = FASE_0;
	  UNLOCK (sp->splock);
	  return;
	}
      UNLOCK (sp->splock);
      FARE (POSIZIONE, m);
      valutazione (POSIZIONE, -0x7fff0000, 0x7fff0000, m); /* usa LAZY ? */
      if (mio_re_scacco)
        {
          ANNULLA (POSIZIONE, m);
          continue;
        }
      m &= 0x7fff;
      if (tuo_re_scacco)
	{
	  v = -tuo_nodo_totale_scacco (POSIZIONE, 1 - scout, abisso - 1);
	}
      else
	{
	  int ai = AI (m);
	  int ESTENSIONE = 0;
	  if (pedone_libero_mossa (ai, TRAVERSA_SESTA (ai)))
	    ESTENSIONE = 1;
	  if (ph == mossa_ordinaria && !ESTENSIONE)
	    {
	      int RIDUZIONE = 4 + BSR (5 + c);
	      int nuovo_abisso = MASSIMO (8, abisso - RIDUZIONE);
	      v = -tuo_nodo_totale (POSIZIONE, 1 - scout, nuovo_abisso);
	      if (v < scout)
		goto I;
	    }	  
	  v = -tuo_nodo_totale (POSIZIONE, 1 - scout, abisso - 2 + ESTENSIONE);
	}
    I:
      ANNULLA (POSIZIONE, m);
      if (POSIZIONE->halt)
	return;
      if (v >= scout)
	{
	  ivan_fail_high (sp, POSIZIONE, m);
	  return;
	}
      SMP_BAD_HISTORY (POSIZIONE, m, sp);
    }
}
#endif
