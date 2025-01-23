TILESET tileset_Font "Font.png"
PALETTE palette_Font "Font.png"
IMAGE tileset1 "tilesSEGA2.png" BEST ALL
IMAGE tileset2 "tiles2.png" BEST ALL

TILESET  our_tileset "tileset_1.png" BEST ALL
MAP our_level_map "layer_1.png" our_tileset BEST 0
PALETTE  our_palette "layer_1.png" 
IMAGE bg1 "layer_1.png" BEST ALL
IMAGE fg1 "croakusTitleSEGA.png" BEST ALL
SPRITE  our_sprite "sg_leggup_walk2.png" 2 2 FAST 10

XGM test_vgm "ikag.vgm"
WAV sfx_swoosh "swoosh.wav" XGM
