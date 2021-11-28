#ifndef BUILD_mossa_gen
#define BUILD_mossa_gen

#include "RobboLito.h"

#define ALLEGA(L, x, y) { (L++)->mossa = (x) | (y);}
#define ALLEGA_AI(T, w)\
  { while (T)\
      { ai = BSF(T);\
	c = POSIZIONE->qu[ai];\
	ALLEGA (LISTA, (qu << 6) | ai, w);\
	bitLIBERO (ai, T); } }

#include "iniz_gen.i"

#define pedone_donna ( 0xd8 << 24 )
#define pedone_cavallo ( 0xc2 << 24 )

#define pietra_scacco 0x8000

#ifdef MULTI_STORIA
#define mossa_ALLEGA(L, x, pezzo, ai, sca)\
  { (L++)->mossa = (x)\
      | ( (quadretto_fisso[ai] & (sca) ) ? pietra_scacco : 0)\
      | ( storia_valu[POSIZIONE->cpu][pezzo][ai] << 16); }
#else
#define mossa_ALLEGA(L, x, pezzo, ai, sca)\
  { (L++)->mossa = (x)\
      | ( (quadretto_fisso[ai] & (sca) ) ? pietra_scacco : 0)\
      | ( storia_valu[pezzo][ai] << 16); }
#endif

#define ALLEGA_AI_ORD(T, pezzo, sca)\
 { while (T)\
     { ai = BSF(T);\
       mossa_ALLEGA (LISTA, (qu << 6) | ai, pezzo, ai, sca);\
       bitLIBERO (ai, T); } }

#define sotto_promozione_bianco()\
  { if ( (attaco_cavallo[ai] & bitbordo_nero_re) == 0)\
      mossa_ALLEGA ( LISTA, pietra_promozione_cavallo | (qu << 6) | ai,\
                     conto_pedone_bianco, ai, 0);\
    mossa_ALLEGA (LISTA, pietra_promozione_torre | (qu << 6) | ai,\
		  conto_pedone_bianco, ai, 0);\
    mossa_ALLEGA (LISTA, pietra_promozione_alfiere | (qu << 6) | ai,\
		  conto_pedone_bianco, ai, 0); }

#define sotto_promozione_nero()\
  { if ( (attaco_cavallo[ai] & bitbordo_bianco_re) == 0)\
      mossa_ALLEGA (LISTA, pietra_promozione_cavallo | (qu << 6) | ai,\
		    conto_pedone_nero, ai, 0);\
    mossa_ALLEGA (LISTA, pietra_promozione_torre | (qu << 6) | ai,\
		  conto_pedone_nero, ai, 0);\
    mossa_ALLEGA (LISTA, pietra_promozione_alfiere | (qu << 6) | ai,\
		  conto_pedone_nero, ai, 0); }

#define OK(x)\
  ( ((x & 0x7fff) != s1) && ((x & 0x7fff) != s2) && ((x & 0x7fff) != s3) )

void
sorta_ordinaria
(tipo_lista_mossa * m1, tipo_lista_mossa * m2, uint32 s1, uint32 s2, uint32 s3)
{
  tipo_lista_mossa *p, *q;
  int mossa;
  if (m1 == m2)
    return;
  for (p = m2 - 1; p >= m1; p--)
    {
      if (OK (p->mossa))
	break;
      p->mossa = 0;
    }
  while (p > m1)
    {
      p--;
      mossa = p->mossa;
      if (OK (mossa))
	{
	  for (q = p + 1; q < m2; q++)
	    {
	      if (mossa < q->mossa)
		(q - 1)->mossa = q->mossa;
	      else
		break;
	    }
	  q--;
	  q->mossa = mossa;
	}
      else
	{
	  m2--;
	  for (q = p; q < m2; q++)
	    q->mossa = (q + 1)->mossa;
	  m2->mossa = 0;
	}
    }
}

#ifdef MULTI_GUADAGNO
#define guadagno_ALLEGA(L, x, pezzo, ai) \
  { int v = ( (int) massimo_posizionale_guadagno[POSIZIONE->cpu][pezzo][(x) & 07777]); \
    if (v >= av) (L++)->mossa = (x) | (v << 16); }
#else
#define guadagno_ALLEGA(L, x, pezzo, ai) \
  { int v = ( (int) massimo_posizionale_guadagno[pezzo][(x) & 07777]); \
    if (v >= av) (L++)->mossa = (x) | (v << 16); }
#endif

#define guadagno_ALLEGA_AI(T, pezzo)\
  { while (T)\
      { ai = BSF(T);\
	guadagno_ALLEGA (LISTA, (qu << 6) | ai, pezzo, ai);\
	bitLIBERO(ai, T); } }

#define SORTA\
  for (p = LISTA - 1; p >= sm; p--)\
    { mossa = p->mossa;\
      for (q = p + 1; q < LISTA; q++)\
	{ if ( mossa<q->mossa ) (q - 1)->mossa = q->mossa;\
	  else break;\
	}\
      q--;\
      q->mossa = mossa; }

tipo_lista_mossa * evasione_mossa
(tipo_posizione *POSIZIONE, tipo_lista_mossa * lista, uint64 cel)
{
  if (POSIZIONE->bianco_en_mossa)
    return evasione_bianco (POSIZIONE, lista, cel);
  return evasione_nero (POSIZIONE, lista, cel);
}

tipo_lista_mossa * ordinaria_mossa
(tipo_posizione *POSIZIONE, tipo_lista_mossa * lista)
{
  if (POSIZIONE->bianco_en_mossa)
    return ordinaria_bianca (POSIZIONE, lista);
  return ordinaria_nera (POSIZIONE, lista);
}

tipo_lista_mossa * cattura_mossa
(tipo_posizione *POSIZIONE, tipo_lista_mossa * lista, uint64 cel)
{
  if (POSIZIONE->bianco_en_mossa)
    return cattura_bianca (POSIZIONE, lista, cel & bitbordo_occupato_nero);
  return cattura_nera (POSIZIONE, lista, cel & bitbordo_occupato_bianco);
}

#include "mossa_gen.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

tipo_lista_mossa * mio_evasione
(tipo_posizione *POSIZIONE, tipo_lista_mossa * LISTA, uint64 c2)
{
  uint64 U, T, attacco, cel;
  int qu, ai, di, c, re, pezzo;
  re = mio_quadretto_re;
  attacco = mio_re_scacco;
  qu = BSF (attacco);
  pezzo = POSIZIONE->qu[qu];
  cel =
    (~tuo_attaco) & (((pezzo == conto_tuo_pedone) ? attaco_re[re] : 0) |
		     EVASIONE[re][qu]) & (~mio_occupato) & c2;
  bitLIBERO (qu, attacco);
  if (attacco)
    {
      qu = BSF (attacco);
      pezzo = POSIZIONE->qu[qu];
      cel = cel & (tuo_pedone (pezzo) | EVASIONE[re][qu]);
      qu = re;
      ALLEGA_AI (cel, cattura_valu[conto_mio_re][c]);
      LISTA->mossa = 0;
      return LISTA;
    }
  c2 &= INTERPOSIZIONE[re][qu];
  qu = re;
  ALLEGA_AI (cel, cattura_valu[conto_mio_re][c]);
  if (!c2)
    {
      LISTA->mossa = 0;
      return LISTA;
    }

  if (cattura_retta & (c2 & tuo_occupato))
    {
      ai = BSF (c2 & tuo_occupato);
      c = POSIZIONE->qu[ai];
      if (TRAVERSA_OTTAVA (ai))
	{
	  ALLEGA (LISTA, pietra_promozione_donna | di_sinistra (ai) | ai,
		  (0x20 << 24) + cattura_valu[conto_mio_pedone][c]);
	  ALLEGA (LISTA, pietra_promozione_cavallo | di_sinistra (ai) | ai,
		  0);
	  ALLEGA (LISTA, pietra_promozione_torre | di_sinistra (ai) | ai, 0);
	  ALLEGA (LISTA, pietra_promozione_alfiere | di_sinistra (ai) | ai,
		  0);
	}
      else
	ALLEGA (LISTA, di_sinistra (ai) | ai,
		cattura_valu[conto_mio_pedone][c]);
    }

  if (cattura_sinistra & (c2 & tuo_occupato))
    {
      ai = BSF (c2 & tuo_occupato);
      c = POSIZIONE->qu[ai];
      if (TRAVERSA_OTTAVA (ai))
	{
	  ALLEGA (LISTA, pietra_promozione_donna | di_retto (ai) | ai,
		  (0x20 << 24) + cattura_valu[conto_mio_pedone][c]);
	  ALLEGA (LISTA, pietra_promozione_cavallo | di_retto (ai) | ai, 0);
	  ALLEGA (LISTA, pietra_promozione_torre | di_retto (ai) | ai, 0);
	  ALLEGA (LISTA, pietra_promozione_alfiere | di_retto (ai) | ai, 0);
	}
      else
	ALLEGA (LISTA, di_retto (ai) | ai, cattura_valu[conto_mio_pedone][c]);
    }
  ai = ep_targa;
  if (ai)
    {
      if (cattura_retta & quadretto_fisso[ai]
	  && quadretto_fisso[indietro (ai)] & c2)
	ALLEGA (LISTA, pietra_en_passant | di_sinistra (ai) | ai,
		cattura_valu[conto_mio_pedone][conto_tuo_pedone]);
      if (cattura_sinistra & quadretto_fisso[ai]
	  && quadretto_fisso[indietro (ai)] & c2)
	ALLEGA (LISTA, pietra_en_passant | di_retto (ai) | ai,
		cattura_valu[conto_mio_pedone][conto_tuo_pedone]);
    }
  T = bitbordo_mio_pedone & indietro_turno ((c2 & tuo_occupato) ^ c2);
  while (T)
    {
      di = BSF (T);
      bitLIBERO (di, T);
      if (TRAVERSA_SETTIMA (di))
	{
	  ALLEGA (LISTA, pietra_promozione_donna | (di << 6) | avanti (di),
		  cattura_valu[conto_mio_pedone][0]);
	  ALLEGA (LISTA, pietra_promozione_cavallo | (di << 6) | avanti (di), 0);
	  ALLEGA (LISTA, pietra_promozione_torre | (di << 6) | avanti (di),
		  0);
	  ALLEGA (LISTA, pietra_promozione_alfiere | (di << 6) | avanti (di), 0);
	}
      else
	ALLEGA (LISTA, (di << 6) | avanti (di),
		cattura_valu[conto_mio_pedone][0]);
    }

  T =
    bitbordo_mio_pedone & indietro_turno_due ((c2 & tuo_occupato) ^ c2) &
    TRAVERSA_SECONDA & indietro_turno (~POSIZIONE->occupato_nero_bianco);
  while (T)
    {
      di = BSF (T);
      bitLIBERO (di, T);
      ALLEGA (LISTA, (di << 6) | avanti_due (di),
	      cattura_valu[conto_mio_pedone][0]);
    }
  for (U = bitbordo_mio_cavallo; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_cavallo[qu] & c2;
      ALLEGA_AI (T, cattura_valu[conto_mio_cavallo][c]);
    }
  for (U = bitbordo_mio_alfiere; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_alfiere (qu) & c2;
      ALLEGA_AI (T, cattura_valu[conto_mio_chiaro][c]);
    }
  for (U = bitbordo_mio_torre; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_torre (qu) & c2;
      ALLEGA_AI (T, cattura_valu[conto_mio_torre][c]);
    }
  for (U = bitbordo_mio_donna; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_donna (qu) & c2;
      ALLEGA_AI (T, cattura_valu[conto_mio_donna][c]);
    }
  LISTA->mossa = 0;
  return LISTA;
}

tipo_lista_mossa * mio_guadagno
(tipo_posizione *POSIZIONE, tipo_lista_mossa * LISTA, int av)
{
  uint64 vuoto = ~POSIZIONE->occupato_nero_bianco, U, T;
  int ai, qu;
  tipo_lista_mossa *sm, *p, *q;
  int mossa;
  sm = LISTA;
  for (U =
       avanti_turno (bitbordo_mio_pedone & TRAVERSA_SECONDA_SESTA) & vuoto;
       U; bitLIBERO (qu, U))
    {
      ai = BSF (U);
      if (en_traversa_terza (ai) && POSIZIONE->qu[avanti (ai)] == 0)
	guadagno_ALLEGA (LISTA, (indietro (ai) << 6) | avanti (ai),
			 conto_mio_pedone, avanti (ai));
      guadagno_ALLEGA (LISTA, (indietro (ai) << 6) | ai, conto_mio_pedone,
		       ai);
    }
  for (U = bitbordo_mio_cavallo; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_cavallo[qu] & vuoto;
      guadagno_ALLEGA_AI (T, conto_mio_cavallo);
    }
  for (U = bitbordo_mio_chiaro; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_alfiere (qu) & vuoto;
      guadagno_ALLEGA_AI (T, conto_mio_chiaro);
    }
  for (U = bitbordo_mio_scuro; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_alfiere (qu) & vuoto;
      guadagno_ALLEGA_AI (T, conto_mio_scuro);
    }
  for (U = bitbordo_mio_torre; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_torre (qu) & vuoto;
      guadagno_ALLEGA_AI (T, conto_mio_torre);
    }
  for (U = bitbordo_mio_donna; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_donna (qu) & vuoto;
      guadagno_ALLEGA_AI (T, conto_mio_donna);
    }
  qu = mio_quadretto_re;
  T = attaco_re[qu] & vuoto & (~tuo_attaco);
  guadagno_ALLEGA_AI (T, conto_mio_re);
  LISTA->mossa = 0;
  SORTA;
  return LISTA;
}

tipo_lista_mossa * mio_cattura
(tipo_posizione *POSIZIONE, tipo_lista_mossa * LISTA, uint64 cel)
{
  uint64 U, T, AT, AA;
  int qu, ai, c;
  ai = ep_targa;
  if (ai)
    {
      if (cattura_sinistra & quadretto_fisso[ai])
	ALLEGA (LISTA, pietra_en_passant | di_retto (ai) | ai,
		cattura_en_passant);
      if (cattura_retta & quadretto_fisso[ai])
	ALLEGA (LISTA, pietra_en_passant | di_sinistra (ai) | ai,
		cattura_en_passant);
    }
  if ((cel & mio_attaco) == 0)
    goto TARGA_NON;

  T = cattura_sinistra & (~bitbordo_traversa_ottava) & cel;
  while (T)
    {
      ai = BSF (T);
      c = POSIZIONE->qu[ai];
      ALLEGA (LISTA, di_retto (ai) | ai, cattura_valu[conto_mio_pedone][c]);
      bitLIBERO (ai, T);
    }
  T = cattura_retta & (~bitbordo_traversa_ottava) & cel;
  while (T)
    {
      ai = BSF (T);
      c = POSIZIONE->qu[ai];
      ALLEGA (LISTA, di_sinistra (ai) | ai,
	      cattura_valu[conto_mio_pedone][c]);
      bitLIBERO (ai, T);
    }

  for (U = bitbordo_mio_cavallo; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_cavallo[qu] & cel;
      ALLEGA_AI (T, cattura_valu[conto_mio_cavallo][c]);
    }
  for (U = bitbordo_mio_alfiere; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      AA = attaco_alfiere (qu);
      T = AA & cel;
      ALLEGA_AI (T, cattura_valu[conto_mio_chiaro][c]);
    }
  for (U = bitbordo_mio_torre; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      AT = attaco_torre (qu);
      T = AT & cel;
      ALLEGA_AI (T, cattura_valu[conto_mio_torre][c]);
    }
  for (U = bitbordo_mio_donna; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      AT = attaco_torre (qu);
      AA = attaco_alfiere (qu);
      T = (AA | AT) & cel;
      ALLEGA_AI (T, cattura_valu[conto_mio_donna][c]);
    }
  qu = BSF (bitbordo_mio_re);
  T = attaco_re[qu] & cel;
  ALLEGA_AI (T, cattura_valu[conto_mio_re][c]);
TARGA_NON:
  for (U = bitbordo_mio_pedone & bitbordo_traversa_settima; U;
       bitLIBERO (qu, U))
    {
      qu = BSF (U);
      ai = avanti (qu);
      if (POSIZIONE->qu[ai] == 0)
	{
	  ALLEGA (LISTA, pietra_promozione_donna | (qu << 6) | ai,
		  pedone_donna);
	  if (attaco_cavallo[ai] & bitbordo_tuo_re)
	    ALLEGA (LISTA, pietra_promozione_cavallo | (qu << 6) | ai,
		    pedone_cavallo);
	}
      ai = avanti_sinistra (qu);
      if (qu != BIANCO_A7 && quadretto_fisso[ai] & cel)
	{
	  c = POSIZIONE->qu[ai];
	  ALLEGA (LISTA, pietra_promozione_donna | (qu << 6) | ai,
		  promozione_donna_cattura);
	  if (attaco_cavallo[ai] & bitbordo_tuo_re)
	    ALLEGA (LISTA, pietra_promozione_cavallo | (qu << 6) | ai,
		    promozione_cavallo_cattura);
	}
      ai = avanti_retto (qu);
      if (qu != BIANCO_H7 && quadretto_fisso[ai] & cel)
	{
	  c = POSIZIONE->qu[ai];
	  ALLEGA (LISTA, pietra_promozione_donna | (qu << 6) | ai,
		  promozione_donna_cattura);
	  if (attaco_cavallo[ai] & bitbordo_tuo_re)
	    ALLEGA (LISTA, pietra_promozione_cavallo | (qu << 6) | ai,
		    promozione_cavallo_cattura);
	}
    }
  LISTA->mossa = 0;
  return LISTA;
}

tipo_lista_mossa * mio_ordinaria
(tipo_posizione *POSIZIONE, tipo_lista_mossa * LISTA)
{
  uint64 vuoto = ~POSIZIONE->occupato_nero_bianco, U, T, TOR, ALF, RE;
  int ai, qu, requ = tuo_quadretto_re;

  if (arrocco_oo
      && ((POSIZIONE->occupato_nero_bianco | tuo_attaco) & BIANCO_F1G1) == 0)
    mossa_ALLEGA (LISTA, pietra_arrocco | (BIANCO_E1 << 6) | BIANCO_G1,
		  conto_mio_re, BIANCO_G1, 0);
  if (arrocco_ooo && (POSIZIONE->occupato_nero_bianco & BIANCO_B1C1D1) == 0
      && (tuo_attaco & BIANCO_C1D1) == 0)
    mossa_ALLEGA (LISTA, pietra_arrocco | (BIANCO_E1 << 6) | BIANCO_C1,
		  conto_mio_re, BIANCO_C1, 0);

  RE = mio_attaco_pedone[requ];
  if (bitbordo_mio_donna | bitbordo_mio_torre)
    TOR = attaco_torre (requ);
  if (bitbordo_mio_donna | bitbordo_mio_alfiere)
    ALF = attaco_alfiere (requ);
  for (U =
       avanti_turno (bitbordo_mio_pedone & TRAVERSA_SECONDA_SESTA) & vuoto;
       U; bitLIBERO (qu, U))
    {
      ai = BSF (U);
      if (en_traversa_terza (ai) && POSIZIONE->qu[avanti (ai)] == 0)
	mossa_ALLEGA (LISTA, (indietro (ai) << 6) | avanti (ai),
		      conto_mio_pedone, avanti (ai), RE);
      mossa_ALLEGA (LISTA, (indietro (ai) << 6) | ai, conto_mio_pedone, ai,
		    RE);
    }

  for (U = bitbordo_mio_donna; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_donna (qu) & vuoto;
      ALLEGA_AI_ORD (T, conto_mio_donna, TOR | ALF);
    }
  for (U = bitbordo_mio_torre; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_torre (qu) & vuoto;
      ALLEGA_AI_ORD (T, conto_mio_torre, TOR);
    }
  for (U = bitbordo_mio_alfiere; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_alfiere (qu) & vuoto;
      ALLEGA_AI_ORD (T,
		     ((quadretto_fisso[qu] & SCURO) ? conto_mio_scuro :
		      conto_mio_chiaro), ALF);
    }
  qu = BSF (bitbordo_mio_re);
  T = attaco_re[qu] & vuoto & (~tuo_attaco);
  ALLEGA_AI_ORD (T, conto_mio_re, 0);
  for (U = bitbordo_mio_cavallo; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_cavallo[qu] & vuoto;
      ALLEGA_AI_ORD (T, conto_mio_cavallo, attaco_cavallo[requ]);
    }
  for (U = bitbordo_mio_pedone & bitbordo_traversa_settima; U;
       bitLIBERO (qu, U))
    {
      qu = BSF (U);
      ai = avanti (qu);
      if (POSIZIONE->qu[ai] == 0)
	sotto_promozione ();
      ai = avanti_sinistra (qu);
      if (qu != BIANCO_A7 && quadretto_fisso[ai] & tuo_occupato)
	sotto_promozione ();
      ai = avanti_retto (qu);
      if (qu != BIANCO_H7 && quadretto_fisso[ai] & tuo_occupato)
	sotto_promozione ();
    }
  LISTA->mossa = 0;
  return LISTA;
}

tipo_lista_mossa * mio_scacco_zitto
(tipo_posizione *POSIZIONE, tipo_lista_mossa * LISTA, uint64 cel)
{
  int requ, re, qu, ai, di, pezzo;
  uint64 U, T, V;
  tipo_lista_mossa *lista;
  uint32 mossa;
  uint64 gcm;
  gcm = ~mio_xray;
  cel = (~cel) & ~mio_occupato;;
  lista = LISTA;
  re = tuo_quadretto_re;
  lista = LISTA;

  for (U = mio_xray & mio_occupato; U; bitLIBERO (di, U))
    {
      di = BSF (U);
      pezzo = POSIZIONE->qu[di];
      if (pezzo == conto_mio_pedone)
	{
	  if (COLONNA (di) != COLONNA (re) && !TRAVERSA_SETTIMA (di)
	      && POSIZIONE->qu[avanti (di)] == 0)
	    {
	      (LISTA++)->mossa = (di << 6) | avanti (di);
	      if (en_traversa_seconda (di) && POSIZIONE->qu[avanti_due (di)] == 0)
		(LISTA++)->mossa = (di << 6) | avanti_due (di);
	    }
	  if (retto_cattura)
	    (LISTA++)->mossa = (di << 6) | avanti_retto (di);
	  if (sinistra_cattura)
	    (LISTA++)->mossa = (di << 6) | avanti_sinistra (di);
	}
      else if (pezzo == conto_mio_cavallo)
	{
	  V = attaco_cavallo[di] & cel;
	  while (V)
	    {
	      ai = BSF (V);
	      (LISTA++)->mossa = (di << 6) | ai;
	      bitLIBERO (ai, V);
	    }
	}
      else if (pezzo == conto_mio_chiaro || pezzo == conto_mio_scuro)
	{
	  V = attaco_alfiere (di) & cel;
	  while (V)
	    {
	      ai = BSF (V);
	      (LISTA++)->mossa = (di << 6) | ai;
	      bitLIBERO (ai, V);
	    }
	}
      else if (pezzo == conto_mio_torre)
	{
	  V = attaco_torre (di) & cel;
	  while (V)
	    {
	      ai = BSF (V);
	      (LISTA++)->mossa = (di << 6) | ai;
	      bitLIBERO (ai, V);
	    }
	}
      else if (pezzo == conto_mio_re)
	{
	  if (COLONNA (di) == COLONNA (re) || TRAVERSA (di) == TRAVERSA (re))
	    V = attaco_re[di] & non_ortogonale[re] & cel & (~tuo_attaco);
	  else
	    V = attaco_re[di] & non_diagonale[re] & cel & (~tuo_attaco);
	  while (V)
	    {
	      ai = BSF (V);
	      (LISTA++)->mossa = (di << 6) | ai;
	      bitLIBERO (ai, V);
	    }
	}
    }

  requ = tuo_quadretto_re;
  T =
    cattura_sinistra & (~bitbordo_traversa_ottava) & cel & tuo_occupato &
    mio_attaco_pedone[requ];
  while (T)
    {
      ai = BSF (T);
      (LISTA++)->mossa = di_retto (ai) | ai;
      bitLIBERO (ai, T);
    }
  T =
    cattura_retta & (~bitbordo_traversa_ottava) & cel & tuo_occupato &
    mio_attaco_pedone[requ];
  while (T)
    {
      ai = BSF (T);
      (LISTA++)->mossa = di_sinistra (ai) | ai;
      bitLIBERO (ai, T);
    }

  for (U = bitbordo_mio_donna; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_donna (qu) & attaco_donna (re) & cel;
      while (T)
	{
	  ai = BSF (T);
	  bitLIBERO (ai, T);
	  if ((tuo_attaco_pedone[ai] & bitbordo_tuo_pedone & gcm) == 0
	      && (attaco_cavallo[ai] & bitbordo_tuo_cavallo & gcm) == 0)
	    {
	      mossa = (qu << 6) | ai;
	      if (mio_SEE (POSIZIONE, mossa))
		(LISTA++)->mossa = (qu << 6) | ai;
	    }
	}
    }
  for (U = bitbordo_mio_torre; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_torre (qu) & attaco_torre (re) & cel;
      while (T)
	{
	  ai = BSF (T);
	  bitLIBERO (ai, T);
	  if ((tuo_attaco_pedone[ai] & bitbordo_tuo_pedone & gcm) == 0
	      && (attaco_cavallo[ai] & bitbordo_tuo_cavallo & gcm) == 0)
	    {
	      mossa = (qu << 6) | ai;
	      if (mio_SEE (POSIZIONE, mossa))
		(LISTA++)->mossa = (qu << 6) | ai;
	    }
	}
    }
  for (U = bitbordo_mio_alfiere; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_alfiere (qu) & attaco_alfiere (re) & cel;
      while (T)
	{
	  ai = BSF (T);
	  bitLIBERO (ai, T);
	  if ((tuo_attaco_pedone[ai] & bitbordo_tuo_pedone & gcm) == 0)
	    {
	      mossa = (qu << 6) | ai;
	      if (mio_SEE (POSIZIONE, mossa))
		(LISTA++)->mossa = (qu << 6) | ai;
	    }
	}
    }
  for (U = bitbordo_mio_cavallo; U; bitLIBERO (qu, U))
    {
      qu = BSF (U);
      T = attaco_cavallo[qu] & attaco_cavallo[re] & cel;
      while (T)
	{
	  ai = BSF (T);
	  bitLIBERO (ai, T);
	  if ((tuo_attaco_pedone[ai] & bitbordo_tuo_pedone & gcm) == 0)
	    {
	      mossa = (qu << 6) | ai;
	      if (mio_SEE (POSIZIONE, mossa))
		(LISTA++)->mossa = (qu << 6) | ai;
	    }
	}
    }

  if (bitbordo_tuo_re & TRAVERSA_QUARTA_OTTAVA_noH
      && POSIZIONE->qu[indietro_retto (requ)] == 0)
    {
      if (POSIZIONE->qu[indietro_retto_due (requ)] == conto_mio_pedone)
	{
	  di = indietro_retto_due (requ);
	  ai = indietro_retto (requ);
	  mossa = (di << 6) | ai;
	  if (pedone_guardiano (ai, di) && mio_SEE (POSIZIONE, mossa))
	    (LISTA++)->mossa = mossa;
	}
      if (TRAVERSA (requ) == numero_traversa5
	  && POSIZIONE->qu[indietro_retto_due (requ)] == 0
	  && POSIZIONE->qu[indietro_retto_tre (requ)] == conto_mio_pedone)
	{
	  ai = indietro_retto (requ);
	  di = indietro_retto_tre (requ);
	  mossa = (di << 6) | ai;
	  if (pedone_guardiano (ai, di) && mio_SEE (POSIZIONE, mossa))
	    (LISTA++)->mossa = mossa;
	}
    }
  if (bitbordo_tuo_re & TRAVERSA_QUARTA_OTTAVA_noA
      && POSIZIONE->qu[indietro_sinistro (requ)] == 0)
    {
      if (POSIZIONE->qu[indietro_sinistro_due (requ)] == conto_mio_pedone)
	{
	  di = indietro_sinistro_due (requ);
	  ai = indietro_sinistro (requ);
	  mossa = (di << 6) | ai;
	  if (pedone_guardiano (ai, di) && mio_SEE (POSIZIONE, mossa))
	    (LISTA++)->mossa = mossa;
	}
      if (TRAVERSA (requ) == numero_traversa5
	  && POSIZIONE->qu[indietro_sinistro_due (requ)] == 0
	  && POSIZIONE->qu[indietro_sinistro_tre (requ)] == conto_mio_pedone)
	{
	  ai = indietro_sinistro (requ);
	  di = indietro_sinistro_tre (requ);
	  mossa = (di << 6) | ai;
	  if (pedone_guardiano (ai, di) && mio_SEE (POSIZIONE, mossa))
	    (LISTA++)->mossa = mossa;
	}
    }
  LISTA->mossa = MOSSA_NON;
  return LISTA;
}
