#include "RobboLito.h"
#include <string.h>

char * mossa_notazione (uint32 mossa, char *M)
{
  int di, ai, pr;
  char c[8] = "0123nbrq";
  di = DI (mossa);
  ai = AI (mossa);
  if (mossa == MOSSA_NON)
    {
      M[0] = 'N';
      M[1] = 'U';
      M[2] = M[3] = 'L';
      M[4] = 0;
      return M;
    }
  sprintf (M, "%c%c%c%c", 'a' + (di & 7), '1' + ((di >> 3) & 7),
	   'a' + (ai & 7), '1' + ((ai >> 3) & 7));
  if (mossa_promozione (mossa))
    {
      pr = (mossa & pietra_celare) >> 12;
      sprintf (M + 4, "%c", c[pr]);
    }
  return M;
}

static void
MALATO (char *x)
{
  ERRORE ("errore posizionale: %s\n", x);
}

#include "valore.i"

#define PIZZICA (0x74d3c012a8bf965e)
void
bitbordo_inizializzazione (tipo_posizione *POSIZIONE)
{
  int i, b, pezzo;
  uint64 O;
  bordo_legittimo = FALSO;
  for (i = 0; i < 16; i++)
    POSIZIONE->bitbordo[i] = 0;
  DINAMICO->Zobrist = DINAMICO->ZobristPedone = 0;
  DINAMICO->materiale = 0;
  DINAMICO->statico = 0;
  for (i = A1; i <= H8; i++)
    {
      if ((pezzo = POSIZIONE->qu[i]))
	{
	  DINAMICO->statico += statico_valu[pezzo][i];
	  DINAMICO->Zobrist ^= ZOBRIST[pezzo][i];
	  if (pezzo == conto_pedone_bianco || pezzo == conto_pedone_nero)
	    DINAMICO->ZobristPedone ^= ZOBRIST[pezzo][i];
	  DINAMICO->materiale += valu_materiale[pezzo];
	  bitFISSO (i, POSIZIONE->bitbordo[POSIZIONE->qu[i]]);
	}
    }
  bitbordo_occupato_bianco =
    bitbordo_bianco_re | bitbordo_bianca_donna | bitbordo_bianco_torre |
    bitbordo_bianco_alfiere | bitbordo_bianco_cavallo |
    bitbordo_bianco_pedone;
  bitbordo_occupato_nero =
    bitbordo_nero_re | bitbordo_nera_donna | bitbordo_nero_torre |
    bitbordo_nero_alfiere | bitbordo_nero_cavallo | bitbordo_nero_pedone;
  POSIZIONE->occupato_nero_bianco =
    bitbordo_occupato_bianco | bitbordo_occupato_nero;
  POSIZIONE->occupato_sinistro90 = POSIZIONE->occupato_sinistro45 =
    POSIZIONE->occupato_retto45 = 0;
  O = POSIZIONE->occupato_nero_bianco;
  if (POPCNT (bitbordo_bianca_donna) > 1
      || POPCNT (bitbordo_nera_donna) > 1
      || POPCNT (bitbordo_bianco_torre) > 2
      || POPCNT (bitbordo_nero_torre) > 2
      || POPCNT (bitbordo_bianco_chiaro) > 1
      || POPCNT (bitbordo_nero_chiaro) > 1
      || POPCNT (bitbordo_bianco_cavallo) > 2
      || POPCNT (bitbordo_nero_cavallo) > 2
      || POPCNT (bitbordo_bianco_scuro) > 1
      || POPCNT (bitbordo_nero_scuro) > 1)
    DINAMICO->materiale |= 0x80000000;
  if (POPCNT (bitbordo_bianco_re) != 1)
    MALATO ("re bianco != 1");
  if (POPCNT (bitbordo_nero_re) != 1)
    MALATO ("re nero != 1");
  if (POPCNT (bitbordo_bianca_donna) > 9)
    MALATO ("donna bianca > 9");
  if (POPCNT (bitbordo_nera_donna) > 9)
    MALATO ("donna nera > 9");
  if (POPCNT (bitbordo_bianco_torre) > 10)
    MALATO ("torre bianco > 10");
  if (POPCNT (bitbordo_nero_torre) > 10)
    MALATO ("torre nero > 10");
  if (POPCNT (bitbordo_bianco_chiaro) > 9)
    MALATO ("chiaro bianco > 9");
  if (POPCNT (bitbordo_nero_chiaro) > 9)
    MALATO ("chiaro nero > 9");
  if (POPCNT (bitbordo_bianco_scuro) > 9)
    MALATO ("scuro bianco > 9");
  if (POPCNT (bitbordo_nero_scuro) > 9)
    MALATO ("scuro nero > 9");
  if (POPCNT (bitbordo_bianco_chiaro | bitbordo_bianco_scuro) > 10)
    MALATO ("alfiere bianco > 10");
  if (POPCNT (bitbordo_nero_chiaro | bitbordo_nero_scuro) > 10)
    MALATO ("alfiere nero > 10");
  if (POPCNT (bitbordo_bianco_cavallo) > 10)
    MALATO ("cavallo bianco > 10");
  if (POPCNT (bitbordo_nero_cavallo) > 10)
    MALATO ("cavallo nero > 10");
  if (POPCNT (bitbordo_bianco_pedone) > 8)
    MALATO ("pedone bianco > 8");
  if (POPCNT (bitbordo_nero_pedone) > 8)
    MALATO ("pedone nero > 8");
  if (POPCNT (bitbordo_occupato_bianco) > 16)
    MALATO ("pezzo bianco > 16");
  if (POPCNT (bitbordo_occupato_nero) > 16)
    MALATO ("pezzo nero > 16");
  if ((bitbordo_bianco_pedone | bitbordo_nero_pedone) &
      (TRAVERSA1 | TRAVERSA8))
    MALATO ("pedone traversa prima o ottava");
  while (O)
    {
      b = BSF (O);
      bitLIBERO (b, O);
      bitFISSO (sinistro90[b], POSIZIONE->occupato_sinistro90);
      bitFISSO (sinistro45[b], POSIZIONE->occupato_sinistro45);
      bitFISSO (retto45[b], POSIZIONE->occupato_retto45);
    }
  POSIZIONE->bianco_re_quadretto = BSF (bitbordo_bianco_re);
  POSIZIONE->nero_re_quadretto = BSF (bitbordo_nero_re);
  if ((arrocco_bianco_oo
       && (POSIZIONE->bianco_re_quadretto != E1
	   || !(bitbordo_bianco_torre & quadretto_fisso[H1])))
      || (arrocco_bianco_ooo
	  && (POSIZIONE->bianco_re_quadretto != E1
	      || !(bitbordo_bianco_torre & quadretto_fisso[A1])))
      || (arrocco_nero_oo
	  && (POSIZIONE->nero_re_quadretto != E8
	      || !(bitbordo_nero_torre & quadretto_fisso[H8])))
      || (arrocco_nero_ooo
	  && (POSIZIONE->nero_re_quadretto != E8
	      || !(bitbordo_nero_torre & quadretto_fisso[A8]))))
    MALATO ("arrocco illegale");
  DINAMICO->Zobrist ^= zobrist_oo[DINAMICO->arrocco];
  if (DINAMICO->en_passant)
    DINAMICO->Zobrist ^= zobrist_ep[DINAMICO->en_passant & 7];
  DINAMICO->ZobristPedone ^=
    zobrist_oo[DINAMICO-> arrocco] ^ PIZZICA ^
    ZOBRIST[conto_re_bianco][POSIZIONE->bianco_re_quadretto] ^
    ZOBRIST[conto_re_nero][POSIZIONE->nero_re_quadretto];
  if (POSIZIONE->bianco_en_mossa)
    DINAMICO->Zobrist ^= zobrist_mossa_bianca;
  chiamata_valu_piena (0);
  if (POSIZIONE->bianco_en_mossa
      && DINAMICO->attacco_bianco & bitbordo_nero_re)
    MALATO ("bianco cattura re");
  if (!POSIZIONE->bianco_en_mossa
      && DINAMICO->attacco_nero & bitbordo_bianco_re)
    MALATO ("nero catture re");
  bordo_legittimo = VERO;
}

#ifdef WINDOWS
#include <time.h>
#ifdef NO_INPUT_PIPES
#include <conio.h>
logico interrogativo_ingresso ()
{
	return _kbhit();
}
#else
logico interrogativo_ingresso ()
{
  static int init = 0, is_pipe;
  static HANDLE stdin_h;
  DWORD val;

  if (!init)
    {
      init = 1;
      stdin_h = GetStdHandle (STD_INPUT_HANDLE);
      is_pipe = !GetConsoleMode (stdin_h, &val);
      if (!is_pipe)
        {
          SetConsoleMode (stdin_h, val & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
          FlushConsoleInputBuffer (stdin_h);
        }
    }
  if (is_pipe)
    {
      if (!PeekNamedPipe (stdin_h, NULL, 0, NULL, &val, NULL))
        return 1;
      return val > 0;
    }
  else
    {
      GetNumberOfConsoleInputEvents (stdin_h, &val);
      return val > 1;
    }
  return 0;
}
#endif

uint64 orologio()
{
  return (GetTickCount () * (uint64)1000);
}

int check_num_CPU()
{
	int num_CPU;
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	num_CPU = sysinfo.dwNumberOfProcessors;
	return num_CPU;
}

#else /* !WINDOWS */
#include <unistd.h>
logico
interrogativo_ingresso ()
{
  int v;
  fd_set s[1];
  struct timeval tv[1];
  FD_ZERO (s);
  FD_SET (STDIN_FILENO, s);
  tv->tv_sec = 0;
  tv->tv_usec = 0;
  v = select (STDIN_FILENO + 1, s, NULL, NULL, tv);
  return (v > 0);
}

#include <sys/time.h>
uint64
orologio ()
{
  uint64 x;
  struct timeval tv;
  gettimeofday (&tv, NULL);
  x = tv.tv_sec;
  x *= 1000000;
  x += tv.tv_usec;
  return x;
}
#endif /* ifdef WINDOWS */

/****************************************************************/

#include <stdarg.h>

void
TRASMISSIONE (char *fmt, ...)
{
  va_list valu;
  va_start (valu, fmt);
  vfprintf (stdout, fmt, valu);
  va_end (valu);
  fflush (stdout);
}

void
ERRORE (char *fmt, ...)
{
  va_list valu;
  va_start (valu, fmt);
  va_end (valu);
  //  stdout = stderr; // WINDOWS
  fprintf (stdout, "*** ERRORE ***\n");
  vfprintf (stdout, fmt, valu);
  exit (1);
}

void
ERRORE_fen (char *fmt, ...)
{
  va_list valu;
  va_start (valu, fmt);
  va_end (valu);
  //  stdout = stderr; // WINDOWS
  fprintf (stdout, "*** FEN ERR ***\n");
  vfprintf (stdout, fmt, valu);
  exit (1);
}

int abisso_precedente, radice_rapido;
void
partita_nuova (tipo_posizione *POSIZIONE, logico totale)
{
  int i;
  for (i = A1; i <= H8; i++)
    POSIZIONE->qu[i] = 0;
  memset (dinamico_inizio, 0, 256 * sizeof (tipo_dinamico));
  DINAMICO = dinamico_inizio;
  POSIZIONE->bianco_en_mossa = VERO;
  POSIZIONE->height = 0;
  DINAMICO->arrocco = 0x0f;
  DINAMICO->en_passant = 0;
  DINAMICO->mossa50 = 0;
  for (i = A2; i <= H2; i++)
    POSIZIONE->qu[i] = conto_pedone_bianco;
  for (i = A7; i <= H7; i++)
    POSIZIONE->qu[i] = conto_pedone_nero;
  POSIZIONE->qu[D1] = conto_donna_bianca;
  POSIZIONE->qu[D8] = conto_donna_nera;
  POSIZIONE->qu[E1] = conto_re_bianco;
  POSIZIONE->qu[E8] = conto_re_nero;
  POSIZIONE->qu[A1] = POSIZIONE->qu[H1] = conto_torre_bianco;
  POSIZIONE->qu[A8] = POSIZIONE->qu[H8] = conto_torre_nero;
  POSIZIONE->qu[B1] = POSIZIONE->qu[G1] = conto_cavallo_bianco;
  POSIZIONE->qu[B8] = POSIZIONE->qu[G8] = conto_cavallo_nero;
  POSIZIONE->qu[C1] = conto_scuro_bianco;
  POSIZIONE->qu[F1] = conto_chiaro_bianco;
  POSIZIONE->qu[C8] = conto_chiaro_nero;
  POSIZIONE->qu[F8] = conto_scuro_nero;
  abisso_precedente = 1000;
  radice_rapido = FALSO;
  PARTITA_NUOVA = VERO;
  mucchio_altezzo = 0;
  bitbordo_inizializzazione (POSIZIONE);
  if (!totale)
    return;
  zobrist_libero ();
  valu_zobrist_libero ();
  storia_reset ();
  guadagno_reset ();
  pedone_reset ();
}
