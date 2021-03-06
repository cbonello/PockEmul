//TODO: Key management
// FIXME: Image ratio

#include <QPainter>

#include "fp100.h"
#include "Connect.h"
#include "Keyb.h"
#include "dialogconsole.h"
#include "watchpoint.h"
#include "Inter.h"
#include "Log.h"
#include "paperwidget.h"

#define DOWN	0
#define UP		1


Cfp100::Cfp100(CPObject *parent):Cce515p(parent) {
    Q_UNUSED(parent)

    setfrequency( 4000);
    ioFreq = 0;
    setcfgfname(QString("fp100"));
    BackGroundFname	= P_RES(":/fp100/fp100.png");

    pKEYB->setMap("x710.map");

    setDXmm(302);
    setDYmm(120);
    setDZmm(36);

    setDX(1078);
    setDY(817);

    printerACK = false;
    printerBUSY = false;

    capot = CreateImage(QSize(849,274),P_RES(":/fp100/fp100-capot.png"));
    head = CreateImage(QSize(79,161),P_RES(":/fp100/fp100head.png"));
    cable = CreateImage(QSize(75,10),P_RES(":/fp100/fp100cable.png"));

    margin = 40;
    Paper_DX = 960+2*margin;
    setPaperPos(QRect(154,26,731,300));
}

Cfp100::~Cfp100() {
    delete pCONNECTOR;
    delete pSavedCONNECTOR;
    delete capot;
    delete head;
    delete cable;
}

bool Cfp100::init(void) {

    Cce515p::init();

    pCONNECTOR = new Cconnector(this,36,0,Cconnector::Centronics_36,"Parrallel Connector",true,QPoint(631,468)); publish(pCONNECTOR);
    pSavedCONNECTOR = new Cconnector(this,36,1,Cconnector::Canon_15,"Saved Parrallel Connector",true,QPoint(631,468));

    QHash<int,QString> lbl;
    lbl[1] = "STROBE";
    lbl[2] = "D1";
    lbl[3] = "D2";
    lbl[4] = "D3";
    lbl[5] = "D4";
    lbl[6] = "D5";
    lbl[7] = "D6";
    lbl[8] = "D7";
    lbl[9] = "D8";
    lbl[10]= "ACK";
    lbl[11]= "BUSY";
    lbl[31]= "INIT";
    lbl[32]= "ERROR";
    WatchPoint.add(&pCONNECTOR_value,64,36,this,"// 36pins connector",lbl);

    paperWidget->hide();

    setPaperPos(QRect(154* internalImageRatio,26* internalImageRatio,731* internalImageRatio,300* internalImageRatio));

    return true;
}

bool Cfp100::run(void) {

    if (pKEYB->LastKey == K_PFEED) { PaperFeed();}

    // Si strobe change de status et passe bas vers haut, alors prelever les data
    // mettre le busy
    //
    if (GoUp(1)) {
        UINT8 car = (pCONNECTOR->Get_values() >> 1) & 0xFF;
        AddLog(LOG_PRINTER,tr("RECIEVED CHAR : %1").arg(car,2,16,QChar('0')));
        if (car != 0xff && car !=0x0a) Command( car );
        printerACK = true;
        pTIMER->resetTimer(6);
    }


    if (printerACK && (pTIMER->nsElapsedId(6) > 500)) {
        AddLog(LOG_PRINTER,tr("PRINTER printerStatusPort ACK OFF, BUSY OFF"));
        printerACK = false;
    }

    printerBUSY = (moveBuffer.size()>100) ? true:false;
    pCONNECTOR->Set_pin(10,printerACK);
    pCONNECTOR->Set_pin(11,printerBUSY);
    pCONNECTOR->Set_pin(32,true);

    pSavedCONNECTOR->Set_values(pCONNECTOR->Get_values());
    pCONNECTOR_value = pCONNECTOR->Get_values();

    Draw();

    return true;
}

bool Cfp100::exit(void) {

    Cce515p::exit();

    return true;
}

bool Cfp100::GoDown(int pin) {

    return (( pCONNECTOR->Get_pin(pin) == DOWN ) && (pSavedCONNECTOR->Get_pin(pin) == UP)) ? true:false;
}
bool Cfp100::GoUp(int pin) {

    return (( pCONNECTOR->Get_pin(pin) == UP ) && (pSavedCONNECTOR->Get_pin(pin) == DOWN)) ? true:false;
}
bool Cfp100::Change(int pin) {
    return (pCONNECTOR->Get_pin(pin) != pSavedCONNECTOR->Get_pin(pin) ) ? true:false;
}

void Cfp100::contextMenuEvent ( QContextMenuEvent * event )
{
    QMenu *menu= new QMenu(this);

    BuildContextMenu(menu);

    QMenu * menuPaper = menu->addMenu(tr("Paper"));
    menuPaper->addAction(tr("Copy"),paperWidget,SLOT(paperCopy()));
    menuPaper->addAction(tr("Cut"),paperWidget,SLOT(paperCut()));
    menuPaper->addAction(tr("Save Image ..."),paperWidget,SLOT(paperSaveImage()));
    menuPaper->addAction(tr("Save Text ..."),paperWidget,SLOT(paperSaveText()));

    menu->popup(event->globalPos () );
    event->accept();
}

void Cfp100::ComputeKey(KEYEVENT ke, int scancode, QMouseEvent *event)
{
    Q_UNUSED(ke)
    Q_UNUSED(scancode)


//    if (pKEYB->LastKey == K_PRINT_ON) {
//        printerSwitch = true;
//    }
//    if (pKEYB->LastKey == K_PRINT_OFF) {
//        printerSwitch = false;
//    }
}

bool Cfp100::UpdateFinalImage(void) {

//    Cce515p::UpdateFinalImage();

    paintingImage.lock();
    QPainter painter;
    painter.begin(FinalImage);


    painter.drawImage(QPoint(0,0),*BackgroundImage);

    float ratio = ( (float) paperWidget->width() ) / ( paperWidget->bufferImage->width() - paperWidget->getOffset().x() );

    QRect source = QRect( QPoint(paperWidget->getOffset().x() ,
                                 paperWidget->getOffset().y()  - paperWidget->height() / ratio ) ,
                          QPoint(paperWidget->bufferImage->width(),
                                 paperWidget->getOffset().y() +10)
                          );
//    MSG_ERROR(QString("%1 - %2").arg(source.width()).arg(PaperPos().width()));
    QRect _p = PaperPos();
    QRect _paper = QRect(_p.x()*internalImageRatio,_p.y()*internalImageRatio,_p.width()*internalImageRatio,_p.height()*internalImageRatio);
    painter.drawImage(_paper,
                      paperWidget->bufferImage->copy(source).scaled(_paper.size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation )
                      );
#if 1
    painter.setOpacity(0.5);
    painter.fillRect(_paper,Qt::black);
    painter.setOpacity(1);

    int _w = capot->width() * internalImageRatio;
    int _h = capot->height()* internalImageRatio;
    painter.drawImage(112 * internalImageRatio,145 * internalImageRatio,
                      capot->scaled(_w,_h,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    int offset = (lastX ) * ratio ;
    painter.drawImage((152+offset) * internalImageRatio,178 * internalImageRatio,
                      head->scaled(head->width() * internalImageRatio,
                                   head->height() * internalImageRatio,
                                   Qt::IgnoreAspectRatio,Qt::SmoothTransformation));       // Draw head
    painter.drawImage((793 - offset) * internalImageRatio,214 * internalImageRatio,
                      cable->scaled(cable->width() * internalImageRatio,
                                    cable->height() * internalImageRatio,
                                    Qt::IgnoreAspectRatio,Qt::SmoothTransformation));    // Draw cable
#endif
    painter.end();

    paintingImage.unlock();
    return true;
}

bool Cfp100::InitDisplay()
{
    Cce515p::InitDisplay();
    UpdateFinalImage();

    return true;
}

void Cfp100::paintEvent(QPaintEvent *event)
{
    Cce515p::paintEvent(event);
}

void Cfp100::CommandSlot(qint8 data) {
    Command(data);
}


