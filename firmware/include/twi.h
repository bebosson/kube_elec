
#ifndef twi_h
#define twi_h

    #include <inttypes.h>

    //#define ATMEGA8

    #ifndef TWI_FREQ
    #define TWI_FREQ 100000L
    #endif

    #ifndef TWI_BUFFER_LENGTH
    #define TWI_BUFFER_LENGTH 32
    #endif

    #define TWI_READY 0
    #define TWI_MRX   1
    #define TWI_MTX   2
    #define TWI_SRX   3
    #define TWI_STX   4

    void twi_init(void);
    void twi_setAddress(uint8_t);
    uint8_t twi_readFrom(uint8_t, uint8_t*, uint8_t, uint8_t);
    uint8_t twi_writeTo(uint8_t, uint8_t*, uint8_t, uint8_t, uint8_t);
    uint8_t twi_transmit(const uint8_t*, uint8_t);
    void twi_attachSlaveRxEvent( void (*)(uint8_t*, int) );
    void twi_attachSlaveTxEvent( void (*)(void) );
    void twi_reply(uint8_t);
    void twi_stop(void);
    void twi_releaseBus(void);

    #include <avr/io.h>

    /** \file */
    /** \defgroup TWI bit mask definitions
        \code #include "twi.h" \endcode
    */
    /** \name TWSR values
     Mnemonics:
    <br>TW_MT_xxx - master transmitter
    <br>TW_MR_xxx - master receiver
    <br>TW_ST_xxx - slave transmitter
    <br>TW_SR_xxx - slave receiver
    */

    /*@{*/
    /* Master */
    /** \ingroup util_twi
        \def TW_START
        start condition transmitted */
    #define TW_START		0x08

    /** \ingroup util_twi
        \def TW_REP_START
        repeated start condition transmitted */
    #define TW_REP_START		0x10

    /* Master Transmitter */
    /** \ingroup util_twi
        \def TW_MT_SLA_ACK
        SLA+W transmitted, ACK received */
    #define TW_MT_SLA_ACK		0x18

    /** \ingroup util_twi
        \def TW_MT_SLA_NACK
        SLA+W transmitted, NACK received */
    #define TW_MT_SLA_NACK		0x20

    /** \ingroup util_twi
        \def TW_MT_DATA_ACK
        data transmitted, ACK received */
    #define TW_MT_DATA_ACK		0x28

    /** \ingroup util_twi
        \def TW_MT_DATA_NACK
        data transmitted, NACK received */
    #define TW_MT_DATA_NACK		0x30

    /** \ingroup util_twi
        \def TW_MT_ARB_LOST
        arbitration lost in SLA+W or data */
    #define TW_MT_ARB_LOST		0x38

    /* Master Receiver */
    /** \ingroup util_twi
        \def TW_MR_ARB_LOST
        arbitration lost in SLA+R or NACK */
    #define TW_MR_ARB_LOST		0x38

    /** \ingroup util_twi
        \def TW_MR_SLA_ACK
        SLA+R transmitted, ACK received */
    #define TW_MR_SLA_ACK		0x40

    /** \ingroup util_twi
        \def TW_MR_SLA_NACK
        SLA+R transmitted, NACK received */
    #define TW_MR_SLA_NACK		0x48

    /** \ingroup util_twi
        \def TW_MR_DATA_ACK
        data received, ACK returned */
    #define TW_MR_DATA_ACK		0x50

    /** \ingroup util_twi
        \def TW_MR_DATA_NACK
        data received, NACK returned */
    #define TW_MR_DATA_NACK		0x58

    /* Slave Transmitter */
    /** \ingroup util_twi
        \def TW_ST_SLA_ACK
        SLA+R received, ACK returned */
    #define TW_ST_SLA_ACK		0xA8

    /** \ingroup util_twi
        \def TW_ST_ARB_LOST_SLA_ACK
        arbitration lost in SLA+RW, SLA+R received, ACK returned */
    #define TW_ST_ARB_LOST_SLA_ACK	0xB0

    /** \ingroup util_twi
        \def TW_ST_DATA_ACK
        data transmitted, ACK received */
    #define TW_ST_DATA_ACK		0xB8

    /** \ingroup util_twi
        \def TW_ST_DATA_NACK
        data transmitted, NACK received */
    #define TW_ST_DATA_NACK		0xC0

    /** \ingroup util_twi
        \def TW_ST_LAST_DATA
        last data byte transmitted, ACK received */
    #define TW_ST_LAST_DATA		0xC8

    /* Slave Receiver */
    /** \ingroup util_twi
        \def TW_SR_SLA_ACK
        SLA+W received, ACK returned */
    #define TW_SR_SLA_ACK		0x60

    /** \ingroup util_twi
        \def TW_SR_ARB_LOST_SLA_ACK
        arbitration lost in SLA+RW, SLA+W received, ACK returned */
    #define TW_SR_ARB_LOST_SLA_ACK	0x68

    /** \ingroup util_twi
        \def TW_SR_GCALL_ACK
        general call received, ACK returned */
    #define TW_SR_GCALL_ACK		0x70

    /** \ingroup util_twi
        \def TW_SR_ARB_LOST_GCALL_ACK
        arbitration lost in SLA+RW, general call received, ACK returned */
    #define TW_SR_ARB_LOST_GCALL_ACK 0x78

    /** \ingroup util_twi
        \def TW_SR_DATA_ACK
        data received, ACK returned */
    #define TW_SR_DATA_ACK		0x80

    /** \ingroup util_twi
        \def TW_SR_DATA_NACK
        data received, NACK returned */
    #define TW_SR_DATA_NACK		0x88

    /** \ingroup util_twi
        \def TW_SR_GCALL_DATA_ACK
        general call data received, ACK returned */
    #define TW_SR_GCALL_DATA_ACK	0x90

    /** \ingroup util_twi
        \def TW_SR_GCALL_DATA_NACK
        general call data received, NACK returned */
    #define TW_SR_GCALL_DATA_NACK	0x98

    /** \ingroup util_twi
        \def TW_SR_STOP
        stop or repeated start condition received while selected */
    #define TW_SR_STOP		0xA0

    /* Misc */
    /** \ingroup util_twi
        \def TW_NO_INFO
        no state information available */
    #define TW_NO_INFO		0xF8

    /** \ingroup util_twi
        \def TW_BUS_ERROR
        illegal start or stop condition */
    #define TW_BUS_ERROR		0x00


    /**
     * \ingroup util_twi
     * \def TW_STATUS_MASK
     * The lower 3 bits of TWSR are reserved on the ATmega163.
     * The 2 LSB carry the prescaler bits on the newer ATmegas.
     */
    #define TW_STATUS_MASK		(_BV(TWS7)|_BV(TWS6)|_BV(TWS5)|_BV(TWS4)|\
                    _BV(TWS3))
    /**
     * \ingroup util_twi
     * \def TW_STATUS
     *
     * TWSR, masked by TW_STATUS_MASK
     */
    #define TW_STATUS		(TWSR0 & TW_STATUS_MASK)
    /*@}*/

    /**
     * \name R/~W bit in SLA+R/W address field.
     */

    /*@{*/
    /** \ingroup util_twi
        \def TW_READ
        SLA+R address */
    #define TW_READ		1

    /** \ingroup util_twi
        \def TW_WRITE
        SLA+W address */
    #define TW_WRITE	0
    /*@}*/

#endif