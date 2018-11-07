/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2004 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/


#ifndef _PV_DMA_H_
#define _PV_DMA_H_

/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/ 
#include "systemc.h"

#include "pv_slave_base.h"
#include "pv_target_port.h"
#include "pv_initiator_port.h"

//------------------------------------------------------------------------------
class pv_dma : 
  public sc_module,
  public pv_slave_base<int,int> {

public :

  // Registers relative addresses
  static const int SRC_ADDR  = 0x00;  // First register;
  static const int DST_ADDR  = SRC_ADDR  + sizeof(int);
  static const int LENGTH    = DST_ADDR  + sizeof(int);;
  static const int CONTROL   = LENGTH + sizeof(int);
  
  // Control register bits
  static const int START = 0x01;
  static const int IRQ   = 0x10;


  //---------------
  // Module ports
  //---------------
  pv_target_port<int,int> target_port;
  pv_initiator_port<int,int> initiator_port;
  sc_out<bool> pv_dma_irq_out;

  //----------------
  // Constructor
  //----------------  
  SC_HAS_PROCESS(pv_dma);
  pv_dma(sc_module_name module_name,
	 tlm::tlm_endianness endian = TLM_HOST_ENDIAN);

  //--------------------------------------------------------------------------------------------
  //Abstract class pv_if methods implementation (overides pv_slave_base default implementation)
  //--------------------------------------------------------------------------------------------
  
  //----------------------------
  /// Read access 
  tlm::tlm_status read(const int& address,
		       int& data,
		       const unsigned int byte_enable = tlm::NO_BE,
		       const tlm::tlm_mode mode = tlm::REGULAR,
		       const unsigned int export_id = 0
		       );
  
  //----------------------------
  /// Write access 
  tlm::tlm_status write(const int& address,
			const int& data,
			const unsigned int byte_enable = tlm::NO_BE,
			const tlm::tlm_mode mode = tlm::REGULAR,
			const unsigned int export_id = 0
			);


protected:
  
  tlm::tlm_endianness m_endian;       // dma endianness    
  sc_event m_start_transfer;          // pv_dma process related event (start)
  sc_event m_irq_to_change;           // rise/clear interrupt signal related event
  int m_pv_dma_src_addr;              // Source address register
  int m_pv_dma_dst_addr;              // Destination address register
  int m_pv_dma_length;                // Length register (expressed in byte)
  tlm::tlm_uint8_t m_pv_dma_control;  // control register (8 bits register)    
  
  void transfer();                    // pv_dma transfer management process 
  void irq();                         // pv_dma transfer end IRQ signal management process
};

#endif /* _PV_DMA_H_ */


