
static INLINE void
nullo_fare (tipo_posizione *POSIZIONE)
{
  POSIZIONE->nodes++;
  DINAMICO->pietras_salvas = DINAMICO->pietras;
  memcpy (DINAMICO + 1, DINAMICO, 64);
  DINAMICO++;
  DINAMICO->Zobrist ^= zobrist_mossa_bianca;
  POSIZIONE->bianco_en_mossa ^= 1;
  POSIZIONE->height++;
  DINAMICO->mossa50++;
  if (DINAMICO->en_passant)
    {
      DINAMICO->Zobrist ^= zobrist_ep[DINAMICO->en_passant & 7];
      DINAMICO->en_passant = 0;
    }
  DINAMICO->valu = -((DINAMICO - 1)->valu + valu_tempo);
  DINAMICO->valu_posizionale = (DINAMICO - 1)->valu_posizionale;
  DINAMICO->pigro = (DINAMICO - 1)->pigro;
  DINAMICO->pietras &= ~3;
  DINAMICO->mossa = 0;
  MUCCHIO[++mucchio_altezzo] = DINAMICO->Zobrist;
}

static INLINE void
nullo_annulla (tipo_posizione *POSIZIONE)
{
  DINAMICO--;
  mucchio_altezzo--;
  POSIZIONE->height--;
  POSIZIONE->bianco_en_mossa ^= 1;
  DINAMICO->pietras = DINAMICO->pietras_salvas;
}

#define nullo_riduzione 8
