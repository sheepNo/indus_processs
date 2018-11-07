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

#include "dma_testbench.h"


//-----------------------------------------------------------------------------------
dma_testbench::dma_testbench(sc_module_name module_name,
			     tlm::tlm_endianness endian,
			     int dma_base_address) :
  sc_module( module_name ), 
  initiator_port("initiator_port"),
  m_endian(endian),
  m_dma_base_address(dma_base_address) {

  SC_THREAD( run );

  SC_METHOD( irq );
  sensitive << pv_dma_irq_in.pos();
  dont_initialize();

  std::cout << name() << " module created\n";
}


//-----------------------------------------------------------------------------------
void dma_testbench::irq() {
  m_end_transfer.notify();
}


//-----------------------------------------------------------------------------------
void dma_testbench::run() {

  tlm::tlm_status status;
  int address,value = 0;

  //---------------------------------------
  // dma programming: write register values
  //---------------------------------------

  // Write dma source address register  
  value = 0x20;
  status = initiator_port.write(m_dma_base_address + pv_dma::SRC_ADDR,value);  
  if (status.is_error())
    std::cout << "ERROR\t" << name () << ": write in dma source address register failure\n";

  // Write dma destination address register  
  value = 0x100;
  status = initiator_port.write(m_dma_base_address + pv_dma::DST_ADDR,value);  
  if (status.is_error())
    std::cout << "ERROR\t" << name () << ": write in dma destination address register failure\n";

  // Write dma length register  
  value = 64;
  status = initiator_port.write(m_dma_base_address + pv_dma::LENGTH,value);  
  if (status.is_error())
    std::cout << "ERROR\t" << name () << ": write in dma length register failure\n";

  //------------------------------------------------
  // dma programming: starts the dma transfer
  //------------------------------------------------

  // Start the dma transfer (control register witdh is 8 bits, the write value must be aligned before write )
  address = m_dma_base_address + pv_dma::CONTROL;
  status = initiator_port.write(address,
				tlm::align_byte_to_tlm<int,int>(address,pv_dma::START,m_endian),
				tlm::be_byte<int,int>(address));
  if (status.is_error()) std::cout << "ERROR\t" << name () << ": write in dma control register failure\n";

  // Verification: read dma control register  (8 bits value)
  status = initiator_port.read(address,value,tlm::be_byte<int,int>(address));
  value = tlm::align_byte_from_tlm<int,int>(address,value,m_endian); // 8 bits register value alignment
  if (status.is_error()) std::cout << "ERROR\t" << name () << ": read in dma control register failure\n";


  //--------------------------
  // waiting for dma end of transfer
  //--------------------------
  if (!(value & pv_dma::IRQ)) wait(m_end_transfer);


  //---------------------
  // Clears dma interrupt
  //---------------------

  // read dma control register 
  status = initiator_port.read(address,value,tlm::be_byte<int,int>(address));
  value = tlm::align_byte_from_tlm<int,int>(address,value,m_endian);  // 8 bits register value alignment
  if (status.is_error()) std::cout << "ERROR\t" << name () << ": read in dma control register failure\n";

  // Clears the IRQ bit of the dma control register (8 bits value to align before writing)
  value &= ~pv_dma::IRQ;      
  status = initiator_port.write(address,
				tlm::align_byte_to_tlm<int,int>(address,value,m_endian),
				tlm::be_byte<int,int>(address));
  if (status.is_error()) std::cout << "ERROR\t" << name () << ": write in dma control register failure\n";
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  //---------------------------------------------------------------------------
  //---------------------------------------
  // dma programming: write register values
  //---------------------------------------


  // Write dma destination address register  
  value = 0x10;
  status = initiator_port.write(m_dma_base_address + pv_dma::DST_ADDR,value);  
  if (status.is_error())
    std::cout << "ERROR\t" << name () << ": write in dma destination address register failure\n";

  // Write dma length register  
  value = 64;
  status = initiator_port.write(m_dma_base_address + pv_dma::LENGTH,value);  
  if (status.is_error())
    std::cout << "ERROR\t" << name () << ": write in dma length register failure\n";

  //------------------------------------------------
  // dma programming: starts the dma transfer
  //------------------------------------------------

  // Start the dma transfer (control register witdh is 8 bits, the write value must be aligned before write )
  address = m_dma_base_address + pv_dma::CONTROL;
  status = initiator_port.write(address,
				tlm::align_byte_to_tlm<int,int>(address,pv_dma::START,m_endian),
				tlm::be_byte<int,int>(address));
  if (status.is_error()) std::cout << "ERROR\t" << name () << ": write in dma control register failure\n";

  // Verification: read dma control register  (8 bits value)
  status = initiator_port.read(address,value,tlm::be_byte<int,int>(address));
  value = tlm::align_byte_from_tlm<int,int>(address,value,m_endian); // 8 bits register value alignment
  if (status.is_error()) std::cout << "ERROR\t" << name () << ": read in dma control register failure\n";


  //--------------------------
  // waiting for dma end of transfer
  //--------------------------
  if (!(value & pv_dma::IRQ)) wait(m_end_transfer);


  //---------------------
  // Clears dma interrupt
  //---------------------

  // read dma control register 
  status = initiator_port.read(address,value,tlm::be_byte<int,int>(address));
  value = tlm::align_byte_from_tlm<int,int>(address,value,m_endian);  // 8 bits register value alignment
  if (status.is_error()) std::cout << "ERROR\t" << name () << ": read in dma control register failure\n";

  // Clears the IRQ bit of the dma control register (8 bits value to align before writing)
  value &= ~pv_dma::IRQ;      
  status = initiator_port.write(address,
				tlm::align_byte_to_tlm<int,int>(address,value,m_endian),
				tlm::be_byte<int,int>(address));
  if (status.is_error()) std::cout << "ERROR\t" << name () << ": write in dma control register failure\n";
}


/* END of dma_testbench.cpp */
