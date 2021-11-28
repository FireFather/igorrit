
#define TRAVERSAS78 0xffff000000000000
#define TRAVERSAS678 0xffffff0000000000
#define TRAVERSAS12 0x000000000000ffff
#define TRAVERSAS123 0x00000000000ffffff

#include "RobboLito.h"
#include "valu.v"

uint8 Yakov_bianco[24576] = {
#include "YakovChart.bi"
};

uint8 Yakov_nero[24576] = {
#include "YakovChart.ne"
};

void
mobilita (tipo_posizione *POSIZIONE)
{
  uint64 U, A, T, AA, AT;
  int b;
  DINAMICO->bianco_xray = 0;
  DINAMICO->nero_xray = 0;
  A = attaco_re[POSIZIONE->bianco_re_quadretto];
  DINAMICO->attacco_bianco = A;
  if (A & bitbordo_nero_re)
    DINAMICO->nero_re_scacco = quadretto_fisso[POSIZIONE->bianco_re_quadretto];
  else
    DINAMICO->nero_re_scacco = 0;
  A = attaco_re[POSIZIONE->nero_re_quadretto];
  DINAMICO->attacco_nero = A;
  if (A & bitbordo_bianco_re)
    DINAMICO->bianco_re_scacco = quadretto_fisso[POSIZIONE->nero_re_quadretto];
  else
    DINAMICO->bianco_re_scacco = 0;
  for (U = bitbordo_bianco_cavallo; U; bitLIBERO (b, U))
    {
      b = BSF (U);
      A = attaco_cavallo[b];
      DINAMICO->attacco_bianco |= A;
      if (A & bitbordo_nero_re)
	DINAMICO->nero_re_scacco |= quadretto_fisso[b];
    }
  for (U = bitbordo_bianco_alfiere; U; bitLIBERO (b, U))
    {
      b = BSF (U);
      A = attaco_alfiere (b);
      DINAMICO->attacco_bianco |= A;
      if (A & bitbordo_nero_re)
	DINAMICO->nero_re_scacco |= quadretto_fisso[b];
      else if (bitbordo_nero_re & diagonale[b])
	{
	  T = attaco_alfiere (POSIZIONE->nero_re_quadretto) & A;
	  DINAMICO->bianco_xray |= T;
	  if (T)
	    xray_bianco_lista[BSF (T)] = b;
	}
    }
  for (U = bitbordo_bianco_torre; U; bitLIBERO (b, U))
    {
      b = BSF (U);
      A = attaco_torre (b);
      DINAMICO->attacco_bianco |= A;
      if (A & bitbordo_nero_re)
	DINAMICO->nero_re_scacco |= quadretto_fisso[b];
      else if (bitbordo_nero_re & ortogonale[b])
	{
	  T = attaco_torre (POSIZIONE->nero_re_quadretto) & A;
	  DINAMICO->bianco_xray |= T;
	  if (T)
	    xray_bianco_lista[BSF (T)] = b;
	}
    }
  for (U = bitbordo_bianca_donna; U; bitLIBERO (b, U))
    {
      b = BSF (U);
      AT = attaco_torre (b);
      AA = attaco_alfiere (b);
      A = AA | AT;
      DINAMICO->attacco_bianco |= A;
      if (A & bitbordo_nero_re)
	DINAMICO->nero_re_scacco |= quadretto_fisso[b];
      else if (bitbordo_nero_re & diagonale[b])
	{
	  T = attaco_alfiere (POSIZIONE->nero_re_quadretto) & AA;
	  DINAMICO->bianco_xray |= T;
	  if (T)
	    xray_bianco_lista[BSF (T)] = b;
	}
      else if (bitbordo_nero_re & ortogonale[b])
	{
	  T = attaco_torre (POSIZIONE->nero_re_quadretto) & AT;
	  DINAMICO->bianco_xray |= T;
	  if (T)
	    xray_bianco_lista[BSF (T)] = b;
	}
    }
  for (U = bitbordo_nero_cavallo; U; bitLIBERO (b, U))
    {
      b = BSF (U);
      A = attaco_cavallo[b];
      DINAMICO->attacco_nero |= A;
      if (A & bitbordo_bianco_re)
	DINAMICO->bianco_re_scacco |= quadretto_fisso[b];
    }
  for (U = bitbordo_nero_alfiere; U; bitLIBERO (b, U))
    {
      b = BSF (U);
      A = attaco_alfiere (b);
      DINAMICO->attacco_nero |= A;
      if (A & bitbordo_bianco_re)
	DINAMICO->bianco_re_scacco |= quadretto_fisso[b];
      else if (bitbordo_bianco_re & diagonale[b])
	{
	  T = attaco_alfiere (POSIZIONE->bianco_re_quadretto) & A;
	  DINAMICO->nero_xray |= T;
	  if (T)
	    xray_nero_lista[BSF (T)] = b;
	}
    }
  for (U = bitbordo_nero_torre; U; bitLIBERO (b, U))
    {
      b = BSF (U);
      A = attaco_torre (b);
      DINAMICO->attacco_nero |= A;
      if (A & bitbordo_bianco_re)
	DINAMICO->bianco_re_scacco |= quadretto_fisso[b];
      else if (bitbordo_bianco_re & ortogonale[b])
	{
	  T = attaco_torre (POSIZIONE->bianco_re_quadretto) & A;
	  DINAMICO->nero_xray |= T;
	  if (T)
	    xray_nero_lista[BSF (T)] = b;
	}
    }
  for (U = bitbordo_nera_donna; U; bitLIBERO (b, U))
    {
      b = BSF (U);
      AA = attaco_alfiere (b);
      AT = attaco_torre (b);
      A = AA | AT;
      DINAMICO->attacco_nero |= A;
      if (A & bitbordo_bianco_re)
	DINAMICO->bianco_re_scacco |= quadretto_fisso[b];
      else if (bitbordo_bianco_re & diagonale[b])
	{
	  T = attaco_alfiere (POSIZIONE->bianco_re_quadretto) & AA;
	  DINAMICO->nero_xray |= T;
	  if (T)
	    xray_nero_lista[BSF (T)] = b;
	}
      else if (bitbordo_bianco_re & ortogonale[b])
	{
	  T = attaco_torre (POSIZIONE->bianco_re_quadretto) & AT;
	  DINAMICO->nero_xray |= T;
	  if (T)
	    xray_nero_lista[BSF (T)] = b;
	}
    }
  A = (bitbordo_bianco_pedone & (~COLONNAa)) << 7;
  T = A & bitbordo_nero_re;
  DINAMICO->nero_re_scacco |= (T >> 7);
  DINAMICO->attacco_bianco |= A;
  A = (bitbordo_bianco_pedone & (~COLONNAh)) << 9;
  T = A & bitbordo_nero_re;
  DINAMICO->nero_re_scacco |= (T >> 9);
  DINAMICO->attacco_bianco |= A;
  A = (bitbordo_nero_pedone & (~COLONNAh)) >> 7;
  T = A & bitbordo_bianco_re;
  DINAMICO->bianco_re_scacco |= (T << 7);
  DINAMICO->attacco_nero |= A;
  A = (bitbordo_nero_pedone & (~COLONNAa)) >> 9;
  T = A & bitbordo_bianco_re;
  DINAMICO->bianco_re_scacco |= (T << 9);
  DINAMICO->attacco_nero |= A;
}

static void
truffa_nera (tipo_posizione *POSIZIONE, int mossa)
{
  int v, p, m;
  if (DINAMICO->cattura)
    return;
  if (DINAMICO->cattura && POSIZIONE->qu[AI (mossa)] != conto_pedone_bianco)
    return;
  p = POSIZIONE->qu[AI (mossa)];
  m = mossa & 07777;
  v = DINAMICO->valu_posizionale - ((DINAMICO - 1)->valu_posizionale);
  if (MAX_POS_GAIN (p ,m) <= v)
    MAX_POS_GAIN (p, m) = v;
  else
    MAX_POS_GAIN (p, m)--;
}

static void
truffa_bianca (tipo_posizione *POSIZIONE, int mossa)
{
  int v, p, m;
  if (DINAMICO->cattura)
    return;
  if (DINAMICO->cattura && POSIZIONE->qu[AI (mossa)] != conto_pedone_nero)
    return;
  p = POSIZIONE->qu[AI (mossa)];
  m = mossa & 07777;
  v = ((DINAMICO - 1)->valu_posizionale) - DINAMICO->valu_posizionale;
  if (MAX_POS_GAIN (p ,m) <= v)
    MAX_POS_GAIN (p, m) = v;
  else
    MAX_POS_GAIN (p, m)--;
}

#define valu_zobrist_dimensione ( 0x8000 )
#define valu_zobrist_celare ( valu_zobrist_dimensione - 1 )

uint64 valu_zobrist[valu_zobrist_dimensione];

void
valu_zobrist_libero ()
{
  int c;
  for (c = 0; c < valu_zobrist_dimensione; c++)
    valu_zobrist[c] = 0;
}

#ifdef HAS_PREFETCH
#define PREFETCH_PEDONE_ZOBRIST __builtin_prefetch(&PEDONE_PTR, 1, 2);
#else
#define PREFETCH_PEDONE_ZOBRIST
#endif

static int
valore_material (tipo_posizione *POSIZIONE)
{
  int valu =
    975 * (POPCNT (bitbordo_bianca_donna) - POPCNT (bitbordo_nera_donna));
  valu +=
    500 * (POPCNT (bitbordo_bianco_torre) - POPCNT (bitbordo_nero_torre));
  valu +=
    325 * (POPCNT (bitbordo_bianco_alfiere) - POPCNT (bitbordo_nero_alfiere));
  valu +=
    325 * (POPCNT (bitbordo_bianco_cavallo) - POPCNT (bitbordo_nero_cavallo));
  valu +=
    100 * (POPCNT (bitbordo_bianco_pedone) - POPCNT (bitbordo_nero_pedone));
  if (bitbordo_bianco_chiaro && bitbordo_bianco_scuro)
    valu += 50;
  if (bitbordo_nero_chiaro && bitbordo_nero_scuro)
    valu -= 50;
  return valu;
}

static void
re_pedone_bianco (tipo_posizione *POSIZIONE, int materiale_valu, uint8 SEGNO,
		  tipo_pedone_valu * PEDONE_VALU)
{
  int valu, bianco_primo, nero_primo, qu, tr;
  uint8 C;
  uint64 A, T;
  if (PEDONE_VALU->ZobristPedone != DINAMICO->ZobristPedone)
    valutazione_pedone (POSIZIONE, PEDONE_VALU);
  DINAMICO->bianco_xray = DINAMICO->nero_xray = 0;
  valu = ((DINAMICO->statico) + (PEDONE_VALU->puntos));
  valu = (sint16) (valu & 0xffff);
  bianco_primo = 0;
  C = PEDONE_VALU->bianco_pedone_libero_linea;
  while (C)
    {
      qu = BSR (pila_colonna[BSF (C)] & bitbordo_bianco_pedone);
      tr = TRAVERSA (qu);
      bitLIBERO (0, C);
      if ((bianco_guido[qu] & bitbordo_bianco_re) == 0)
	{
	  if (bitbordo_occupato_bianco & colonna_aperta_bianca[qu]
	      || (bitbordo_nero_re & quadrante_nero_mossa_bianca[qu]) == 0)
	    continue;
	  if (bianco_primo <= tr)
	    bianco_primo = tr;
	}
      else if (bianco_primo <= tr)
	bianco_primo = tr;
    }
  nero_primo = 0;
  C = PEDONE_VALU->nero_pedone_libero_linea;
  while (C)
    {
      qu = BSF (pila_colonna[BSF (C)] & bitbordo_nero_pedone);
      tr = T8 - TRAVERSA (qu);
      bitLIBERO (0, C);
      if ((nero_guido[qu] & bitbordo_nero_re) == 0)
	{
	  if (bitbordo_occupato_nero & colonna_aperta_nera[qu]
	      || (bitbordo_bianco_re & quadrante_bianco_mossa_bianca[qu]) ==
	      0)
	    continue;
	  if (nero_primo <= tr)
	    nero_primo = tr;
	}
      else if (nero_primo <= tr)
	nero_primo = tr;
    }
  DINAMICO->valu = (SEGNO * (valu + materiale_valu)) / 128;
  DINAMICO->nero_re_scacco = DINAMICO->bianco_re_scacco = 0;
  if (bianco_primo > nero_primo
      && (bitbordo_nero_pedone & anteriore_nero[T8 - bianco_primo + 1]) == 0)
    DINAMICO->valu += 150 + 50 * bianco_primo;
  if (nero_primo > bianco_primo + 1
      && (bitbordo_bianco_pedone & anteriore_bianco[nero_primo - 2]) == 0)
    DINAMICO->valu -= 150 + 50 * nero_primo;

  A = (bitbordo_bianco_pedone & (~COLONNAa)) << 7;
  T = A & bitbordo_nero_re;
  DINAMICO->nero_re_scacco |= (T >> 7);
  DINAMICO->attacco_bianco = A;
  A = (bitbordo_bianco_pedone & (~COLONNAh)) << 9;
  T = A & bitbordo_nero_re;
  DINAMICO->nero_re_scacco |= (T >> 9);
  DINAMICO->attacco_bianco |= A | attaco_re[POSIZIONE->bianco_re_quadretto];
  A = (bitbordo_nero_pedone & (~COLONNAh)) >> 7;
  T = A & bitbordo_bianco_re;
  DINAMICO->bianco_re_scacco |= (T << 7);
  DINAMICO->attacco_nero = A;
  A = (bitbordo_nero_pedone & (~COLONNAa)) >> 9;
  T = A & bitbordo_bianco_re;
  DINAMICO->bianco_re_scacco |= (T << 9);
  DINAMICO->attacco_nero |= A | attaco_re[POSIZIONE->nero_re_quadretto];
  if (bitbordo_nero_re & attaco_re[POSIZIONE->bianco_re_quadretto])
    {
      DINAMICO->nero_re_scacco |=
	quadretto_fisso[POSIZIONE->bianco_re_quadretto];
      DINAMICO->bianco_re_scacco |=
	quadretto_fisso[POSIZIONE->nero_re_quadretto];
    }
  if (DINAMICO->valu > 0 && !bitbordo_bianco_pedone)
    DINAMICO->valu = 0;
  if (DINAMICO->valu < 0 && !bitbordo_nero_pedone)
    DINAMICO->valu = 0;
  if (DINAMICO->valu > 0)
    {
      if ((bitbordo_bianco_pedone & ~COLONNAh) == 0
	  && (bitbordo_nero_re | attaco_re[POSIZIONE->nero_re_quadretto]) &
	  quadretto_fisso[H8])
	DINAMICO->valu = 0;
      if ((bitbordo_bianco_pedone & ~COLONNAa) == 0
	  && (bitbordo_nero_re | attaco_re[POSIZIONE->nero_re_quadretto]) &
	  quadretto_fisso[A8])
	DINAMICO->valu = 0;
      if ((DINAMICO->pietras & 28) == 28)
	{
	  qu = BSF (bitbordo_bianco_pedone);
	  tr = TRAVERSA (qu);
	  valu =
	    Yakov_bianco[384 * POSIZIONE->bianco_re_quadretto +
			 6 * POSIZIONE->nero_re_quadretto + tr - 1] &
	    (1 << COLONNA (qu));
	  if (!valu)
	    DINAMICO->valu = 0;
	  else
	    DINAMICO->valu =
	      ((sint16) (DINAMICO->statico & 0xffff)) + 75 * tr + 250;
	}
    }
  if (DINAMICO->valu < 0)
    {
      if ((bitbordo_nero_pedone & ~COLONNAh) == 0
	  && (bitbordo_bianco_re | attaco_re[POSIZIONE->bianco_re_quadretto]) &
	  quadretto_fisso[H1])
	DINAMICO->valu = 0;
      if ((bitbordo_nero_pedone & ~COLONNAa) == 0
	  && (bitbordo_bianco_re | attaco_re[POSIZIONE->bianco_re_quadretto]) &
	  quadretto_fisso[A1])
	DINAMICO->valu = 0;
      if ((DINAMICO->pietras & 28) == 28)
	{
	  qu = H8 - BSR (bitbordo_nero_pedone);
	  tr = TRAVERSA (qu);
	  valu =
	    Yakov_nero[384 * (H8 - POSIZIONE->nero_re_quadretto) +
		       6 * (H8 - POSIZIONE->bianco_re_quadretto) + tr - 1] &
	    (1 << COLONNA (qu));
	  if (!valu)
	    DINAMICO->valu = 0;
	  else
	    DINAMICO->valu =
	      ((sint16) (DINAMICO->statico & 0xffff)) - 75 * tr - 250;
	}
    }
}

static void
re_pedone_nero (tipo_posizione *POSIZIONE, int materiale_valu, uint8 SEGNO,
		tipo_pedone_valu * PEDONE_VALU)
{
  int valu, bianco_primo, nero_primo, qu, tr;
  uint8 C;
  uint64 A, T;
  if (PEDONE_VALU->ZobristPedone != DINAMICO->ZobristPedone)
    valutazione_pedone (POSIZIONE, PEDONE_VALU);
  DINAMICO->bianco_xray = DINAMICO->nero_xray = 0;
  valu = ((DINAMICO->statico) + (PEDONE_VALU->puntos));
  valu = (sint16) (valu & 0xffff);
  bianco_primo = 0;
  C = PEDONE_VALU->bianco_pedone_libero_linea;
  while (C)
    {
      qu = BSR (pila_colonna[BSF (C)] & bitbordo_bianco_pedone);
      tr = TRAVERSA (qu);
      bitLIBERO (0, C);
      if ((bianco_guido[qu] & bitbordo_bianco_re) == 0)
	{
	  if (bitbordo_occupato_bianco & colonna_aperta_bianca[qu]
	      || (bitbordo_nero_re & quadrante_nero_mossa_nera[qu]) == 0)
	    continue;
	  if (bianco_primo <= tr)
	    bianco_primo = tr;
	}
      else if (bianco_primo <= tr)
	bianco_primo = tr;
    }
  nero_primo = 0;
  C = PEDONE_VALU->nero_pedone_libero_linea;
  while (C)
    {
      qu = BSF (pila_colonna[BSF (C)] & bitbordo_nero_pedone);
      tr = T8 - TRAVERSA (qu);
      bitLIBERO (0, C);
      if ((nero_guido[qu] & bitbordo_nero_re) == 0)
	{
	  if (bitbordo_occupato_nero & colonna_aperta_nera[qu]
	      || (bitbordo_bianco_re & quadrante_bianco_mossa_nera[qu]) == 0)
	    continue;
	  if (nero_primo <= tr)
	    nero_primo = tr;
	}
      else if (nero_primo <= tr)
	nero_primo = tr;
    }
  DINAMICO->valu = -(SEGNO * (valu + materiale_valu)) / 128;
  DINAMICO->nero_re_scacco = DINAMICO->bianco_re_scacco = 0;
  if (bianco_primo > nero_primo + 1
      && (bitbordo_nero_pedone & anteriore_nero[T8 - bianco_primo + 2]) == 0)
    DINAMICO->valu -= 150 + 50 * bianco_primo;
  if (nero_primo > bianco_primo
      && (bitbordo_bianco_pedone & anteriore_bianco[nero_primo - 1]) == 0)
    DINAMICO->valu += 150 + 50 * nero_primo;

  A = (bitbordo_bianco_pedone & (~COLONNAa)) << 7;
  T = A & bitbordo_nero_re;
  DINAMICO->nero_re_scacco |= (T >> 7);
  DINAMICO->attacco_bianco = A;
  A = (bitbordo_bianco_pedone & (~COLONNAh)) << 9;
  T = A & bitbordo_nero_re;
  DINAMICO->nero_re_scacco |= (T >> 9);
  DINAMICO->attacco_bianco |= A | attaco_re[POSIZIONE->bianco_re_quadretto];
  A = (bitbordo_nero_pedone & (~COLONNAh)) >> 7;
  T = A & bitbordo_bianco_re;
  DINAMICO->bianco_re_scacco |= (T << 7);
  DINAMICO->attacco_nero = A;
  A = (bitbordo_nero_pedone & (~COLONNAa)) >> 9;
  T = A & bitbordo_bianco_re;
  DINAMICO->bianco_re_scacco |= (T << 9);
  DINAMICO->attacco_nero |= A | attaco_re[POSIZIONE->nero_re_quadretto];
  if (bitbordo_nero_re & attaco_re[POSIZIONE->bianco_re_quadretto])
    {
      DINAMICO->nero_re_scacco |=
	quadretto_fisso[POSIZIONE->bianco_re_quadretto];
      DINAMICO->bianco_re_scacco |=
	quadretto_fisso[POSIZIONE->nero_re_quadretto];
    }
  if (DINAMICO->valu < 0 && !bitbordo_bianco_pedone)
    DINAMICO->valu = 0;
  if (DINAMICO->valu > 0 && !bitbordo_nero_pedone)
    DINAMICO->valu = 0;
  if (DINAMICO->valu < 0)
    {
      if ((bitbordo_bianco_pedone & ~COLONNAh) == 0
	  && (attaco_re[POSIZIONE->nero_re_quadretto] | bitbordo_nero_re) &
	  quadretto_fisso[H8])
	DINAMICO->valu = 0;
      if ((bitbordo_bianco_pedone & ~COLONNAa) == 0
	  && (attaco_re[POSIZIONE->nero_re_quadretto] | bitbordo_nero_re) &
	  quadretto_fisso[A8])
	DINAMICO->valu = 0;
      if ((DINAMICO->pietras & 28) == 28)
	{
	  qu = BSF (bitbordo_bianco_pedone);
	  tr = TRAVERSA (qu);
	  valu =
	    Yakov_nero[384 * POSIZIONE->bianco_re_quadretto +
		       6 * POSIZIONE->nero_re_quadretto + tr -
		       1] & (1 << COLONNA (qu));
	  if (!valu)
	    DINAMICO->valu = 0;
	  else
	    DINAMICO->valu =
	      -((sint16) (DINAMICO->statico & 0xffff)) - 75 * tr - 250;
	}
    }
  if (DINAMICO->valu > 0)
    {
      if ((bitbordo_nero_pedone & ~COLONNAh) == 0
	  && (attaco_re[POSIZIONE->bianco_re_quadretto] | bitbordo_bianco_re) &
	  quadretto_fisso[H1])
	DINAMICO->valu = 0;
      if ((bitbordo_nero_pedone & ~COLONNAa) == 0
	  && (attaco_re[POSIZIONE->bianco_re_quadretto] | bitbordo_bianco_re) &
	  quadretto_fisso[A1])
	DINAMICO->valu = 0;
      if ((DINAMICO->pietras & 28) == 28)
	{
	  qu = H8 - BSR (bitbordo_nero_pedone);
	  tr = TRAVERSA (qu);
	  valu =
	    Yakov_bianco[384 * (H8 - POSIZIONE->nero_re_quadretto) +
			 6 * (H8 - POSIZIONE->bianco_re_quadretto) + tr - 1]
	    & (1 << COLONNA (qu));
	  if (!valu)
	    DINAMICO->valu = 0;
	  else
	    DINAMICO->valu =
	      -((sint16) (DINAMICO->statico & 0xffff)) + 75 * tr + 250;
	}
    }
}

void
valutazione (tipo_posizione *POSIZIONE, int minimo, int massimo, int mossa)
{
  tipo_pedone_valu *PEDONE_PTR;
  int indice, materiale_valu, valu;
  int b, tr, anti_fase, fase;
  int ai, cattura, re_bianco_qu, re_nero_qu;
  uint64 U, attacco_re_bianco, attacco_re_nero, A, AA, AT;
  sint32 colpo_re_bianco, colpo_re_nero;
  uint64 bianco_buono_minore, nero_buono_minore, bianca_mobilita_salva,
    nera_mobilita_salva, bianco_ok_xray, nero_ok_xray;
  uint64 T, attacco_pedone_nero, attacco_pedone_bianco;
  int apertura, finale;
  uint8 attacco_buono_nero, attacco_buono_bianco;
  uint8 SEGNO;
  int v, posizionale;
  tipo_pedone_valu PEDONE_VALU[1];
  int ch;

  PEDONE_PTR =
    pedone_zobrist + (DINAMICO->ZobristPedone & (elenco_pedone_numero - 1));
  PREFETCH_PEDONE_ZOBRIST;
  indice = (DINAMICO->materiale >> 8) & 0x7ffff;
  SEGNO = MATERIALE[indice].segno;
  DINAMICO->pietras = MATERIALE[indice].pietras;
  if (!(DINAMICO->materiale & 0x80000000))
    materiale_valu = MATERIALE[indice].valu;
  else
    {
      if (POPCNT (bitbordo_bianca_donna) > 1
	  || POPCNT (bitbordo_nera_donna) > 1
	  || POPCNT (bitbordo_bianco_torre) > 2
	  || POPCNT (bitbordo_nero_torre) > 2
	  || POPCNT (bitbordo_bianco_chiaro) > 1
	  || POPCNT (bitbordo_nero_chiaro) > 1
	  || POPCNT (bitbordo_bianco_scuro) > 1
	  || POPCNT (bitbordo_nero_scuro) > 1
	  || POPCNT (bitbordo_bianco_cavallo) > 2
	  || POPCNT (bitbordo_nero_cavallo) > 2)
	{
	  SEGNO = 0x80;
	  materiale_valu = valore_material (POSIZIONE);
	  DINAMICO->pietras = 0;
	  if (bitbordo_bianca_donna | bitbordo_bianco_torre |
	      bitbordo_bianco_alfiere | bitbordo_bianco_cavallo)
	    DINAMICO->pietras |= 2;
	  if (bitbordo_nera_donna | bitbordo_nero_torre |
	      bitbordo_nero_alfiere | bitbordo_nero_cavallo)
	    DINAMICO->pietras |= 1;
	}
      else
	{
	  materiale_valu = MATERIALE[indice].valu;
	  DINAMICO->materiale &= 0x7fffffff;
	}
    }

  if (((DINAMICO->Zobrist ^
	valu_zobrist[DINAMICO->Zobrist & valu_zobrist_celare]) &
       0xffffffffffff0000) == 0)
    {
      valu =
	(int) ((sint16)
	       (valu_zobrist[DINAMICO->Zobrist & valu_zobrist_celare] &
		0xffff));
      DINAMICO->pigro = 0;
      mobilita (POSIZIONE);
      DINAMICO->valu_posizionale =
	((POSIZIONE->bianco_en_mossa) ? valu : -valu) - materiale_valu;
      DINAMICO->valu = valu;
      if (mossa && !(DINAMICO - 1)->pigro)
	POSIZIONE->bianco_en_mossa ?
	  truffa_bianca (POSIZIONE, mossa) : truffa_nera (POSIZIONE, mossa);
      return;
    }

#ifdef CON_ROBBO_BUILD
  int c = POPCNT (POSIZIONE->occupato_nero_bianco);
  if (c <= 5)
    {
      int va;
      mobilita (POSIZIONE);
      if (TRIPLE_VALU (POSIZIONE, &va))
	{
	  int molt = 21000 - (128 * c) - 64 * HEIGHT (POSIZIONE);
	  if (va == 0)
	    DINAMICO->valu = 0;
	  else
	    {
	      DINAMICO->valu = (va > 0) ? molt : -molt;
	      DINAMICO->valu +=
		(POSIZIONE->bianco_en_mossa ? materiale_valu : -materiale_valu);
	    }
	  DINAMICO->pigro = 1;
	  DINAMICO->valu_posizionale = 0;
	  valu_zobrist[DINAMICO->Zobrist & valu_zobrist_celare] =
	    (DINAMICO->Zobrist & 0xffffffffffff0000) | (DINAMICO->valu & 0xffff);
	  return;
	}
    }
#endif

  memcpy (PEDONE_VALU, PEDONE_PTR, sizeof (tipo_pedone_valu));
#if 1 /* HYATT hashing */
  PEDONE_VALU->ZobristPedone ^= (((uint64*) (PEDONE_VALU)) + 0x1)[0];
  PEDONE_VALU->ZobristPedone ^= (((uint64*) (PEDONE_VALU)) + 0x2)[0];
  PEDONE_VALU->ZobristPedone ^= (((uint64*) (PEDONE_VALU)) + 0x3)[0];
#endif

  if ((DINAMICO->materiale & 0xff) == 0)
    {
      POSIZIONE->bianco_en_mossa ?
	re_pedone_bianco (POSIZIONE, materiale_valu, SEGNO, PEDONE_VALU) :
	re_pedone_nero (POSIZIONE, materiale_valu, SEGNO, PEDONE_VALU);
      DINAMICO->pigro = 1;
      DINAMICO->valu_posizionale = 0;
      valu_zobrist[DINAMICO->Zobrist & valu_zobrist_celare] =
	(DINAMICO->Zobrist & 0xffffffffffff0000) | (DINAMICO->valu & 0xffff);
      return;
    }

#define bianco_minore_pietra (8 << 2)
#define nero_minore_pietra (16 << 2)

  if ((DINAMICO->pietras & bianco_minore_pietra
       || DINAMICO->pietras & nero_minore_pietra)
      && PEDONE_VALU->ZobristPedone != DINAMICO->ZobristPedone)
    valutazione_pedone (POSIZIONE, PEDONE_VALU);

  if ((DINAMICO->pietras & bianco_minore_pietra
       && PEDONE_VALU->bianco_pedone_linea_conteggio <= 1)
      || (DINAMICO->pietras & nero_minore_pietra
	  && PEDONE_VALU->nero_pedone_linea_conteggio <= 1)
      || (DINAMICO->pietras & 128));
  else
    {
      if (POSIZIONE->bianco_en_mossa)
	{
	  posizionale = (DINAMICO - 1)->valu_posizionale;
	  cattura = DINAMICO->cattura;
	  ai = AI (mossa);
	  valu = statico_valu[POSIZIONE->qu[ai]][ai] - statico_valu[POSIZIONE->qu[ai]][DI (mossa)];
	  if (cattura)
	    valu -= statico_valu[cattura][ai];
	  fase = DINAMICO->materiale & 0xff;
	  finale = (sint16) (valu & 0xffff);
	  apertura = (finale < 0) + (sint16) ((valu >> 16) & 0xffff);
	  anti_fase = 32 - fase;
	  valu = (finale * anti_fase + apertura * fase) / 32;
	  posizionale += valu;
	  v = posizionale + materiale_valu;
	  if (v < -massimo - 16 * (int) (DINAMICO - 1)->pigro
	      || v > -minimo + 16 * (int) (DINAMICO - 1)->pigro)
	    {
	      DINAMICO->pigro = (DINAMICO - 1)->pigro + 1;
	      DINAMICO->valu = v;
	      DINAMICO->valu_posizionale = posizionale;
	      mobilita (POSIZIONE);
	      return;
	    }
	}
      else
	{
	  posizionale = (DINAMICO - 1)->valu_posizionale;
	  cattura = DINAMICO->cattura;
	  ai = AI (mossa);
	  valu = statico_valu[POSIZIONE->qu[ai]][ai] - statico_valu[POSIZIONE->qu[ai]][DI (mossa)];
	  if (cattura)
	    valu -= statico_valu[cattura][ai];
	  fase = DINAMICO->materiale & 0xff;
	  finale = (sint16) (valu & 0xffff);
	  apertura = (finale < 0) + (sint16) ((valu >> 16) & 0xffff);
	  anti_fase = 32 - fase;
	  valu = (finale * anti_fase + apertura * fase) / 32;
	  posizionale += valu;
	  v = posizionale + materiale_valu;
	  if (v < minimo - 16 * (int) (DINAMICO - 1)->pigro
	      || v > massimo + 16 * (int) (DINAMICO - 1)->pigro)
	    {
	      DINAMICO->pigro = (DINAMICO - 1)->pigro + 1;
	      DINAMICO->valu = -v;
	      DINAMICO->valu_posizionale = posizionale;
	      mobilita (POSIZIONE);
	      return;
	    }
	}
    }

  re_bianco_qu = POSIZIONE->bianco_re_quadretto;
  re_nero_qu = POSIZIONE->nero_re_quadretto;
  attacco_re_bianco = attaco_re[re_bianco_qu];
  attacco_re_nero = attaco_re[re_nero_qu];
  attacco_buono_nero = attacco_buono_bianco = 0;
  if (PEDONE_VALU->ZobristPedone != DINAMICO->ZobristPedone)
    valutazione_pedone (POSIZIONE, PEDONE_VALU);
  valu = (DINAMICO->statico) + (PEDONE_VALU->puntos);

  DINAMICO->bianco_xray = 0;

  A = (bitbordo_bianco_pedone & (~COLONNAa)) << 7;
  T = A & bitbordo_nero_re;
  DINAMICO->nero_re_scacco = (T >> 7);
  attacco_pedone_bianco = A;
  A = (bitbordo_bianco_pedone & (~COLONNAh)) << 9;
  T = A & bitbordo_nero_re;
  DINAMICO->nero_re_scacco |= (T >> 9);
  attacco_pedone_bianco |= A;
  DINAMICO->attacco_bianco = attacco_pedone_bianco;
  A = (bitbordo_nero_pedone & (~COLONNAh)) >> 7;
  T = A & bitbordo_bianco_re;
  DINAMICO->bianco_re_scacco = (T << 7);
  attacco_pedone_nero = A;
  A = (bitbordo_nero_pedone & (~COLONNAa)) >> 9;
  T = A & bitbordo_bianco_re;
  DINAMICO->bianco_re_scacco |= (T << 9);
  attacco_pedone_nero |= A;
  DINAMICO->attacco_nero = attacco_pedone_nero;

  nero_ok_xray = (~bitbordo_nero_pedone) & ~attacco_pedone_bianco;
  bianco_ok_xray = (~bitbordo_bianco_pedone) & ~attacco_pedone_nero;
  bianco_buono_minore =
    (bitbordo_bianco_cavallo | bitbordo_bianco_alfiere) &
    attacco_pedone_bianco;
  nero_buono_minore =
    (bitbordo_nero_cavallo | bitbordo_nero_alfiere) & attacco_pedone_nero;

  if (attacco_pedone_bianco & attacco_re_nero)
    colpo_re_nero = COLPO_pedone;
  else
    colpo_re_nero = 0;
  U = (POSIZIONE->occupato_nero_bianco >> 8) & bitbordo_bianco_pedone;
  while (U)
    {
      b = BSF (U);
      valu -= pedone_mobilita_non;
      bitLIBERO (b, U);
    }
  bianca_mobilita_salva = ~(attacco_pedone_nero | bitbordo_occupato_bianco);

  U = bitbordo_bianca_donna;
  while (U)
    {
      b = BSF (U);
      bitLIBERO (b, U);
      AA = attaco_alfiere (b);
      AT = attaco_torre (b);
      if (bitbordo_nero_re & diagonale[b])
	{
	  T = attaco_alfiere (re_nero_qu) & AA;
	  if (T)
	    {
	      valu += bianco_donna_diag_xray[POSIZIONE->qu[BSF (T)]];
	      DINAMICO->bianco_xray |= T;
	      xray_bianco_lista[BSF (T)] = b;
	    }
	}
      else if (bitbordo_nero_re & ortogonale[b])
	{
	  T = attaco_torre (re_nero_qu) & AT;
	  if (T)
	    {
	      valu += bianco_donna_orto_xray[POSIZIONE->qu[BSF (T)]];
	      DINAMICO->bianco_xray |= T;
	      xray_bianco_lista[BSF (T)] = b;
	    }
	}
      A = AA | AT;
      T = A & bianca_mobilita_salva;
      DINAMICO->attacco_bianco |= A;
      if (A & attacco_re_nero)
	colpo_re_nero += COLPO_donna;
      if (A & bitbordo_nero_re)
	DINAMICO->nero_re_scacco |= quadretto_fisso[b];
      if (A & attacco_re_bianco)
	{
	  valu += donna_guardato;
	}
      valu += mobilita_donna (T);
      if (A & (~attacco_pedone_nero) & bitbordo_occupato_nero)
	{
	  valu += donna_attacco;
	}
      if (bitbordo_nero_pedone & attaco_pedone_nero[b])
	{
	  valu -= pedone_attacco_donna;
	  attacco_buono_nero += 1;
	}
      if (TRAVERSA (b) == T7)
	{
	  if ((bitbordo_nero_pedone | bitbordo_nero_re) & TRAVERSAS78)
	    {
	      valu += donna_settima;
	      if (bitbordo_bianco_torre & TRAVERSA7 & AT
		  && bitbordo_nero_re & TRAVERSA8)
		{
		  valu += donna_settima_doppia;
		}
	    }
	}
    }

  U = bitbordo_bianco_torre;
  while (U)
    {
      b = BSF (U);
      bitLIBERO (b, U);
      A = attaco_torre (b);
      DINAMICO->attacco_bianco |= A;
      if (bitbordo_nero_re & ortogonale[b])
	{
	  T = attaco_torre (re_nero_qu) & A;
	  if (T)
	    {
	      valu += bianco_torre_xray[POSIZIONE->qu[BSF (T)]];
	      DINAMICO->bianco_xray |= T;
	      xray_bianco_lista[BSF (T)] = b;
	    }
	}
      if (A & attacco_re_nero)
	colpo_re_nero += COLPO_torre;
      if (A & bitbordo_nero_re)
	DINAMICO->nero_re_scacco |= quadretto_fisso[b];
      if (A & attacco_re_bianco)
	{
	  valu += torre_guardato;
	}
      valu += mobilita_torre (A & bianco_ok_xray);

      if (A & (~attacco_pedone_nero) & bitbordo_nero_pedone)
	{
	  valu += torre_attacco_pedone;
	}
      if (A &
	  ((bitbordo_nero_cavallo | bitbordo_nero_alfiere) &
	   ~attacco_pedone_nero))
	{
	  valu += torre_attacco_minore;
	}
      if (A & bitbordo_nera_donna)
	{
	  valu += torre_attacco_donna;
	  attacco_buono_bianco += 1;
	}
      if (bitbordo_nero_pedone & attaco_pedone_nero[b])
	{
	  valu -= pedone_attacco_torre;
	  attacco_buono_nero += 1;
	}

      if ((bitbordo_bianco_pedone & colonna_aperta_bianca[b]) == 0)
	{
	  valu += torre_aperto_mezzo;
	  if ((bitbordo_nero_pedone & colonna_aperta_bianca[b]) == 0)
	    {
	      T = nero_buono_minore & colonna_aperta_bianca[b];
	      if (!T)
		{
		  valu += torre_aperto;
		}
	      else
		{
		  int t = BSF (T);
		  if ((colonna_isolata[COLONNA (t)] &
		       anteriore_nero[TRAVERSA (t)] & bitbordo_bianco_pedone)
		      == 0)
		    {
		      valu += torre_aperto_minore_fisso;
		    }
		  else
		    {
		      valu += torre_aperto_minore;
		    }
		}
	    }
	  else
	    {
	      T = colonna_aperta_bianca[b] & bitbordo_nero_pedone;
	      if (T)
		{
		  int t = BSF (T);
		  if ((colonna_isolata[COLONNA (t)] &
		       anteriore_bianco[TRAVERSA (t)] & bitbordo_nero_pedone)
		      == 0)
		    {
		      valu += torre_mezzo_pedone;
		    }
		}
	    }
	  if (bitbordo_nero_re & colonna_aperta_bianca[b])
	    {
	      valu += torre_mezzo_re;
	    }
	}

      if (quadretto_fisso[b] & avamposto_bianco
	  && (colonna_isolata[COLONNA (b)] & anteriore_bianco[TRAVERSA (b)] &
	      bitbordo_nero_pedone) == 0)
	{
	  if (bitbordo_bianco_pedone & attaco_pedone_bianco[b])
	    {
	      valu += avamposto_torre;
	      if (A &
		  (attacco_re_nero |
		   (bitbordo_occupato_nero & ~attacco_pedone_nero)) &
		  pila_traversa[TRAVERSA (b)])
		valu += avamposto_torre_guardato;
	    }
	}

      if (TRAVERSA (b) == T8)
	{
	  if (bitbordo_nero_re & TRAVERSA8)
	    {
	      valu += torre_ottava_re;
	    }
	}
      if (TRAVERSA (b) == T7)
	{
	  if ((bitbordo_nero_pedone | bitbordo_nero_re) & TRAVERSAS78)
	    {
	      valu += torre_settima_re_pedone;
	      if (bitbordo_nero_re & TRAVERSA8
		  && (bitbordo_bianca_donna | bitbordo_bianco_torre) &
		  TRAVERSA7 & A)
		{
		  valu += torre_settima_re_pedone_doppia;
		}
	    }
	}
      if (TRAVERSA (b) == T6
	  && (bitbordo_nero_pedone | bitbordo_nero_re) & TRAVERSAS678)
	{
	  valu += torra_sessa_re_pedone;
	}
    }

  bianca_mobilita_salva |= bitbordo_occupato_nero ^ bitbordo_nero_pedone;

  U = bitbordo_bianco_alfiere;
  while (U)
    {
      b = BSF (U);
      bitLIBERO (b, U);
      A = attaco_alfiere (b);
      DINAMICO->attacco_bianco |= A;
      if (bitbordo_nero_re & diagonale[b])
	{
	  T = attaco_alfiere (re_nero_qu) & A;
	  if (T)
	    {
	      valu += bianco_alfiere_xray[POSIZIONE->qu[BSF (T)]];
	      DINAMICO->bianco_xray |= T;
	      xray_bianco_lista[BSF (T)] = b;
	    }
	}
      if (A & attacco_re_nero)
	colpo_re_nero += COLPO_alfiere;
      if (A & bitbordo_nero_re)
	DINAMICO->nero_re_scacco |= quadretto_fisso[b];
      if (A & attacco_re_bianco)
	{
	  valu += alfiere_guardato;
	}
      valu +=
	mobilita_alfiere (A & bianca_mobilita_salva,
			  anteriore_bianco[TRAVERSA (b)]);

      if (A & (~attacco_pedone_nero) & bitbordo_nero_pedone)
	{
	  valu += alfiere_attacco_pedone;
	}
      if (A & (~attacco_pedone_nero) & bitbordo_nero_cavallo)
	{
	  valu += alfiere_attacco_cavallo;
	}
      if (A & (bitbordo_nero_torre | bitbordo_nera_donna))
	{
	  valu += cavallo_attacco_donna_torre;
	  attacco_buono_bianco += 1;
	}
      if (bitbordo_nero_pedone & attaco_pedone_nero[b])
	{
	  valu -= pedone_attacco_alfiere;
	  attacco_buono_nero += 1;
	}

      if (quadretto_fisso[b] & CHIARO)
	{
	  valu -=
	    (PEDONE_VALU->bianco_pedone_chiaro +
	     PEDONE_VALU->nero_pedone_chiaro / 2) * puntos (1, 1);
	  valu +=
	    POPCNT (bitbordo_nero_pedone & CHIARO &
		    anteriore_nero[TRAVERSA (b)] & ~attacco_pedone_nero) *
	    puntos (0, 2);
	}
      else
	{
	  valu -=
	    (PEDONE_VALU->bianco_pedone_scuro +
	     PEDONE_VALU->nero_pedone_scuro / 2) * puntos (1, 1);
	  valu +=
	    POPCNT (bitbordo_nero_pedone & SCURO &
		    anteriore_nero[TRAVERSA (b)] & ~attacco_pedone_nero) *
	    puntos (0, 2);
	}

      if (quadretto_fisso[b] & avamposto_bianco
	  && (colonna_isolata[COLONNA (b)] & anteriore_bianco[TRAVERSA (b)] &
	      bitbordo_nero_pedone) == 0)
	{
	  if (bitbordo_bianco_pedone & attaco_pedone_bianco[b])
	    {
	      valu += avamposto_alfiere;
	      if (A &
		  (attacco_re_nero |
		   (bitbordo_occupato_nero & ~attacco_pedone_nero)))
		valu += avamposto_alfiere_guardato;
	    }
	}
      if (POSIZIONE->qu[alfiere_trappola_quadretto[b]] == conto_pedone_nero)
	{
	  valu -= alfiere_trappola_valu;
	  if (POSIZIONE->qu[buono_trappola_alfiere_quadretto[b]] == conto_pedone_nero)
	    {
	      valu -= alfiere_trappola_guardato_valu;
	    }
	}
    }

  U = bitbordo_bianco_cavallo;
  while (U)
    {
      b = BSF (U);
      bitLIBERO (b, U);
      A = attaco_cavallo[b];
      DINAMICO->attacco_bianco |= A;
      if (A & (attacco_re_nero | bitbordo_nero_re))
	colpo_re_nero += COLPO_cavallo;
      if (A & bitbordo_nero_re)
	DINAMICO->nero_re_scacco |= quadretto_fisso[b];
      if (A & (attacco_re_bianco | bitbordo_bianco_re))
	{
	  valu += cavallo_guardato;
	}
      valu +=
	mobilita_cavallo (A & bianca_mobilita_salva,
			  anteriore_bianco[TRAVERSA (b)]);

      if (A & (~attacco_pedone_nero) & bitbordo_nero_pedone)
	{
	  valu += cavallo_attacco_pedone;
	}
      if (A & (~attacco_pedone_nero) & bitbordo_nero_alfiere)
	{
	  valu += cavallo_attacco_alfiere;
	}
      if (A & (bitbordo_nero_torre | bitbordo_nera_donna))
	{
	  valu += alfiere_attacco_donna_torre;
	  attacco_buono_bianco += 1;
	}
      if (bitbordo_nero_pedone & attaco_pedone_nero[b])
	{
	  valu -= pedone_attacco_cavallo;
	  attacco_buono_nero += 1;
	}

      if (quadretto_fisso[b] & avamposto_bianco
	  && (colonna_isolata[COLONNA (b)] & anteriore_bianco[TRAVERSA (b)] &
	      bitbordo_nero_pedone) == 0)
	{
	  valu += avamposto_cavallo;
	  if (bitbordo_bianco_pedone & attaco_pedone_bianco[b])
	    {
	      valu += avamposto_cavallo_pedone;
	      if (A &
		  (attacco_re_nero |
		   (bitbordo_occupato_nero & ~attacco_pedone_nero)))
		{
		  valu += avamposto_cavallo_guardato;
		  if (TRAVERSA (b) == T5)
		    valu += avamposto_cavallo_quinto;
		  if (COLONNA (b) == CD || COLONNA (b) == CE)
		    valu += avamposto_cavallo_DE;
		}
	    }
	}
    }

  if (attacco_pedone_nero & attacco_re_bianco)
    colpo_re_bianco = COLPO_pedone;
  else
    colpo_re_bianco = 0;
  U = (POSIZIONE->occupato_nero_bianco << 8) & bitbordo_nero_pedone;
  DINAMICO->nero_xray = 0;
  while (U)
    {
      b = BSF (U);
      valu += pedone_mobilita_non;
      bitLIBERO (b, U);
    }
  nera_mobilita_salva = ~(attacco_pedone_bianco | bitbordo_occupato_nero);

  U = bitbordo_nera_donna;
  while (U)
    {
      b = BSF (U);
      bitLIBERO (b, U);
      AA = attaco_alfiere (b);
      AT = attaco_torre (b);
      if (bitbordo_bianco_re & diagonale[b])
	{
	  T = attaco_alfiere (re_bianco_qu) & AA;
	  if (T)
	    {
	      valu -= nero_donna_diag_xray[POSIZIONE->qu[BSF (T)]];
	      DINAMICO->nero_xray |= T;
	      xray_nero_lista[BSF (T)] = b;
	    }
	}
      else if (bitbordo_bianco_re & ortogonale[b])
	{
	  T = attaco_torre (re_bianco_qu) & AT;
	  if (T)
	    {
	      valu -= nero_donna_orto_xray[POSIZIONE->qu[BSF (T)]];
	      DINAMICO->nero_xray |= T;
	      xray_nero_lista[BSF (T)] = b;
	    }
	}
      A = AA | AT;
      T = A & nera_mobilita_salva;
      DINAMICO->attacco_nero |= A;
      if (A & attacco_re_bianco)
	colpo_re_bianco += COLPO_donna;
      if (A & bitbordo_bianco_re)
	DINAMICO->bianco_re_scacco |= quadretto_fisso[b];
      if (A & attacco_re_nero)
	{
	  valu -= donna_guardato;
	}
      valu -= mobilita_donna (T);

      if (A & (~attacco_pedone_bianco) & bitbordo_occupato_bianco)
	{
	  valu -= donna_attacco;
	}
      if (bitbordo_bianco_pedone & attaco_pedone_bianco[b])
	{
	  valu += pedone_attacco_donna;
	  attacco_buono_bianco += 1;
	}

      if (TRAVERSA (b) == T2)
	{
	  if ((bitbordo_bianco_pedone | bitbordo_bianco_re) & TRAVERSAS12)
	    {
	      valu -= donna_settima;
	      if (bitbordo_nero_torre & TRAVERSA2 & AT
		  && bitbordo_bianco_re & TRAVERSA1)
		{
		  valu -= donna_settima_doppia;
		}
	    }
	}
    }

  U = bitbordo_nero_torre;
  while (U)
    {
      b = BSF (U);
      bitLIBERO (b, U);
      A = attaco_torre (b);
      DINAMICO->attacco_nero |= A;
      if (bitbordo_bianco_re & ortogonale[b])
	{
	  T = A & attaco_torre (re_bianco_qu);
	  if (T)
	    {
	      valu -= nero_torre_xray[POSIZIONE->qu[BSF (T)]];
	      DINAMICO->nero_xray |= T;
	      xray_nero_lista[BSF (T)] = b;
	    }
	}
      if (A & attacco_re_bianco)
	colpo_re_bianco += COLPO_torre;
      if (A & bitbordo_bianco_re)
	DINAMICO->bianco_re_scacco |= quadretto_fisso[b];
      if (A & attacco_re_nero)
	{
	  valu -= torre_guardato;
	}
      valu -= mobilita_torre (A & nero_ok_xray);

      if (A & (~attacco_pedone_bianco) & bitbordo_bianco_pedone)
	{
	  valu -= torre_attacco_pedone;
	}
      if (A & (bitbordo_bianco_cavallo | bitbordo_bianco_alfiere) &
	  ~attacco_pedone_bianco)
	{
	  valu -= torre_attacco_minore;
	}
      if (A & bitbordo_bianca_donna)
	{
	  valu -= torre_attacco_donna;
	  attacco_buono_nero += 1;
	}
      if (bitbordo_bianco_pedone & attaco_pedone_bianco[b])
	{
	  valu += pedone_attacco_torre;
	  attacco_buono_bianco += 1;
	}

      if ((bitbordo_nero_pedone & colonna_aperta_nera[b]) == 0)
	{
	  valu -= torre_aperto_mezzo;
	  if ((bitbordo_bianco_pedone & colonna_aperta_nera[b]) == 0)
	    {
	      T = bianco_buono_minore & colonna_aperta_nera[b];
	      if (!T)
		{
		  valu -= torre_aperto;
		}
	      else
		{
		  int t = BSR (T);
		  if ((colonna_isolata[COLONNA (t)] &
		       anteriore_bianco[TRAVERSA (t)] & bitbordo_nero_pedone)
		      == 0)
		    {
		      valu -= torre_aperto_minore_fisso;
		    }
		  else
		    {
		      valu -= torre_aperto_minore;
		    }
		}
	    }
	  else
	    {
	      T = colonna_aperta_nera[b] & bitbordo_bianco_pedone;
	      if (T)
		{
		  int t = BSR (T);
		  if ((colonna_isolata[COLONNA (t)] &
		       anteriore_nero[TRAVERSA (t)] & bitbordo_bianco_pedone)
		      == 0)
		    {
		      valu -= torre_mezzo_pedone;
		    }
		}
	    }
	  if (bitbordo_bianco_re & colonna_aperta_nera[b])
	    {
	      valu -= torre_mezzo_re;
	    }
	}

      if (quadretto_fisso[b] & avamposto_nero
	  && (colonna_isolata[COLONNA (b)] & anteriore_nero[TRAVERSA (b)] &
	      bitbordo_bianco_pedone) == 0)
	{
	  if (bitbordo_nero_pedone & attaco_pedone_nero[b])
	    {
	      valu -= avamposto_torre;
	      if (A &
		  (attacco_re_bianco |
		   (bitbordo_occupato_bianco & ~attacco_pedone_bianco)) &
		  pila_traversa[TRAVERSA (b)])
		valu -= avamposto_torre_guardato;
	    }
	}

      if (TRAVERSA (b) == T1)
	{
	  if (bitbordo_bianco_re & TRAVERSA1)
	    {
	      valu -= torre_ottava_re;
	    }
	}
      if (TRAVERSA (b) == T2)
	{
	  if ((bitbordo_bianco_pedone | bitbordo_bianco_re) & TRAVERSAS12)
	    {
	      valu -= torre_settima_re_pedone;
	      if (bitbordo_bianco_re & TRAVERSA1
		  && (bitbordo_nera_donna | bitbordo_nero_torre) & TRAVERSA2 &
		  A)
		{
		  valu -= torre_settima_re_pedone_doppia;
		}
	    }
	}
      if (TRAVERSA (b) == T3
	  && (bitbordo_bianco_pedone | bitbordo_bianco_re) & TRAVERSAS123)
	{
	  valu -= torra_sessa_re_pedone;
	}
    }

  nera_mobilita_salva |= bitbordo_occupato_bianco ^ bitbordo_bianco_pedone;

  U = bitbordo_nero_alfiere;
  while (U)
    {
      b = BSF (U);
      bitLIBERO (b, U);
      A = attaco_alfiere (b);
      DINAMICO->attacco_nero |= A;
      if (bitbordo_bianco_re & diagonale[b])
	{
	  T = A & attaco_alfiere (re_bianco_qu);
	  if (T)
	    {
	      valu -= nero_alfiere_xray[POSIZIONE->qu[BSF (T)]];
	      DINAMICO->nero_xray |= T;
	      xray_nero_lista[BSF (T)] = b;
	    }
	}
      if (A & attacco_re_bianco)
	colpo_re_bianco += COLPO_alfiere;
      if (A & bitbordo_bianco_re)
	DINAMICO->bianco_re_scacco |= quadretto_fisso[b];
      if (A & attacco_re_nero)
	{
	  valu -= alfiere_guardato;
	}
      valu -=
	mobilita_alfiere (A & nera_mobilita_salva,
			  anteriore_nero[TRAVERSA (b)]);

      if (A & (~attacco_pedone_bianco) & bitbordo_bianco_pedone)
	{
	  valu -= alfiere_attacco_pedone;
	}
      if (A & (~attacco_pedone_bianco) & bitbordo_bianco_cavallo)
	{
	  valu -= alfiere_attacco_cavallo;
	}
      if (A & (bitbordo_bianco_torre | bitbordo_bianca_donna))
	{
	  valu -= cavallo_attacco_donna_torre;
	  attacco_buono_nero += 1;
	}
      if (bitbordo_bianco_pedone & attaco_pedone_bianco[b])
	{
	  valu += pedone_attacco_alfiere;
	  attacco_buono_bianco += 1;
	}

      if (quadretto_fisso[b] & CHIARO)
	{
	  valu +=
	    (PEDONE_VALU->nero_pedone_chiaro +
	     PEDONE_VALU->bianco_pedone_chiaro / 2) * puntos (1, 1);
	  valu -=
	    POPCNT (bitbordo_bianco_pedone & CHIARO &
		    anteriore_bianco[TRAVERSA (b)] & ~attacco_pedone_bianco) *
	    puntos (0, 2);
	}
      else
	{
	  valu +=
	    (PEDONE_VALU->nero_pedone_scuro +
	     PEDONE_VALU->bianco_pedone_scuro / 2) * puntos (1, 1);
	  valu -=
	    POPCNT (bitbordo_bianco_pedone & SCURO &
		    anteriore_bianco[TRAVERSA (b)] & ~attacco_pedone_bianco) *
	    puntos (0, 2);
	}

      if (quadretto_fisso[b] & avamposto_nero
	  && (colonna_isolata[COLONNA (b)] & anteriore_nero[TRAVERSA (b)] &
	      bitbordo_bianco_pedone) == 0)
	{
	  if (bitbordo_nero_pedone & attaco_pedone_nero[b])
	    {
	      valu -= avamposto_alfiere;
	      if (A &
		  (attacco_re_bianco |
		   (bitbordo_occupato_bianco & ~attacco_pedone_bianco)))
		valu -= avamposto_alfiere_guardato;
	    }
	}

      if (POSIZIONE->qu[alfiere_trappola_quadretto[b]] == conto_pedone_bianco)
	{
	  valu += alfiere_trappola_valu;
	  if (POSIZIONE->qu[buono_trappola_alfiere_quadretto[b]] == conto_pedone_bianco)
	    {
	      valu += alfiere_trappola_guardato_valu;
	    }
	}
    }

  U = bitbordo_nero_cavallo;
  while (U)
    {
      b = BSF (U);
      bitLIBERO (b, U);
      A = attaco_cavallo[b];
      DINAMICO->attacco_nero |= A;
      if (A & (attacco_re_bianco | bitbordo_bianco_re))
	colpo_re_bianco += COLPO_cavallo;
      if (A & bitbordo_bianco_re)
	DINAMICO->bianco_re_scacco |= quadretto_fisso[b];
      if (A & (attacco_re_nero | bitbordo_nero_re))
	{
	  valu -= cavallo_guardato;
	}
      valu -=
	mobilita_cavallo (A & nera_mobilita_salva,
			  anteriore_nero[TRAVERSA (b)]);

      if (A & (~attacco_pedone_bianco) & bitbordo_bianco_pedone)
	{
	  valu -= cavallo_attacco_pedone;
	}
      if (A & (~attacco_pedone_bianco) & bitbordo_bianco_alfiere)
	{
	  valu -= cavallo_attacco_alfiere;
	}
      if (A & (bitbordo_bianco_torre | bitbordo_bianca_donna))
	{
	  valu -= alfiere_attacco_donna_torre;
	  attacco_buono_nero += 1;
	}
      if (bitbordo_bianco_pedone & attaco_pedone_bianco[b])
	{
	  valu += pedone_attacco_cavallo;
	  attacco_buono_bianco += 1;
	}

      if (quadretto_fisso[b] & avamposto_nero
	  && (colonna_isolata[COLONNA (b)] & anteriore_nero[TRAVERSA (b)] &
	      bitbordo_bianco_pedone) == 0)
	{
	  valu -= avamposto_cavallo;
	  if (bitbordo_nero_pedone & attaco_pedone_nero[b])
	    {
	      valu -= avamposto_cavallo_pedone;
	      if (A &
		  (attacco_re_bianco |
		   (bitbordo_occupato_bianco & ~attacco_pedone_bianco)))
		{
		  valu -= avamposto_cavallo_guardato;
		  if (TRAVERSA (b) == T4)
		    valu -= avamposto_cavallo_quinto;
		  if (COLONNA (b) == CD || COLONNA (b) == CE)
		    valu -= avamposto_cavallo_DE;
		}
	    }
	}
    }

  DINAMICO->attacco_bianco |= attacco_re_bianco;
  DINAMICO->attacco_nero |= attacco_re_nero;
  if (attacco_re_nero & bitbordo_bianco_re)
    {
      DINAMICO->bianco_re_scacco |=
	quadretto_fisso[POSIZIONE->nero_re_quadretto];
      DINAMICO->nero_re_scacco |=
	quadretto_fisso[POSIZIONE->bianco_re_quadretto];
    }

  if ((~DINAMICO->attacco_nero) & attacco_re_bianco & bitbordo_nero_pedone)
    {
      valu += re_attacco_nonguardato_pedone;
    }

  T = trappola_torre[re_bianco_qu] & bitbordo_bianco_torre;
  if (T)
    {
      int t = BSF (T);
      T = colonna_aperta_bianca[t] & bitbordo_bianco_pedone;
      if (T)
	{
	  t = BSF (T);
	  t >>= 3;
	  valu -= puntos (10 * (6 - t), 0);
	}
    }

  if (attacco_re_bianco & attacco_re_nero)
    colpo_re_bianco += COLPO_re;
  ch =
    (((re_sicura_moltiplicatore[colpo_re_bianco >> 16] *
       (colpo_re_bianco & 0xffff)) / re_sicura_dividendo) << 16) +
    PEDONE_VALU->bianco_re_periglio;
  if (!bitbordo_nera_donna)
    {
      ch >>= 16;
      ch *=
	POPCNT (bitbordo_nero_torre | bitbordo_nero_cavallo |
		bitbordo_nero_alfiere);
      ch >>= 3;
      ch <<= 16;
    }
  valu -= ch;

  if ((~DINAMICO->attacco_bianco) & attacco_re_nero & bitbordo_bianco_pedone)
    {
      valu -= re_attacco_nonguardato_pedone;
    }

  T = trappola_torre[re_nero_qu] & bitbordo_nero_torre;
  if (T)
    {
      int t = BSR (T);
      T = colonna_aperta_nera[t] & bitbordo_nero_pedone;
      if (T)
	{
	  t = BSR (T);
	  t >>= 3;
	  valu += puntos (10 * (t - 1), 0);
	}
    }

  if (attacco_re_bianco & attacco_re_nero)
    colpo_re_nero += COLPO_re;
  ch =
    (((re_sicura_moltiplicatore[colpo_re_nero >> 16] *
       (colpo_re_nero & 0xffff)) / re_sicura_dividendo) << 16) +
    PEDONE_VALU->nero_re_periglio;
  if (!bitbordo_bianca_donna)
    {
      ch >>= 16;
      ch *=
	POPCNT (bitbordo_bianco_torre | bitbordo_bianco_cavallo |
		bitbordo_bianco_alfiere);
      ch >>= 3;
      ch <<= 16;
    }
  valu += ch;

  if (attacco_buono_bianco >= 2)
    {
      valu += attacco_molteplice;
    }
  if (attacco_buono_nero >= 2)
    {
      valu -= attacco_molteplice;
    }

#define FINALE_donna ( (DINAMICO->pietras & 28) == 4)
#define FINAL_torre ( (DINAMICO->pietras & 28) == 8)

  if ((bitbordo_bianco_torre | bitbordo_bianca_donna) &
      crampo_colonna[COLONNA (re_nero_qu)])
    {
      valu += puntos (0, 5);
      if ((crampo_colonna[COLONNA (re_nero_qu)] &
	   (bitbordo_bianco_pedone | bitbordo_nero_pedone)) == 0)
	valu += puntos (5, 15);
    }
  if ((bitbordo_nero_torre | bitbordo_nera_donna) &
      crampo_colonna[COLONNA (re_bianco_qu)])
    {
      valu -= puntos (0, 5);
      if ((crampo_colonna[COLONNA (re_bianco_qu)] &
	   (bitbordo_nero_pedone | bitbordo_bianco_pedone)) == 0)
	valu -= puntos (5, 15);
    }

  U = PEDONE_VALU->bianco_pedone_libero_linea;
  while (U)
    {
      b = BSR (pila_colonna[BSF (U)] & bitbordo_bianco_pedone);
      bitLIBERO (0, U);
      tr = TRAVERSA (b);
      if (tr <= T3)
	continue;
      if (FINAL_torre)
	{
	  if (bitbordo_bianco_torre & colonna_aperta_bianca[b])
	    {
	      if (tr == T7)
		{
		  valu -= finale_torre_settima;
		}
	      else if (tr == T6)
		{
		  valu -= finale_torre_sessa;
		}
	    }
	  if (colonna_aperta_bianca[b] & bitbordo_bianco_re
	      && crampo_colonna[COLONNA (re_bianco_qu)] & bitbordo_nero_torre)
	    valu -= puntos (0, 1 << (tr - T2));
	}
      if (POSIZIONE->qu[b + 8] == 0)
	{
	  valu += pedone_libero_mobilita[tr];
	}
      if ((colonna_aperta_bianca[b] & bitbordo_occupato_bianco) == 0)
	{
	  valu += pedone_libero_mio_ok[tr];
	}
      if ((colonna_aperta_bianca[b] & bitbordo_occupato_nero) == 0)
	{
	  valu += pedone_libero_tuo_ok[tr];
	}
      if ((colonna_aperta_bianca[b] & (~DINAMICO->attacco_bianco) &
	   DINAMICO->attacco_nero) == 0)
	{
	  valu += pedone_libero_libero[tr];
	}
      if (FINALE_donna)
	{
	  if (tr == T7 && bitbordo_bianca_donna & colonna_aperta_bianca[b])
	    {
	      valu -= finale_donna_settima;
	    }
	  valu += traversa_finale_donna[tr];
	}
    }

  U = PEDONE_VALU->nero_pedone_libero_linea;
  while (U)
    while (U)
      {
	b = BSF (pila_colonna[BSF (U)] & bitbordo_nero_pedone);
	bitLIBERO (0, U);
	tr = TRAVERSA (b);
	if (tr >= T6)
	  continue;
	if (FINAL_torre)
	  {
	    if (bitbordo_nero_torre & colonna_aperta_nera[b])
	      {
		if (tr == T2)
		  {
		    valu += finale_torre_settima;
		  }
		else if (tr == T3)
		  {
		    valu += finale_torre_sessa;
		  }
	      }
	    if (colonna_aperta_nera[b] & bitbordo_nero_re
		&& crampo_colonna[COLONNA (re_nero_qu)] &
		bitbordo_bianco_torre)
	      valu += puntos (0, 1 << (T7 - tr));
	  }
	if (POSIZIONE->qu[b - 8] == 0)
	  {
	    valu -= pedone_libero_mobilita[7 - tr];
	  }
	if ((colonna_aperta_nera[b] & bitbordo_occupato_nero) == 0)
	  {
	    valu -= pedone_libero_mio_ok[7 - tr];
	  }
	if ((colonna_aperta_nera[b] & bitbordo_occupato_bianco) == 0)
	  {
	    valu -= pedone_libero_tuo_ok[7 - tr];
	  }
	if ((colonna_aperta_nera[b] & DINAMICO->
	     attacco_bianco & ~DINAMICO->attacco_nero) == 0)
	  {
	    valu -= pedone_libero_libero[7 - tr];
	  }
	if (FINALE_donna)
	  {
	    if (tr == T2 && bitbordo_nera_donna & colonna_aperta_nera[b])
	      {
		valu += finale_donna_settima;
	      }
	    valu -= traversa_finale_donna[7 - tr];
	  }
      }

  fase = DINAMICO->materiale & 0xff;
  finale = (sint16) (valu & 0xffff);
  apertura = (finale < 0) + (sint16) ((valu >> 16) & 0xffff);
  anti_fase = 32 - fase;
  valu = finale * anti_fase + apertura * fase;
  valu = valu / 32 + materiale_valu;
  valu = (valu * SEGNO) / 128;

  if (valu > 0)
    valu -= (PEDONE_VALU->bianco_patta_numero * MINIMO (valu, 100)) / 64;
  else
    valu += (PEDONE_VALU->nero_patta_numero * MINIMO (-valu, 100)) / 64;

#define matto_alfiere_cavallo (DINAMICO->pietras & 128)
#define LUNGI(i,j)\
 ( MASSIMO (colonna_distacco (i, j), traversa_distacco (i, j) ) )
#define VICINO(i,j)\
 ( MINIMO ( colonna_distacco (i, j), traversa_distacco (i, j) ) )

  if (matto_alfiere_cavallo)
    {
      if (valu > 0)
	{
	  if (bitbordo_bianco_chiaro)
	    valu -=
	      20 * MINIMO (LUNGI (A8, POSIZIONE->nero_re_quadretto),
			   LUNGI (H1, POSIZIONE->nero_re_quadretto)) +
	      10 * MINIMO (VICINO (A8, POSIZIONE->nero_re_quadretto),
			   VICINO (H1, POSIZIONE->nero_re_quadretto));
	  else
	    valu -=
	      20 * MINIMO (LUNGI (A1, POSIZIONE->nero_re_quadretto),
			   LUNGI (H8, POSIZIONE->nero_re_quadretto)) +
	      10 * MINIMO (VICINO (A1, POSIZIONE->nero_re_quadretto),
			   VICINO (H8, POSIZIONE->nero_re_quadretto));
	}
      else
	{
	  if (bitbordo_nero_chiaro)
	    valu +=
	      20 * MINIMO (LUNGI (A8, POSIZIONE->bianco_re_quadretto),
			   LUNGI (H1, POSIZIONE->bianco_re_quadretto)) +
	      10 * MINIMO (VICINO (A8, POSIZIONE->bianco_re_quadretto),
			   VICINO (H1, POSIZIONE->bianco_re_quadretto));
	  else
	    valu +=
	      20 * MINIMO (LUNGI (A1, POSIZIONE->bianco_re_quadretto),
			   LUNGI (H8, POSIZIONE->bianco_re_quadretto)) +
	      10 * MINIMO (VICINO (A1, POSIZIONE->bianco_re_quadretto),
			   VICINO (H8, POSIZIONE->bianco_re_quadretto));
	}
    }

  if (DINAMICO->mossa50 > 50)
    {
      valu *= (114 - DINAMICO->mossa50);
      valu /= 64;
    }

#define BIANCO_MINORE (DINAMICO->pietras & 32)
#define BIANCO_PEZZO (DINAMICO->pietras & 2)
#define NERO_MINORE (DINAMICO->pietras & 64)
#define NERO_PEZZO (DINAMICO->pietras & 1)

  if (valu > 0)
    {
      if (!POSIZIONE->bianco_en_mossa && !NERO_PEZZO
	  && (bitbordo_nero_re ^ bitbordo_nero_pedone) == bitbordo_occupato_nero
	  && !((bitbordo_nero_pedone >> 8) & ~POSIZIONE->occupato_nero_bianco)
	  && !(attaco_re[POSIZIONE->nero_re_quadretto] &
	       ~DINAMICO->attacco_bianco)
	  && !DINAMICO->nero_re_scacco)
	valu = 0;
      if (BIANCO_MINORE)
	{
	  if (bitbordo_bianco_cavallo)
	    {
	      if (bitbordo_bianco_pedone == quadretto_fisso[A7]
		  && (bitbordo_nero_re |
		      attaco_re[POSIZIONE->nero_re_quadretto]) &
		  quadretto_fisso[A8])
		valu = 0;
	      if (bitbordo_bianco_pedone == quadretto_fisso[H7]
		  && (bitbordo_nero_re |
		      attaco_re[POSIZIONE->nero_re_quadretto]) &
		  quadretto_fisso[H8])
		valu = 0;
	    }
	  else if (bitbordo_bianco_chiaro && !(bitbordo_bianco_pedone & nonH)
		   && (bitbordo_nero_re |
		       attaco_re[POSIZIONE->nero_re_quadretto]) &
		   quadretto_fisso[H8])
	    {
	      if (bitbordo_bianco_pedone & quadretto_fisso[H5]
		  && bitbordo_nero_pedone ==
		  (quadretto_fisso[G7] | quadretto_fisso[H6]));
	      else
		valu = 0;
	    }
	  else if (bitbordo_bianco_scuro && !(bitbordo_bianco_pedone & nonA)
		   && (bitbordo_nero_re |
		       attaco_re[POSIZIONE->nero_re_quadretto]) &
		   quadretto_fisso[A8])
	    {
	      if (bitbordo_bianco_pedone & quadretto_fisso[A5]
		  && bitbordo_nero_pedone ==
		  (quadretto_fisso[B7] | quadretto_fisso[A6]));
	      else
		valu = 0;
	    }
	  if (!bitbordo_bianco_pedone)
	    valu = 0;
	}
    }

  else
    {
      if (POSIZIONE->bianco_en_mossa && !BIANCO_PEZZO
	  && (bitbordo_bianco_re ^ bitbordo_bianco_pedone) == bitbordo_occupato_bianco
	  && !((bitbordo_bianco_pedone << 8) & ~POSIZIONE->occupato_nero_bianco)
	  && !(attaco_re[POSIZIONE->bianco_re_quadretto] &
	       ~DINAMICO->attacco_nero)
	  && !DINAMICO->bianco_re_scacco)
	valu = 0;

      if (NERO_MINORE)
	{
	  if (bitbordo_nero_cavallo)
	    {
	      if (bitbordo_nero_pedone == quadretto_fisso[A2]
		  && (bitbordo_bianco_re |
		      attaco_re[POSIZIONE->bianco_re_quadretto]) &
		  quadretto_fisso[A1])
		valu = 0;
	      if (bitbordo_nero_pedone == quadretto_fisso[H2]
		  && (bitbordo_bianco_re |
		      attaco_re[POSIZIONE->bianco_re_quadretto]) &
		  quadretto_fisso[H1])
		valu = 0;
	    }
	  else if (bitbordo_nero_scuro && !(bitbordo_nero_pedone & nonH)
		   && (bitbordo_bianco_re |
		       attaco_re[POSIZIONE->bianco_re_quadretto]) &
		   quadretto_fisso[H1])
	    {
	      if (bitbordo_nero_pedone & quadretto_fisso[H4]
		  && bitbordo_bianco_pedone ==
		  (quadretto_fisso[G2] | quadretto_fisso[H3]));
	      else
		valu = 0;
	    }
	  else if (bitbordo_nero_chiaro && !(bitbordo_nero_pedone & nonA)
		   && (bitbordo_bianco_re |
		       attaco_re[POSIZIONE->bianco_re_quadretto]) &
		   quadretto_fisso[A1])
	    {
	      if (bitbordo_nero_pedone & quadretto_fisso[A4]
		  && bitbordo_bianco_pedone ==
		  (quadretto_fisso[B2] | quadretto_fisso[A3]));
	      else
		valu = 0;
	    }
	  if (!bitbordo_nero_pedone)
	    valu = 0;
	}
    }

  DINAMICO->valu = POSIZIONE->bianco_en_mossa ? valu : -valu;
  DINAMICO->valu_posizionale = valu - materiale_valu;
  DINAMICO->pigro = 0;
  valu_zobrist[DINAMICO->Zobrist & valu_zobrist_celare] =
    (DINAMICO->Zobrist & 0xffffffffffff0000) | (DINAMICO->valu & 0xffff);
  if (mossa && !(DINAMICO - 1)->pigro)
    POSIZIONE->bianco_en_mossa ?
      truffa_bianca (POSIZIONE, mossa) : truffa_nera (POSIZIONE, mossa);

}
