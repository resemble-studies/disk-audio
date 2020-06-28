/* License: MIT. See LICENSE in root directory. */

/**
 * @date 2020/06/29
 */

#include <ensemble/disk-audio.hpp>

namespace Ensemble {

////////////////////////////////////////////////////////////////////////////////////////////////////

DiskAudio::~DiskAudio()
{
    BOOL playing = FALSE;
    IsPlaying(playing);

    if (playing)
    {
        Stop();
    }

    Close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::Open(HWND* Wnd)
{
    m_Wnd = Wnd;

    if (m_bOpened)
    {
        Close();
    }

    MCI_OPEN_PARMSA mciOpenParms {};
    mciOpenParms.lpstrDeviceType = (LPCSTR)MCI_DEVTYPE_CD_AUDIO;

    MCIERROR dwError = ::mciSendCommand(
        0,
        MCI_OPEN,
        MCI_WAIT | MCI_OPEN_SHAREABLE | MCI_OPEN_TYPE_ID | MCI_OPEN_TYPE,
        (DWORD_PTR)&mciOpenParms);

    if (dwError == 0)
    {
        m_bOpened = TRUE;
        m_nDeviceID = mciOpenParms.wDeviceID;

        MCI_SET_PARMS mciSetInfo {};
        mciSetInfo.dwTimeFormat = MCI_FORMAT_TMSF;

        dwError = ::mciSendCommand(
            m_nDeviceID,
            MCI_SET,
            MCI_WAIT | MCI_SET_TIME_FORMAT,
            (DWORD_PTR)&mciSetInfo);
    }

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::Close()
{
    if (m_bOpened)
    {
        MCIERROR dwError = ::mciSendCommand(
            m_nDeviceID,
            MCI_CLOSE,
            MCI_WAIT,
            0);

        if (dwError == 0)
        {
            m_bOpened = FALSE;
        }

        return DisplayErrorMsg(dwError);
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::Stop()
{
    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_STOP,
        MCI_WAIT,
        0);

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::Play(BYTE nTrack)
{
    m_mciPlayParms.dwCallback = (DWORD)m_Wnd;
    m_mciPlayParms.dwFrom = nTrack;
    m_mciPlayParms.dwTo = nTrack + 1;

    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_PLAY,
        MCI_NOTIFY | MCI_FROM | MCI_TO,
        (DWORD_PTR)&m_mciPlayParms);

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::Pause()
{
    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_PAUSE,
        MCI_WAIT,
        0);

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::ResumePlay()
{
    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_PLAY,
        0,
        (DWORD_PTR)&m_mciPlayParms);

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::Eject()
{
    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_SET,
        MCI_SET_DOOR_OPEN,
        0);

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::GetNumberOfTracks(BYTE& nNumTracks)
{
    MCI_STATUS_PARMS status {};
    status.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;

    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_STATUS,
        MCI_STATUS_ITEM | MCI_WAIT,
        (DWORD_PTR)&status);

    nNumTracks = status.dwReturn;

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::IsMediaPresent(BOOL& bPresent)
{
    MCI_STATUS_PARMS mciStatusParms {};
    mciStatusParms.dwItem = MCI_STATUS_MEDIA_PRESENT;

    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_STATUS,
        MCI_STATUS_ITEM | MCI_WAIT,
        (DWORD_PTR)&mciStatusParms);

    bPresent = mciStatusParms.dwReturn != 0;

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::IsPlaying(BOOL& bPlaying)
{
    MCI_STATUS_PARMS mciStatusParms {};
    mciStatusParms.dwItem = MCI_STATUS_MODE;

    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_STATUS,
        MCI_STATUS_ITEM | MCI_WAIT,
        (DWORD_PTR)&mciStatusParms);

    bPlaying = mciStatusParms.dwReturn == MCI_MODE_PLAY;

    if (bPlaying)
    {
        mciStatusParms.dwItem = MCI_STATUS_CURRENT_TRACK;

        dwError = ::mciSendCommand(
            m_nDeviceID,
            MCI_STATUS,
            MCI_STATUS_ITEM | MCI_WAIT,
            (DWORD_PTR)&mciStatusParms);

        bPlaying = mciStatusParms.dwReturn != 0;
    }

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::GetLength(BYTE& nMinutes, BYTE& nSeconds)
{
    MCI_STATUS_PARMS mciStatusParms {};
    mciStatusParms.dwItem = MCI_STATUS_LENGTH;

    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_STATUS,
        MCI_STATUS_ITEM | MCI_WAIT,
        (DWORD_PTR)&mciStatusParms);

    nMinutes = MCI_MSF_MINUTE(mciStatusParms.dwReturn);
    nSeconds = MCI_MSF_SECOND(mciStatusParms.dwReturn);

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::GetTrackLength(BYTE nTrack, BYTE& nMinutes, BYTE& nSeconds)
{
    MCI_STATUS_PARMS mciStatusParms {};
    mciStatusParms.dwItem = MCI_STATUS_LENGTH;
    mciStatusParms.dwTrack = nTrack;

    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_STATUS,
        MCI_STATUS_ITEM | MCI_WAIT | MCI_TRACK,
        (DWORD_PTR)&mciStatusParms);

    nMinutes = MCI_MSF_MINUTE(mciStatusParms.dwReturn);
    nSeconds = MCI_MSF_SECOND(mciStatusParms.dwReturn);

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::GetPosition(BYTE& nTrack, BYTE& nMinutes, BYTE& nSeconds)
{
    MCI_STATUS_PARMS mciStatusParms {};
    mciStatusParms.dwItem = MCI_STATUS_POSITION;

    MCIERROR dwError = ::mciSendCommand(
        m_nDeviceID,
        MCI_STATUS,
        MCI_STATUS_ITEM | MCI_WAIT,
        (DWORD_PTR)&mciStatusParms);

    nTrack = MCI_TMSF_TRACK(mciStatusParms.dwReturn);
    nMinutes = MCI_TMSF_MINUTE(mciStatusParms.dwReturn);
    nSeconds = MCI_TMSF_SECOND(mciStatusParms.dwReturn);

    return DisplayErrorMsg(dwError);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MCIERROR DiskAudio::DisplayErrorMsg(MCIERROR dwError)
{
    CHAR szErrorMsg[0x100];

    if (dwError != 0 && !mciGetErrorString(dwError, szErrorMsg, sizeof(szErrorMsg)))
    {
        strcpy(szErrorMsg, "Unknown Error");
    }

    return dwError;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace Ensemble
