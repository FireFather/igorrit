#ifndef BUILD_escludo
#define BUILD_escludo
#include "RobboLito.h"
#include "storia.i"
#include "mossa_nullo.i"

#include "escludo.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

#define RETURN(x) return(x)

int
mio_escludo (tipo_posizione *POSIZIONE, int VALU, int abisso, uint32 MOSSA)
{
  int mossa, i;
  int k;
  tipo_zobrist *tr;
  int tras_abisso, mossa_abisso = 0, mossa_tras = 0, valu, cnt;
  int v, ESTENSIONE, abisso_nuovo, scacco_mossa;
  tipo_prossimo PROSSIMO[1];
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  uint64 zob = DINAMICO->Zobrist;
  int ai, di;
  //YUSUF128 H[1];
  int RIDUZIONE;
  logico SPLIT;
  int SPLIT_ABISSO;

  if (VALU < -VALU_MATTO + 1)
    RETURN (-VALU_MATTO + 1);
  if (VALU > VALU_MATTO - 1)
    RETURN (VALU_MATTO - 1);
  (POSIZIONE0 + 1)->mossa = 0;
  ripetizione_verificare;
  occupato5_verificare;
  zob ^=
    ZOBRIST[conto_mio_re][DI (MOSSA)] ^ ZOBRIST[conto_tuo_re][AI (MOSSA)];
  k = zob & elenco_celare;

  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (zob >> 32)) == 0)
	{
	  tras_abisso = tr->abisso_basso;
	  mossa = tr->mossa;
	  if (mossa && tras_abisso > mossa_abisso)
	    {
	      mossa_abisso = tras_abisso;
	      (POSIZIONE0 + 1)->mossa = mossa_tras = mossa;
	    }
	  tras_abisso = MASSIMO (tr->abisso_basso, tr->abisso_alto);
	  if (tr->abisso_basso >= abisso)
	    {
	      valu = zobrist_valu_bassa (tr);
	      if (valu >= VALU)
		{
		  if (mio_nullo || mossa)
		    {
		      UPDATE_ANNI();
		      RETURN (valu);
		    }
		}
	    }
	  if (tr->abisso_alto >= abisso)
	    {
	      valu = zobrist_valu_alta (tr);
	      if (valu < VALU)
		{
		  if (1)
		    {
		      UPDATE_ANNI();
		      RETURN (valu);
		    }
		}
	    }
	}
    }
  PROSSIMO->mossa_tras = mossa_tras;

  if (POSIZIONE0->valu >= VALU && mio_nullo)
    {
      nullo_fare (POSIZIONE);
      abisso_nuovo = abisso - nullo_riduzione;
      abisso_nuovo -= ((uint32) (MINIMO (POSIZIONE0->valu - VALU, 96))) / 32;
      if (abisso_nuovo <= 1)
	v = -tuo_cerca_fine (POSIZIONE, 1 - VALU, 0);
      else if (abisso_nuovo <= 7)
	v = -tuo_cerca_mezzo (POSIZIONE, 1 - VALU, abisso_nuovo);
      else
	v = -tuo_nodo_tagliato (POSIZIONE, 1 - VALU, abisso_nuovo);
      nullo_annulla (POSIZIONE);
      CHECK_HALT ();
      if (v >= VALU)
	{
	  if (mossa_tras == 0)
	    Zobrist_basso (zob, 0, abisso, v);
	  RETURN (v);
	}
    }

  cnt = 0;
  PROSSIMO->mossa_tras = mossa_tras;
  PROSSIMO->fase = TRAS;
  PROSSIMO->targa = tuo_occupato;
  if (abisso < 20 && VALU - POSIZIONE0->valu >= 48 * (abisso - 4))
    {
      PROSSIMO->fase = TRAS2;
      cnt = 1;
      if (VALU - POSIZIONE0->valu >= 48 * (abisso - 2))
	PROSSIMO->targa ^= bitbordo_tuo_pedone;
    }

  PROSSIMO->mossa = 0;
  PROSSIMO->cb = 0;
  PROSSIMO->exclude = MOSSA & 0x7fff;
  v = VALU;
  SPLIT = FALSO;
  SPLIT_ABISSO = 14;

  while (VERO)
    {
#if 0 && defined (YUSUF_MULTICORE) /* con MOSSA ? -- zobrist ? */
      if (TITANIC_MODE && SMP_FREE != 0 && abisso >= SPLIT_ABISSO && !SPLIT &&
          PROSSIMO->fase != TRAS && cnt >= 1 && PROSSIMO->fase <= mossa_ordinaria)
        {
          int r;
          logico b;
          SPLIT = VERO;
          b = IVAN_SPLIT (POSIZIONE, PROSSIMO, abisso, VALU, VALU, NODE_TYPE_ALL, &r);
          if (b)
            return r;
        }
#endif
      mossa = mio_prossimo (POSIZIONE, PROSSIMO);
      if (!mossa)
	break;
      if ((mossa & 0x7fff) == (MOSSA & 0x7fff))
	continue;
      ai = AI (mossa);
      di = DI (mossa);
      if (ripetizione_interrogativo (0))
	{
	  cnt++;
	  continue;
	}
      if (cnt > 5 && PROSSIMO->fase == mossa_ordinaria
	  && (mossa & 0xe000) == 0 && quadretto_fisso[di] & ~mio_xray
	  && abisso < 20)
	{
	  if ((6 << (abisso - 6)) + MASSIMO_POSIZIONALE (mossa) +
	      (POSIZIONE0->valu) < VALU + 30 + 2 * cnt)
	    {
	      cnt++;
	      continue;
	    }
	}
      RIDUZIONE = 0;
      if (abisso < 20 && (2 << (abisso - 6)) + (POSIZIONE0->valu) < VALU + 125
	  && PROSSIMO->fase == mossa_ordinaria && mio_quadretto_re != di
	  && quadretto_fisso[di] & ~mio_xray && (mossa & 0x8000) == 0
	  && !mio_SEE (POSIZIONE, mossa))
	{
	  cnt++;
	  continue;
	}
      mossa &= 0x7fff;
      FARE (POSIZIONE, mossa);
      chiamata_valu_pigra (VALU, VALU, valu_pigro2, mossa);
      if (mossa_illegale)
	{
	  ANNULLA (POSIZIONE, mossa);
	  continue;
	}
      if (mossa_scacco)
	scacco_mossa = 1;
      else
	scacco_mossa = 0;
      ESTENSIONE = 0;
      if (pedone_libero_mossa (ai, TRAVERSA_SESTA (ai)))
	ESTENSIONE = 1;
      if (scacco_mossa)
	v = -tuo_nodo_tagliato_scacco (POSIZIONE, 1 - VALU, abisso - 1);
      else
	{
	  if (cnt > 5 && abisso < 20 && POSIZIONE1->cattura == 0
	      && (2 << (abisso - 6)) - POSIZIONE1->valu < VALU + cnt - 15)
	    {
	      ANNULLA (POSIZIONE, mossa);
	      cnt++;
	      continue;
	    }
	  if (PROSSIMO->fase == mossa_ordinaria && (cnt >= 3 || RIDUZIONE))
	    {
	      abisso_nuovo =
		abisso - 2 + ESTENSIONE - BSR (1 + cnt) - RIDUZIONE;
	      if (abisso_nuovo <= 1)
		v = -tuo_cerca_fine (POSIZIONE, 1 - VALU, 0);
	      else if (abisso_nuovo <= 7)
		v = -tuo_cerca_mezzo (POSIZIONE, 1 - VALU, abisso_nuovo);
	      else
		v = -tuo_nodo_tagliato (POSIZIONE, 1 - VALU, abisso_nuovo);
	      if (v < VALU)
		goto FATTO;
	    }
	  abisso_nuovo = abisso - 2 + ESTENSIONE;
	  if (abisso_nuovo <= 7)
	    v = -tuo_cerca_mezzo (POSIZIONE, 1 - VALU, abisso_nuovo);
	  else
	    v = -tuo_nodo_tagliato (POSIZIONE, 1 - VALU, abisso_nuovo);
	}
    FATTO:
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      cnt++;
      if (v >= VALU)
	{
	  if ((POSIZIONE0 + 1)->cattura == 0 && mossa_storia (mossa))
	    STORIA_BUONA (mossa, abisso);
	  Zobrist_basso (zob, mossa, abisso, v);
	  RETURN (v);
	}
      if ((POSIZIONE0 + 1)->cattura == 0 && mossa_storia (mossa))
	STORIA_BRUTTA (mossa, abisso);
    }
  v = VALU - 1;
  Zobrist_alto (zob, abisso, v);
  RETURN (v);
}

int
mio_escludo_scacco (tipo_posizione *POSIZIONE, int VALU, int abisso, uint32 MOSSA)
{
  int mossa, k, cnt, ESTENSIONE;
  int tras_abisso, mossa_abisso = 0, mossa_tras = 0, valu, abisso_nuovo, v, i;
  tipo_zobrist *tr;
  tipo_lista_mossa LISTA[256], *lista, *p, *q;
  uint64 zob = DINAMICO->Zobrist;
  int valu_migliore;
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  logico GEN;
  //YUSUF128 H[1];

  if (VALU < -VALU_MATTO + 1)
    RETURN (-VALU_MATTO + 1);
  if (VALU > VALU_MATTO - 1)
    RETURN (VALU_MATTO - 1);
  zob ^=
    ZOBRIST[conto_mio_re][DI (MOSSA)] ^ ZOBRIST[conto_tuo_re][AI (MOSSA)];
  (POSIZIONE0 + 1)->mossa = 0;
  ripetizione_verificare;
  occupato5_verificare;
  k = zob & elenco_celare;

  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (zob >> 32)) == 0)
	{
	  tras_abisso = tr->abisso_basso;
	  mossa = tr->mossa;
	  if (mossa && tras_abisso > mossa_abisso)
	    {
	      mossa_abisso = tras_abisso;
	      (POSIZIONE0 + 1)->mossa = mossa_tras = mossa;
	    }
	  tras_abisso = MASSIMO (tr->abisso_basso, tr->abisso_alto);
	  if (tr->abisso_basso >= abisso)
	    {
	      valu = zobrist_valu_bassa (tr);
	      if (valu >= VALU)
		{
		  if (1)
		    {
		      UPDATE_ANNI();
		      RETURN (valu);
		    }
		}
	    }
	  if (tr->abisso_alto >= abisso)
	    {
	      valu = zobrist_valu_alta (tr);
	      if (valu < VALU)
		{
		  if (1)
		    {
		      UPDATE_ANNI();
		      RETURN (valu);
		    }
		}
	    }
	}
    }
  if (mossa_tras && !mio_garantire (POSIZIONE, mossa_tras))
    mossa_tras = 0;

  valu_migliore = HEIGHT (POSIZIONE) - VALU_MATTO;
  p = LISTA;
  LISTA[0].mossa = mossa_tras;
  cnt = 0;
  GEN = FALSO;
  LISTA[1].mossa = 0;
  while (p->mossa || !GEN)
    {
      if (!p->mossa)
	{
	  lista = mio_evasione (POSIZIONE, LISTA + 1, 0xffffffffffffffff);
	  GEN = VERO;
	  for (p = lista - 1; p >= LISTA + 1; p--)
	    {
	      if ((p->mossa & 0x7fff) == mossa_tras)
		p->mossa = 0;
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
	  p = LISTA + 1;
	  continue;
	}
      mossa = p->mossa & 0x7fff;
      p++;
      if (mossa == MOSSA)
	continue;
      if (ripetizione_interrogativo (0))
	{
	  valu_migliore = MASSIMO (0, valu_migliore);
	  cnt++;
	  continue;
	}
      FARE (POSIZIONE, mossa);
      chiamata_valu_pigra (VALU, VALU, valu_pigro2, mossa);
      if (mossa_illegale)
	{
	  ANNULLA (POSIZIONE, mossa);
	  continue;
	}
      if (mossa_scacco)
	{
	  abisso_nuovo = abisso - 1;
	  if (abisso_nuovo <= 7)
	    v = -tuo_cerca_mezzo_scacco (POSIZIONE, 1 - VALU, abisso_nuovo);
	  else
	    v = -tuo_nodo_tagliato_scacco (POSIZIONE, 1 - VALU, abisso_nuovo);
	}
      else
	{
	  if (cnt >= 1)
	    {
	      if (partita_prima)
		ESTENSIONE = 1;
	      else
		ESTENSIONE = 0;
	      abisso_nuovo = abisso - 2 - MINIMO (2, cnt) + ESTENSIONE;
	      if (abisso_nuovo <= 1)
		v = -tuo_cerca_fine (POSIZIONE, 1 - VALU, 0);
	      else if (abisso_nuovo <= 7)
		v = -tuo_cerca_mezzo (POSIZIONE, 1 - VALU, abisso_nuovo);
	      else
		v = -tuo_nodo_tagliato (POSIZIONE, 1 - VALU, abisso_nuovo);
	      if (v < VALU)
		goto LACCIO;
	    }
	  if (partita_prima)
	    ESTENSIONE = 1;
	  else
	    ESTENSIONE = 0;
	  abisso_nuovo = abisso - 2 + ESTENSIONE;
	  if (abisso_nuovo <= 7)
	    v = -tuo_cerca_mezzo (POSIZIONE, 1 - VALU, abisso_nuovo);
	  else
	    v = -tuo_nodo_tagliato (POSIZIONE, 1 - VALU, abisso_nuovo);
	}
    LACCIO:
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      if (v > valu_migliore)
	valu_migliore = v;
      if (v < VALU)
	{
	  cnt++;
	  continue;
	}
      Zobrist_basso (zob, mossa, MASSIMO (1, abisso), v);
      RETURN (v);
    }
  Zobrist_alto (zob, MASSIMO (1, abisso), valu_migliore);
  RETURN (valu_migliore);
}
