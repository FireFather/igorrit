#ifndef BUILD_nodo_alto
#define BUILD_nodo_alto
#include "RobboLito.h"

#include "sorvegliare.h"
#define scacco\
  ( POSIZIONE->bianco_en_mossa ?\
    (bitbordo_bianco_re & DINAMICO->attacco_nero) :\
    (bitbordo_nero_re & DINAMICO->attacco_bianco) )

tipo_lista_mossa RADICE_LISTA_MOSSA[256];
#include "nodo_alto.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

void
mio_alto (tipo_posizione *POSIZIONE)
{
  int i, k, abisso, A, L, U, v, valu = 0, tras_abisso,
    mossa_abisso = 0, precisa_abisso = 0;
  uint32 mossa, zob_mossa = 0, precisa_mossa = 0, ai, di;
  tipo_lista_mossa *p, *q, *lista;
  tipo_zobrist *tr;
  tipo_dinamico *POSIZIONE0 = DINAMICO;
  int pezzo_valu[16] = { 0, 1, 3, 0, 3, 3, 5, 9, 0, 1, 3, 0, 3, 3, 5, 9 };
  //YUSUF128 H[1];

  chiamata_valu_piena (0);
  if (scacco)
    lista = mio_evasione (POSIZIONE, RADICE_LISTA_MOSSA, 0xffffffffffffffff);
  else
    {
      q = mio_cattura (POSIZIONE, RADICE_LISTA_MOSSA, tuo_occupato);
      lista = mio_ordinaria (POSIZIONE, q);
      sorta_ordinaria (lista, q, 0, 0, 0);
    }

  k = DINAMICO->Zobrist & elenco_celare;
  for (i = 0; i < 4; i++)
    {
      tr = elenco_zobrist + (k + i);
      HYATT_HASH();
      if ((tr->zobrist ^ (DINAMICO->Zobrist >> 32)) == 0)
	{
	  tras_abisso = tr->abisso_basso;
	  mossa = tr->mossa;
	  if (zobrist_preciso (tr))
	    {
	      precisa_abisso = tras_abisso;
	      precisa_mossa = mossa;
	      valu = zobrist_valu_alta (tr);
	    }
	  if (mossa && tras_abisso > mossa_abisso)
	    {
	      mossa_abisso = tras_abisso;
	      zob_mossa = mossa;
	    }
	}
    }

  if (precisa_abisso >= abisso_precedente - 6 && precisa_mossa == zob_mossa
      && precisa_mossa && radice_rapido && abisso_precedente >= 18
      && mio_garantire (POSIZIONE, precisa_mossa) && valu < 25000 && valu > -25000
      && !ANALISI && !PONDERING)
    {
      radice_valu = valu;
      radice_mossa = precisa_mossa;
      radice_abisso = precisa_abisso;
      radice_rapido = FALSO;
      if (!scacco)
	v = mio_escludo (POSIZIONE, valu - 50, abisso_precedente - 6, precisa_mossa);
      else
	v =
	  mio_escludo_scacco (POSIZIONE, valu - 50, abisso_precedente - 6,
			      precisa_mossa);
      if (v < valu - 50)
	return;
    }

  radice_rapido = VERO;
  q = RADICE_LISTA_MOSSA;
  for (p = RADICE_LISTA_MOSSA; p < lista; p++)
    {
      mossa = p->mossa & 0x7fff;
      FARE (POSIZIONE, mossa);
      chiamata_valu_piena (0);
      if (!mossa_illegale)
	(q++)->mossa = mossa & 0x7fff;
      ANNULLA (POSIZIONE, mossa);
    }
  q->mossa = 0;
  lista = q;

  for (p = RADICE_LISTA_MOSSA; p < lista; p++)
    {
      if (POSIZIONE->qu[AI (p->mossa)])
	{
	  ai = POSIZIONE->qu[AI (p->mossa)];
	  di = POSIZIONE->qu[DI (p->mossa)];
	  p->mossa |=
	    0xff000000 + ((16 * pezzo_valu[ai] - pezzo_valu[di]) << 16);
	}
    }
  for (p = RADICE_LISTA_MOSSA; p < lista; p++)
    if (p->mossa == zob_mossa)
      p->mossa |= 0xffff0000;
  for (p = lista - 1; p >= RADICE_LISTA_MOSSA; p--)
    {
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

  L = -VALU_MATTO;
  U = VALU_MATTO;
  if (!RADICE_LISTA_MOSSA[0].mossa)
    {
      if (scacco)
	{
	  radice_valu = L;
	}
      else
	{
	  radice_valu = 0;
	}
      radice_mossa = 0;
      radice_abisso = 0;
      return;
    }
  for (abisso = 2; abisso <= 250; abisso += 2)
    {
      MOSSA_BRUTTA = FALSO;
      MOSSA_BATTAGLIA = FALSO;
      if (abisso >= 14 && radice_valu <= 25000 && -25000 <= radice_valu)
	{
	  A = 8;
	  L = radice_valu - A;
	  U = radice_valu + A;
	  if (L < -25000)
	    L = -VALU_MATTO;
	  if (U > 25000)
	    U = VALU_MATTO;
	ANCORA:
	  v = mio_nodo_superiore (POSIZIONE, L, U, abisso);
	  if (v > L && v < U)
	    goto TERMINE;
	  if (v <= L)
	    {
	      L -= A;
	      A += A / 2;
	      radice_valu = L;
	      goto ANCORA;
	    }
	  else
	    {
	      U += A;
	      A += A / 2;
	      radice_valu = U;
	      goto ANCORA;
	    }
	}
      else
	v = mio_nodo_superiore (POSIZIONE, -VALU_MATTO, VALU_MATTO, abisso);
    TERMINE:
      if (abisso == 2)
	{
	  if (!RADICE_LISTA_MOSSA[1].mossa
	      || (RADICE_LISTA_MOSSA[0].mossa - RADICE_LISTA_MOSSA[1].mossa >=
		  (200 << 16)))
	    FACILE = VERO;
	}
      radice_precedente = radice_valu;
      risolvere_termine (POSIZIONE, abisso / 2);
    }
}
