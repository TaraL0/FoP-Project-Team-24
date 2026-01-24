
#ifndef SOUND_BLOCK_H
#define SOUND_BLOCK_H

void startSound (Sprite &sprite)
{
    Mix_PlayChannel (-1, sprite.m_sound, 0);
}

void playSoundUntilDone (Sprite &sprite)
{
    int channel = Mix_PlayChannel (0, sprite.m_sound, 0);
    while (Mix_Playing (channel)) {continue;}
}

void StopSound (Sprite &sprite)
{
    Mix_HaltChannel (-1);
}

void setSoundVolume (Sprite &sprite, int n)
{
    sprite.vol = n;
    Mix_Volume (-1, sprite.vol);
}

void changeSoundVolume (Sprite &sprite, int n)
{
    sprite.vol += n;
    Mix_Volume (-1, sprite.vol);
}

#endif //SOUND_BLOCK_H