

#define HEIGHT 51

#define LEFT_SECTION    138 
#define MID_SECTION     190
#define RIGHT_SECTION   83
#define DIAL_SECTION    11

#define LEFT_SECTION_RECT_WIDTH     116
#define RIGHT_SECTION_RECT_WIDTH    61

#define BODY_WIDTH (LEFT_SECTION_RECT_WIDTH + MID_SECTION + RIGHT_SECTION_RECT_WIDTH  + 2*DIAL_SECTION - 7/*??? huh ???*/)
#define BODY_HEIGHT PLAYER_WINDOW_HEIGHT

#define CAP_CIRCUMFERENCE PLAYER_WINDOW_HEIGHT

#define PLAYER_WINDOW_WIDTH (BODY_WIDTH + CAP_CIRCUMFERENCE + 5)
#define PLAYER_WINDOW_HEIGHT HEIGHT

typedef enum{
    Deactivated,
    Activated,
    Pressed,
    Selected
} ControlState;

typedef enum{
    Intro,
    Volume,
    TotalTime,
    CurrentTime,
    SeekTime,
    RemainingTime
} DisplayMode;

typedef struct DisplayInfo{
    DisplayMode     state;
    DisplayMode     oldstate;
    BOOL            dirty;
    char            path[MAX_PATH];
    int32           frame;
    int32           seekframe;
    int32           range;
    int32           hours;
    int32           minutes;
    int32           seconds;
    int32           seekhours;
    int32           seekminutes;
    int32           seekseconds;
    int32           totalhours;
    int32           totalminutes;
    int32           totalseconds;
    int32           volume;
    int32           scrollOffset;
    int32           introOffset;
	int32			totalSongs;
	int32			indexOfSong;
    bool            shuffled;
    bool            repeat;
    bool            repeatAll;

}DisplayInfo;

typedef struct ControlInfo{
    int32           control_id;
    int32           position;
    HRGN            region;
    ControlState    state;
    ControlState    oldstate;
    BOOL            enabled;
    BOOL            shown;
    BOOL            dirty;
    HBITMAP         bitmap;

}ControlInfo;

#define kPlayControl        0
#define kPauseControl       1
#define kLastControl        2
#define kNextControl        3
#define kStopControl        4
#define kModeControl        5
#define kMinimizeControl    6
#define kCloseControl       7
#define kRepeatControl      8
#define kShuffleControl     9
#define kOpenControl        10
#define kVolumeControl      11
#define kSeekControl        12
#define kPlaylistControl    13
#define kDisplayControl     14
#define kFinalControl       15

#define kNumControls        14

#ifdef __cplusplus
extern "C" {
#endif
LRESULT WINAPI MainWndProc( HWND hwnd, 
                            UINT msg, 
                            WPARAM wParam, 
                            LPARAM lParam );

void UpdateControls(HWND hwnd);
void UpdateDisplay(HWND hwnd);
bool NeedToScroll();


#ifdef __cplusplus
}
#endif
