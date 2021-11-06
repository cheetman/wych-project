#include "vlclib.h"
#include <QFile>
#include <QMessageBox>

VlcLib::VlcLib(QWidget *parent) : QWidget(parent)
{
    const char * const vlc_args[] = {
            "--verbose=2",
            "--plugin-path=D:\\vlc-3.0.0\\plugins\\"
        };
    vlc_player = nullptr;
    vlc_inst = libvlc_new(sizeof (vlc_args) / sizeof (vlc_args[0]), vlc_args);
}

void VlcLib::playFile(const char* filename, HWND hwnd)
{
    if (QString(filename).startsWith("rtsp") ||
           QString(filename).startsWith("rtmp") ||
           QString(filename).startsWith("rtp") ||
           QString(filename).startsWith("tcp") ||
           QString(filename).startsWith("udp") ||
           QString(filename).startsWith("ftp") ||
           QString(filename).startsWith("http") ||
           QString(filename).startsWith("https"))
    {
        vlc_media = libvlc_media_new_location(vlc_inst, filename);
    }
    else if (!QFile::exists(filename))
    {
        QMessageBox::warning(this, "player", "local file not found");
        return ;
    }
    else if (QFile::exists(filename))
    {
        vlc_media = libvlc_media_new_path(vlc_inst, filename);
    }
    if (vlc_player && libvlc_media_player_is_playing(vlc_player))
    {
        libvlc_media_player_stop(vlc_player);
        libvlc_media_player_release(vlc_player);
        vlc_player = nullptr;
    }
    if (vlc_media)
    {
        vlc_player = libvlc_media_player_new_from_media(vlc_media);
    }
    if (vlc_player)
    {
        libvlc_media_release(vlc_media);
        if (hwnd)
        {
            libvlc_media_player_set_hwnd(vlc_player, hwnd);
        }
        libvlc_media_player_play(vlc_player);
    }
}

void VlcLib::play()
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    if (libvlc_media_player_is_playing(vlc_player))
    {
        libvlc_media_player_pause(vlc_player);
    }
    else
    {
        libvlc_media_player_play(vlc_player);
    }
}

void VlcLib::stop()
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    libvlc_state_t state = libvlc_media_player_get_state(vlc_player);
    switch (state) {
    case libvlc_Paused:
    case libvlc_Playing:
    case libvlc_Ended:
        libvlc_media_player_stop(vlc_player);
        break;
    default:
        break;
    }
}

void VlcLib::mute()
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    libvlc_audio_toggle_mute(vlc_player);
}

void VlcLib::release()
{

    libvlc_release(vlc_inst);
}

libvlc_time_t VlcLib::getLength()
{
    if (vlc_player == nullptr)
    {
        return 0;
    }
    return libvlc_media_player_get_length(vlc_player);
}

libvlc_time_t VlcLib::getTime()
{
    if (vlc_player == nullptr)
    {
        return 0;
    }
    return libvlc_media_player_get_time(vlc_player);
}

float VlcLib::getPosition()
{
    if (vlc_player == nullptr)
    {
        return 0;
    }
    return libvlc_media_player_get_position(vlc_player);
}

int VlcLib::getVolume()
{
    if (vlc_player == nullptr)
    {
        return 0;
    }
    return libvlc_audio_get_volume(vlc_player);
}

void VlcLib::setPosition(float pos)
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    libvlc_media_player_set_position(vlc_player, pos);
}

void VlcLib::setVolume(int vol)
{
    if (vlc_player == nullptr)
    {
        return ;
    }
    libvlc_audio_set_volume(vlc_player, vol);
}

bool VlcLib::isEnd()
{
    if (vlc_player == nullptr)
    {
        return false;
    }
    return libvlc_media_get_state(vlc_media) == libvlc_Ended;
}
