
#include "RobboLito.h"

#define donna_finale 1
#define torre_finale 2
#define alfiere_opposto_finale 3
#define alfiere_finale 4
#define cavallo_finale 5
#define alfiere_cavallo_finale 6
#define pedone_finale 7
#define bianco_minore_pietra 8
#define nero_minore_pietra 16
#define alfiere_cavallo_matto 32

static int
pietras_inizializzazione (int bianco_pedone, int bianco_cavallo,
			  int bianco_alfiere, int bianco_chiaro,
			  int bianco_scuro, int bianco_torre, int bianco_donna,
			  int nero_pedone, int nero_cavallo, int nero_alfiere,
			  int nero_chiaro, int nero_scuro,
			  int nero_torre, int nero_donna)
{
  uint8 PIETRAS =
    ((bianco_cavallo || bianco_alfiere || bianco_donna || bianco_torre) << 1) |
    ((nero_cavallo || nero_alfiere || nero_donna || nero_torre) << 0);

#if 1
  if (!bianco_donna && !bianco_torre && bianco_alfiere + bianco_cavallo == 1
      && bianco_pedone <= 4 && nero_pedone <= 4)
    PIETRAS &= 1;
  if (!nero_donna && !nero_torre && nero_alfiere + nero_cavallo == 1
      && bianco_pedone <= 4 && nero_pedone <= 4)
    PIETRAS &= 2;
#endif

  if (bianco_donna == 1 && nero_donna == 1 && !bianco_torre && !nero_torre
      && !bianco_alfiere && !nero_alfiere && !bianco_cavallo && !nero_cavallo)
    PIETRAS |= donna_finale << 2;
  if (bianco_torre == 1 && nero_torre == 1 && !bianco_donna && !nero_donna
      && !bianco_alfiere && !nero_alfiere && !bianco_cavallo && !nero_cavallo)
    PIETRAS |= torre_finale << 2;
  if (bianco_alfiere == 1 && nero_alfiere == 1 && !bianco_donna && !nero_donna
      && !bianco_torre && !nero_torre && !bianco_cavallo && !nero_cavallo)
    {
      if ((bianco_chiaro == 1 && nero_scuro == 1)
	  || (bianco_scuro == 1 && nero_chiaro == 1))
	PIETRAS |= alfiere_finale << 2;
      else
	PIETRAS |= alfiere_opposto_finale << 2;
      PIETRAS |= (bianco_minore_pietra | nero_minore_pietra) << 2;
    }
  if (bianco_cavallo == 1 && nero_cavallo == 1 && !bianco_donna && !nero_donna
      && !bianco_torre && !nero_torre && !bianco_alfiere && !nero_alfiere)
    PIETRAS |= cavallo_finale << 2;
  if (bianco_cavallo == 1 && nero_alfiere == 1 && !bianco_donna && !nero_donna
      && !bianco_torre && !nero_torre && !bianco_alfiere && !nero_cavallo)
    PIETRAS |= alfiere_cavallo_finale << 2;
  if (bianco_alfiere == 1 && nero_cavallo == 1 && !bianco_donna && !nero_donna
      && !bianco_torre && !nero_torre && !nero_alfiere && !bianco_cavallo)
    PIETRAS |= alfiere_cavallo_finale << 2;
  if (bianco_alfiere == 1 && !bianco_donna && !bianco_torre
      && !bianco_cavallo)
    PIETRAS |= bianco_minore_pietra << 2;
  if (nero_alfiere == 1 && !nero_donna && !nero_torre && !nero_cavallo)
    PIETRAS |= nero_minore_pietra << 2;
  if (bianco_cavallo == 1 && !bianco_donna && !bianco_torre
      && !bianco_alfiere)
    PIETRAS |= bianco_minore_pietra << 2;
  if (nero_cavallo == 1 && !nero_donna && !nero_torre && !nero_alfiere)
    PIETRAS |= nero_minore_pietra << 2;
  if (!bianco_cavallo && !bianco_alfiere && !bianco_torre && !bianco_donna
      && !nero_cavallo && !nero_alfiere && !nero_donna && !nero_donna
      && bianco_pedone + nero_pedone == 1)
    PIETRAS |= pedone_finale << 2;
  if (bianco_cavallo == 1 && bianco_alfiere == 1 && !bianco_torre
      && !bianco_donna && !bianco_pedone && !nero_donna && !nero_torre
      && !nero_alfiere && !nero_cavallo && !nero_pedone)
    PIETRAS |= alfiere_cavallo_matto << 2;
  if (nero_cavallo == 1 && nero_alfiere == 1 && !nero_torre && !nero_donna
      && !nero_pedone && !bianco_donna && !bianco_torre && !bianco_alfiere
      && !bianco_cavallo && !bianco_pedone)
    PIETRAS |= alfiere_cavallo_matto << 2;
  return PIETRAS;
}

static int
sengo_inizializzazione (int bianco_pedone, int bianco_cavallo,
			int bianco_alfiere, int bianco_chiaro,
			int bianco_scuro, int bianco_torre, int bianco_donna,
			int nero_pedone, int nero_cavallo, int nero_alfiere,
			int nero_chiaro, int nero_scuro,
			int nero_torre, int nero_donna)
{
  int segno = 0x80;
  if (bianco_cavallo == 0 && nero_cavallo == 0 && bianco_alfiere == 0
      && nero_alfiere == 0 && bianco_torre == 0 && nero_torre == 0
      && bianco_donna == 1 && nero_donna == 1)
    segno = 0x70 + MASSIMO (bianco_pedone, nero_pedone);
  if (bianco_cavallo == 0 && nero_cavallo == 0 && bianco_alfiere == 0
      && nero_alfiere == 0 && bianco_donna == 0 && nero_donna == 0
      && bianco_torre == 1 && nero_torre == 1)
    segno = 0x60 + 2 * MASSIMO (bianco_pedone, nero_pedone);
  if (bianco_cavallo == 0 && nero_cavallo == 0 && bianco_torre == 0
      && nero_torre == 0 && bianco_donna == 0 && nero_donna == 0
      && bianco_alfiere == 1 && nero_alfiere == 1)
    {
      if ((bianco_chiaro == 1 && bianco_scuro == 0 && nero_chiaro == 0
	   && nero_scuro == 1) || (bianco_chiaro == 0 && bianco_scuro == 1
				   && nero_chiaro == 1 && nero_scuro == 0))
	segno = 0x30 + 4 * MASSIMO (bianco_pedone, nero_pedone);
      else
	segno = 0x78 + 2 * MASSIMO (bianco_pedone, nero_pedone);
    }
  if (bianco_cavallo == 1 && nero_cavallo == 1 && bianco_torre == 0
      && nero_torre == 0 && bianco_donna == 0 && nero_donna == 0
      && bianco_alfiere == 0 && nero_alfiere == 0)
    segno = 0x80 + MASSIMO (bianco_pedone, nero_pedone);
  if (bianco_cavallo == 0 && nero_cavallo == 0 && bianco_torre == 0
      && nero_torre == 0 && bianco_donna == 0 && nero_donna == 0
      && bianco_alfiere == 0 && nero_alfiere == 0)
    segno = 0xc0 - 8 * MASSIMO (bianco_pedone, nero_pedone);
  if (bianco_cavallo == 0 && nero_cavallo == 0 && bianco_alfiere == 1
      && nero_alfiere == 1 && bianco_donna == 0 && nero_donna == 0
      && bianco_torre == 1 && nero_torre == 1)
    {
      if ((bianco_chiaro == 1 && bianco_scuro == 0 && nero_chiaro == 0
	   && nero_scuro == 1) || (bianco_chiaro == 0 && bianco_scuro == 1
				   && nero_chiaro == 1 && nero_scuro == 0))
	segno = 0x70 + MASSIMO (bianco_pedone, nero_pedone);
    }
  return segno;
}

static int
carico_bianco (int bianco_pedone, int bianco_cavallo, int bianco_alfiere,
	       int bianco_chiaro, int bianco_scuro,
	       int bianco_torre, int bianco_donna,
	       int nero_pedone, int nero_cavallo,
	       int nero_alfiere, int nero_chiaro, int nero_scuro,
	       int nero_torre, int nero_donna)
{
  int bianco_minore, nero_minore, bianco_fase, nero_fase, bianco_carico,
    bianco_valu, nero_valu;
  bianco_minore = bianco_alfiere + bianco_cavallo;
  nero_minore = nero_alfiere + nero_cavallo;
  bianco_fase = bianco_minore + 2 * bianco_torre + 4 * bianco_donna;
  nero_fase = nero_minore + 2 * nero_torre + 4 * nero_donna;
  bianco_valu =
    3 * (bianco_alfiere + bianco_cavallo) + 5 * bianco_torre +
    9 * bianco_donna;
  nero_valu =
    3 * (nero_alfiere + nero_cavallo) + 5 * nero_torre + 9 * nero_donna;
  bianco_carico = 10;
  if (!bianco_pedone)
    {
      if (bianco_fase == 1)
	bianco_carico = 0;
      if (bianco_fase == 2)
	{
	  if (nero_fase == 0)
	    {
	      if (bianco_cavallo == 2)
		{
		  if (nero_pedone >= 1)
		    bianco_carico = 3;
		  else
		    bianco_carico = 0;
		}
	    }
	  if (nero_fase == 1)
	    {
	      bianco_carico = 1;
	      if (bianco_alfiere == 2 && nero_cavallo == 1)
		bianco_carico = 8;
	      if (bianco_torre == 1 && nero_cavallo == 1)
		bianco_carico = 2;
	    }
	  if (nero_fase == 2)
	    bianco_carico = 1;
	}
      if (bianco_fase == 3 && bianco_torre == 1)
	{
	  if (nero_fase == 2 && nero_torre == 1)
	    {
	      if (bianco_cavallo == 1)
		bianco_carico = 1;
	      if (bianco_alfiere == 1)
		bianco_carico = 1;
	    }
	  if (nero_fase == 2 && nero_torre == 0)
	    {
	      bianco_carico = 2;
	      if (bianco_alfiere == 1 && nero_cavallo == 2)
		bianco_carico = 6;
	      if (nero_cavallo == 1
		  && ((bianco_chiaro == 1 && nero_chiaro == 1)
		      || (bianco_scuro == 1 && nero_scuro == 1)))
		bianco_carico = 2;
	      if (nero_cavallo == 1
		  && ((bianco_scuro == 1 && nero_chiaro == 1)
		      || (bianco_chiaro == 1 && nero_scuro == 1)))
		bianco_carico = 7;
	    }
	  if (nero_fase == 3)
	    bianco_carico = 2;
	}
      if (bianco_fase == 3 && bianco_torre == 0)
	{
	  if (nero_fase == 2 && nero_torre == 1)
	    {
	      if (bianco_cavallo == 2)
		bianco_carico = 2;
	      if (bianco_alfiere == 2)
		bianco_carico = 7;
	    }
	  if (nero_fase == 2 && nero_torre == 0)
	    {
	      bianco_carico = 2;
	      if (bianco_alfiere == 2 && nero_cavallo == 2)
		bianco_carico = 4;
	    }
	  if (nero_fase == 3)
	    bianco_carico = 2;
	}
      if (bianco_fase == 4 && bianco_donna)
	{
	  if (nero_fase == 2 && nero_cavallo == 2)
	    bianco_carico = 2;
	  if (nero_fase == 2 && nero_cavallo == 1)
	    bianco_carico = 8;
	  if (nero_fase == 2 && nero_cavallo == 0)
	    bianco_carico = 7;
	  if (nero_fase == 3)
	    bianco_carico = 1;
	  if (nero_fase == 4)
	    bianco_carico = 1;
	}
      if (bianco_fase == 4 && bianco_torre == 2)
	{
	  if (nero_fase == 2 && nero_torre == 0)
	    bianco_carico = 7;
	  if (nero_fase == 3)
	    bianco_carico = 2;
	  if (nero_fase == 4)
	    bianco_carico = 1;
	}
      if (bianco_fase == 4 && bianco_torre == 1)
	{
	  if (nero_fase == 3 && nero_torre == 1)
	    bianco_carico = 3;
	  if (nero_fase == 3 && nero_torre == 0)
	    bianco_carico = 2;
	  if (nero_fase == 4)
	    bianco_carico = 2;
	}
      if (bianco_fase == 4 && bianco_torre == 0 && bianco_donna == 0)
	{
	  if (nero_fase == 3 && nero_torre == 1)
	    bianco_carico = 4;
	  if (nero_fase == 3 && nero_torre == 0)
	    bianco_carico = 2;
	  if (nero_fase == 4 && nero_donna)
	    bianco_carico = 8;
	  if (nero_fase == 4 && nero_donna == 0)
	    bianco_carico = 1;
	}
      if (bianco_fase == 5 && bianco_donna)
	{
	  if (nero_fase == 4)
	    bianco_carico = 2;
	  if (nero_fase == 5)
	    bianco_carico = 1;
	  if (nero_fase == 4 && nero_torre == 2)
	    {
	      if (bianco_cavallo)
		bianco_carico = 3;
	      if (bianco_alfiere)
		bianco_carico = 7;
	    }
	  if (nero_fase == 5)
	    bianco_carico = 1;
	}
      if (bianco_fase == 5 && bianco_torre == 1)
	{
	  if (nero_fase == 4 && nero_donna)
	    bianco_carico = 9;
	  if (nero_fase == 4 && nero_torre == 2)
	    bianco_carico = 7;
	  if (nero_fase == 4 && nero_torre == 1)
	    bianco_carico = 3;
	  if (nero_fase == 4 && nero_donna == 0 && nero_torre == 0)
	    bianco_carico = 1;
	  if (nero_fase == 5)
	    bianco_carico = 2;
	}
      if (bianco_fase == 5 && bianco_torre == 2)
	{
	  if (nero_fase == 4 && nero_donna && bianco_alfiere == 1)
	    bianco_carico = 8;
	  if (nero_fase == 4 && nero_donna && bianco_cavallo == 1)
	    bianco_carico = 7;
	  if (nero_fase == 4 && nero_torre == 2)
	    bianco_carico = 3;
	  if (nero_fase == 4 && nero_torre == 1)
	    bianco_carico = 2;
	  if (nero_fase == 4 && nero_donna == 0 && nero_torre == 0)
	    bianco_carico = 1;
	  if (nero_fase == 5)
	    bianco_carico = 1;
	}
      if (bianco_fase == 6 && bianco_donna && bianco_torre)
	{
	  if (nero_fase == 4 && nero_donna == 0 && nero_torre == 0)
	    bianco_carico = 2;
	  if (nero_fase == 5 && nero_donna)
	    bianco_carico = 1;
	  if (nero_fase == 4 && nero_torre == 1)
	    bianco_carico = 6;
	  if (nero_fase == 4 && nero_torre == 2)
	    bianco_carico = 3;
	  if (nero_fase == 5 && nero_torre)
	    bianco_carico = 1;
	  if (nero_fase == 6)
	    bianco_carico = 1;
	}
      if (bianco_fase == 6 && bianco_donna && bianco_torre == 0)
	{
	  if (nero_fase == 4 && nero_donna == 0 && nero_torre == 0)
	    bianco_carico = 5;
	  if (nero_fase == 5 && nero_donna)
	    bianco_carico = 2;
	  if (nero_fase == 5 && nero_torre == 2)
	    bianco_carico = 2;
	  if (nero_fase == 5 && nero_torre == 1)
	    bianco_carico = 1;
	  if (nero_fase == 6)
	    bianco_carico = 1;
	}
      if (bianco_fase == 6 && bianco_donna == 0 && bianco_torre == 2)
	{
	  if (nero_fase == 5 && nero_donna)
	    bianco_carico = 7;
	  if (nero_fase == 5 && nero_torre == 1)
	    bianco_carico = 1;
	  if (nero_fase == 5 && nero_torre == 2)
	    bianco_carico = 2;
	  if (nero_fase == 6)
	    bianco_carico = 1;
	}
      if (bianco_fase == 6 && bianco_donna == 0 && bianco_torre == 1)
	{
	  if (nero_fase == 5 && nero_donna)
	    bianco_carico = 9;
	  if (nero_fase == 5 && nero_torre == 2)
	    bianco_carico = 3;
	  if (nero_fase == 5 && nero_torre == 1)
	    bianco_carico = 2;
	  if (nero_fase == 6)
	    bianco_carico = 1;
	  if (nero_fase == 6 && nero_donna)
	    bianco_carico = 2;
	  if (nero_fase == 6 && nero_donna && nero_torre)
	    bianco_carico = 4;
	}
      if (bianco_fase >= 7)
	{
	  if (bianco_valu > nero_valu + 4)
	    bianco_carico = 9;
	  if (bianco_valu == nero_valu + 4)
	    bianco_carico = 7;
	  if (bianco_valu == nero_valu + 3)
	    bianco_carico = 4;
	  if (bianco_valu == nero_valu + 2)
	    bianco_carico = 2;
	  if (bianco_valu < nero_valu + 2)
	    bianco_carico = 1;
	}
    }
  if (bianco_pedone == 1)
    {
      if (nero_fase == 1)
	{
	  if (bianco_fase == 1)
	    bianco_carico = 3;
	  if (bianco_fase == 2 && bianco_cavallo == 2)
	    {
	      if (nero_pedone == 0)
		bianco_carico = 3;
	      else
		bianco_carico = 5;
	    }
	  if (bianco_fase == 2 && bianco_torre == 1)
	    bianco_carico = 7;
	}
      if (nero_fase == 2 && nero_torre == 1 && bianco_fase == 2
	  && bianco_torre == 1)
	bianco_carico = 8;
      if (nero_fase == 2 && nero_torre == 0 && bianco_fase == 2)
	bianco_carico = 4;
      if (nero_fase >= 3 && nero_minore > 0 && bianco_fase == nero_fase)
	bianco_carico = 3;
      if (nero_fase >= 3 && nero_minore == 0 && bianco_fase == nero_fase)
	bianco_carico = 5;
      if (nero_fase == 4 && nero_donna == 1 && bianco_fase == nero_fase)
	bianco_carico = 7;
    }
  return bianco_carico;
}

static int
carico_nero (int bianco_pedone, int bianco_cavallo, int bianco_alfiere,
	     int bianco_chiaro, int bianco_scuro,
	     int bianco_torre, int bianco_donna,
	     int nero_pedone, int nero_cavallo,
	     int nero_alfiere, int nero_chiaro, int nero_scuro,
	     int nero_torre, int nero_donna)
{
  int bianco_minore, nero_minore, bianco_fase, nero_fase, nero_carico,
    bianco_valu, nero_valu;
  bianco_minore = bianco_alfiere + bianco_cavallo;
  nero_minore = nero_alfiere + nero_cavallo;
  bianco_fase = bianco_minore + 2 * bianco_torre + 4 * bianco_donna;
  nero_fase = nero_minore + 2 * nero_torre + 4 * nero_donna;
  bianco_valu =
    3 * (bianco_alfiere + bianco_cavallo) + 5 * bianco_torre +
    9 * bianco_donna;
  nero_valu =
    3 * (nero_alfiere + nero_cavallo) + 5 * nero_torre + 9 * nero_donna;
  nero_carico = 10;
  if (!nero_pedone)
    {
      if (nero_fase == 1)
	nero_carico = 0;
      if (nero_fase == 2)
	{
	  if (bianco_fase == 0)
	    {
	      if (nero_cavallo == 2)
		{
		  if (nero_pedone >= 1)
		    nero_carico = 3;
		  else
		    nero_carico = 0;
		}
	    }
	  if (bianco_fase == 1)
	    {
	      nero_carico = 1;
	      if (nero_alfiere == 2 && bianco_cavallo == 1)
		nero_carico = 8;
	      if (nero_torre == 1 && bianco_cavallo == 1)
		nero_carico = 2;
	    }
	  if (bianco_fase == 2)
	    nero_carico = 1;
	}
      if (nero_fase == 3 && nero_torre == 1)
	{
	  if (bianco_fase == 2 && bianco_torre == 1)
	    {
	      if (nero_cavallo == 1)
		nero_carico = 1;
	      if (nero_alfiere == 1)
		nero_carico = 1;
	    }
	  if (bianco_fase == 2 && bianco_torre == 0)
	    {
	      nero_carico = 2;
	      if (nero_alfiere == 1 && bianco_cavallo == 2)
		nero_carico = 6;
	      if (bianco_cavallo == 1
		  && ((nero_chiaro == 1 && bianco_chiaro == 1)
		      || (nero_scuro == 1 && bianco_scuro == 1)))
		nero_carico = 2;
	      if (bianco_cavallo == 1
		  && ((nero_scuro == 1 && bianco_chiaro == 1)
		      || (nero_chiaro == 1 && bianco_scuro == 1)))
		nero_carico = 7;
	    }
	  if (bianco_fase == 3)
	    nero_carico = 2;
	}
      if (nero_fase == 3 && nero_torre == 0)
	{
	  if (bianco_fase == 2 && bianco_torre == 1)
	    {
	      if (nero_cavallo == 2)
		nero_carico = 2;
	      if (nero_alfiere == 2)
		nero_carico = 7;
	    }
	  if (bianco_fase == 2 && bianco_torre == 0)
	    {
	      nero_carico = 2;
	      if (nero_alfiere == 2 && bianco_cavallo == 2)
		nero_carico = 4;
	    }
	  if (bianco_fase == 3)
	    nero_carico = 2;
	}
      if (nero_fase == 4 && nero_donna)
	{
	  if (bianco_fase == 2 && bianco_cavallo == 2)
	    nero_carico = 2;
	  if (bianco_fase == 2 && bianco_cavallo == 1)
	    nero_carico = 8;
	  if (bianco_fase == 2 && bianco_cavallo == 0)
	    nero_carico = 7;
	  if (bianco_fase == 3)
	    nero_carico = 1;
	  if (bianco_fase == 4)
	    nero_carico = 1;
	}
      if (nero_fase == 4 && nero_torre == 2)
	{
	  if (bianco_fase == 2 && bianco_torre == 0)
	    nero_carico = 7;
	  if (bianco_fase == 3)
	    nero_carico = 2;
	  if (bianco_fase == 4)
	    nero_carico = 1;
	}
      if (nero_fase == 4 && nero_torre == 1)
	{
	  if (bianco_fase == 3 && bianco_torre == 1)
	    nero_carico = 3;
	  if (bianco_fase == 3 && bianco_torre == 0)
	    nero_carico = 2;
	  if (bianco_fase == 4)
	    nero_carico = 2;
	}
      if (nero_fase == 4 && nero_torre == 0 && nero_donna == 0)
	{
	  if (bianco_fase == 3 && bianco_torre == 1)
	    nero_carico = 4;
	  if (bianco_fase == 3 && bianco_torre == 0)
	    nero_carico = 2;
	  if (bianco_fase == 4 && bianco_donna)
	    nero_carico = 8;
	  if (bianco_fase == 4 && bianco_donna == 0)
	    nero_carico = 1;
	}
      if (nero_fase == 5 && nero_donna)
	{
	  if (bianco_fase == 4)
	    nero_carico = 2;
	  if (bianco_fase == 5)
	    nero_carico = 1;
	  if (bianco_fase == 4 && bianco_torre == 2)
	    {
	      if (nero_cavallo)
		nero_carico = 3;
	      if (nero_alfiere)
		nero_carico = 7;
	    }
	  if (bianco_fase == 5)
	    nero_carico = 1;
	}
      if (nero_fase == 5 && nero_torre == 1)
	{
	  if (bianco_fase == 4 && bianco_donna)
	    nero_carico = 9;
	  if (bianco_fase == 4 && bianco_torre == 2)
	    nero_carico = 7;
	  if (bianco_fase == 4 && bianco_torre == 1)
	    nero_carico = 3;
	  if (bianco_fase == 4 && bianco_donna == 0 && bianco_torre == 0)
	    nero_carico = 1;
	  if (bianco_fase == 5)
	    nero_carico = 2;
	}
      if (nero_fase == 5 && nero_torre == 2)
	{
	  if (bianco_fase == 4 && bianco_donna && nero_alfiere == 1)
	    nero_carico = 8;
	  if (bianco_fase == 4 && bianco_donna && nero_cavallo == 1)
	    nero_carico = 7;
	  if (bianco_fase == 4 && bianco_torre == 2)
	    nero_carico = 3;
	  if (bianco_fase == 4 && bianco_torre == 1)
	    nero_carico = 2;
	  if (bianco_fase == 4 && bianco_donna == 0 && bianco_torre == 0)
	    nero_carico = 1;
	  if (bianco_fase == 5)
	    nero_carico = 1;
	}
      if (nero_fase == 6 && nero_donna && nero_torre)
	{
	  if (bianco_fase == 4 && bianco_donna == 0 && bianco_torre == 0)
	    nero_carico = 2;
	  if (bianco_fase == 5 && bianco_donna)
	    nero_carico = 1;
	  if (bianco_fase == 4 && bianco_torre == 1)
	    nero_carico = 6;
	  if (bianco_fase == 4 && bianco_torre == 2)
	    nero_carico = 3;
	  if (bianco_fase == 5 && bianco_torre)
	    nero_carico = 1;
	  if (bianco_fase == 6)
	    nero_carico = 1;
	}
      if (nero_fase == 6 && nero_donna && nero_torre == 0)
	{
	  if (bianco_fase == 4 && bianco_donna == 0 && bianco_torre == 0)
	    nero_carico = 5;
	  if (bianco_fase == 5 && bianco_donna)
	    nero_carico = 2;
	  if (bianco_fase == 5 && bianco_torre == 2)
	    nero_carico = 2;
	  if (bianco_fase == 5 && bianco_torre == 1)
	    nero_carico = 1;
	  if (bianco_fase == 6)
	    nero_carico = 1;
	}
      if (nero_fase == 6 && nero_donna == 0 && nero_torre == 2)
	{
	  if (bianco_fase == 5 && bianco_donna)
	    nero_carico = 7;
	  if (bianco_fase == 5 && bianco_torre == 1)
	    nero_carico = 1;
	  if (bianco_fase == 5 && bianco_torre == 2)
	    nero_carico = 2;
	  if (bianco_fase == 6)
	    nero_carico = 1;
	}
      if (nero_fase == 6 && nero_donna == 0 && nero_torre == 1)
	{
	  if (bianco_fase == 5 && bianco_donna)
	    nero_carico = 9;
	  if (bianco_fase == 5 && bianco_torre == 2)
	    nero_carico = 3;
	  if (bianco_fase == 5 && bianco_torre == 1)
	    nero_carico = 2;
	  if (bianco_fase == 6)
	    nero_carico = 1;
	  if (bianco_fase == 6 && bianco_donna)
	    nero_carico = 2;
	  if (bianco_fase == 6 && bianco_donna && bianco_torre)
	    nero_carico = 4;
	}
      if (nero_fase >= 7)
	{
	  if (nero_valu > bianco_valu + 4)
	    nero_carico = 9;
	  if (nero_valu == bianco_valu + 4)
	    nero_carico = 7;
	  if (nero_valu == bianco_valu + 3)
	    nero_carico = 4;
	  if (nero_valu == bianco_valu + 2)
	    nero_carico = 2;
	  if (nero_valu < bianco_valu + 2)
	    nero_carico = 1;
	}
    }
  if (nero_pedone == 1)
    {
      if (bianco_fase == 1)
	{
	  if (nero_fase == 1)
	    nero_carico = 3;
	  if (nero_fase == 2 && nero_cavallo == 2)
	    {
	      if (bianco_pedone == 0)
		nero_carico = 3;
	      else
		nero_carico = 5;
	    }
	  if (nero_fase == 2 && nero_torre == 1)
	    nero_carico = 7;
	}
      if (bianco_fase == 2 && bianco_torre == 1 && nero_fase == 2
	  && nero_torre == 1)
	nero_carico = 8;
      if (bianco_fase == 2 && bianco_torre == 0 && nero_fase == 2)
	nero_carico = 4;
      if (bianco_fase >= 3 && bianco_minore > 0 && nero_fase == bianco_fase)
	nero_carico = 3;
      if (bianco_fase >= 3 && bianco_minore == 0 && nero_fase == bianco_fase)
	nero_carico = 5;
      if (bianco_fase == 4 && bianco_donna == 1 && nero_fase == bianco_fase)
	nero_carico = 7;
    }
  return nero_carico;
}

#define VALU4(w,x,y,z)\
 ( ( ( (uint64) z) << 48) + ( ( (uint64) y) << 32) +\
   ( ( (uint64) x) << 16) + ( ( (uint64) w) << 0))
#define alfiere_due VALU4(35, 40, 50, 55)
#define valu_pedone VALU4(80, 90, 110, 125)
#define valu_cavallo VALU4(265, 280, 320, 355)
#define valu_torre VALU4(405, 450, 550, 610)
#define valu_donna VALU4(800, 875, 1025, 1150)
#define valu_alfiere VALU4(280, 295, 325, 360)
#define mescolanza_cavallo_pedone VALU4(0, 2, 4, 5)
#define mescolanza_torre_pedone VALU4(5, 4, 2, 0)

static uint64
valu_valore (int bianco_pedone, int bianco_cavallo, int bianco_alfiere,
	     int bianco_chiaro, int bianco_scuro,
	     int bianco_torre, int bianco_donna,
	     int nero_pedone, int nero_cavallo,
	     int nero_alfiere, int nero_chiaro, int nero_scuro,
	     int nero_torre, int nero_donna)
{
  uint64 va = 0;
  int bianco_minore = bianco_cavallo + bianco_alfiere;
  int nero_minore = nero_cavallo + nero_alfiere;
  va += (bianco_alfiere / 2 - nero_alfiere / 2) * alfiere_due;
  va += (bianco_pedone - nero_pedone) * valu_pedone;
  va += (bianco_cavallo - nero_cavallo) * valu_cavallo;
  va += (bianco_torre - nero_torre) * valu_torre;
  va += (bianco_donna - nero_donna) * valu_donna;
  va += (bianco_alfiere - nero_alfiere) * valu_alfiere;
  if (bianco_torre == 2)
    va -= VALU4 (16, 20, 28, 32);
  if (nero_torre == 2)
    va += VALU4 (16, 20, 28, 32);
  if (bianco_donna + bianco_torre >= 2)
    va -= VALU4 (8, 10, 14, 16);
  if (nero_donna + nero_torre >= 2)
    va += VALU4 (8, 10, 14, 16);
  if (bianco_minore > nero_minore)
    va += VALU4 (20, 15, 10, 5);
  if (nero_minore > bianco_minore)
    va -= VALU4 (20, 15, 10, 5);
  va -= (bianco_pedone - 5) * bianco_torre * mescolanza_torre_pedone;
  va += (bianco_pedone - 5) * bianco_cavallo * mescolanza_cavallo_pedone;
  va += (nero_pedone - 5) * nero_torre * mescolanza_torre_pedone;
  va -= (nero_pedone - 5) * nero_cavallo * mescolanza_cavallo_pedone;
  return va;
}

#define FASE_MINORE (1)
#define FASE_TORRE (3)
#define FASE_DONNA (6)

static void
materiale_calcolo (int c)
{
  int bianco_donna, nero_donna, bianco_torre, nero_torre, bianco_chiaro,
    nero_chiaro, bianco_scuro, nero_scuro, bianco_cavallo, nero_cavallo,
    bianco_pedone, nero_pedone, n, valu, bianco_alfiere, nero_alfiere;
  int ca, bianco_carico, nero_carico, fase, va1, va2, va3, va4;
  uint64 va;
  n = c;
  bianco_donna = n % 2;
  n /= 2;
  nero_donna = n % 2;
  n /= 2;
  bianco_torre = n % 3;
  n /= 3;
  nero_torre = n % 3;
  n /= 3;
  bianco_chiaro = n % 2;
  n /= 2;
  bianco_scuro = n % 2;
  n /= 2;
  nero_chiaro = n % 2;
  n /= 2;
  nero_scuro = n % 2;
  n /= 2;
  bianco_cavallo = n % 3;
  n /= 3;
  nero_cavallo = n % 3;
  n /= 3;
  bianco_pedone = n % 9;
  n /= 9;
  nero_pedone = n % 9;
  bianco_alfiere = bianco_chiaro + bianco_scuro;
  nero_alfiere = nero_chiaro + nero_scuro;
  va =
    valu_valore (bianco_pedone, bianco_cavallo, bianco_alfiere,
		 bianco_chiaro, bianco_scuro, bianco_torre, bianco_donna,
		 nero_pedone, nero_cavallo, nero_alfiere,
		 nero_chiaro, nero_scuro, nero_torre, nero_donna);
  fase =
    FASE_MINORE *
    (bianco_cavallo + bianco_alfiere + nero_cavallo +  nero_alfiere) +
    FASE_TORRE * (bianco_torre + nero_torre) +
    FASE_DONNA * (bianco_donna + nero_donna);
  va1 = va & 0xffff;
  va2 = ((va >> 16) & 0xffff) + (va1 > 0x8000);
  va1 = (sint16) va1;
  va3 = ((va >> 32) & 0xffff) + (va2 > 0x8000);
  va2 = (sint16) va2;
  va4 = ((va >> 48) & 0xffff) + (va3 > 0x8000);
  va3 = (sint16) va3;
  va4 = (sint16) va4;
  if (fase < 8)
    {
      va4 *= 8 - fase;
      va3 *= fase;
      va = va3 + va4;
      valu = ((int) va) / 8;
    }
  else if (fase < 24)
    {
      va3 *= 24 - fase;
      va2 *= fase - 8;
      va = va2 + va3;
      valu = ((int) va) / 16;
    }
  else
    {
      va2 *= 32 - fase;
      va1 *= fase - 24;
      va = va1 + va2;
      valu = ((int) va) / 8;
    }
  bianco_carico =
    carico_bianco (bianco_pedone, bianco_cavallo, bianco_alfiere,
		   bianco_chiaro, bianco_scuro, bianco_torre, bianco_donna,
		   nero_pedone, nero_cavallo, nero_alfiere,
		   nero_chiaro, nero_scuro, nero_torre, nero_donna);
  nero_carico =
    carico_nero (bianco_pedone, bianco_cavallo, bianco_alfiere,
		 bianco_chiaro, bianco_scuro, bianco_torre, bianco_donna,
		 nero_pedone, nero_cavallo, nero_alfiere, nero_chiaro,
		 nero_scuro, nero_torre, nero_donna);
  if (valu > 0)
    ca = bianco_carico;
  else
    ca = nero_carico;
  valu *= ca;
  valu /= 10;
  MATERIALE[c].valu = valu;
  MATERIALE[c].segno =
    sengo_inizializzazione (bianco_pedone, bianco_cavallo,
			    bianco_alfiere, bianco_chiaro, bianco_scuro,
			    bianco_torre, bianco_donna,
			    nero_pedone, nero_cavallo,
			    nero_alfiere, nero_chiaro, nero_scuro,
			    nero_torre, nero_donna);
  MATERIALE[c].pietras =
    pietras_inizializzazione (bianco_pedone, bianco_cavallo,
			      bianco_alfiere, bianco_chiaro, bianco_scuro,
			      bianco_torre, bianco_donna,
			      nero_pedone, nero_cavallo,
			      nero_alfiere, nero_chiaro, nero_scuro,
			      nero_torre, nero_donna);
}

void
inizializzazione_valore ()
{
  int c;
  for (c = 0; c < 419904; c++)
    materiale_calcolo (c);
}
