#ifdef __cplusplus
extern "C" {
#endif
LRESULT WINAPI MainWndProc( HWND hwnd, 
                            UINT msg, 
                            WPARAM wParam, 
                            LPARAM lParam );
#ifdef __cplusplus
}
#endif

#define HEIGHT 51

#define LEFT_SECTION    148 
#define MID_SECTION     168
#define RIGHT_SECTION   83
#define DIAL_SECTION    11

#define LEFT_SECTION_RECT_WIDTH     126
#define RIGHT_SECTION_RECT_WIDTH    61

#define BODY_WIDTH LEFT_SECTION_RECT_WIDTH + MID_SECTION + RIGHT_SECTION_RECT_WIDTH  + 2*DIAL_SECTION
#define BODY_HEIGHT PLAYER_WINDOW_HEIGHT

#define CAP_CIRCUMFERENCE PLAYER_WINDOW_HEIGHT

#define PLAYER_WINDOW_WIDTH BODY_WIDTH + CAP_CIRCUMFERENCE
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

}DisplayInfo;

typedef struct ControlInfo{
    int32           control_id;
    int32           position;
    HRGN            region;
    ControlState    state;
    ControlState    oldstate;
    BOOL            enabled;
    BOOL            dirty;
    HBITMAP         bitmap;

}ControlInfo;

#define kStopControl        0
#define kPlayControl        1
#define kLastControl        2
#define kNextControl        3
#define kPauseControl       4
#define kModeControl        5
#define kMinimizeControl    6
#define kCloseControl       7
#define kVolumeControl      8
#define kSeekControl        9
#define kPlaylistControl    10
#define kDisplayControl     11


#define kNumControls        10

#define INTRO_COUNT         12