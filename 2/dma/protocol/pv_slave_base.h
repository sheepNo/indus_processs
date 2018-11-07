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


#ifndef _PV_SLAVE_BASE_H_
#define _PV_SLAVE_BASE_H_

/*------------------------------------------------------------------------------
 * Includes							       
 *----------------------------------------------------------------------------*/
#include <string>

#include "tlm.h"

#include "pv_if.h"


//----------------------------------------------------------------------------
template<typename ADDRESS,typename DATA>
class pv_slave_base :
#ifdef DATA_PASSED_BY_VALUE 
  public virtual tlm::tlm_transport_if<tlm::tlm_request<ADDRESS,DATA>,
				       tlm::tlm_response<DATA> >,    
#else
  public virtual tlm::tlm_transport_if<tlm::tlm_request<ADDRESS,DATA,tlm::TLM_PASS_BY_POINTER>,
				       tlm::tlm_response<DATA,tlm::TLM_PASS_BY_POINTER> >,
#endif
  public virtual pv_if<ADDRESS,DATA> {

#ifdef DATA_PASSED_BY_VALUE 
  typedef tlm::tlm_request<ADDRESS,DATA> request_type;
  typedef tlm::tlm_response<DATA> response_type;
#else
  typedef tlm::tlm_request<ADDRESS,DATA,tlm::TLM_PASS_BY_POINTER> request_type;
  typedef tlm::tlm_response<DATA,tlm::TLM_PASS_BY_POINTER> response_type;
#endif

  typedef tlm::tlm_transport_if<request_type,response_type> interface_type;

  typedef tlm::tlm_target_port<interface_type> target_port_type;

public:

  //---------------
  // Constructor 
  //---------------
  pv_slave_base(const char * module_name);

  //-------------------------------------------------
  // tlm_transport_if methods implementation   
  //-------------------------------------------------
  virtual response_type transport(const request_type& request);
  virtual void transport(const request_type& request,response_type& response );


  //----------------------------------------------------
  // Abstract class pv_if methods default implementation
  //----------------------------------------------------

  virtual tlm::tlm_status read(const ADDRESS& address,
			       DATA& data,
			       const unsigned int byte_enable = tlm::NO_BE,
			       const tlm::tlm_mode mode = tlm::REGULAR,
			       const unsigned int export_id = 0);
    
  virtual tlm::tlm_status write(const ADDRESS& address,
				const DATA& data,
				const unsigned int byte_enable = tlm::NO_BE,
				const tlm::tlm_mode mode = tlm::REGULAR,
				const unsigned int export_id = 0);  
    
  virtual tlm::tlm_status read_block(const ADDRESS& address,
				     DATA * data,
				     const unsigned int block_size,
				     const unsigned int * byte_enable = NULL,
				     const unsigned int byte_enable_period = 0,
				     const tlm::tlm_mode mode = tlm::REGULAR,
				     const unsigned int export_id = 0);
    
  virtual tlm::tlm_status write_block(const ADDRESS& address,
				      const DATA * data,
				      const unsigned int block_size,
				      const unsigned int * byte_enable = NULL,
				      const unsigned int byte_enable_period = 0,
				      const tlm::tlm_mode mode = tlm::REGULAR,
				      const unsigned int export_id = 0);

protected:
    
  /// Slave name (used for debug and error messages: pv_slave_base is not a sc_object )
  std::string m_slave_name;

};


// Class implementation
#include "./pv_slave_base.tpp"



#endif /* _PV_SLAVE_BASE_H_ */


