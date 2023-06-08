#ifndef BUILD_nodo_pv
#define BUILD_nodo_pv
#include "RobboLito.h"
#include "storia.i"
#include "sorvegliare.h"

#include "nodo_pv.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

#define RETURN(x) return(x)
int
mio_nodo_pv (tipo_posizione *POSIZIONE, int BASSO, int ALTO, int abisso, int sca)
{
  tipo_prossimo PROSSIMO[1];
  tipo_zobrist *tr;
  int mossa_buona, v, valu, k, i, tras_abisso, mossa,
    mossa_abisso = 0, mossa_tras = 0, zob_abisso;
  tipo_lista_mossa *lista, *p, *q;
  int ESTENSIONE, valu_migliore, abisso_nuovo, scacco_mossa, ai, di;
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  //YUSUF128 H[1];
  int SINGOLARE;
  logico SPLIT;
  int SPLIT_ABISSO;

  if (ALTO < -VALU_MATTO)
    RETURN (-VALU_MATTO);
  if (BASSO > VALU_MATTO)
    RETURN (VALU_MATTO);
  if (abisso <= 1)
    {
      if (sca)
	return mio_pv_cerca_fine_scacco (POSIZIONE, BASSO, ALTO, 1);
      else
	return mio_pv_cerca_fine (POSIZIONE, BASSO, ALTO, 1);
    }
  ripetizione_verificare;
  PROSSIMO->mossa_tras = 0;
  zob_abisso = 0;
  PROSSIMO->mossa = 0;
  PROSSIMO->cb = 0;

  k = DINAMICO->Zobrist & elenco_celare;
  (POSIZIONE0 + 1)->mossa = 0;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0)
	{
	  tras_abisso = tr->abisso_basso;
	  mossa = tr->mossa;
	  if (mossa && tras_abisso > mossa_abisso)
	    {
	      mossa_abisso = tras_abisso;
	      (POSIZIONE0 + 1)->mossa = mossa_tras = mossa;
	    }
	  if (tr->abisso_basso > tr->abisso_alto)
	    {
	      tras_abisso = tr->abisso_basso;
	      valu = zobrist_valu_bassa (tr);
	    }
	  else
	    {
	      tras_abisso = tr->abisso_alto;
	      valu = zobrist_valu_alta (tr);
	    }
	  if (tras_abisso > zob_abisso)
	    zob_abisso = tras_abisso;
	  if (zobrist_preciso (tr) && tras_abisso >= abisso)
	    {
	      UPDATE_ANNI();
	      if (!ANALISI)
		RETURN (valu);
	    }
	}
    }

  if (!mossa_tras && abisso >= 6)
    {
      v = BASSO;
      if (abisso >= 10)
	{
	  v = mio_nodo_pv (POSIZIONE, BASSO - abisso, ALTO + abisso, abisso - 8, sca);
	  CHECK_HALT ();
	  if (v > BASSO - abisso)
	    mossa_tras = (POSIZIONE0 + 1)->mossa;
	}
      if (v > BASSO - abisso)
	v = mio_nodo_pv (POSIZIONE, BASSO - abisso, ALTO + abisso, abisso - 4, sca);
      CHECK_HALT ();
      if (v > BASSO - abisso)
	mossa_tras = (POSIZIONE0 + 1)->mossa;
    }
  else if (abisso >= 10 && abisso > zob_abisso + 8)
    {
      v = mio_nodo_pv (POSIZIONE, BASSO - abisso, ALTO + abisso, abisso - 8, sca);
      CHECK_HALT ();
      if (v > BASSO - abisso)
	mossa_tras = (POSIZIONE0 + 1)->mossa;
      if (v > BASSO - abisso)
	{
	  v = mio_nodo_pv (POSIZIONE, BASSO - abisso, ALTO + abisso, abisso - 4, sca);
	  CHECK_HALT ();
	  if (v > BASSO - abisso)
	    mossa_tras = (POSIZIONE0 + 1)->mossa;
	}
    }

  PROSSIMO->mossa_tras = mossa_tras;
  PROSSIMO->fase = TRAS;
  ESTENSIONE = 0;
  PROSSIMO->targa = tuo_occupato;

  SINGOLARE = 0;
  if (sca)
    {
      lista = mio_evasione (POSIZIONE, PROSSIMO->LISTA, 0xffffffffffffffff);
      PROSSIMO->fase = elusione;
      for (p = lista - 1; p >= PROSSIMO->LISTA; p--)
	{
	  if ((p->mossa & 0x7fff) == mossa_tras)
	    p->mossa |= 0xffff0000;
	  else if (p->mossa <= (0x80 << 24))
	    {
	      if ((p->mossa & 0x7fff) == POSIZIONE0->uccisore_uno)
		p->mossa |= 0x7fff8000;
	      else if ((p->mossa & 0x7fff) == POSIZIONE0->uccisore_due)
		p->mossa |= 0x7fff0000;
	      else
		p->mossa |=
		  (p->mossa & 0x7fff) |
		  (STORIA_VALU (POSIZIONE, p->mossa) << 15);
	    }
	  mossa = p->mossa;
	  for (q = p + 1; q < lista; q++)
	    {
	      if (mossa < q->mossa)
		(q - 1)->mossa = q->mossa;
	      else
		break;
	    }
	  q--;
	  q->mossa = mossa;
	}
      if ((lista - PROSSIMO->LISTA) <= 1)
	SINGOLARE = 2;
      if ((lista - PROSSIMO->LISTA) == 2)
	SINGOLARE = 1;
      if ((lista - PROSSIMO->LISTA) > 2)
	SINGOLARE = 0;
    }

  if (abisso >= 16 && PROSSIMO->mossa_tras && SINGOLARE < 2
      && mio_garantire (POSIZIONE, PROSSIMO->mossa_tras))
    {
      mossa = PROSSIMO->mossa_tras;
      ai = AI (mossa);
      di = DI (mossa);
      FARE (POSIZIONE, mossa);
      chiamata_valu_piena (mossa);
      if (mossa_illegale)
	{
	  ANNULLA (POSIZIONE, mossa);
	  goto IGNORATO;
	}
      valu = -tuo_nodo_pv (POSIZIONE, -ALTO, -BASSO, abisso - 10, (mossa_scacco) != 0);
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      if (sca)
	v =
	  mio_escludo_scacco (POSIZIONE, valu - abisso / 2,
			      abisso - MINIMO (12, abisso / 2), mossa & 0x7fff);
      else
	v =
	  mio_escludo (POSIZIONE, valu - abisso / 2,
		       abisso - MINIMO (12, abisso / 2), mossa & 0x7fff);
      CHECK_HALT ();
      if (v < valu - abisso / 2)
	{
	  SINGOLARE = 1;
	  if (sca)
	    v =
	      mio_escludo_scacco (POSIZIONE, valu - abisso,
				  abisso - MINIMO (12, abisso / 2), mossa & 0x7fff);
	  else
	    v =
	      mio_escludo (POSIZIONE, valu - abisso,
			   abisso - MINIMO (12, abisso / 2), mossa & 0x7fff);
	  CHECK_HALT ();
	  if (v < valu - abisso)
	    SINGOLARE = 2;
	}
    }
 IGNORATO:
  valu_migliore = -VALU_INFINITO;
  PROSSIMO->mossa = 0;
  PROSSIMO->cb = 0;
  mossa_buona = 0;

  SPLIT = FALSO;
  SPLIT_ABISSO = 14;
  while (VERO)
    {
#if 1 && defined (YUSUF_MULTICORE) /* nein sca ? */ /* abisso ya 10 ? */
      /* Personal Opinion: with the *increase* for the abisso ! */
      /* now what is the best? decembrist testing for best params to find it! */
      if (TITANIC_MODE && SMP_FREE != 0 && !sca &&
	  abisso >= SPLIT_ABISSO && !SPLIT && valu_migliore != -VALU_INFINITO)
	{
          int r;
	  logico b;
          SPLIT = VERO;
          b = IVAN_SPLIT (POSIZIONE, PROSSIMO, abisso, ALTO, BASSO, NODE_TYPE_PV, &r);
	  CHECK_HALT ();
	  if (b)
	    {
	      if (r > BASSO || !mossa_buona)
		return r;
	      mossa = mossa_buona; /* SMP PV */
	      (POSIZIONE0 + 1)->mossa = mossa_buona & 0x7fff;
	      valu_migliore = r;
	      goto IVAN;
	    }
        }
#endif
      mossa = mio_prossimo (POSIZIONE, PROSSIMO);
      if (!mossa)
	break;
      ai = AI (mossa);
      di = DI (mossa);
      if (BASSO > 0 && POSIZIONE0->mossa50 >= 2
	  && ((AI (mossa) << 6) | DI (mossa)) == (POSIZIONE0 - 1)->mossa
	  && POSIZIONE->qu[AI (mossa)] == 0)
	{
	  valu_migliore = MASSIMO (0, valu_migliore);
	  continue;
	}
      mossa &= 0x7fff;
      FARE (POSIZIONE, mossa);
      chiamata_valu_piena (mossa);
      if (mossa_illegale)
	{
	  ANNULLA (POSIZIONE, mossa);
	  continue;
	}
      scacco_mossa = (mossa_scacco != 0);
      /* LMR ? */
      ESTENSIONE = 0;
      if (ESTENSIONE < 2)
	{
	  if (pedone_libero_mossa (ai, TRAVERSA_SESTA (ai)))
	    ESTENSIONE = 2;
	}
      if (ESTENSIONE < 2)
	{
	  if (POSIZIONE1->cattura != 0 || scacco_mossa
	      || (sca && partita_prima))
	    ESTENSIONE = 1;
	  else if (pedone_libero_mossa (ai, TRAVERSA_QUARTA (ai)))
	    ESTENSIONE = 1;
	}
      if (PROSSIMO->mossa_tras != mossa)
	SINGOLARE = 0;
      abisso_nuovo = abisso - 2 + MASSIMO (ESTENSIONE, SINGOLARE);

      if (PROSSIMO->mossa_tras != mossa && abisso_nuovo > 1)
	{
	  if (abisso_nuovo <= 7)
	    {
	      if (scacco_mossa)
		v = -tuo_cerca_mezzo_scacco (POSIZIONE, -BASSO, abisso_nuovo);
	      else
		v = -tuo_cerca_mezzo (POSIZIONE, -BASSO, abisso_nuovo);
	    }
	  else
	    {
	      if (scacco_mossa)
		v = -tuo_nodo_tagliato_scacco (POSIZIONE, -BASSO, abisso_nuovo);
	      else
		v = -tuo_nodo_tagliato (POSIZIONE, -BASSO, abisso_nuovo);
	    }
	  if (v > BASSO)
	    v = -tuo_nodo_pv (POSIZIONE, -ALTO, -BASSO, abisso_nuovo, scacco_mossa);
	}
      else
	v = -tuo_nodo_pv (POSIZIONE, -ALTO, -BASSO, abisso_nuovo, scacco_mossa);
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
	  
      if (v <= BASSO && POSIZIONE->qu[AI (mossa)] == 0 && mossa_storia (mossa))
	STORIA_BRUTTA_pv (mossa, abisso);
      if (v <= valu_migliore)
	continue;
      valu_migliore = v;
      if (v <= BASSO)
	continue;
      BASSO = v;
      mossa_buona = mossa;
      Zobrist_basso (DINAMICO->Zobrist, mossa, abisso, v);
      if (v >= ALTO)
	{
	  if (POSIZIONE->qu[AI (mossa)] == 0 && mossa_storia (mossa))
	    STORIA_BUONA (mossa, abisso);
	  RETURN (v);
	}
    }

  mossa = mossa_buona;
  (POSIZIONE0 + 1)->mossa = mossa_buona & 0x7fff;
  if (valu_migliore == -VALU_INFINITO) /* con SMP ? */
    {
      if (!sca)
	RETURN (0);
      RETURN (HEIGHT (POSIZIONE) - VALU_MATTO);
    }
 IVAN:
  if (mossa)
    {
      if (POSIZIONE->qu[AI (mossa)] == 0 && mossa_storia (mossa))
	STORIA_BUONA (mossa, abisso);
      Zobrist_preciso (POSIZIONE, mossa, abisso, valu_migliore, pietra_precisa);
      RETURN (valu_migliore);
    }
  Zobrist_alto (DINAMICO->Zobrist, abisso, valu_migliore);
  RETURN (valu_migliore);
}
