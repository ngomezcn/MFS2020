#include <functional>
#include <windows.h>
#include <iostream>
#include <SimConnect.h>

#include "DataDefine.h"
#include "DataRequest.h"

int quit = 0;
SIMCONNECT_RECV* p_data;
DWORD cb_data;
HANDLE h_sim_connect = nullptr;


namespace sv
{
	//SimVar indicated_altitude;
	//SimVar knots;
}

// SimVars
auto indicated_altitude = DataDefine("INDICATED ALTITUDE", "feet", SIMCONNECT_DATATYPE_FLOAT64);
auto knots = DataDefine("Airspeed Indicated", "knots", SIMCONNECT_DATATYPE_FLOAT64);

// SimVars Groups
namespace defined_data_group
{
	std::vector flight_model = {indicated_altitude, knots};
}

// Requests
auto r1 = DataRequest(defined_data_group::flight_model, SIMCONNECT_PERIOD_VISUAL_FRAME);

int main()
{
	HRESULT sc_open = SimConnect_Open(&h_sim_connect, "Client Event Demo", nullptr, 0, nullptr, 0);
	while (!SUCCEEDED(sc_open))
	{
		sc_open = SimConnect_Open(&h_sim_connect, "Client Event Demo", nullptr, 0, nullptr, 0);

		std::cout << "Awaiting to connect with Flight Simulator 2020...." << std::endl;
		Sleep(1000);
	}
	std::cout << "Connected to Flight Simulator 2020!" << std::endl;

	// Send request to fs2020
	r1.init(&h_sim_connect);

	while (quit == 0)
	{
		HRESULT sc_dispatch = SimConnect_GetNextDispatch(h_sim_connect, &p_data, &cb_data);
		if (SUCCEEDED(sc_dispatch))
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
							auto p_s = reinterpret_cast<double*>(&p_obj_data->dwData);
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
