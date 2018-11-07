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


#ifndef _MASTER_H_
#define _MASTER_H_

#include <iomanip>

#include "systemc.h"

#include "tlm.h"

#include "pv_initiator_port.h"
#include "pv_dma.h"


//-----------------------------------------------------------------------------------
class dma_testbench : public sc_module {

public:

  //---------------
  // Module ports
  //---------------
  pv_initiator_port<int,int> initiator_port;
  sc_in<bool> pv_dma_irq_in;

  //----------------
  // Constructor
  //----------------  
  SC_HAS_PROCESS(dma_testbench);
  dma_testbench(sc_module_name module_name,
		tlm::tlm_endianness endian = TLM_HOST_ENDIAN,
		int dma_base_address = 0);

private:

  tlm::tlm_endianness m_endian;  // Testbench endianness
  int m_dma_base_address;        // dma base address
  sc_event m_end_transfer;       // dma transfer end event

  void run();                    // Testbench process
  void irq();                    // Testbench irq process 
};


#endif /* _MASTER_H_ */


