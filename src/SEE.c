#ifndef BUILD_SEE
#define BUILD_SEE
#include "RobboLito.h"
typedef enum
{ valu_pedone = 100, valu_cavallo = 325, valu_alfiere = 325,
  valu_torre = 500, valu_donna = 975, valu_re = 12345 } enum_valu;
static const int valu[16] =
  { 0, valu_pedone, valu_cavallo, 12345678,
    valu_alfiere, valu_alfiere, valu_torre, valu_donna,
    0, valu_pedone, valu_cavallo, 12345678,
    valu_alfiere, valu_alfiere, valu_torre, valu_donna
};

#include "SEE.c"
#include "bianco.h"
#else
#include "nero.h"
#endif

logico
mio_SEE (tipo_posizione *POSIZIONE, uint32 mossa)
{
  int di, ai, valu_pezzo, valu_cattura, d, dir;
  uint64 bit, cbf, cel, indice_pila[4], gcm = 0, T;
  int indice_turno[4], b, w;
  T = mio_xray & tuo_occupato;
  di = DI (mossa);
  ai = AI (mossa);
  while (T)
    {
      b = BSF (T);
      w = mio_xray_lista[b];
      bitLIBERO (b, T);
      if (di != w && indice_linea[ai][b] != indice_linea[b][tuo_quadretto_re])
	gcm |= quadretto_fisso[b];
    }
  gcm = ~gcm;
  valu_pezzo = valu[POSIZIONE->qu[di]];
  valu_cattura = valu[POSIZIONE->qu[ai]];
  if (valu_pezzo - valu_cattura > valu_pedone
      && tuo_attaco_pedone[ai] & bitbordo_tuo_pedone & gcm)
    return FALSO;
  bit =
    (bitbordo_mio_cavallo | (bitbordo_tuo_cavallo & gcm)) &
    attaco_cavallo[ai];
  d = valu_pezzo - valu_cattura;
  if (d > valu_cavallo && bitbordo_tuo_cavallo & bit)
    return FALSO;
  indice_turno[direzione_h1a8] =
    (POSIZIONE->occupato_sinistro45 >> linea_turno[direzione_h1a8][ai]) & 077;
  indice_turno[direzione_a1h8] =
    (POSIZIONE->occupato_retto45 >> linea_turno[direzione_a1h8][ai]) & 077;
  cel =
    bitbordo_mio_donna | bitbordo_mio_alfiere |
    ((bitbordo_tuo_donna | bitbordo_tuo_alfiere) & gcm);
  indice_pila[direzione_h1a8] = indice_pila[direzione_a1h8] = cel;
  bit |=
    (linea_obscura[direzione_h1a8][ai][indice_turno[direzione_h1a8]] |
     linea_obscura[direzione_a1h8][ai][indice_turno[direzione_a1h8]]) & cel;
  if (d > valu_alfiere && (bitbordo_tuo_alfiere & bit))
    return FALSO;
  indice_turno[direzione_orizzontale] =
    (POSIZIONE->
     occupato_nero_bianco >> linea_turno[direzione_orizzontale][ai]) & 077;
  indice_turno[direzione_verticale] =
    (POSIZIONE->
     occupato_sinistro90 >> linea_turno[direzione_verticale][ai]) & 077;
  cel =
    bitbordo_mio_donna | bitbordo_mio_torre |
    ((bitbordo_tuo_donna | bitbordo_tuo_torre) & gcm);
  indice_pila[direzione_orizzontale] = indice_pila[direzione_verticale] = cel;
  bit |=
    (linea_obscura[direzione_orizzontale][ai]
     [indice_turno[direzione_orizzontale]] |
     linea_obscura[direzione_verticale][ai][indice_turno
					    [direzione_verticale]]) & cel;
  bit |= (bitbordo_mio_re | bitbordo_tuo_re) & attaco_re[ai];
  bit |= bitbordo_tuo_pedone & tuo_attaco_pedone[ai] & gcm;
  bit |= bitbordo_mio_pedone & mio_attaco_pedone[ai];
  cbf = ~(quadretto_fisso[di] | quadretto_fisso[ai]);
  bit &= cbf;
  dir = indice_linea[di][ai];
  if (dir != direzione_malato)
    bit |= linea_obscura[dir][di][indice_turno[dir]] & indice_pila[dir] & cbf;
  valu_cattura -= valu_pezzo;
  do
    {
      cbf &= ~bit;
      cel = bitbordo_tuo_pedone & bit;
      if (cel)
	{
	  bit ^= (~(cel - 1)) & cel;
	  valu_pezzo = valu_pedone;
	}
      else
	{
	  cel = bitbordo_tuo_cavallo & bit;
	  if (cel)
	    {
	      bit ^= (~(cel - 1)) & cel;
	      valu_pezzo = valu_cavallo;
	    }
	  else
	    {
	      cel = bitbordo_tuo_alfiere & bit;
	      if (cel)
		{
		  valu_pezzo = valu_alfiere;
		  di = BSF (cel);
		  dir = indice_linea[di][ai];
		  cel =
		    linea_obscura[dir][di][indice_turno[dir]] & cbf &
		    indice_pila[direzione_a1h8];
		  bit = cel | (quadretto_libero[di] & bit);
		}
	      else
		{
		  cel = bitbordo_tuo_torre & bit;
		  if (cel)
		    {
		      valu_pezzo = valu_torre;
		      di = BSF (cel);
		      dir = indice_linea[di][ai];
		      cel =
			linea_obscura[dir][di][indice_turno[dir]] & cbf &
			indice_pila[direzione_orizzontale];
		      bit = cel | (quadretto_libero[di] & bit);
		    }
		  else
		    {
		      cel = bitbordo_tuo_donna & bit;
		      if (cel)
			{
			  valu_pezzo = valu_donna;
			  di = BSF (cel);
			  dir = indice_linea[di][ai];
			  cel =
			    linea_obscura[dir][di][indice_turno[dir]] & cbf &
			    indice_pila[dir];
			  bit = cel | (quadretto_libero[di] & bit);
			}
		      else
			{
			  if (!(bitbordo_tuo_re & bit))
			    return VERO;
			  valu_pezzo = 12345;
			}
		    }
		}
	    }
	}
      valu_cattura += valu_pezzo;
      if (valu_cattura < -60)
	return FALSO;
      cel = bitbordo_mio_pedone & bit;
      if (cel)
	{
	  bit ^= (~(cel - 1)) & cel;
	  valu_pezzo = valu_pedone;
	}
      else
	{
	  cel = bitbordo_mio_cavallo & bit;
	  if (cel)
	    {
	      bit ^= (~(cel - 1)) & cel;
	      valu_pezzo = valu_cavallo;
	    }
	  else
	    {
	      cel = bitbordo_mio_alfiere & bit;
	      if (cel)
		{
		  valu_pezzo = valu_alfiere;
		  di = BSF (cel);
		  dir = indice_linea[di][ai];
		  cel =
		    linea_obscura[dir][di][indice_turno[dir]] & cbf &
		    indice_pila[direzione_a1h8];
		  bit = cel | (quadretto_libero[di] & bit);
		}
	      else
		{
		  cel = bitbordo_mio_torre & bit;
		  if (cel)
		    {
		      valu_pezzo = valu_torre;
		      di = BSF (cel);
		      dir = indice_linea[di][ai];
		      cel =
			linea_obscura[dir][di][indice_turno[dir]] & cbf &
			indice_pila[direzione_orizzontale];
		      bit = cel | (quadretto_libero[di] & bit);
		    }
		  else
		    {
		      cel = bitbordo_mio_donna & bit;
		      if (cel)
			{
			  valu_pezzo = valu_donna;
			  di = BSF (cel);
			  dir = indice_linea[di][ai];
			  cel =
			    linea_obscura[dir][di][indice_turno[dir]] & cbf &
			    indice_pila[dir];
			  bit = cel | (quadretto_libero[di] & bit);
			}
		      else
			{
			  if (!(bitbordo_mio_re & bit))
			    return FALSO;
			  if (valu_cattura > 6174)
			    return VERO;
			  valu_pezzo = 23456;
			}
		    }
		}
	    }
	}
      valu_cattura -= valu_pezzo;
    }
  while (valu_cattura < -60);
  return VERO;
}
