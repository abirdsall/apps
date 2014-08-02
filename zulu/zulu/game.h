#ifndef GAME_H
#define GAME_H

void gameInit();
void gameTick( f32 dt );
void gameTouch( const os::Touch* touches, s32 touchCount );
void gameDraw();
void gameKill();

#endif