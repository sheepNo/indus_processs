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
#include "pv_memory.h"


//----------------
// Constructor
//----------------

pv_memory::pv_memory(sc_module_name module_name,
		     unsigned int memory_size,
		     tlm::tlm_endianness endian,
		     bool memory_init) :
  sc_module(module_name),
  pv_slave_base<int,int>(name()),
  target_port("target_port"),
  m_array(NULL),
  m_memory_size(memory_size),
  m_endian(endian)  {

  // Bind pv target sc_export to the slave 
  target_port(*this);
  
  m_memory_limit = (int)(m_memory_size);

  try { // Memory creation 
    m_array = new int[m_memory_size/tlm::get_nb_byte<int>()];
  }
  
  catch(std::exception &e) {
    std::string msg(sc_object::name());
    msg += (std::string)(": Failed to create memory (too large memory ?): ") + (std::string)(e.what());
    SC_REPORT_ERROR("pv_memory construction",msg.c_str());    
  }

  std::cout << name() << " module created - "<< (float)(memory_size)/1024 << " KBytes - "
	    << ((m_endian == tlm::TLM_BIG_ENDIAN) ? "big": "little") << " endian";

  if (memory_init) {
    std::cout << " - memory content initialized with test pattern";
    for (int i=0;i<(int)(m_memory_limit/tlm::get_nb_byte<int>());i++) m_array[i] = i;
  }
  
  std::cout << std::endl;
    
}


//-------------
// Destructor 
//-------------  
pv_memory::~pv_memory() {
  if (m_array) delete [] m_array; 
}

//----------------------------------------------
// Abstract class pv_if methods implementation
//----------------------------------------------

//---------------------
// Memory read access 

tlm::tlm_status pv_memory::read(const int& address,
				int& data,
				const unsigned int byte_enable,
				const tlm::tlm_mode mode,
				const unsigned int export_id
				) {
  tlm::tlm_status status;
  
  wait(40, SC_NS);
  if (address >= m_memory_limit ) {
    std::cout << "ERROR\t" << name() << std::showbase << std::hex;
    std::cout << ": Memory has received a request with input address out of range: " << address << std::endl;
    return(status);
  }
    
  int addr = address/tlm::get_nb_byte<int>();
  
  if (!tlm::is_no_be<int>(byte_enable)) {
    data = 0;
    if (m_endian == tlm::TLM_BIG_ENDIAN ) {
      if (byte_enable & tlm::BYTE_LANE_0)  data |= m_array[addr] & 0xff000000;
      if (byte_enable & tlm::BYTE_LANE_1)  data |= m_array[addr] & 0x00ff0000;
      if (byte_enable & tlm::BYTE_LANE_2)  data |= m_array[addr] & 0x0000ff00;
      if (byte_enable & tlm::BYTE_LANE_3)  data |= m_array[addr] & 0x000000ff;
    }
    else {
      if (byte_enable & tlm::BYTE_LANE_0)  data |= m_array[addr] & 0x000000ff;
      if (byte_enable & tlm::BYTE_LANE_1)  data |= m_array[addr] & 0x0000ff00;
      if (byte_enable & tlm::BYTE_LANE_2)  data |= m_array[addr] & 0x00ff0000;
      if (byte_enable & tlm::BYTE_LANE_3)  data |= m_array[addr] & 0xff000000;
    }
  }
  else data =  m_array[addr];

#ifdef PV_MEMORY_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": Read " << data <<  " at " << address;
    if (!tlm::is_no_be<int>(byte_enable)) std::cout << " with byte_enable " << byte_enable; 
    std::cout << std::endl;
#endif

  status.set_ok();        
  return(status);
}

//----------------------  
// Memory write access

tlm::tlm_status pv_memory::write(const int& address,
				 const int& data,
				 const unsigned int byte_enable,
				 const tlm::tlm_mode mode,
				 const unsigned int export_id
				 ) {

  tlm::tlm_status status;    
  wait(50, SC_NS);

  if (address >= m_memory_limit ) {
    std::cout << "ERROR\t" << name() << std::showbase << std::hex;
    std::cout << ": Memory has received a request with input address out of range: " << address << std::endl;
    return(status);
  }

  int addr = address/tlm::get_nb_byte<int>();
  int tmp = m_array[addr];

  if (!tlm::is_no_be<int>(byte_enable)) {
    if (m_endian == tlm::TLM_BIG_ENDIAN ) {
      if (byte_enable & tlm::BYTE_LANE_0) tmp = (tmp & 0x00ffffff) | (data & 0xff000000);
      if (byte_enable & tlm::BYTE_LANE_1) tmp = (tmp & 0xff00ffff) | (data & 0x00ff0000);
      if (byte_enable & tlm::BYTE_LANE_2) tmp = (tmp & 0xffff00ff) | (data & 0x0000ff00);
      if (byte_enable & tlm::BYTE_LANE_3) tmp = (tmp & 0xffffff00) | (data & 0x000000ff);
    }
    else {
      if (byte_enable & tlm::BYTE_LANE_0) tmp = (tmp & 0xffffff00) | (data & 0x000000ff);
      if (byte_enable & tlm::BYTE_LANE_1) tmp = (tmp & 0xffff00ff) | (data & 0x0000ff00);
      if (byte_enable & tlm::BYTE_LANE_2) tmp = (tmp & 0xff00ffff) | (data & 0x00ff0000);
      if (byte_enable & tlm::BYTE_LANE_3) tmp = (tmp & 0x00ffffff) | (data & 0xff000000);
    }
    m_array[addr] = tmp;
  }
  else m_array[addr] = data;

#ifdef PV_MEMORY_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": Write " << data <<  " at " << address;
    if (!tlm::is_no_be<int>(byte_enable)) std::cout << " with byte_enable " << byte_enable; 
    std::cout << std::endl;
#endif

  status.set_ok();  
  return(status);
}

 
//----------------------
// Read access block

tlm::tlm_status pv_memory::read_block(const int& address,
				      int * block_data,
				      const unsigned int block_size,
				      const unsigned int * byte_enable,
				      const unsigned int byte_enable_period,
				      const tlm::tlm_mode mode,
				      const unsigned int export_id
				      ) {
  tlm::tlm_status status;


  if ((int)(address + block_size*tlm::get_nb_byte<int>()) > m_memory_limit ) {
    std::cout << "ERROR\t" << name () << std::showbase << std::hex;
    std::cout << ": Memory has received a request with input address out of range, start: " 
	      << address << " end: " << (address +  block_size*tlm::get_nb_byte<int>()-1)
	      << std::endl;
    return(status);
  }


#ifdef PV_MEMORY_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": Read block (" << block_size <<  ") at " << address << std::endl;
#endif 

  if (byte_enable != NULL) {
    for (unsigned int i = 0 ; i < block_size; i++){ 
        wait(30, SC_NS);
      read(address + i*tlm::get_nb_byte<int>(),block_data[i],byte_enable[(byte_enable_period?i%byte_enable_period:i)],mode);
    }
  }
  else  {
    memcpy(block_data,m_array + (address/tlm::get_nb_byte<int>()),block_size*sizeof(int));
#ifdef PV_MEMORY_DEBUG
    for (unsigned int i =0;i< block_size;i++) {
        wait(30, SC_NS);
      std::cout << std::hex << resetiosflags (std::ios_base::showbase);
      if (!(i%4)) std::cout << "\t0x" << (address+i*tlm::get_nb_byte<int>());
      std::cout << ": 0x" << std::setfill('0') << std::setw(2*tlm::get_nb_byte<int>())
		<< m_array[i+(address/tlm::get_nb_byte<int>())];
      if (((i%4) == 3) || (i ==(block_size-1)))  std::cout << std::endl;
    }
#endif
  }

  status.set_ok();
  return(status);
}

  

//----------------------
// Write access block

tlm::tlm_status pv_memory::write_block(const int& address,
				       const int * block_data,
				       const unsigned int block_size,
				       const unsigned int * byte_enable,
				       const unsigned int byte_enable_period,
				       const tlm::tlm_mode mode,
				       const unsigned int export_id
				       ) {
  tlm::tlm_status status;

  if ((int)(address + block_size*tlm::get_nb_byte<int>()) > m_memory_limit ) {
    std::cout << "ERROR\t" << name () << std::showbase << std::hex;
    std::cout << ": Memory has received a request with input address out of range, start: " 
	      << address << " end: " << (address +  block_size*tlm::get_nb_byte<int>()-1)
	      << std::endl;
    return(status);
  }

#ifdef PV_MEMORY_DEBUG
    std::cout << "DEBUG\t" << name() << std::showbase << std::hex << ": Write block (" << block_size <<  ") at " << address << std::endl;
#endif 

  if (byte_enable != NULL) {
    for (unsigned int i = 0 ; i < block_size; i++){ 
        wait(40, SC_NS);
      write(address + i*tlm::get_nb_byte<int>(),block_data[i],byte_enable[(byte_enable_period?i%byte_enable_period:i)],mode);
    }
  }
  else  {
    memcpy(m_array + (address/tlm::get_nb_byte<int>()),block_data,block_size*sizeof(int));
#ifdef PV_MEMORY_DEBUG
    for (unsigned int i =0;i< block_size;i++) {
        wait(40, SC_NS);
      std::cout << std::hex << resetiosflags (std::ios_base::showbase);
      if (!(i%4)) std::cout << "\t0x" << (address+i*tlm::get_nb_byte<int>());
      std::cout << ": 0x" << std::setfill('0') << std::setw(2*tlm::get_nb_byte<int>())
		<< m_array[i+(address/tlm::get_nb_byte<int>())];
      if (((i%4) == 3) || (i ==(block_size-1)))  std::cout << std::endl;
    }
#endif
  }

  status.set_ok();
  return(status);
}


/* END of pv_memory.cpp */
