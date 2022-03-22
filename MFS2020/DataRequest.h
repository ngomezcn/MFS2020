#ifndef _DATAREQUEST_H_
#define _DATAREQUEST_H_

#include <SimConnect.h>
#include <vector>

#include "DataDefine.h"

class DataRequest
{
public:
	static int request_id;
	static int  definition_id;


	std::vector<DataDefine>& defined_data;
	SIMCONNECT_PERIOD update_period;
	HRESULT hr{};
private:
	HANDLE* h_sim_connect = nullptr;

public:
	DataRequest(std::vector<DataDefine>& data_defined_group, SIMCONNECT_PERIOD update_period = SIMCONNECT_PERIOD_VISUAL_FRAME);

	void init(HANDLE* sim_connect_handler);
	void close();
private:
	void add_data_definition();
	void request_data_on_sim_object();
};

#endif // _DATAREQUEST_H_
