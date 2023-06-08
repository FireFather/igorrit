#include "RobboLito.h"

static void
nero_bianco ()
{
  int qu;
  for (qu = A1; qu <= H8; qu++)
    {
      statico_valu[conto_pedone_nero][qu] =
	-statico_valu[conto_pedone_bianco][H8 - qu];
      statico_valu[conto_cavallo_nero][qu] =
	-statico_valu[conto_cavallo_bianco][H8 - qu];
      statico_valu[conto_chiaro_nero][qu] =
	-statico_valu[conto_chiaro_bianco][H8 - qu];
      statico_valu[conto_scuro_nero][qu] =
	-statico_valu[conto_scuro_bianco][H8 - qu];
      statico_valu[conto_torre_nero][qu] =
	-statico_valu[conto_torre_bianco][H8 - qu];
      statico_valu[conto_donna_nera][qu] =
	-statico_valu[conto_donna_bianca][H8 - qu];
      statico_valu[conto_re_nero][qu] =
	-statico_valu[conto_re_bianco][H8 - qu];
    }
}

static int
apertura_pedone (int qu)
{
  int C[8] = { -20, -8, -2, 5, 5, -2, -8, -20 };
  int T[8] = { 0, -3, -2, -1, 1, 2, 3, 0 };
  int co = COLONNA (qu), tr = TRAVERSA (qu);
  if (tr == T1 || tr == T8)
    return 0;
  return C[co] + T[tr];
}
static int
finale_pedone (int qu)
{
  int C[8] = { -4, -6, -8, -10, -10, -8, -6, -4 };
  int T[8] = { 0, -3, -3, -2, -1, 0, 2, 0 };
  int co = COLONNA (qu), tr = TRAVERSA (qu);
  if (tr == T1 || tr == T8)
    return 0;
  return C[co] + T[tr];
}

static int
apertura_cavallo (int qu)
{
  int T[8] = { -32, -10, 6, 15, 21, 19, 10, -11 };
  int C[8] = { -26, -10, 1, 5, 5, 1, -10, -26 };
  int tr = TRAVERSA (qu), co = COLONNA (qu);
  return (qu == A8 || qu == H8) ? (-120) : (T[tr] + C[co]);
}
static int
finale_cavallo (int qu)
{
  int V[8] = { 2, 1, 0, -1, -2, -4, -7, -10 };
  int T[8] = { -10, -5, -2, 1, 3, 5, 2, -3 };
  int C[8] = { -4, -1, 2, 4, 4, 2, -1, -4 };
  int tr = TRAVERSA (qu), co = COLONNA (qu);
  int d = ASSOLUTO (co - tr), e = ASSOLUTO (co + tr - 7);
  return V[d] + V[e] + T[tr] + C[co];
}

static int
apertura_alfiere (int qu)
{
  int V[8] = { 10, 5, 1, -3, -5, -7, -8, -12 };
  int W[8] = { -5, 0, 0, 0, 0, 0, 0, 0 };
  int tr = TRAVERSA (qu), co = COLONNA (qu);
  int d = ASSOLUTO (co - tr), e = ASSOLUTO (co + tr - 7);
  return V[d] + V[e] + W[tr];
}
static int
finale_alfiere (int qu)
{
  int V[8] = { 3, 2, 0, 0, -2, -2, -3, -3 };
  int tr = TRAVERSA (qu), co = COLONNA (qu);
  int d = ASSOLUTO (co - tr), e = ASSOLUTO (co + tr - 7);
  return V[d] + V[e];
}

static int
apertura_torre (int qu)
{
  int C[8] = { -4, 0, 4, 8, 8, 4, 0, -4 };
  return C[COLONNA (qu)];
}
static int
finale_torre (int qu)
{
  int T[8] = { 0, 0, 0, 0, 1, 1, 1, -2 };
  return T[TRAVERSA (qu)];
}

static int
apertura_donna (int qu)
{
  int V[8] = { 3, 2, 1, 0, -2, -4, -7, -10 };
  int W[8] = { -2, 0, 1, 2, 2, 1, 0, -2 };
  int tr = TRAVERSA (qu), co = COLONNA (qu);
  int d = ASSOLUTO (co - tr), e = ASSOLUTO (co + tr - 7);
  return V[d] + V[e] + W[tr] + W[co] - 5 * (tr == T1);
}
static int
finale_donna (int qu)
{
  int V[8] = { 1, 0, -1, -3, -4, -6, -8, -12 };
  int W[8] = { -2, 0, 1, 2, 2, 1, 0, -2 };
  int tr = TRAVERSA (qu), co = COLONNA (qu);
  int d = ASSOLUTO (co - tr), e = ASSOLUTO (co + tr - 7);
  return V[d] + V[e] + W[tr] + W[co];
}

static int
apertura_re (int qu)
{
  int T[8] = { 4, 1, -2, -5, -10, -15, -25, -35 };
  int C[8] = { 40, 45, 15, -5, -5, 15, 45, 40 };
  int tr = TRAVERSA (qu), co = COLONNA (qu);
  return T[tr] + C[co];
}
static int
finale_re (int qu)
{
  int V[8] = { 2, 0, -2, -5, -8, -12, -20, -30 };
  int T[8] = { -30, -5, 0, 5, 10, 5, 0, -10 };
  int C[8] = { -15, 0, 10, 15, 15, 10, 0, -15 };
  int tr = TRAVERSA (qu), co = COLONNA (qu);
  int d = ASSOLUTO (co - tr), e = ASSOLUTO (co + tr - 7);
  return V[d] + V[e] + T[tr] + C[co];
}

#define assorbo(x,y) ( ( ( x ) << 16 ) + ( y ) )

void
statico_inizializzazione ()
{
  int qu;
  for (qu = A1; qu <= H8; qu++)
    statico_valu[conto_pedone_bianco][qu] =
      assorbo (apertura_pedone (qu), finale_pedone (qu));
  for (qu = A1; qu <= H8; qu++)
    statico_valu[conto_cavallo_bianco][qu] =
      assorbo (apertura_cavallo (qu), finale_cavallo (qu));
  for (qu = A1; qu <= H8; qu++)
    statico_valu[conto_chiaro_bianco][qu] =
      assorbo (apertura_alfiere (qu), finale_alfiere (qu));
  for (qu = A1; qu <= H8; qu++)
    statico_valu[conto_scuro_bianco][qu] =
      assorbo (apertura_alfiere (qu), finale_alfiere (qu));
  for (qu = A1; qu <= H8; qu++)
    statico_valu[conto_torre_bianco][qu] =
      assorbo (apertura_torre (qu), finale_torre (qu));
  for (qu = A1; qu <= H8; qu++)
    statico_valu[conto_donna_bianca][qu] =
      assorbo (apertura_donna (qu), finale_donna (qu));
  for (qu = A1; qu <= H8; qu++)
    statico_valu[conto_re_bianco][qu] =
      assorbo (apertura_re (qu), finale_re (qu));
  nero_bianco ();
}
