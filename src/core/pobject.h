#ifndef _POBJECT_H_
#define _POBJECT_H_

//#include <QObject>

#include <QWidget>
#include <QPoint>
#include <QImage>
#include <QMenu>
#include <QFile>
#include <QMutex>

#ifndef NO_SOUND
#include <QAudioOutput>
#endif

#include <QXmlStreamWriter>
#include <QPixmap>

#include "common.h"

class CpcXXXX;
class Ckeyb;
class CCPU;
class Ctimer;
class Clcdc;
class Cconnector;
class CExtensionArray;
class DialogKeyList;
class DialogDump;
class DialogDasm;
class DialogVKeyboard;
class QSwipeGesture;
class Cbus;
class COverlay;
class Filter;

#include "slot.h"
#include "viewobject.h"


#define PS_OFF	0
#define PS_RUN	1
#define PS_RSV	2
#define PS_PRO	3


#define KEY(c)	(pKEYB ? pKEYB->isKey(c) : false)


class CPObject:public CViewObject
{

    Q_OBJECT

public:
    enum KEYEVENT{ KEY_PRESSED,KEY_RELEASED};

    CPObject(CPObject *parent=0, QString _cfg=QString());
	virtual ~CPObject();
	
    virtual	bool	init();			// initialize
    virtual bool	InitDisplay(void);
    virtual bool	UpdateFinalImage(void);
    virtual bool	LastDrawFinalImage(void);
    virtual	bool	run(void);					// emulator main step
    virtual bool	exit();					// exit
    virtual bool	Set_Connector(Cbus *_bus = 0) { Q_UNUSED(_bus); return true; }
    virtual bool	Get_Connector(Cbus *_bus = 0) { Q_UNUSED(_bus); return true; }
    virtual void    keyPressEvent(QKeyEvent *event);
    virtual void    keyReleaseEvent(QKeyEvent *event);
    virtual void    TurnON();
    virtual void    TurnOFF();
    virtual int     initsound();
    virtual int     exitsound();
    virtual void    Reset() {}
    virtual bool    SaveSession_File(QXmlStreamWriter *xmlOut);
    virtual bool	LoadSession_File(QXmlStreamReader *);

    virtual bool	LoadConfig(QXmlStreamReader *);	// Load PC Configuration
    virtual bool	SaveConfig(QXmlStreamWriter *);	// Save PC Configuration
    virtual bool	LoadConfigExtra(QXmlStreamReader *);	// Load PC Configuration
    virtual bool	SaveConfigExtra(QXmlStreamWriter *);	// Save PC Configuration

    QString	Initial_Session_Fname;
    virtual bool Initial_Session_Load();
    virtual bool Initial_Session_Save();

    quint64 runRange(quint64);

    QList<CSlot> SlotList;


    virtual void ComputeKey(KEYEVENT ke = KEY_PRESSED,int scancode=0,QMouseEvent *event=0);
	CPObject	*Parent;
	CpcXXXX		*pPC;

    CCPU		*pCPU;
	Ctimer		*pTIMER;
	Clcdc		*pLCDC;					// create LCDC object



    Cbus   *bus,*busMem;
    QMutex       busMutex;



    FILE	*fp_log;

    Filter *my_filter;

    int rate;

    bool	off;
    bool closed;

    void setDisp_on(bool v);
    bool getdisp_onRaised();
    bool getDisp_on(void);
    int     ioFreq;
	
    void setName(QString val);
    QString getName(){ return Name;	}
    void setDisplayName(QString val);
    QString getDisplayName(){ return displayName;	}
	
    QString getcfgfname() { return cfgfname; }
    void	setcfgfname(QString s);
	
    QString getresName() { return resName; }
    void	setresName(QString s) { resName = s;}

    void    serialize(QXmlStreamWriter *,int id);

    void    MoveWithLinkedRel(QPointF);
    void    MoveWithLinkedAbs(QPointF p);


	QImage* BackgroundImage;



    /**
     * @brief Return the frequency of the emulated object
     *
     * @return int freq Hz
     *
     * @sa setfrequency
     */
    int		getfrequency() { return frequency; }

    void	setfrequency(int f);

	QList<Cconnector*> ConnList;
	void publish(Cconnector* newConn);
    void remove(Cconnector* newConn);

    virtual void BuildContextMenu(QMenu * menu);
    void computeWebLinksMenu(QMenu * menu);

    enum LINKTYPE {WEBLINK,DOCUMENT};
    void insertLinkAction(LINKTYPE type, QString desc, QString link);
    QMenu *contextMenu;
    QMenu *menupocket;
    QMenu *menuext;
    QMenu *menuconfig;
    QMenu *menucpuspeed;
    QMenu *menuAudioVolume;
    QMenu *menulcd;
    QMenu *menulink;
    QMenu *menuunlink;
    QMenu *menuweblink;
    QMenu *menuDocument;
	void computeLinkMenu(QMenu * menu);
	void computeUnLinkMenu(QMenu * menu);
	
    //TODO transform Extensions to QList
	CExtensionArray *extensionArray[5];

	bool	toDestroy;

	TransMap *KeyMap;
	int		KeyMapLenght;
	BYTE	LastKey;
    bool	Front;
    bool fullscreenMode;
	
	void fillSoundBuffer(BYTE val);
    QList<unsigned char> soundBuffer;
#ifndef NO_SOUND
    QAudioDeviceInfo* m_device;
    QAudioFormat    m_format;
    QAudioOutput*   m_audioOutput;
#endif
    QIODevice*      m_output;
    int DataFrequencyHz;
    int BufferSize;

    bool hardreset;

	DialogKeyList	*dialogkeylist;
    DialogDump		*dialogdump;
    DialogDasm      *dialogdasm;
    DialogVKeyboard  *dialogVKeyboard;

	void setCpu(int );

    bool getPower() { return Power; }
    void setPower(bool _p);
    qint8	PowerSwitch;

    bool forceStackOver,forceStackUnder;

    void    SwitchFrontBack(QPoint);
    bool    isFront() {return Front;}
    QList<Cconnector *> nearConnectors(Cconnector *refConnector,qint8 snaprange);
    void manageStackPos(QList<CPObject *> *);

    BYTE	*mem;
    int		InitMemValue;
    int		memsize;
    void	ClearRam(BYTE data) {	memset((void *)mem ,data,memsize); }	//initialize memory

		
    QString	SessionHeader;

    void	Mem_Load(QFile *file,BYTE s);
    void    Mem_Load(QXmlStreamReader *xmlIn,BYTE s);
    bool	Mem_Load(BYTE s);
    bool    Mem_Load(qint32 adr, QByteArray data);

    void	Mem_Save(QFile *file,BYTE s);
    void    Mem_Save(QXmlStreamWriter *xmlOut, BYTE s, bool dumphex= true);
    void	Mem_Save(BYTE s);

public:

    void paintEvent(QPaintEvent *);

    bool event(QEvent *event);
    void mouseDoubleClickEvent ( QMouseEvent * event );
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent( QMouseEvent * event );
    void wheelEvent(QWheelEvent *event);
    void focusInEvent (QFocusEvent *);
    void focusOutEvent ( QFocusEvent *  );


    virtual void raise();

    virtual void TurnCLOSE(void);

    virtual int mapKey(QKeyEvent *event);
    QRectF RectWithLinked();



    void swipeTriggered(QSwipeGesture *gesture);
    void manageBus(Cbus *_bus = 0);
    void writeBus(Cbus *bus,UINT32 *d, UINT32 data);
    void readBus(Cbus *bus, UINT32 *d, UINT32 *data);
    void FindAllLinked(CPObject *search, QList<CPObject *> *liste);

signals:
    void msgError(QString);
    void stackPosChanged();
    void ObjLinkedTo(Cconnector *, Cconnector *);
    void sigTurnOff();
    void sigTurnOn();
    void sigPower();
    void PowerChanged(CPObject*);

public slots:
    void slotDoubleClick(QPoint pos);
#ifndef NO_SOUND
    void audioStateChanged(QAudio::State state);
#endif

    void maximize(QPoint pos = QPoint());
    void minimize(QPoint pos = QPoint());
    void maximizeWidth();
    void maximizeHeight();
    void contextMenuEvent ( QContextMenuEvent * event );
	void slotExit();
	void KeyList();

    void slotCpu(QAction*);
    void slotAudioVolume(QAction*);

    void slotContrast(QAction *);

    void slotBackLight(bool _checked);
	void slotPower();
    void slotTurnOff();
    void slotTurnOn();
    void slotResetNow();
    void slotReset();
    void slotHardReset();
    void slotDetach();
    void slotLoadSession();
    void slotSaveSession();
	void Dump();
    void Dasm();
    void Postit();
    void VirtualKeyboard();
    void RefreshDisplay();
    void Overlay(QAction *action);


    void createShortcut();
private:
    bool	Power;
	bool	startKeyDrag;
	QPoint	KeyDrag;
	bool	startPosDrag;
	QPoint	PosDrag;
	int		frequency;

	QString Name;
    int     indexName;
    QString displayName;
	QString cfgfname;				// configration file name
    QString resName;
	

	int		iChanIndex;
    quint64	fillSoundBuffer_old_state;
    QByteArray audioBuff;
    QByteArray tempBuff;



    bool disp_on;
    bool disp_onRaised;

    quint64 resetAt,hardresetAt;

protected:
    // Overlay image
    QList<COverlay*> overlays;
    int currentOverlay;
    quint64 resetFiredState;
};


#endif
