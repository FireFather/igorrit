#include "RobboLito.h"
#include <string.h>
#include "sorvegliare.h"

static void
uci ()
{
  TRASMISSIONE ("id name %s %s\n", EPONYM, VERSION);
  TRASMISSIONE ("id author Yakov Petrovich Golyadkin, ");
  TRASMISSIONE ("Igor Igorovich Igoronov, Roberto Pescatore, ");
  TRASMISSIONE ("Yusuf Ralf Weisskopf, ");
  TRASMISSIONE ("Ivan Skavinsky Skavar plus Decembrists (all)\n");
  TRASMISSIONE ("id copyright Yakov Petrovich Golyadkin, "
		"92th plus 93th year from Revolution, PUBLICDOMAIN (workers)\n");
  TRASMISSIONE ("id dedication To Vladimir Ilyich\n");
  TRASMISSIONE ("option name Hash type spin default 128 min 4 max 16384\n");
  TRASMISSIONE ("option name Pawn Hash type spin min 1 max 1024 default 2\n");
#ifdef YUSUF_MULTICORE
  TRASMISSIONE ("option name MultiProcessing type check default true\n");
  TRASMISSIONE ("option name NumCPUs type spin min 1 max %d default %d\n", MAX_CPUS, CPUS_SIZE);
#endif
  TRASMISSIONE ("option name Ponder type check default false\n");
  TRASMISSIONE ("option name Move on Ponderhit type combo var Never var Sometimes var Always default Sometimes\n");
  TRASMISSIONE ("uciok\n");
}

static void
readyok ()
{
  TRASMISSIONE ("readyok\n");
}

static void
quit ()
{
  exit (0);
}

static void parse_options(char *string)
{
	char *name, *value;
	int size;
	
	name = strstr(string, "name ");
	value = strstr(string, "value ");

	if (name == NULL || value == NULL || name >= value)
		return;
	value[-1] = 0;
	name += 5;
	value += 6;
	
	if (!strcmp(name, "Ponder"))
	{
		if (!strcmp(value, "true") || !strcmp(value, "yes") || !strcmp(value, "1"))
			PONDER = TRUE;
		else if (!strcmp(value, "false") || !strcmp(value, "no") || !strcmp(value, "0"))
			PONDER = FALSE;
	}
	if (!strcmp(name, "Move on Ponderhit"))
	{
		if (!strcmp(value, "Never"))
			MPH = 1;
		else if (!strcmp(value, "Sometimes"))
			MPH = 2;
		else if (!strcmp(value, "Always"))
			MPH = 3;
	}
#ifdef YUSUF_MULTICORE
	if (!strcmp (name, "MultiProcessing"))
	{
		if (!strcmp(value, "true") || !strcmp(value, "yes") || !strcmp(value, "1"))
			TITANIC_MODE = VERO;
		else if (!strcmp(value, "false") || !strcmp(value, "no") || !strcmp(value, "0"))
			TITANIC_MODE = FALSO;
	}

	if (!strcmp (name, "NumCPUs"))
	{
		size = _atoi64(value);
		if (size <= 1)
			size = 1;
		else if (size > MAX_CPUS)
			size = MAX_CPUS;
		else if (size > 64)
			size = 64;
		CPUS_SIZE = size;
	}
#endif
	if (!strcmp (name, "Pawn Hash"))
	{
		size = _atoi64(value);
		size = pedone_zobrist_inizializzazione(size);
	}

	if (!strcmp(name, "Hash"))
	{
		size = _atoi64(value);
		if (size >= 4 && size <= 16384)
			size = inizializzazione_zobrist(size);
	}
}

static void
ingresso_analizi (tipo_posizione *POSIZIONE, char *I)
{
	if (!memcmp(I, "setoption", 9))
	{
		cerca_alt(0);
		parse_options(I + 10);
	}

  if (!strcmp(I, "ponderhit"))
	{
		PONDERING = FALSO;
		if (MPH == 3)
			cerca_alt(5);
		else if (MPH == 1)
			OROLOGIO = orologio() - 10000;
	}
  if (!strcmp (I, "uci"))
    uci ();
  if (!strcmp (I, "quit"))
    quit ();
  if (!strcmp (I, "stop"))
    cerca_alt (0);
  if (!strcmp (I, "isready"))
    readyok ();
  if (!strcmp (I, "ucinewgame"))
    {
      cerca_alt (0);
      partita_nuova (POSIZIONE, VERO);
    }
  if (!memcmp (I, "position", 8))
    posizionale_inizializzazione (POSIZIONE, I + 9);
  if (!memcmp (I, "go", 2))
    {
      cerca_inizializzazione (POSIZIONE, I);
      if (bordo_legittimo)
	cerca (POSIZIONE);
    }
  if (salto_ok)
    return;
}

void
ingresso (tipo_posizione *POSIZIONE)
{
  char I[65536];
  fgets (I, 65536, stdin);
  I[strlen (I) - 1] = 0;
  ingresso_analizi (POSIZIONE, I);
}
