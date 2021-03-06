#ifndef HD44352_H
#define HD44352_H

#include <QObject>
#include "common.h"


/***************************************************************************

        Hitachi HD44352 LCD controller

***************************************************************************/

typedef struct {
    UINT8 m_cursor[16];
    UINT8 m_cursor_x;
    UINT8 m_cursor_y;
    UINT8 m_cursor_lcd;
    UINT8 offset;
} HD44352_cursor;

typedef struct {
    UINT8 m_video_ram[2][0x300];
    UINT8 m_control_lines;
    UINT8 m_data_bus;
    UINT8 m_par[6];
    UINT8 m_state;
    UINT16 m_bank;
    UINT16 m_offset;
    UINT8 m_char_width;
    UINT8 m_lcd_on;
    UINT8 m_scroll;
    UINT32 m_contrast;

    UINT8 m_custom_char[4][8];		// 4 chars * 8 bytes
    UINT8 m_byte_count;
    UINT8 m_cursor_status;

    HD44352_cursor cursor[2];
} HD44352info;



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************


// ======================> hd44352_interface

struct hd44352_interface
{
    /*devcb_write_line*/	int m_on_cb;		// ON line
};

// ======================> hd44352_device
class CpcXXXX;


class CHD44352 : public QObject
{
public:
    // construction/destruction
    explicit CHD44352(CpcXXXX *parent,QString fnCharSet);

    CpcXXXX *pPC;
    // device interface
    QString fncharset;
    UINT8 data_read();
    void data_write(UINT8 data);
    void control_write(UINT8 data);
    void data_write4(UINT8 data);
    HD44352info getInfo();
    HD44352info *getInfoRef();
    void sync();

    quint64 on_timer_rate;
//    int video_update(bitmap_t &bitmap, const rectangle &cliprect);
static UINT8 compute_newval(UINT8 type, UINT8 oldval, UINT8 newval);

    // device-level overrides
    bool	init(void);						//initialize
    bool	exit(void);						//end
    void	Reset(void);
    bool	step(void);

    BYTE OP_bit;
    BYTE byteLenght;
    BYTE nibble;
    BYTE DataByte;



private:

    UINT8 get_char(UINT16 pos);
    UINT8 get_char4(BYTE car, BYTE col);

    HD44352info info;
    UINT8 charset[0x1000];

    QList<quint32> contrastList;

};


#endif
