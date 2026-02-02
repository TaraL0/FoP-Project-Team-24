
#ifndef SOUND_BLOCK_H
#define SOUND_BLOCK_H

void startSound (Sprite &sprite)
{
    Mix_PlayChannel (-1, sprite.m_sound, 0);
}

void playSoundUntilDone (Sprite &sprite)
{
    int channel = Mix_PlayChannel (-1, sprite.m_sound, 0);
    while (Mix_Playing (channel)) {continue;}
}

void StopSound (Sprite &sprite)
{
    Mix_HaltChannel (-1);
}

void setSoundVolume (Sprite &sprite, double n)
{
    if (n > 100) {sprite.vol = 128;}
    else if (n < 0) {sprite.vol = 0;}
    else {sprite.vol = n * 128 / 100.0;}
    Mix_VolumeChunk (sprite.m_sound, sprite.vol);
}

void changeSoundVolume (Sprite &sprite, double n)
{
    n *= 1.28;
    if (sprite.vol + n > 128) {sprite.vol = 128;}
    else if (sprite.vol + n < 0) {sprite.vol = 0;}
    else {sprite.vol += n;}
    Mix_VolumeChunk (sprite.m_sound, sprite.vol);
}

#endif //SOUND_BLOCK_H