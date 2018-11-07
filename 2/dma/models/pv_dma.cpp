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


/*------------------------------------------------------------------------------
 * Includes							       
 *----------------------------------------------------------------------------*/
#include "pv_dma.h"


/*------------------------------------------------------------------------------
 * Constants							       
 *----------------------------------------------------------------------------*/
// Register relative addresses
const int pv_dma::SRC_ADDR;
const int pv_dma::DST_ADDR;
const int pv_dma::LENGTH;
const int pv_dma::CONTROL;

// Control register bit
const int pv_dma::START;
const int pv_dma::IRQ;

//----------------
// Constructor
//----------------
pv_dma::pv_dma(sc_module_name module_name,
	       tlm::tlm_endianness endian) :
  sc_module(module_name),
  pv_slave_base<int,int>(name()),
  target_port("target_port"),
  initiator_port("initiator_port"),
  m_endian(endian),
  m_pv_dma_src_addr(0),
  m_pv_dma_dst_addr(0),
  m_pv_dma_length(0),
  m_pv_dma_control(0) {

  target_port(*this);

  // mettre ici l'indication du sc_thread "transfer" avec sa sensibilite
  // (put here the declaration of sc_thread "transfer" with its sensitivity
  // ...
  SC_THREAD(transfer);
  sensitive<<m_start_transfer;
  dont_initialize();

  // mettre ici l'indication du sc_method "irq" avec sa sensibilite
  // (put here the declaration of sc_method "irq" with its sensitivity
  // ...
  SC_METHOD(irq);
  sensitive<<m_irq_to_change;
  dont_initialize();
  
  pv_dma_irq_out.initialize(false);

  std::cout << name() << " module created - "<< ((m_endian == tlm::TLM_BIG_ENDIAN) ? "big": "little") << " endian\n";

}


//----------------------------------------
// pv_if virtual method implementation
//----------------------------------------

//------------------------------------------------------------------------------
// DMA read access 

tlm::tlm_status pv_dma::read(const int& address,
			     int& data,
			     const unsigned int byte_enable,
			     const tlm::tlm_mode mode,
			     const unsigned int export_id
			     ) {
  tlm::tlm_status status;
  
  switch (address)
    {      
    case SRC_ADDR:       
      data = m_pv_dma_src_addr; // Read pv_dma source address register
#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": read pv_dma source address register, returns " << m_pv_dma_src_addr << std::endl;
#endif
      break;
      
    case DST_ADDR: 
      data = m_pv_dma_dst_addr; // Read pv_dma destination address register
#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": read pv_dma destination address register, returns " << m_pv_dma_dst_addr << std::endl;
#endif
      break;  

    case LENGTH: 
      data = m_pv_dma_length;   // Read pv_dma length register
#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": read pv_dma length address register, returns " << m_pv_dma_length << std::endl;
#endif
      break;  
      
    case CONTROL: 
      /* Read pv_dma control register. This register is 8 bits, the valid byte of the 
	 register is the one located at the word address + 0 : tlm::BYTE_LANE_0 (0x1)
      */
      if ( (byte_enable & tlm::get_no_be<int>()) == tlm::BYTE_LANE_0) {
	data = tlm::align_byte_to_tlm<int,int>(CONTROL,m_pv_dma_control,m_endian);
#ifdef PV_DMA_DEBUG
	std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": read pv_dma control register, returns " << (int)m_pv_dma_control << std::endl;
#endif
	if (mode != tlm::DEBUG) {
	  wait(SC_ZERO_TIME); // control register is a system synchronization point
	}
      }
      else {
	std::cout << "ERROR\t" << name() << ": invalid read access on control register\n";
	return(status);
      }
      break;

    default:       
      std::cout << "ERROR\t" << name() << std::showbase << std::hex;
      std::cout << ": DMA has received a request with input address out of range: " << address << std::endl;
      return(status);
    }
    
  status.set_ok();

  return(status);
}


//------------------------------------------------------------------------------
// Write access 
tlm::tlm_status pv_dma::write(const int& address,
			      const int& data,
			      const unsigned int byte_enable,
			      const tlm::tlm_mode mode,
			      const unsigned int export_id
			      ) {
  tlm::tlm_status status;

  switch (address)
    {
    case SRC_ADDR: 
      m_pv_dma_src_addr  = data; // Write pv_dma source address register
#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": write "<< m_pv_dma_src_addr << " in pv_dma source address register\n";
#endif
      break;

    case DST_ADDR: 
      m_pv_dma_dst_addr  = data; // Write pv_dma destination address register
#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": write " << m_pv_dma_dst_addr << " in pv_dma destination address register\n";
#endif
      break;

    case LENGTH: 
      m_pv_dma_length = data; // Write pv_dma length register
#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": write " << m_pv_dma_length << " in pv_dma length address register\n";
#endif
      break;  

    case CONTROL: 
      {	
	int input_data; 
	/* Read pv_dma control register. This register is 8 bits, the valid byte of the 
	   register is the one located at the word address + 0 : tlm::BYTE_LANE_0 (0x1)
	*/
	if ( (byte_enable & tlm::get_no_be<int>()) == tlm::BYTE_LANE_0) {	
	  input_data = tlm::align_byte_from_tlm<int,int>(CONTROL,data,m_endian); // 8 bits register
#ifdef PV_DMA_DEBUG
	std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": write " << input_data << " in pv_dma control register\n";
#endif
	}
	else {
	  std::cout << "ERROR\t" << name() << ": invalid write access on control register\n";
	  return(status);
	}
		
	if ((input_data & START) && (!( m_pv_dma_control & START))) {
	  
	  m_start_transfer.notify();
	  if (mode != tlm::DEBUG) m_pv_dma_control |= START; 
#ifdef PV_DMA_DEBUG
	  std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": pv_dma started\n";
#endif
	}	
	
	if ((!(input_data & IRQ)) && (m_pv_dma_control & IRQ)) {
	  m_pv_dma_control &= ~IRQ;
	  if (mode != tlm::DEBUG) m_irq_to_change.notify();
	}	
	if (mode != tlm::DEBUG) wait(SC_ZERO_TIME); // control register is a system synchronization point
      }
      break;

    default: 
      std::cout << "ERROR\t" << name() << std::showbase << std::hex;
      std::cout << ": DMA has received a request with input address out of range: " << address << std::endl;
      return(status);
    }

  status.set_ok();

  return(status);
}


//------------------------------------------------------------------------------
// Transfer end IRQ signal management
void pv_dma::irq() {

  if (m_pv_dma_control & IRQ) {
    pv_dma_irq_out.write(true);
#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": rise transfer end IRQ \n";
#endif
  }
  else {
    pv_dma_irq_out.write(false);  
#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": clear transfer end IRQ \n";
#endif  
  }
}


//------------------------------------------------------------------------------
// dma transfer management
void pv_dma::transfer() {

  tlm::tlm_status status;
  
  while(1) {

    if (m_pv_dma_control & START) {

#ifdef PV_DMA_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": pv_dma transfer started. Source address: "
	      << m_pv_dma_src_addr << " - destination address: " << m_pv_dma_dst_addr
	      << " - length: " << std::dec <<m_pv_dma_length << std::endl;
#endif

    // starts the memory transfer
#ifdef PV_DMA_BLOCK_TRANSFER
    int nb_word = m_pv_dma_length/tlm::get_nb_byte<int>();
    int tmp[nb_word];
    status = initiator_port.read_block(m_pv_dma_src_addr,tmp,nb_word);
    if (status.is_error()) 
      std::cout << "ERROR\t" << name() << ": read block memory failure at " << m_pv_dma_src_addr << std::endl;
    else {
      status = initiator_port.write_block(m_pv_dma_dst_addr,tmp,nb_word);
      if (status.is_error()) 
	std::cout << "ERROR\t" << name() << ": write block memory failure at " << m_pv_dma_dst_addr << std::endl;
    }
#else
      int tmp;
      for(int i = 0;i<m_pv_dma_length;i+=tlm::get_nb_byte<int>()) {
	status = initiator_port.read(m_pv_dma_src_addr + i,tmp);
	if (status.is_error()) 
	  std::cout << "ERROR\t" << name() << ": read memory failure at " << (m_pv_dma_src_addr +i) << std::endl;
	else {
	  status = initiator_port.write(m_pv_dma_dst_addr + i,tmp);
	  if (status.is_error()) 
	    std::cout << "ERROR\t" << name() << ": write memory failure at " << (m_pv_dma_dst_addr +i) << std::endl;
	}
      }
#endif
      
      // Clear the START bit of the control register 
      m_pv_dma_control &= ~START;

      // Interrupt generation
      if (!(m_pv_dma_control&IRQ)) {
	m_pv_dma_control |= IRQ;
	m_irq_to_change.notify();
      }
    }
    else wait(); // wait for start transfer event `
  } 
}





/* END of pv_dma.cpp */
