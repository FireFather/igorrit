#ifndef BUILD_cerca_mezzo
#define BUILD_cerca_mezzo
#include "RobboLito.h"

#include "storia.i"
#include "mossa_nullo.i"
#include "cerca_mezzo.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

#define RETURN(x) return(x)
int
mio_cerca_mezzo (tipo_posizione *POSIZIONE, int VALU, int abisso)
{
  int cnt, valu, valu_migliore, v, k, i, mossa_tras = 0,
    mossa, mossa_abisso = 0, tras_abisso, ai, di;
  tipo_prossimo PROSSIMO[1];
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  tipo_zobrist *tr;
  //YUSUF128 H[1];
  if (VALU < -VALU_MATTO + 1)
    RETURN (-VALU_MATTO + 1);
  if (VALU > VALU_MATTO - 1)
    RETURN (VALU_MATTO - 1);
  (POSIZIONE0 + 1)->mossa = 0;
  /* */
  v = POSIZIONE0->valu + 1125;
  if (v < VALU)
    RETURN (VALU - 1);
  /* */

  ripetizione_verificare;
  occupato5_verificare;
  k = DINAMICO->Zobrist & elenco_celare;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0)
	{
	  if (tr->abisso_basso >= abisso)
	    {
	      valu = zobrist_valu_bassa (tr);
	      if (valu >= VALU)
		{
		  (POSIZIONE0 + 1)->mossa = tr->mossa;
		  RETURN (valu);
		}
	    }
	  if (tr->abisso_alto >= abisso)
	    {
	      valu = zobrist_valu_alta (tr);
	      if (valu < VALU)
		RETURN (valu);
	    }
	  tras_abisso = tr->abisso_basso;
	  mossa = tr->mossa;
	  if (mossa && tras_abisso > mossa_abisso)
	    {
	      mossa_abisso = tras_abisso;
	      mossa_tras = mossa;
	    }
	}
    }
  v = POSIZIONE0->valu - (70 + 10 * abisso);
  if (v >= VALU)
    RETURN (POSIZIONE0->valu);
  valu_migliore = MINIMO (POSIZIONE0->valu, VALU - 1);
  if (POSIZIONE0->valu >= VALU && mio_nullo)
    {
      nullo_fare (POSIZIONE);
      v = -tuo_cerca_fine (POSIZIONE, 1 - VALU, 0);
      nullo_annulla (POSIZIONE);
      if (v >= VALU)
	{
	  Zobrist_basso (DINAMICO->Zobrist, mossa_tras, abisso, v);
	  RETURN (v);
	}
    }
  PROSSIMO->fase = TRAS;
  PROSSIMO->targa = tuo_occupato;
  if (POSIZIONE0->valu + 50 + 8 * abisso < VALU)
    {
      PROSSIMO->fase = TRAS2;
      if (VALU >= POSIZIONE0->valu + 75 + 32 * abisso)
	{
	  PROSSIMO->targa ^= bitbordo_tuo_pedone;
	  if (bitbordo_tuo_pedone & mio_attaco)
	    valu_migliore += 125;
	  if (abisso <= 3 && VALU >= POSIZIONE0->valu + 400 + 32 * abisso)
	    {
	      PROSSIMO->targa ^=
		(bitbordo_tuo_cavallo | bitbordo_tuo_alfiere);
	      valu_migliore += 300;
	      if (VALU >= POSIZIONE0->valu + 600 + 32 * abisso)
		{
		  PROSSIMO->targa ^= bitbordo_tuo_torre;
		  valu_migliore += 200;
		}
	    }
	}
    }
  else if (abisso <= 3 && POSIZIONE0->valu + 4 * abisso < VALU)
    {
      PROSSIMO->fase = TRAS3;
      PROSSIMO->celare = (VALU - POSIZIONE0->valu) + 4 * abisso + 5;
    }
  PROSSIMO->cb = 0;
  PROSSIMO->mossa = 0;
  PROSSIMO->mossa_tras = mossa_tras;
  cnt = 0;
  while ((mossa = mio_prossimo (POSIZIONE, PROSSIMO)))
    {
      ai = AI (mossa);
      di = DI (mossa);
      if (ripetizione_interrogativo (0))
	{
	  valu_migliore = MASSIMO (0, valu_migliore);
	  cnt++;
	  continue;
	}
      if (cnt >= abisso && PROSSIMO->fase == mossa_ordinaria
	  && (mossa & 0xe000) == 0 && quadretto_fisso[di] & ~mio_xray
	  && mio_occupato ^ (bitbordo_mio_pedone | bitbordo_mio_re))
	{
	  if ((2 * abisso) + MASSIMO_POSIZIONALE (mossa) + POSIZIONE0->valu <
	      VALU + 40 + 2 * cnt)
	    {
	      cnt++;
	      continue;
	    }
	}
      if ((POSIZIONE->qu[ai] == 0 || (abisso <= 5 && !SEE_facile (mossa)))
	  && quadretto_fisso[di] & ~mio_xray && POSIZIONE->qu[di] != conto_mio_re
	  && !mossa_en_passant (mossa) && mossa != mossa_tras
	  && !mio_SEE (POSIZIONE, mossa))
	{
	  cnt++;
	  continue;
	}

      mossa &= 0x7fff;
      FARE (POSIZIONE, mossa);
      chiamata_valu_pigra (VALU, VALU, valu_pigro, mossa);
      if (mossa_illegale || (PROSSIMO->fase == guadagno && mossa_scacco))
	{
	  ANNULLA (POSIZIONE, mossa);
	  continue;
	}
      if (mossa_scacco)
	v = -tuo_cerca_mezzo_scacco (POSIZIONE, 1 - VALU, abisso - 1);
      else
	{
	  if (cnt >= abisso && (2 * abisso) - POSIZIONE1->valu < VALU + cnt)
	    {
	      ANNULLA (POSIZIONE, mossa);
	      cnt++;
	      continue;
	    }
	  if (abisso <= 3)
	    v = -tuo_cerca_fine (POSIZIONE, 1 - VALU, 0);
	  else
	    v = -tuo_cerca_mezzo (POSIZIONE, 1 - VALU, abisso - 2);
	}
      cnt++;
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      if (v >= VALU)
	{
	  if ((POSIZIONE0 + 1)->cattura == 0 && mossa_storia (mossa))
	    STORIA_BUONA (mossa, abisso);
	  Zobrist_basso (DINAMICO->Zobrist, mossa, abisso, v);
	  RETURN (v);
	}
      if (v >= valu_migliore)
	valu_migliore = v;
      if ((POSIZIONE0 + 1)->cattura == 0 && mossa_storia (mossa))
	STORIA_BRUTTA (mossa, abisso);
    }
  if (!cnt && PROSSIMO->fase <= TRAS2)
    RETURN (0);
  Zobrist_alto (DINAMICO->Zobrist, abisso, valu_migliore);
  RETURN (valu_migliore);
}

int
mio_cerca_mezzo_scacco (tipo_posizione *POSIZIONE, int VALU, int abisso)
{
  int ignorato, k, mossa_tras = 0, tras_abisso, mossa_abisso = 0,
    valu, i, mossa, valu_migliore, v, abisso_nuovo;
  logico GEN;
  tipo_zobrist *tr;
  tipo_lista_mossa LISTA[256], *lista, *p, *q;
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  //YUSUF128 H[1];

  if (VALU < -VALU_MATTO + 1)
    RETURN (-VALU_MATTO + 1);
  if (VALU > VALU_MATTO - 1)
    RETURN (VALU_MATTO - 1);

  ripetizione_verificare;
  occupato5_verificare;
  k = DINAMICO->Zobrist & elenco_celare;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0)
	{
	  if (tr->abisso_basso && tr->abisso_basso >= abisso)
	    {
	      valu = zobrist_valu_bassa (tr);
	      if (valu >= VALU)
		RETURN (valu);
	    }
	  if (tr->abisso_alto && tr->abisso_alto >= abisso)
	    {
	      valu = zobrist_valu_alta (tr);
	      if (valu < VALU)
		RETURN (valu);
	    }
	  tras_abisso = tr->abisso_basso;
	  mossa = tr->mossa;
	  if (mossa && tras_abisso > mossa_abisso)
	    {
	      mossa_abisso = tras_abisso;
	      mossa_tras = mossa;
	    }
	}
    }
  if (mossa_tras && !mio_garantire (POSIZIONE, mossa_tras))
    mossa_tras = 0;

  valu_migliore = HEIGHT (POSIZIONE) - VALU_MATTO;
  p = LISTA;
  LISTA[0].mossa = mossa_tras;
  GEN = FALSO;
  LISTA[1].mossa = 0;
  ignorato = 0;
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
		      (p->mossa & 0xffff) |
		      ((STORIA_VALU (POSIZIONE, p->mossa) >> 1) << 16);
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
      mossa = p->mossa;
      p++;
      if (ripetizione_interrogativo (0))
	{
	  valu_migliore = MASSIMO (0, valu_migliore);
	  continue;
	}
      if (INTERROGATIVO_INTERPOSIZIONE (mossa) && VALU > -25000
	  && (mossa & 0x7fff) != mossa_tras && !mio_SEE (POSIZIONE, mossa))
	{
	  ignorato++;
	  continue;
	}
      mossa &= 0x7fff;
      FARE (POSIZIONE, mossa);
      chiamata_valu_pigra (VALU, VALU, valu_pigro, mossa);
      if (mossa_illegale)
	{
	  ANNULLA (POSIZIONE, mossa);
	  continue;
	}
      if (mossa_scacco)
	v = -tuo_cerca_mezzo_scacco (POSIZIONE, 1 - VALU, abisso - 1 + (partita_prima));
      else
	{
	  abisso_nuovo = abisso - 2 + partita_prima;
	  if (abisso_nuovo <= 1)
	    v = -tuo_cerca_fine (POSIZIONE, 1 - VALU, 0);
	  else
	    v = -tuo_cerca_mezzo (POSIZIONE, 1 - VALU, abisso_nuovo);
	}
      ANNULLA (POSIZIONE, mossa);
      CHECK_HALT ();
      if (v <= valu_migliore)
	continue;
      valu_migliore = v;
      if (v >= VALU)
	{
	  Zobrist_basso (DINAMICO->Zobrist, mossa, MASSIMO (1, abisso), v);
	  RETURN (v);
	}
    }
  if (ignorato && valu_migliore < -25000)
    valu_migliore = VALU - 1;
  Zobrist_alto (DINAMICO->Zobrist, MASSIMO (1, abisso), valu_migliore);
  RETURN (valu_migliore);
}
