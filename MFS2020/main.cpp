#include <functional>
#include <windows.h>
#include <iostream>
#include <SimConnect.h>

#include "DataDefine.h"
#include "DataRequest.h"


int quit = 0;
HANDLE h_sim_connect = nullptr;

int DataRequest::request_id = 0;
int DataRequest::definition_id = 0;
int main()
{
	// Sample
	const auto indicated_altitude = DataDefine("INDICATED ALTITUDE", "feet", SIMCONNECT_DATATYPE_FLOAT64);
	const auto speed = DataDefine("Airspeed Indicated", "knots", SIMCONNECT_DATATYPE_FLOAT64);

	std::vector req_sim_vars = {indicated_altitude, speed};
	if (SUCCEEDED(SimConnect_Open(&h_sim_connect, "Client Event Demo", NULL, 0, NULL, 0)))
	{
		std::cout << "init" << std::endl;
		auto r1 = DataRequest(req_sim_vars, SIMCONNECT_PERIOD_VISUAL_FRAME);
		r1.init(&h_sim_connect);

		SIMCONNECT_RECV* p_data;
		DWORD cb_data;

		while (quit == 0)
		{
			const HRESULT dispatch_response = SimConnect_GetNextDispatch(h_sim_connect, &p_data, &cb_data);
			if (SUCCEEDED(dispatch_response))
			{
				switch (p_data->dwID)
				{
				case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
					{
						auto* p_obj_data = reinterpret_cast<SIMCONNECT_RECV_SIMOBJECT_DATA*>(p_data);
						switch (p_obj_data->dwRequestID)
						{
						case 1:
							{
								const auto p_s = reinterpret_cast<double*>(&p_obj_data->dwData);
								std::cout << *p_s << " - " << *p_s << std::endl;
								break;
							}
						}
					}
				}
			}
		}
		r1.close();

		return 0;
	}
}
