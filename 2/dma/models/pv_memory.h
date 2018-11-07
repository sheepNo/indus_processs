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


#ifndef _PV_MEMORY_H_
#define _PV_MEMORY_H_

/*------------------------------------------------------------------------------
 * Includes							       
 *----------------------------------------------------------------------------*/
#include <exception>
#include <iostream>
#include <iomanip>

#include "systemc.h"

#include "pv_slave_base.h"
#include "pv_target_port.h"



//----------------------------------------------------------------------------

class pv_memory : 
  public sc_module,
  public pv_slave_base<int,int> {

  
public :

  //--------------
  // Module port
  //--------------
  pv_target_port<int,int> target_port; 

  //----------------
  // Constructor
  //----------------
  pv_memory(sc_module_name module_name,
	    unsigned int memory_size,
	    tlm::tlm_endianness endian = TLM_HOST_ENDIAN,
	    bool memory_init = false
	    );  

  //-------------
  // Destructor 
  //-------------  
  ~pv_memory();


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

  //----------------------------
  /// Read access block
  tlm::tlm_status read_block(const int& address,
			     int * block_data,
			     const unsigned int block_size,
			     const unsigned int * byte_enable = NULL,
			     const unsigned int byte_enable_period = 0,
			     const tlm::tlm_mode mode = tlm::REGULAR,
			     const unsigned int export_id = 0
			     );
  
  //----------------------------
  /// Write access block
  tlm::tlm_status write_block(const int& address,
			      const int * block_data,
			      const unsigned int block_size,
			      const unsigned int * byte_enable = NULL,
			      const unsigned int byte_enable_period = 0,
			      const tlm::tlm_mode mode = tlm::REGULAR,
			      const unsigned int export_id = 0
			      );

protected:
  
  int * m_array;                // memory data array   
  unsigned int m_memory_size;   // memory size in bytes   
  int m_memory_limit;           // memory address limit (first not allowed address)   
  tlm::tlm_endianness m_endian; // memory endianness
};

#endif /* _PV_MEMORY_H_ */


