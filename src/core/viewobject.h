#ifndef VIEWOBJECT_H
#define VIEWOBJECT_H


#include <QMutex>

#if 1
#define MAINCLASS QWidget
#include <QWidget>
#else
#define MAINCLASS QOpenGLWidget
#endif

class Ckeyb;

enum Direction { TOPdir,LEFTdir, RIGHTdir, BOTTOMdir, NONEdir};
enum View {FRONTview,TOPview,LEFTview,RIGHTview,BOTTOMview,BACKview,BACKviewREV};

class CViewObject: public MAINCLASS {
    Q_OBJECT

    Q_PROPERTY(int angle READ angle WRITE setAngle)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom)

public:

    CViewObject(CViewObject *parent=0);
    virtual ~CViewObject();

    Ckeyb		*pKEYB;

    QString FrontFname,TopFname,LeftFname,RightFname,BottomFname,BackFname,BackGroundFname,BackGroundFnameBackLight,BackGroundFnameRef;
    QImage *FrontImage,*TopImage,*LeftImage,*RightImage,*BottomImage,*BackImage;
    QImage RightImageBackup,LeftImageBackup,TopImageBackup,BottomImageBackup,BackImageBackup;

    QMutex paintingImage;

    QImage* FinalImage;
    float internalImageRatio;
    QImage* BackgroundImageBackup;
    QPixmap mask;
    View currentView,targetView,animationView1,animationView2;
    Direction currentFlipDir;
    bool flipping;
    QImage* AnimatedImage;

    bool visible;

    void setAngle(int value);
    int angle() const { return m_angle; }
    int m_angle;

    void setZoom(qreal value);
    qreal zoom() const { return m_zoom; }
    qreal m_zoom;

    Direction borderClick(QPoint pt);
    QSize viewRect(View v);
    QSize currentViewRect();

    QSize targetSize;
    QImage *getViewImage(View v);

    void    MoveRel(QPointF);
    void    MoveAbs(QPointF p);
    QRectF rect();
    QPointF pos();

    float	posx();
    float   posy();
    void    setPosX(float);
    void    setPosY(float);

    float   getDX();
    float   getDY();
    float   getDZ();
    void    setDX(float v);
    void    setDY(float v);
    void    setDZ(float v);

    int     getDXmm();
    int     getDYmm();
    int     getDZmm();
    void    setDXmm(int v);
    void    setDYmm(int v);
    void    setDZmm(int v);

    void setSizeMM(int w, int h, int p);

    void setRotation(int v);
    int getRotation() { return rotation; }

    virtual bool InitDisplay(void);
    virtual void PreFlip(Direction dir,View targetView);
    virtual void PostFlip();
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void flip(Direction dir);
    virtual bool UpdateFinalImage(void) { return true;}
    virtual bool LastDrawFinalImage(void) { return true;}

    bool	Refresh_Display;

    static QImage * CreateImage(QSize size, QString fname, bool Hmirror=false, bool Vmirror=false, int angle=0);
    void changeGeometry(float newposx, float newposy, float newwidth, float newheight);
    void changeGeometrySize(float newposx, float newposy, float newwidth, float newheight);

    void InitView(View v);

    void showObject();
    void hideObject();

signals:
    void movePObject(CViewObject*,QPointF);
    void sizePObject(CViewObject*,QSizeF);
    void updatedPObject(CViewObject *);
    void rotPObject(CViewObject*,int angle);

public slots:
    void endAnimation();   
    void renderAnimation();

private:
    float		Pc_DX,Pc_DY;
    float	PosX,PosY;
    int		Pc_DX_mm,Pc_DY_mm,Pc_DZ_mm;		// PC size (mm
    int rotation;
};

#endif // VIEWOBJECT_H
