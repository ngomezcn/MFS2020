#include "DataRequest.h"

#include <iostream>

int DataRequest::request_id = 0;
int DataRequest::definition_id = 0;

// Constructors
DataRequest::DataRequest(std::vector<DataDefine>& data_defined_group, SIMCONNECT_PERIOD update_period) :
	defined_data(data_defined_group), update_period(update_period) { request_id++; definition_id++; }


// Methods
void DataRequest::init(HANDLE* sim_connect_handler)
{
	this->h_sim_connect = sim_connect_handler;

	add_data_definition();
	request_data_on_sim_object();
}
void DataRequest::close()
{
	hr = SimConnect_Close(*h_sim_connect);
}
void DataRequest::add_data_definition()
{
	for (int i = 0; i <= defined_data.size() - 1; i++)
	{
		hr = SimConnect_AddToDataDefinition(*h_sim_connect, definition_id, (defined_data)[i].datum_name,
			(defined_data)[i].units_name);

		if (SUCCEEDED(hr))
		{
			std::cout << "added definition successfully" << std::endl;
		}
		else
		{
			std::cout << "failed adding definition data" << std::endl;
		}
	}
}
void DataRequest::request_data_on_sim_object()
{
	hr = SimConnect_RequestDataOnSimObject(*h_sim_connect, request_id, definition_id, SIMCONNECT_OBJECT_ID_USER,
		SIMCONNECT_PERIOD_VISUAL_FRAME);
	if (SUCCEEDED(hr))
	{
		std::cout << "requested data successfully" << std::endl;
	}
	else
	{
		std::cout << "failed request data" << std::endl;
	}
}

