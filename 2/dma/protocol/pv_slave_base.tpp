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
#include "pv_slave_base.h"



//---------------
// Constructor 
//---------------
template<typename ADDRESS,typename DATA>
pv_slave_base<ADDRESS,DATA>::pv_slave_base(const char * module_name) :
  m_slave_name(module_name) {
}
  



//-------------------------------------------------
// tlm_transport_if methods implementation   
//-------------------------------------------------
template<typename ADDRESS,typename DATA>
#ifdef DATA_PASSED_BY_VALUE 
tlm::tlm_response<DATA> 
#else
tlm::tlm_response<DATA,tlm::TLM_PASS_BY_POINTER> 
#endif
pv_slave_base<ADDRESS,DATA>::transport(const request_type& request) {

  response_type response;
  unsigned int block_size = request.get_block_size();

  if (request.is_control_access()) {
    std::cout << "ERROR\t" << m_slave_name << ": Control access is not supported\n";
    return(response);
  }
    
  switch(request.get_command()) {
      
  case tlm::READ:
	
#ifdef DATA_PASSED_BY_VALUE
    response.set_data_array(request.get_block_size());
#else
    response.set_data_array(request.get_block_size(),request.get_data_ptr());
#endif 
      
    if (block_size == 1) response.set_status(this->read(request.get_address(),
							response.get_data(),
							request.get_byte_enable(),
							request.get_mode(),
							request.get_tlm_export_id()
							));    
    else response.set_status(this->read_block(request.get_address(),
					      response.get_data_ptr(),
					      block_size,
					      request.get_byte_enable_ptr(),
					      request.get_byte_enable_period(),
					      request.get_mode(),
					      request.get_tlm_export_id()
					      ));
    break;
    
  case tlm::WRITE:	
    if (block_size == 1) response.set_status(this->write(request.get_address(),
							 request.get_data(),
							 request.get_byte_enable(),
							 request.get_mode(),
							 request.get_tlm_export_id()
							 ));
    else response.set_status(this->write_block(request.get_address(),
					       request.get_data_ptr(),
					       block_size,
					       request.get_byte_enable_ptr(),
					       request.get_byte_enable_period(),
					       request.get_mode(),
					       request.get_tlm_export_id()
					       ));
    break;

  default:
    std::cout << "ERROR\t" << m_slave_name << ": Unknown command \n";
    break;
  }
    
  return response;
}





  
  
//---------------------------------------------------------------
template<typename ADDRESS,typename DATA> 
void
pv_slave_base<ADDRESS,DATA>::transport(const request_type& request,response_type& response) {

  unsigned int block_size = request.get_block_size();


  if (request.is_control_access()) {
    std::cout << "ERROR\t" << m_slave_name << ": Control access is not supported\n";
    return;
  }
    
  switch(request.get_command()) {
	
  case tlm::READ:
	
#ifdef DATA_PASSED_BY_VALUE
    response.set_data_array(request.get_block_size());
#else
    response.set_data_array(request.get_block_size(),request.get_data_ptr());
#endif  
    if (block_size == 1) response.set_status(this->read(request.get_address(),
							*response.get_data_ptr(),
							request.get_byte_enable(),
							request.get_mode(),
							request.get_tlm_export_id()
							));    
    else response.set_status(this->read_block(request.get_address(),
					      response.get_data_ptr(),
					      block_size,
					      request.get_byte_enable_ptr(),
					      request.get_byte_enable_period(),
					      request.get_mode(),
					      request.get_tlm_export_id()
					      ));
    break;
    
  case tlm::WRITE:	
    if (block_size == 1) response.set_status(this->write(request.get_address(),
							 request.get_data(),
							 request.get_byte_enable(),
							 request.get_mode(),
							 request.get_tlm_export_id()
							 ));
    else response.set_status(this->write_block(request.get_address(),
					       request.get_data_ptr(),
					       block_size,
					       request.get_byte_enable_ptr(),
					       request.get_byte_enable_period(),
					       request.get_mode(),
					       request.get_tlm_export_id()
					       ));
    break;
    
  default:
    std::cout << "ERROR\t" << m_slave_name << ": Unknown PV command type\n";
    
    break;
  }  
}




//---------------------
// Read access 
template<typename ADDRESS,typename DATA> 
tlm::tlm_status
pv_slave_base<ADDRESS,DATA>::read(const ADDRESS& address,
				  DATA& data,
				  const unsigned int byte_enable,
				  const tlm::tlm_mode mode,
				  const unsigned int export_id
				  ) {
  tlm::tlm_status status;
  
  std::cout << "ERROR\t" << m_slave_name << ": Read is not implemented\n";
  
  return(status);
}

//----------------------  
// Write access
template<typename ADDRESS,typename DATA> 
tlm::tlm_status
pv_slave_base<ADDRESS,DATA>::write(const ADDRESS& address,
				   const DATA& data,
				   const unsigned int byte_enable,
				   const tlm::tlm_mode mode,
				   const unsigned int export_id
				   ) {
  tlm::tlm_status status;
  
  std::cout << "ERROR\t" << m_slave_name << ": Write is not implemented\n";

  return(status);

}
  
//------------------------------------------------------------------------------
// Read access block
template<typename ADDRESS,typename DATA> 
tlm::tlm_status
pv_slave_base<ADDRESS,DATA>::read_block(const ADDRESS& address,
					DATA * data,
					const unsigned int block_size,
					const unsigned int * byte_enable,
					const unsigned int byte_enable_period,
					const tlm::tlm_mode mode,
					const unsigned int export_id) {
  tlm::tlm_status status;
  
  std::cout << "ERROR\t" << m_slave_name << ": Read block is not implemented\n";
  
  return(status);
}


//------------------------------------------------------------------------------
// Write access block
template<typename ADDRESS,typename DATA> 
tlm::tlm_status
pv_slave_base<ADDRESS,DATA>::write_block(const ADDRESS& address,
					 const DATA * block_data,
					 const unsigned int number,
					 const unsigned int * block_byte_enable,
					 const unsigned int block_byte_enable_period,
					 const tlm::tlm_mode mode,
					 const unsigned int export_id
					 ) {
  tlm::tlm_status status;
  
  std::cout << "ERROR\t" << m_slave_name << ": Write block is not implemented\n";

  return(status);
}


  

/* END of pv_slave_base.tpp */
  
