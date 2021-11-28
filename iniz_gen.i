
static uint32 cattura_valu[16][16];
void
inizializzazione_cattura ()
{
  cattura_valu[conto_pedone_bianco][conto_donna_nera] =
    (0xd0 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_bianco][conto_donna_nera] =
    (0xcf << 24) + (0x02 << 20);
  cattura_valu[conto_chiaro_bianco][conto_donna_nera] =
    (0xce << 24) + (0x02 << 20);
  cattura_valu[conto_scuro_bianco][conto_donna_nera] =
    (0xce << 24) + (0x02 << 20);
  cattura_valu[conto_torre_bianco][conto_donna_nera] =
    (0xcd << 24) + (0x02 << 20);
  cattura_valu[conto_donna_bianca][conto_donna_nera] =
    (0xcc << 24) + (0x01 << 20);

  cattura_valu[conto_pedone_bianco][conto_torre_nero] =
    (0xc8 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_bianco][conto_torre_nero] =
    (0xc7 << 24) + (0x02 << 20);
  cattura_valu[conto_chiaro_bianco][conto_torre_nero] =
    (0xc6 << 24) + (0x02 << 20);
  cattura_valu[conto_scuro_bianco][conto_torre_nero] =
    (0xc6 << 24) + (0x02 << 20);
  cattura_valu[conto_torre_bianco][conto_torre_nero] =
    (0xc5 << 24) + (0x01 << 20);
  cattura_valu[conto_donna_bianca][conto_torre_nero] =
    (0xc4 << 24) + (0x00 << 20);

  cattura_valu[conto_pedone_bianco][conto_chiaro_nero] =
    (0xc0 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_bianco][conto_chiaro_nero] =
    (0xbf << 24) + (0x01 << 20);
  cattura_valu[conto_chiaro_bianco][conto_chiaro_nero] =
    (0xbe << 24) + (0x01 << 20);
  cattura_valu[conto_scuro_bianco][conto_chiaro_nero] =
    (0xbe << 24) + (0x01 << 20);
  cattura_valu[conto_torre_bianco][conto_chiaro_nero] =
    (0xbd << 24) + (0x00 << 20);
  cattura_valu[conto_donna_bianca][conto_chiaro_nero] =
    (0xbc << 24) + (0x00 << 20);

  cattura_valu[conto_pedone_bianco][conto_scuro_nero] =
    (0xc0 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_bianco][conto_scuro_nero] =
    (0xbf << 24) + (0x01 << 20);
  cattura_valu[conto_chiaro_bianco][conto_scuro_nero] =
    (0xbe << 24) + (0x01 << 20);
  cattura_valu[conto_scuro_bianco][conto_scuro_nero] =
    (0xbe << 24) + (0x01 << 20);
  cattura_valu[conto_torre_bianco][conto_scuro_nero] =
    (0xbd << 24) + (0x00 << 20);
  cattura_valu[conto_donna_bianca][conto_scuro_nero] =
    (0xbc << 24) + (0x00 << 20);

  cattura_valu[conto_pedone_bianco][conto_cavallo_nero] =
    (0xb8 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_bianco][conto_cavallo_nero] =
    (0xb7 << 24) + (0x01 << 20);
  cattura_valu[conto_chiaro_bianco][conto_cavallo_nero] =
    (0xb6 << 24) + (0x01 << 20);
  cattura_valu[conto_scuro_bianco][conto_cavallo_nero] =
    (0xb6 << 24) + (0x01 << 20);
  cattura_valu[conto_torre_bianco][conto_cavallo_nero] =
    (0xb5 << 24) + (0x00 << 20);
  cattura_valu[conto_donna_bianca][conto_cavallo_nero] =
    (0xb4 << 24) + (0x00 << 20);

  cattura_valu[conto_pedone_bianco][conto_pedone_nero] =
    (0xb0 << 24) + (0x01 << 20);
  cattura_valu[conto_cavallo_bianco][conto_pedone_nero] =
    (0xaf << 24) + (0x00 << 20);
  cattura_valu[conto_chiaro_bianco][conto_pedone_nero] =
    (0xae << 24) + (0x00 << 20);
  cattura_valu[conto_scuro_bianco][conto_pedone_nero] =
    (0xae << 24) + (0x00 << 20);
  cattura_valu[conto_torre_bianco][conto_pedone_nero] =
    (0xad << 24) + (0x00 << 20);
  cattura_valu[conto_donna_bianca][conto_pedone_nero] =
    (0xac << 24) + (0x00 << 20);

  cattura_valu[conto_re_bianco][conto_donna_nera] =
    (0xcb << 24) + (0x00 << 20);
  cattura_valu[conto_re_bianco][conto_torre_nero] =
    (0xc3 << 24) + (0x00 << 20);
  cattura_valu[conto_re_bianco][conto_chiaro_nero] =
    (0xbb << 24) + (0x00 << 20);
  cattura_valu[conto_re_bianco][conto_scuro_nero] =
    (0xbb << 24) + (0x00 << 20);
  cattura_valu[conto_re_bianco][conto_cavallo_nero] =
    (0xb3 << 24) + (0x00 << 20);
  cattura_valu[conto_re_bianco][conto_pedone_nero] =
    (0xab << 24) + (0x00 << 20);

  cattura_valu[conto_re_bianco][0] = (0x07 << 24) + (0x00 << 15);
  cattura_valu[conto_pedone_bianco][0] = (0x06 << 24) + (0x01 << 15);
  cattura_valu[conto_cavallo_bianco][0] = (0x05 << 24) + (0x01 << 15);
  cattura_valu[conto_chiaro_bianco][0] = (0x04 << 24) + (0x01 << 15);
  cattura_valu[conto_scuro_bianco][0] = (0x04 << 24) + (0x01 << 15);
  cattura_valu[conto_torre_bianco][0] = (0x03 << 24) + (0x01 << 15);
  cattura_valu[conto_donna_bianca][0] = (0x02 << 24) + (0x01 << 15);

  cattura_valu[conto_pedone_nero][conto_donna_bianca] =
    (0xd0 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_nero][conto_donna_bianca] =
    (0xcf << 24) + (0x02 << 20);
  cattura_valu[conto_chiaro_nero][conto_donna_bianca] =
    (0xce << 24) + (0x02 << 20);
  cattura_valu[conto_scuro_nero][conto_donna_bianca] =
    (0xce << 24) + (0x02 << 20);
  cattura_valu[conto_torre_nero][conto_donna_bianca] =
    (0xcd << 24) + (0x02 << 20);
  cattura_valu[conto_donna_nera][conto_donna_bianca] =
    (0xcc << 24) + (0x01 << 20);

  cattura_valu[conto_pedone_nero][conto_torre_bianco] =
    (0xc8 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_nero][conto_torre_bianco] =
    (0xc7 << 24) + (0x02 << 20);
  cattura_valu[conto_chiaro_nero][conto_torre_bianco] =
    (0xc6 << 24) + (0x02 << 20);
  cattura_valu[conto_scuro_nero][conto_torre_bianco] =
    (0xc6 << 24) + (0x02 << 20);
  cattura_valu[conto_torre_nero][conto_torre_bianco] =
    (0xc5 << 24) + (0x01 << 20);
  cattura_valu[conto_donna_nera][conto_torre_bianco] =
    (0xc4 << 24) + (0x00 << 20);

  cattura_valu[conto_pedone_nero][conto_chiaro_bianco] =
    (0xc0 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_nero][conto_chiaro_bianco] =
    (0xbf << 24) + (0x01 << 20);
  cattura_valu[conto_chiaro_nero][conto_chiaro_bianco] =
    (0xbe << 24) + (0x01 << 20);
  cattura_valu[conto_scuro_nero][conto_chiaro_bianco] =
    (0xbe << 24) + (0x01 << 20);
  cattura_valu[conto_torre_nero][conto_chiaro_bianco] =
    (0xbd << 24) + (0x00 << 20);
  cattura_valu[conto_donna_nera][conto_chiaro_bianco] =
    (0xbc << 24) + (0x00 << 20);

  cattura_valu[conto_pedone_nero][conto_scuro_bianco] =
    (0xc0 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_nero][conto_scuro_bianco] =
    (0xbf << 24) + (0x01 << 20);
  cattura_valu[conto_chiaro_nero][conto_scuro_bianco] =
    (0xbe << 24) + (0x01 << 20);
  cattura_valu[conto_scuro_nero][conto_scuro_bianco] =
    (0xbe << 24) + (0x01 << 20);
  cattura_valu[conto_torre_nero][conto_scuro_bianco] =
    (0xbd << 24) + (0x00 << 20);
  cattura_valu[conto_donna_nera][conto_scuro_bianco] =
    (0xbc << 24) + (0x00 << 20);

  cattura_valu[conto_pedone_nero][conto_cavallo_bianco] =
    (0xb8 << 24) + (0x02 << 20);
  cattura_valu[conto_cavallo_nero][conto_cavallo_bianco] =
    (0xb7 << 24) + (0x01 << 20);
  cattura_valu[conto_chiaro_nero][conto_cavallo_bianco] =
    (0xb6 << 24) + (0x01 << 20);
  cattura_valu[conto_scuro_nero][conto_cavallo_bianco] =
    (0xb6 << 24) + (0x01 << 20);
  cattura_valu[conto_torre_nero][conto_cavallo_bianco] =
    (0xb5 << 24) + (0x00 << 20);
  cattura_valu[conto_donna_nera][conto_cavallo_bianco] =
    (0xb4 << 24) + (0x00 << 20);

  cattura_valu[conto_pedone_nero][conto_pedone_bianco] =
    (0xb0 << 24) + (0x01 << 20);
  cattura_valu[conto_cavallo_nero][conto_pedone_bianco] =
    (0xaf << 24) + (0x00 << 20);
  cattura_valu[conto_chiaro_nero][conto_pedone_bianco] =
    (0xae << 24) + (0x00 << 20);
  cattura_valu[conto_scuro_nero][conto_pedone_bianco] =
    (0xae << 24) + (0x00 << 20);
  cattura_valu[conto_torre_nero][conto_pedone_bianco] =
    (0xad << 24) + (0x00 << 20);
  cattura_valu[conto_donna_nera][conto_pedone_bianco] =
    (0xac << 24) + (0x00 << 20);

  cattura_valu[conto_re_nero][conto_donna_bianca] =
    (0xcb << 24) + (0x00 << 20);
  cattura_valu[conto_re_nero][conto_torre_bianco] =
    (0xc3 << 24) + (0x00 << 20);
  cattura_valu[conto_re_nero][conto_chiaro_bianco] =
    (0xbb << 24) + (0x00 << 20);
  cattura_valu[conto_re_nero][conto_scuro_bianco] =
    (0xbb << 24) + (0x00 << 20);
  cattura_valu[conto_re_nero][conto_cavallo_bianco] =
    (0xb3 << 24) + (0x00 << 20);
  cattura_valu[conto_re_nero][conto_pedone_bianco] =
    (0xab << 24) + (0x00 << 20);

  cattura_valu[conto_re_nero][0] = (0x07 << 24) + (0x00 << 15);
  cattura_valu[conto_pedone_nero][0] = (0x06 << 24) + (0x01 << 15);
  cattura_valu[conto_cavallo_nero][0] = (0x05 << 24) + (0x01 << 15);
  cattura_valu[conto_chiaro_nero][0] = (0x04 << 24) + (0x01 << 15);
  cattura_valu[conto_scuro_nero][0] = (0x04 << 24) + (0x01 << 15);
  cattura_valu[conto_torre_nero][0] = (0x03 << 24) + (0x01 << 15);
  cattura_valu[conto_donna_nera][0] = (0x02 << 24) + (0x01 << 15);
}
