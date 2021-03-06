#ifndef UPD16434_H
#define UPD16434_H

// Display Driver
#include "pobject.h"

typedef struct {
    BYTE    on_off;
    BYTE    dataPointer;
    BYTE    mode;
    BYTE    status;
    BYTE    imem[0x80];
} UPD16434info;



class CUPD16434{

public:
    enum  chipModel{
        UPD16434,
        UPD07728
    };

    const char*	GetClassName(){ return("CUPD16434");}
    CpcXXXX		*pPC;

    bool	init(void);						//initialize
    bool	exit(void);						//end
    void	Reset(void);
    bool	step(void);
    BYTE    instruction(quint8 cmd);
    BYTE    data(quint8 data);

    BYTE    cmd_LDPI(quint8 cmd);
    BYTE    cmd_WRCURS(quint8 cmd);
    BYTE    cmd_CLCURS(quint8 cmd);
    BYTE    cmd_MODE(quint8 cmd);
    BYTE    cmd_BSET(quint8 cmd, bool set);
    BYTE    cmd_SMM(quint8 cmd);
    BYTE    cmd_SFF(quint8 cmd);
    BYTE    cmd_DISPON(quint8 cmd);
    BYTE    cmd_DISPOFF(quint8 cmd);
    BYTE    cmd_STOP(quint8 cmd);

    bool getBit();

    UPD16434info info;
    bool    updated;


    void	Load_Internal(QXmlStreamReader *);
    void	save_internal(QXmlStreamWriter *);

    CUPD16434(CpcXXXX *parent, quint8 id=0, chipModel mod = UPD16434);
    ~CUPD16434();

    void	addretrace (void);


    void addChar(quint8 c, bool right);

    int memoryLimit;
    quint8 id;
    quint8 outputRegister;
    int outputBit;
};

#endif // UPD16434_H
