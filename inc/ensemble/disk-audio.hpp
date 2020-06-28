/* License: MIT. See LICENSE in root directory. */

/**
 * @date 2020/06/29
 */

#pragma once

#include <afxres.h>

namespace Ensemble {

class DiskAudio {

protected:

    HWND* m_Wnd = nullptr;

    BOOL m_bOpened = false;

    DWORD m_nDeviceID = 0;

    MCI_PLAY_PARMS m_mciPlayParms {};

public:

    DiskAudio() = default;

    ~DiskAudio();

public:

    MCIERROR Open(HWND*);

    MCIERROR Close();

    MCIERROR Stop();

    MCIERROR Play(BYTE);

    MCIERROR Pause();

    MCIERROR ResumePlay();

    MCIERROR Eject();

    MCIERROR GetNumberOfTracks(BYTE&);

    MCIERROR IsMediaPresent(BOOL&);

    MCIERROR IsPlaying(BOOL&);

    MCIERROR GetLength(BYTE&, BYTE&);

    MCIERROR GetTrackLength(BYTE, BYTE&, BYTE&);

    MCIERROR GetPosition(BYTE&, BYTE&, BYTE&);

    MCIERROR DisplayErrorMsg(MCIERROR);
};

} // namespace Ensemble
