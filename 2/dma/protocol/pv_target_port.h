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


#ifndef _PV_TARGET_PORT_H_
#define _PV_TARGET_PORT_H_

/*------------------------------------------------------------------------------
 * Includes							       
 *----------------------------------------------------------------------------*/
#include "systemc.h"

#include "tlm.h"

//----------------------------------------------------------------------------
template<typename ADDRESS,typename DATA>
class pv_target_port :
#ifdef DATA_PASSED_BY_VALUE 
  public tlm::tlm_target_port<tlm::tlm_transport_if<tlm::tlm_request<ADDRESS,DATA>,
						    tlm::tlm_response<DATA> > >
#else
  public tlm::tlm_target_port<tlm::tlm_transport_if<tlm::tlm_request<ADDRESS,DATA,tlm::TLM_PASS_BY_POINTER>,
						    tlm::tlm_response<DATA,tlm::TLM_PASS_BY_POINTER> > >
#endif
{

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

  //--------------
  // Constructor
  //--------------
  pv_target_port(const char * port_name) :
    target_port_type(port_name)
  {}

  
  
};


#endif /* _PV_TARGET_PORT_H_ */


