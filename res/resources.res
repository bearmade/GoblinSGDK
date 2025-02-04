TILESET tileset_Font "Font.png"
PALETTE palette_Font "Font.png"
IMAGE tileset1 "tilesSEGA2.png" BEST ALL
IMAGE tileset2 "tiles2.png" BEST ALL
IMAGE goblin "goblin1.png" NONE ALL

TILESET  our_tileset "tileset_1.png" BEST ALL
MAP our_level_map "layer_1.png" our_tileset BEST 0
PALETTE  our_palette "croakusTitleSEGA.png" 
IMAGE bg1 "croakusTitleSEGA.png" BEST ALL
IMAGE fg1 "croakusTitleSEGA.png" BEST ALL
IMAGE fg2 "croakusTitleSEGA.png" BEST ALL
SPRITE  our_sprite "heroSega.png" 2 2 FAST 10
SPRITE  merchantSprite "merchant.png" 2 2 FAST 10


XGM title_vgm "ikag.vgm"
XGM world_vgm "iKilledAGoblinWorld.vgm"
XGM battle_vgm "iKilledAGoblinBattle.vgm"
XGM victory_vgm "fanfare.vgm"
WAV sfx_swoosh "swoosh.wav" XGM
