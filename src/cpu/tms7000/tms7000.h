#ifndef TMS7000_H
#define TMS7000_H



/*****************************************************************************
 *
 *   tms7000.h (c header file)
 *   Portable TMS7000 emulator (Texas Instruments 7000)
 *
 *   Copyright tim lindner, all rights reserved.
 *
 *   - This source code is released as freeware for non-commercial purposes.
 *   - You are free to use and redistribute this code in modified or
 *     unmodified form, provided you list me in the credits.
 *   - If you modify this source code, you must add a notice to each modified
 *     source file that it has been changed.  If you're a nice person, you
 *     will clearly mark each change too.  :)
 *   - If you wish to use this for commercial purposes, please contact me at
 *     tlindner@macmess.org
 *   - This entire notice must remain in the source code.
 *
 *****************************************************************************/

#include "cpu.h"
#include "pcxxxx.h"

#define INT8 qint8

enum { TMS7000_PC=1, TMS7000_SP, TMS7000_ST };

enum
{
    /* note: INT2,4,5 are generated internally */
    TMS7000_INT1_LINE = 0,
    TMS7000_INT3_LINE,
    INPUT_LINE_RESET
};

enum
{
    TMS7000_PORTA = 0,      /* read-only on 70x0 */
    TMS7000_PORTB,          /* write-only */
    TMS7000_PORTC,
    TMS7000_PORTD,
    TMS7000_PORTE           /* TMS70C46 only */
};

enum TMS7000_Models
{
    TMS7000,
    TMS7020,
    TMS7020_EXL,
    TMS7040,
    TMS70C00,
    TMS70C20,
    TMS70C40,
    TMS70C46
};

// chip info flags
#define TMS7000_CHIP_IS_CMOS        0x01
#define TMS7000_CHIP_FAMILY_70X0    0x00
#define TMS7000_CHIP_FAMILY_70X2    0x02
#define TMS7000_CHIP_FAMILY_70CX2   0x04
#define TMS7000_CHIP_FAMILY_MASK    0x06

typedef struct {
    UINT32 m_info_flags;
    int m_icount;

    bool m_irq_state[2];
    bool m_idle_state;
    bool m_idle_halt;
    UINT16 m_pc;
    UINT8 m_sp;
    UINT8 m_sr;
    UINT8 m_op;

    UINT8 m_io_control[3];

//    emu_timer *m_timer_handle[2];
    UINT8 m_timer_data[2];
    UINT8 m_timer_control[2];
    int m_timer_decrementer[2];
    int m_timer_prescaler[2];
    UINT16 m_timer_capture_latch[2];

    UINT8 m_port_latch[4];
    UINT8 m_port_ddr[4];
} TMS7000info;

class Ctms7000 : public CCPU
{
public:

    TMS7000info info;

    // construction/destruction
    Ctms7000(CPObject *parent, TMS7000_Models mod = TMS7000) ;
    virtual ~Ctms7000();
//    tms7000_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, UINT32 clock, address_map_constructor internal, UINT32 info_flags, const char *shortname, const char *source);

//	DECLARE_READ8_MEMBER(tms7000_unmapped_rf_r) { logerror("%s: unmapped_rf_r @ $%04x\n", tag(), offset + 0x80); return 0; };
//	DECLARE_WRITE8_MEMBER(tms7000_unmapped_rf_w) { logerror("%s: unmapped_rf_w @ $%04x = $%02x\n", tag(), offset + 0x80, data); };

//    DECLARE_READ8_MEMBER(tms7000_pf_r);
//    DECLARE_WRITE8_MEMBER(tms7000_pf_w);
//    DECLARE_READ8_MEMBER(tms7002_pf_r) { return tms7000_pf_r(space, offset + 0x10); }
//    DECLARE_WRITE8_MEMBER(tms7002_pf_w) { tms7000_pf_w(space, offset + 0x10, data); }

    virtual void pf_write(UINT32 offset, UINT8 data);
    virtual UINT8 pf_read(UINT32 offset);
    void set_input_line(UINT8 line, UINT8 state);

    virtual	bool	init(void);						//initialize
    virtual	bool	exit(void);						//end
    virtual void	step(void);						//step
    virtual void	Reset(void);

    virtual	void	Load_Internal(QXmlStreamReader *);
    virtual	void	save_internal(QXmlStreamWriter *);

    virtual bool    Get_Xin(){ return true;}
    virtual void    Set_Xin(bool){}
    virtual bool    Get_Xout(){ return true;}
    virtual void    Set_Xout(bool){}
    virtual UINT32  get_PC();
    virtual void    Regs_Info(UINT8);

    bool chip_is_cmos() { return (info.m_info_flags & TMS7000_CHIP_IS_CMOS) ? true : false; }
    UINT32 chip_get_family() { return info.m_info_flags & TMS7000_CHIP_FAMILY_MASK; }
    bool chip_is_family_70x0() { return chip_get_family() == TMS7000_CHIP_FAMILY_70X0; }
    bool chip_is_family_70x2() { return chip_get_family() == TMS7000_CHIP_FAMILY_70X2; }
    bool chip_is_family_70cx2() { return chip_get_family() == TMS7000_CHIP_FAMILY_70CX2; }

protected:
    // device-level overrides


    // device_execute_interface overrides
//    virtual UINT64 execute_clocks_to_cycles(UINT64 clocks) const { return (clocks + 2 - 1) / 2; } // internal /2 divider
//    virtual UINT64 execute_cycles_to_clocks(UINT64 cycles) const { return (cycles * 2); } // internal /2 divider
    virtual UINT32 execute_min_cycles() const { return 5; }
    virtual UINT32 execute_max_cycles() const { return 49; }
    virtual UINT32 execute_input_lines() const { return 2; }
    virtual void execute_run();
    virtual void execute_set_input(int inputnum, int state);

    // device_memory_interface overrides
//    virtual const address_space_config *memory_space_config(address_spacenum spacenum = AS_0) const { return (spacenum == AS_PROGRAM) ? &m_program_config : ( (spacenum == AS_IO) ? &m_io_config : NULL ); }

    // device_state_interface overrides
//    void state_string_export(const device_state_entry &entry, astring &string);

    // device_disasm_interface overrides
//    virtual UINT32 disasm_min_opcode_bytes() const { return 1; }
//    virtual UINT32 disasm_max_opcode_bytes() const { return 4; }
//    virtual offs_t disasm_disassemble(char *buffer, offs_t pc, const UINT8 *oprom, const UINT8 *opram, UINT32 options);

    virtual void execute_one(UINT8 op);

//    address_space_config m_program_config;
//    address_space_config m_io_config;



//    address_space *m_program;
//    direct_read_data *m_direct;
//    address_space *m_io;


    void flag_ext_interrupt(int irqline);
    void check_interrupts();
    void do_interrupt(int irqline);

//    TIMER_CALLBACK_MEMBER(simple_timer_cb);
    void timer_run(int tmr);
    void timer_reload(int tmr);
    void timer_tick_pre(int tmr);
    void timer_tick_low(int tmr);

    // internal read/write
    inline UINT8 read_r8(UINT8 address) { return pPC->Get_8(address); }
    inline void write_r8(UINT8 address, UINT8 data) { pPC->Set_8(address, data); }
    inline UINT16 read_r16(UINT8 address) { return pPC->Get_8((address - 1) & 0xff) << 8 | pPC->Get_8(address); }
    inline void write_r16(UINT8 address, UINT16 data) { pPC->Set_8((address - 1) & 0xff, data >> 8 & 0xff); pPC->Set_8(address, data & 0xff); }

    inline UINT8 read_p(UINT8 address) { return pPC->Get_8(0x100 + address); }
    inline void write_p(UINT8 address, UINT8 data) { pPC->Set_8(0x100 + address, data); }

    inline UINT8 read_mem8(UINT16 address) { return pPC->Get_8(address); }
    inline void write_mem8(UINT16 address, UINT8 data) { pPC->Set_8(address, data); }
    inline UINT16 read_mem16(UINT16 address) { return pPC->Get_8(address) << 8 | pPC->Get_8((address + 1) & 0xffff); }
    inline void write_mem16(UINT16 address, UINT16 data) { pPC->Set_8(address, data >> 8 & 0xff); pPC->Set_8((address + 1) & 0xffff, data & 0xff); }

    inline UINT8 imm8() { return pPC->Get_8(info.m_pc++); }
    inline UINT16 imm16() { UINT16 ret = pPC->Get_8(info.m_pc++) << 8; return ret | pPC->Get_8(info.m_pc++); }

    inline UINT8 pull8() { return pPC->Get_8(info.m_sp--); }
    inline void push8(UINT8 data) { pPC->Set_8(++info.m_sp, data); }
    inline UINT16 pull16() { UINT16 ret = pPC->Get_8(info.m_sp--); return ret | pPC->Get_8(info.m_sp--) << 8; }
    inline void push16(UINT16 data) { pPC->Set_8(++info.m_sp, data >> 8 & 0xff); pPC->Set_8(++info.m_sp, data & 0xff); }

    // opcode handlers
    void br_dir();
    void br_inx();
    void br_ind();
    void call_dir();
    void call_inx();
    void call_ind();
    void cmpa_dir();
    void cmpa_inx();
    void cmpa_ind();
    void decd_a();
    void decd_b();
    void decd_r();
    void dint();
    void eint();
    void idle();
    void lda_dir();
    void lda_inx();
    void lda_ind();
    void ldsp();
    void movd_dir();
    void movd_inx();
    void movd_ind();
    void nop();
    void pop_a();
    void pop_b();
    void pop_r();
    void pop_st();
    void push_a();
    void push_b();
    void push_r();
    void push_st();
    void reti();
    void rets();
    void setc();
    void sta_dir();
    void sta_inx();
    void sta_ind();
    void stsp();
    void trap(UINT8 address);
    void illegal(UINT8 op);

    typedef int (Ctms7000::*op_func)(UINT8, UINT8);
    int op_clr(UINT8 param1, UINT8 param2);
    int op_dec(UINT8 param1, UINT8 param2);
    int op_inc(UINT8 param1, UINT8 param2);
    int op_inv(UINT8 param1, UINT8 param2);
    int op_rl(UINT8 param1, UINT8 param2);
    int op_rlc(UINT8 param1, UINT8 param2);
    int op_rr(UINT8 param1, UINT8 param2);
    int op_rrc(UINT8 param1, UINT8 param2);
    int op_swap(UINT8 param1, UINT8 param2);
    int op_xchb(UINT8 param1, UINT8 param2);

    int op_adc(UINT8 param1, UINT8 param2);
    int op_add(UINT8 param1, UINT8 param2);
    int op_and(UINT8 param1, UINT8 param2);
    int op_cmp(UINT8 param1, UINT8 param2);
    int op_dac(UINT8 param1, UINT8 param2);
    int op_dsb(UINT8 param1, UINT8 param2);
    int op_mpy(UINT8 param1, UINT8 param2);
    int op_mov(UINT8 param1, UINT8 param2);
    int op_or(UINT8 param1, UINT8 param2);
    int op_sbb(UINT8 param1, UINT8 param2);
    int op_sub(UINT8 param1, UINT8 param2);
    int op_xor(UINT8 param1, UINT8 param2);

    inline void shortbranch(bool check);
    inline void jmp(bool check);
    int op_djnz(UINT8 param1, UINT8 param2);
    int op_btjo(UINT8 param1, UINT8 param2);
    int op_btjz(UINT8 param1, UINT8 param2);

    void am_a(op_func op);
    void am_b(op_func op);
    void am_r(op_func op);
    void am_a2a(op_func op);
    void am_a2b(op_func op);
    void am_a2r(op_func op);
    void am_a2p(op_func op);
    void am_b2a(op_func op);
    void am_b2b(op_func op);
    void am_b2r(op_func op);
    void am_b2p(op_func op);
    void am_r2a(op_func op);
    void am_r2b(op_func op);
    void am_r2r(op_func op);
    void am_i2a(op_func op);
    void am_i2b(op_func op);
    void am_i2r(op_func op);
    void am_i2p(op_func op);
    void am_p2a(op_func op);
    void am_p2b(op_func op);
};


class Ctms7020 : public Ctms7000
{
public:
    Ctms7020(CPObject *parent, TMS7000_Models mod);
};


class Ctms7020_exl : public Ctms7000
{
public:
    Ctms7020_exl(CPObject *parent, TMS7000_Models mod);
protected:
    virtual void execute_one(UINT8 op);

private:
    void lvdp();
};


class Ctms7040 : public Ctms7000
{
public:
    Ctms7040(CPObject *parent, TMS7000_Models mod);
};


class Ctms70c00 : public Ctms7000
{
public:
    Ctms70c00(CPObject *parent);
};


class Ctms70c20 : public Ctms7000
{
public:
    Ctms70c20(CPObject *parent);
};


class Ctms70c40 : public Ctms7000
{
public:
    Ctms70c40(CPObject *parent, TMS7000_Models mod);
};


class Ctms7001 : public Ctms7000
{
public:
    Ctms7001(CPObject *parent, TMS7000_Models mod);
};


class Ctms7041 : public Ctms7000
{
public:
    Ctms7041(CPObject *parent, TMS7000_Models mod);
};


class Ctms7002 : public Ctms7000
{
public:
    Ctms7002(CPObject *parent, TMS7000_Models mod);
};


class Ctms7042 : public Ctms7000
{
public:
    Ctms7042(CPObject *parent, TMS7000_Models mod);
};

class Ctms70c46 : public Ctms7000
{
public:
    Ctms70c46(CPObject *parent, TMS7000_Models mod = TMS70C46);

//	DECLARE_READ8_MEMBER(control_r);
//	DECLARE_WRITE8_MEMBER(control_w);

//	DECLARE_READ8_MEMBER(dockbus_status_r);
//	DECLARE_WRITE8_MEMBER(dockbus_status_w);
//	DECLARE_READ8_MEMBER(dockbus_data_r);
//	DECLARE_WRITE8_MEMBER(dockbus_data_w);

//	// access I/O port E if databus is disabled
    UINT8 e_bus_data_r() { return ((m_control & 0x20) ? 0xff : portE); }
    void e_bus_data_w(UINT8 data) { if (~m_control & 0x20) portE=data; }

    // device-level overrides
    virtual bool init();
    virtual void Reset();

    virtual void pf_write(UINT32 offset, UINT8 data);
    virtual UINT8 pf_read(UINT32 offset);

    UINT8 control_r();
    void control_w(UINT8 data);

protected:


private:
    UINT8 m_control;
    UINT8 portE;
};

//extern const device_type TMS7000;
//extern const device_type TMS7020;
//extern const device_type TMS7020_EXL;
//extern const device_type TMS7040;
//extern const device_type TMS70C00;
//extern const device_type TMS70C20;
//extern const device_type TMS70C40;
//extern const device_type TMS7001;
//extern const device_type TMS7041;
//extern const device_type TMS7002;
//extern const device_type TMS7042;




#endif // TMS7000_H
