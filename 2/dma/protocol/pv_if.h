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

#ifndef _PV_IF_H_
#define _PV_IF_H_


/*------------------------------------------------------------------------------
 * Includes							       
 *----------------------------------------------------------------------------*/
#include "tlm.h"


//----------------------------------------------------------------------------
template< typename ADDRESS,typename DATA>
class pv_if {
  
public:
  
  //-----------
  // Destructor
  //-----------
  virtual ~pv_if() {}

  //-----------------------------------------
  // Abstract class pv_if methods declaration
  //-----------------------------------------

  virtual tlm::tlm_status read(const ADDRESS& address,
			       DATA& data,
			       const unsigned int byte_enable = tlm::NO_BE,
			       const tlm::tlm_mode mode = tlm::REGULAR,
			       const unsigned int export_id = 0
			       ) = 0;
    
  virtual tlm::tlm_status write(const ADDRESS& address,
				const DATA& data,
				const unsigned int byte_enable = tlm::NO_BE,
				const tlm::tlm_mode mode = tlm::REGULAR,
				const unsigned int export_id = 0
				) = 0;  
    
  virtual tlm::tlm_status read_block(const ADDRESS& address,
				     DATA * data,
				     const unsigned int block_size,
				     const unsigned int * byte_enable = NULL,
				     const unsigned int byte_enable_period = 0,
				     const tlm::tlm_mode mode = tlm::REGULAR,
				     const unsigned int export_id = 0
				     ) = 0;
    
  virtual tlm::tlm_status write_block(const ADDRESS& address,
				      const DATA * data,
				      const unsigned int block_size,
				      const unsigned int * byte_enable = NULL,
				      const unsigned int byte_enable_period = 0,
				      const tlm::tlm_mode mode = tlm::REGULAR,
				      const unsigned int export_id = 0
				      ) = 0;
    

};


#endif /* _PV_IF_H_ */


