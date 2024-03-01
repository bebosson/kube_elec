#include <inttypes.h>
#include "avr/io.h"
#include <stdbool.h>
#include "pins_arduino.h"
#include "twi.h"

static volatile uint8_t twi_state;
static volatile uint8_t twi_slarw;
static volatile uint8_t twi_sendStop;			// should the transaction end with a stop
static volatile uint8_t twi_inRepStart;			// in the middle of a repeated start

static void (*twi_onSlaveTransmit)(void);
static void (*twi_onSlaveReceive)(uint8_t*, int);

static uint8_t twi_masterBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_masterBufferIndex;
static volatile uint8_t twi_masterBufferLength;

static uint8_t twi_txBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_txBufferIndex;
static volatile uint8_t twi_txBufferLength;

static uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_rxBufferIndex;

static volatile uint8_t twi_error;

/* 
 * Function twi_init
 * Desc     readys twi pins and sets twi bitrate
 * Input    none
 * Output   none
 */
void twi_init(void)
{
  // initialize state
  twi_state = TWI_READY;
  twi_sendStop = true;		// default value
  twi_inRepStart = false;
  
  // activate internal pullups for twi.
  //   digitalWrite(SCL, 1);
  //   digitalWrite(SDA, 1);
  PORTC |= (1 << SDA);
  PORTC |= (1 << SCL);


  // initialize twi prescaler and bit rate
//   cbi(TWSR, TWPS0);
//   cbi(TWSR, TWPS1);
  TWSR0 &= ~(1 << TWPS0);
  TWSR0 &= ~(1 << TWPS1);
  TWBR0 = ((F_CPU / TWI_FREQ) - 16) / 2;

  /* twi bit rate formula from atmega128 manual pg 204
  SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
  note: TWBR should be 10 or higher for master mode
  It is 72 for a 16mhz Wiring board with 100kHz TWI */

  // enable twi module, acks, and twi interrupt
  TWCR0 = (1 << TWEN) | (1 << TWIE) | (1 << TWEA);
}

/* 
 * Function twi_slaveInit
 * Desc     sets slave address and enables interrupt
 * Input    none
 * Output   none
 */
void twi_setAddress(uint8_t address)
{
  // set twi slave address (skip over TWGCE bit)
  TWAR0 = address << 1;
}

/* 
 * Function twi_readFrom
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes to read into array
 *          sendStop: Boolean indicating whether to send a stop at the end
 * Output   number of bytes read
 */
uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return 0;
  }

  // wait until twi is ready, become master receiver
  while(TWI_READY != twi_state){
    continue;
  }
  twi_state = TWI_MRX;
  twi_sendStop = sendStop;
  // reset error state (0xFF.. no error occured)
  twi_error = 0xFF;

  // initialize buffer iteration vars
  twi_masterBufferIndex = 0;
  twi_masterBufferLength = length-1;  // This is not intuitive, read on...
  // On receive, the previously configured ACK/NACK setting is transmitted in
  // response to the received byte before the interrupt is signalled. 
  // Therefor we must actually set NACK when the _next_ to last byte is
  // received, causing that NACK to be sent in response to receiving the last
  // expected byte of data.

  // build sla+w, slave device address + w bit
  twi_slarw = TW_READ;
  twi_slarw |= address << 1;

  if (true == twi_inRepStart) {
    // if we're in the repeated start state, then we've already sent the start,
    // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
    // We need to remove ourselves from the repeated start state before we enable interrupts,
    // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
    // up. Also, don't enable the START interrupt. There may be one pending from the 
    // repeated start that we sent outselves, and that would really confuse things.
    twi_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
    TWDR0 = twi_slarw;
    TWCR0 = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);	// enable INTs, but not START
  }
  else
    // send start condition
    TWCR0 = (1 << TWEN) | (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWSTA);

  // wait for read operation to complete
  while(TWI_MRX == twi_state){
    continue;
  }

  if (twi_masterBufferIndex < length)
    length = twi_masterBufferIndex;

  // copy twi buffer to data
  for(i = 0; i < length; ++i){
    data[i] = twi_masterBuffer[i];
  }
	
  return length;
}

/* 
 * Function twi_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 *          sendStop: boolean indicating whether or not to send a stop at the end
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
  uint8_t i;
// uart_tx('1');
  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return 1;
  }

  // wait until twi is ready, become master transmitter
  while(TWI_READY != twi_state){
    continue;
  }
  twi_state = TWI_MTX;
  twi_sendStop = sendStop;
  // reset error state (0xFF.. no error occured)
  twi_error = 0xFF;
  // initialize buffer iteration vars
  twi_masterBufferIndex = 0;
  twi_masterBufferLength = length;
  
  // copy data to twi buffer
  for(i = 0; i < length; ++i){
    twi_masterBuffer[i] = data[i];
  }
  
  // build sla+w, slave device address + w bit
  twi_slarw = TW_WRITE;
  twi_slarw |= address << 1;
  
  // if we're in a repeated start, then we've already sent the START
  // in the ISR. Don't do it again.
  //
  if (true == twi_inRepStart) {
    // if we're in the repeated start state, then we've already sent the start,
    // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
    // We need to remove ourselves from the repeated start state before we enable interrupts,
    // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
    // up. Also, don't enable the START interrupt. There may be one pending from the 
    // repeated start that we sent outselves, and that would really confuse things.
    twi_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
    TWDR0 = twi_slarw;				
    TWCR0 = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);	// enable INTs, but not START
  }
  else
    // send start condition
    TWCR0 = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE) | (1 << TWSTA);	// enable INTs
    // uart_tx('2');
  // wait for write operation to complete
  while(wait && (TWI_MTX == twi_state)){
    continue;
  }
//   uart_tx('3');
  if (twi_error == 0xFF) {
    //   uart_strx(" ret0k ");
      return 0;	// success
  }
  else if (twi_error == TW_MT_SLA_NACK) {
    //   uart_strx(" ErrSLA ");
    return 2;	// error: address send, nack received
  }
  else if (twi_error == TW_MT_DATA_NACK)  {
    //   uart_strx(" ErrDATA ");
        return 3;	// error: data send, nack received
  }
  else {
    //   uart_strx(" Err4");
          return 4;	// other twi error
  }
}

/* 
 * Function twi_transmit
 * Desc     fills slave tx buffer with data
 *          must be called in slave tx event callback
 * Input    data: pointer to byte array
 *          length: number of bytes in array
 * Output   1 length too long for buffer
 *          2 not slave transmitter
 *          0 ok
 */
uint8_t twi_transmit(const uint8_t* data, uint8_t length)
{
  uint8_t i;

  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return 1;
  }
  
  // ensure we are currently a slave transmitter
  if(TWI_STX != twi_state){
    return 2;
  }
  
  // set length and copy data into tx buffer
  twi_txBufferLength = length;
  for(i = 0; i < length; ++i){
    twi_txBuffer[i] = data[i];
  }
  
  return 0;
}

/* 
 * Function twi_attachSlaveRxEvent
 * Desc     sets function called before a slave read operation
 * Input    function: callback function to use
 * Output   none
 */
void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
  twi_onSlaveReceive = function;
}

/* 
 * Function twi_attachSlaveTxEvent
 * Desc     sets function called before a slave write operation
 * Input    function: callback function to use
 * Output   none
 */
void twi_attachSlaveTxEvent( void (*function)(void) )
{
  twi_onSlaveTransmit = function;
}

/* 
 * Function twi_reply
 * Desc     sends byte or readys receive line
 * Input    ack: byte indicating to ack or to nack
 * Output   none
 */
void twi_reply(uint8_t ack)
{
  // transmit master read ready signal, with or without ack
  if(ack){
    TWCR0 = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA);
  }else{
	  TWCR0 = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
  }
}

/* 
 * Function twi_stop
 * Desc     relinquishes bus master status
 * Input    none
 * Output   none
 */
void twi_stop(void)
{
  // send stop condition
  TWCR0 = (1 << TWEN) | (1 << TWIE) | (1 << TWEA) | (1 << TWINT) | (1 << TWSTO);

  // wait for stop condition to be exectued on bus
  // TWINT is not set after a stop condition!
  while(TWCR0 & (1 << TWSTO)){
    continue;
  }

  // update twi state
  twi_state = TWI_READY;
}

/* 
 * Function twi_releaseBus
 * Desc     releases bus control
 * Input    none
 * Output   none
 */
void twi_releaseBus(void)
{
  // release bus
  TWCR0 = (1 << TWEN) | (1 << TWIE) | (1 << TWEA) | (1 << TWINT);

  // update twi state
  twi_state = TWI_READY;
}

// ISR(TWI0_vect)
__attribute__ ((signal)) void TWI0_vect()
{
  switch(TW_STATUS){
    // All Master
    case TW_START:     // sent start condition
    // uart_strx(" c1 ");
    case TW_REP_START: // sent repeated start condition
      // copy device address and r/w bit to output register and ack
    //   uart_strx(" c2 ");
      TWDR0 = twi_slarw;
      twi_reply(1);
      break;

    // Master Transmitter
    case TW_MT_SLA_ACK:  // slave receiver acked address
        // uart_strx(" c3 ");
    case TW_MT_DATA_ACK: // slave receiver acked data
        // uart_strx(" c4 ");
      // if there is data to send, send it, otherwise stop 
      if(twi_masterBufferIndex < twi_masterBufferLength){
        // copy data to output register and ack
        TWDR0 = twi_masterBuffer[twi_masterBufferIndex++];
        twi_reply(1);
      }else{
	if (twi_sendStop)
          twi_stop();
	else {
	  twi_inRepStart = true;	// we're gonna send the START
	  // don't enable the interrupt. We'll generate the start, but we 
	  // avoid handling the interrupt until we're in the next transaction,
	  // at the point where we would normally issue the start.
	  TWCR0 = (1 << TWINT) | (1 << TWSTA)| (1 << TWEN) ;
	  twi_state = TWI_READY;
	}
      }
      break;
    case TW_MT_SLA_NACK:  // address sent, nack received
        // uart_strx(" c4 ");
      twi_error = TW_MT_SLA_NACK;
      twi_stop();
      break;
    case TW_MT_DATA_NACK: // data sent, nack received
        // uart_strx(" c5 ");
      twi_error = TW_MT_DATA_NACK;
      twi_stop();
      break;
    case TW_MT_ARB_LOST: // lost bus arbitration
        // uart_strx(" c6 ");
      twi_error = TW_MT_ARB_LOST;
      twi_releaseBus();
      break;

    // Master Receiver
    case TW_MR_DATA_ACK: // data received, ack sent
        // uart_strx(" c7 ");
      // put byte into buffer
      twi_masterBuffer[twi_masterBufferIndex++] = TWDR0;
    case TW_MR_SLA_ACK:  // address sent, ack received
        // uart_strx(" c8 ");
      // ack if more bytes are expected, otherwise nack
      if(twi_masterBufferIndex < twi_masterBufferLength){
        twi_reply(1);
      }else{
        twi_reply(0);
      }
      break;
    case TW_MR_DATA_NACK: // data received, nack sent
        // uart_strx(" c9 ");
      // put final byte into buffer
      twi_masterBuffer[twi_masterBufferIndex++] = TWDR0;
	if (twi_sendStop)
          twi_stop();
	else {
	  twi_inRepStart = true;	// we're gonna send the START
	  // don't enable the interrupt. We'll generate the start, but we 
	  // avoid handling the interrupt until we're in the next transaction,
	  // at the point where we would normally issue the start.
	  TWCR0 = (1 << TWINT) | (1 << TWSTA)| (1 << TWEN) ;
	  twi_state = TWI_READY;
	}    
	break;
    case TW_MR_SLA_NACK: // address sent, nack received
        // uart_strx(" c10 ");
      twi_stop();
      break;
    // TW_MR_ARB_LOST handled by TW_MT_ARB_LOST case

    // Slave Receiver
    case TW_SR_SLA_ACK:   // addressed, returned ack
    // uart_strx(" c10.1 ");
    case TW_SR_GCALL_ACK: // addressed generally, returned ack
    // uart_strx(" c10.2 ");
    case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
    // uart_strx(" c10.3 ");
    case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
        // uart_strx(" c11 ");
      // enter slave receiver mode
      twi_state = TWI_SRX;
      // indicate that rx buffer can be overwritten and ack
      twi_rxBufferIndex = 0;
      twi_reply(1);
      break;
    case TW_SR_DATA_ACK:       // data received, returned ack
        // uart_strx(" c12 ");
    case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
        // uart_strx(" c13 ");
      // if there is still room in the rx buffer
      if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
        // put byte in buffer and ack
        twi_rxBuffer[twi_rxBufferIndex++] = TWDR0;
        twi_reply(1);
      }else{
        // otherwise nack
        twi_reply(0);
      }
      break;
    case TW_SR_STOP: // stop or repeated start condition received
        // uart_strx(" c14 ");
      // put a null char after data if there's room
      if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
        twi_rxBuffer[twi_rxBufferIndex] = '\0';
      }
      // sends ack and stops interface for clock stretching
      twi_stop();
      // callback to user defined callback
      twi_onSlaveReceive(twi_rxBuffer, twi_rxBufferIndex);
      // since we submit rx buffer to "wire" library, we can reset it
      twi_rxBufferIndex = 0;
      // ack future responses and leave slave receiver state
      twi_releaseBus();
      break;
    case TW_SR_DATA_NACK:       // data received, returned nack
    // uart_strx(" c14.1 ");
    case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
      // nack back at master
    //   uart_strx(" c15 ");
      twi_reply(0);
      break;
    
    // Slave Transmitter
    case TW_ST_SLA_ACK:          // addressed, returned ack
    // uart_strx(" c15.1 ");
    case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
    // uart_strx(" c16 ");
      // enter slave transmitter mode
      twi_state = TWI_STX;
      // ready the tx buffer index for iteration
      twi_txBufferIndex = 0;
      // set tx buffer length to be zero, to verify if user changes it
      twi_txBufferLength = 0;
      // request for txBuffer to be filled and length to be set
      // note: user must call twi_transmit(bytes, length) to do this
      twi_onSlaveTransmit();
      // if they didn't change buffer & length, initialize it
      if(0 == twi_txBufferLength){
        twi_txBufferLength = 1;
        twi_txBuffer[0] = 0x00;
      }
      // transmit first byte from buffer, fall
    case TW_ST_DATA_ACK: // byte sent, ack returned
        // uart_strx(" c17 ");
      // copy data to output register
      TWDR0 = twi_txBuffer[twi_txBufferIndex++];
      // if there is more to send, ack, otherwise nack
      if(twi_txBufferIndex < twi_txBufferLength){
        twi_reply(1);
      }else{
        twi_reply(0);
      }
      break;
    case TW_ST_DATA_NACK: // received nack, we are done 
    // uart_strx(" c17.1 ");
    case TW_ST_LAST_DATA: // received ack, but we are done already!
        // uart_strx(" c18 ");
      // ack future responses
      twi_reply(1);
      // leave slave receiver state
      twi_state = TWI_READY;
      break;

    // All
    case TW_NO_INFO:   // no state information
    // uart_strx(" c18.1 ");
      break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
    // uart_strx(" c18.2 ");
      twi_error = TW_BUS_ERROR;
      twi_stop();
      break;
  }
}
