#include <functional>
#include <windows.h>
#include <iostream>

#include <SimConnect.h>
#include <string>
#include <vector>

int quit = 0;
HANDLE h_sim_connect = nullptr;

using namespace std;

enum data_define_id
{
	DEFINITION_1,
};

enum data_request_id
{
	REQUEST_1,
};

struct data_define
{
	const char* datum_name;
	// file:///C:/MSFS%20SDK/Documentation/html/Programming_Tools/SimVars/Simulation_Variables.htm
	const char* units_name;
	// file:///C:/MSFS%20SDK/Documentation/html/Programming_Tools/SimVars/Simulation_Variable_Units.htm
	SIMCONNECT_DATATYPE datum_type;
	// file:///C:/MSFS%20SDK/Documentation/html/Programming_Tools/SimConnect/API_Reference/Structures_And_Enumerations/SIMCONNECT_DATATYPE.htm

	/*
		If data is requested only when it changes(see the flags parameter of SimConnect_RequestDataOnSimObject) a change 
		will only be reported if it is greater than the value of this parameter(not greater than or equal to).
		Even the tiniest change will initiate the transmission of data.
		
		This can be used with integer data, the floating point fEpsilon value is first truncated to its integer component before the comparison is made(for example, an fEpsilon value of 2.9
		truncates to 2, so a data change of 2 will not trigger a transmission, and a change of 3 will do so).

		SimConnect_RequestDataOnSimObject: file:///C:/MSFS%20SDK/Documentation/html/Programming_Tools/SimConnect/API_Reference/Events_And_Data/SimConnect_RequestDataOnSimObject.htm
	*/
	float f_epsilon;

	data_define(const char* datum_name, const char* units_name,
	            const SIMCONNECT_DATATYPE datum_type = SIMCONNECT_DATATYPE_FLOAT64, const float f_epsilon = 0) :
		datum_name(datum_name),
		units_name(units_name),
		datum_type(datum_type),
		f_epsilon(f_epsilon)
	{
	}
};

struct data_request
{
	data_request_id request_id;
	data_define_id definition_id;

	vector<data_define>* defined_data;
	SIMCONNECT_PERIOD update_period;
	HRESULT hr;

	HANDLE h_sim_connect;


	data_request(data_request_id request_id, data_define_id data_define_id, vector<data_define>* data_defined_group,
	             const SIMCONNECT_PERIOD update_period = SIMCONNECT_PERIOD_VISUAL_FRAME) :
		request_id(request_id), definition_id(data_define_id), defined_data(data_defined_group),
		update_period(update_period)
	{
	}

	void load_request(const HANDLE h_sim_connect)
	{
		this->h_sim_connect = h_sim_connect;
		add_data_definition();
		request_data_on_sim_object();
	}

	void add_data_definition()
	{
		for (int i = 0; i <= defined_data->size() - 1; i++)
		{
			hr = SimConnect_AddToDataDefinition(h_sim_connect, definition_id, (*defined_data)[i].datum_name,
			                                    (*defined_data)[i].units_name);

			if (SUCCEEDED(hr))
			{
				cout << "added definition successfully" << endl;
			}
			else
			{
				cout << "failed adding definition data" << endl;
			}
		}
	}

	void request_data_on_sim_object()
	{
		hr = SimConnect_RequestDataOnSimObject(h_sim_connect, request_id, definition_id, SIMCONNECT_OBJECT_ID_USER,
		                                       SIMCONNECT_PERIOD_VISUAL_FRAME);
		if (SUCCEEDED(hr))
		{
			cout << "requested data successfully" << endl;
		}
		else
		{
			cout << "failed request data" << endl;
		}
	}

	void close()
	{
		hr = SimConnect_Close(h_sim_connect);
	}
};

struct sim_response
{
	double altitude;
	int32_t heading;
	int32_t speed;
	int32_t vertical_speed;
};

struct zz
{
	double airspeed;
	double speed;
};

void CALLBACK dispatch(SIMCONNECT_RECV* p_data, DWORD cb_data, void* p_context)
{
	switch (p_data->dwID)
	{
	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
		{
			// RETRIEVE SIMULATION DATA and cast it to SIMCONNECT_RECV_SIMOBJECT_DATA POINTER
			auto* p_obj_data = reinterpret_cast<SIMCONNECT_RECV_SIMOBJECT_DATA*>(p_data);

			// SWITCH TO FIND THE RIGHT REQUEST (IF THERE WAS MORE THAN ONE REQUEST THERE WOULD BE ANOTHER CASE)
			//cout << "p_obj_data->dwRequestID: " << p_obj_data->dwRequestID << endl;


			switch (p_obj_data->dwRequestID)
			{
			case REQUEST_1:

				// Cast the data_request data to a SimResponse (our defined struct)
				//auto p_s = (double*)&p_obj_data->dwData;
				//cout << *p_s << endl;

				auto p_s = (zz*)&p_obj_data->dwData;
				cout << p_s->airspeed << ' - ' << p_s->speed << endl;


			//auto p_s = (double) & p_obj_data->dwData;
			//auto a = new double[2];
			//vec[i] = *p_s;

			//cout << vec[0] << endl;


			//cout << (*p_s)[0] << endl;


			// DO WHATEVER YOU WANT WITH THE DATA (FOR SIMPLICITY WE WILL JUST PRINT IT TO THE CONSOLE)
			/*cout
				<< "\rAltitude: " << p_s->altitude
				<< " - Heading: " << p_s->heading
				<< " - Speed (knots): " << p_s->speed
				<< " - Vertical Speed: " << p_s->vertical_speed
				<< flush;*/
				break;
			}
			break;
		}
	case SIMCONNECT_RECV_ID_QUIT:
		{
			quit = 1;
			break;
		}
	}
}

int main()
{
	// Sample
	const auto indicated_altitude = data_define("INDICATED ALTITUDE", "feet", SIMCONNECT_DATATYPE_FLOAT64);
	const auto speed = data_define("Airspeed Indicated", "knots", SIMCONNECT_DATATYPE_FLOAT64);

	vector group1 = {indicated_altitude, speed};

	if (SUCCEEDED(SimConnect_Open(&h_sim_connect, "Client Event Demo", NULL, 0, NULL, 0)))
	{
		cout << "init" << endl;
		auto r1 = data_request(REQUEST_1, DEFINITION_1, &group1, SIMCONNECT_PERIOD_VISUAL_FRAME);
		r1.load_request(h_sim_connect);

		while (quit == 0)
		{
			//SimConnect_CallDispatch(h_sim_connect, dispatch, nullptr);

			SIMCONNECT_RECV* pData;
			DWORD cbData;

			HRESULT hr = SimConnect_GetNextDispatch(h_sim_connect, &pData, &cbData);
			if (SUCCEEDED(hr))
			{
				switch (pData->dwID)
				{
				case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
					SIMCONNECT_RECV_SIMOBJECT_DATA* p_obj_data = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
					switch (p_obj_data->dwRequestID)
					{
					case REQUEST_1:

						auto p_s = (zz*)&p_obj_data->dwData;
						cout << p_s->airspeed << "           -          " << p_s->speed << endl;
						break;
					}

					break;
				
				}
			}

			//r1.close();
		}
		return 0;
	}
}
