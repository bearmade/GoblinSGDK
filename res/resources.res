TILESET tileset_Font "Font.png"
PALETTE palette_Font "Font.png"
IMAGE tileset1 "tilesSEGA2.png" BEST ALL
IMAGE tileset2 "tiles2.png" BEST ALL
IMAGE goblin "goblin1.png" NONE ALL
IMAGE boxTiles "boxTiles.png" NONE ALL

TILESET  our_tileset "tileset_1.png" BEST ALL
MAP our_level_map "layer_1.png" our_tileset BEST 0
PALETTE  our_palette "croakusTitleSEGA.png" 
IMAGE bg1 "croakusTitleSEGA.png" BEST ALL
IMAGE fg1 "croakusTitleSEGA.png" BEST ALL
IMAGE fg2 "croakusTitleSEGA.png" BEST ALL
IMAGE title "goglinTitle2.png" NONE ALL
IMAGE titleBase "goblinTitleBase3.png" NONE ALL 
IMAGE battleBack "backdrop1.png" NONE ALL 
IMAGE battleBack2 "backdrop2.png" NONE ALL 
IMAGE titleLetters "goblinTitleLetters2.png" BEST ALL
IMAGE face "face.png" NONE ALL
PALETTE  mytitle "goglinTitle2.png"
SPRITE  our_sprite "heroSega.png" 2 2 FAST 10
SPRITE  merchantSprite "merchant.png" 2 2 FAST 10
SPRITE  goblinSprite "goblins.png" 6 6 NONE 0


XGM title_vgm "ikag.vgm"
XGM world_vgm "iKilledAGoblinWorld.vgm"
XGM battle_vgm "iKilledAGoblinBattle.vgm"
XGM victory_vgm "fanfare.vgm"
WAV sfx_swoosh "swoosh.wav" XGM
