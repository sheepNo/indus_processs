/* -*- C++ -*- */
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
#include "pv_initiator_port.h"



//--------------
// Constructor
//--------------
template<typename ADDRESS,typename DATA,int N>
pv_initiator_port<ADDRESS,DATA,N>::pv_initiator_port(const char * port_name) :
  initiator_port_type(port_name)
{}

//---------------------------------------
//  Execute the transaction transfer
//---------------------------------------
template<typename ADDRESS,typename DATA,int N>
void
pv_initiator_port<ADDRESS,DATA,N>::do_transport(request_type& request,
						response_type& response,
						unsigned int target_port_rank) 
{
  // Add the registered target port id to the request
  request.set_tlm_export_id(this->get_target_port_list()[target_port_rank]->get_tlm_export_id());
        
  // Send initiator request to the target, functionnal call method using  -> operator of sc_port
#ifdef NEW_TRANSPORT_IF
  (*this)[target_port_rank]->transport(request,response);
#else
  response = (*this)[target_port_rank]->transport(request);
#endif
  
}

//-------------------------------
// pv_if methods implementation
//-------------------------------
  
//----------------------------
// Read access 
template<typename ADDRESS,typename DATA,int N>
tlm::tlm_status 
pv_initiator_port<ADDRESS,DATA,N>::read(const ADDRESS& address,
					DATA& data,
					const unsigned int byte_enable,
					const tlm::tlm_mode mode,
					const unsigned int export_id
					) 
{
  response_type response;
  request_type request;
  request.set_command(tlm::READ);
  request.set_address(address);
#ifndef DATA_PASSED_BY_VALUE 
  request.set_data_array(1,&data);
#endif

  request.set_byte_enable_array(1,&byte_enable);
  request.set_mode(mode);

  do_transport(request,response);

#ifdef DATA_PASSED_BY_VALUE 
  data = response.get_data();    
#endif

  return(response.get_status());      
}
    
//----------------------------
// Write access 
template<typename ADDRESS,typename DATA,int N>
tlm::tlm_status 
pv_initiator_port<ADDRESS,DATA,N>::write(const ADDRESS& address,
					 const DATA& data,
					 const unsigned int byte_enable,
					 const tlm::tlm_mode mode,
					 const unsigned int export_id
					 )
{
  response_type response;
  request_type request;
  request.set_command(tlm::WRITE);
  request.set_address(address);
  request.set_data_array(1,&data);
  request.set_byte_enable_array(1,&byte_enable);
  request.set_mode(mode);

  do_transport(request,response);

  return(response.get_status()); 
}

//----------------------------
// Read access block 
template<typename ADDRESS,typename DATA,int N>
tlm::tlm_status 
pv_initiator_port<ADDRESS,DATA,N>::read_block(const ADDRESS& address,
					      DATA * data,
					      const unsigned int block_size,
					      const unsigned int * byte_enable,
					      const unsigned int byte_enable_period,
					      const tlm::tlm_mode mode,
					      const unsigned int export_id
					      )
{
  response_type response;
  request_type request;
  request.set_command(tlm::READ);
  request.set_address(address);
  request.set_block_size(block_size);

#ifndef DATA_PASSED_BY_VALUE 
  request.set_data_array(block_size,data);
#endif
  
  if (byte_enable)
    request.set_byte_enable_array((byte_enable_period?byte_enable_period:block_size),byte_enable);
  request.set_byte_enable_period(byte_enable_period);
  request.set_block_mode(tlm::INCREMENT);
  request.set_block_address_incr(tlm::get_nb_byte<DATA>());
  request.set_mode(mode);

  do_transport(request,response);

#ifdef DATA_PASSED_BY_VALUE 
  for(unsigned int i = 0;i < block_size;i++)
    data[i] = response.get_data(i);      
#endif

  return(response.get_status());
}

//----------------------------
// Write access block
template<typename ADDRESS,typename DATA,int N>
tlm::tlm_status 
pv_initiator_port<ADDRESS,DATA,N>::write_block(const ADDRESS& address,
					       const DATA * data,
					       const unsigned int block_size,
					       const unsigned int * byte_enable,
					       const unsigned int byte_enable_period,
					       const tlm::tlm_mode mode,
					       const unsigned int export_id
					       )
{    
  response_type response;
  request_type request;
  request.set_command(tlm::WRITE);
  request.set_address(address);
  request.set_data_array(block_size,data);
  if (byte_enable)
    request.set_byte_enable_array((byte_enable_period?byte_enable_period:block_size),byte_enable);
  request.set_byte_enable_period(byte_enable_period);
  request.set_block_mode(tlm::INCREMENT);
  request.set_block_address_incr(tlm::get_nb_byte<DATA>());
  request.set_mode(mode);

  do_transport(request,response);

  return(response.get_status());
}


/* END of pv_initiator_port.tpp */
