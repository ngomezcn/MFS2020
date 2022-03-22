#ifndef _DATADEFINE_H_
#define _DATADEFINE_H_

#include <SimConnect.h>

class DataDefine
{
public:
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

	DataDefine(const char* datum_name, const char* units_name,
		const SIMCONNECT_DATATYPE datum_type = SIMCONNECT_DATATYPE_FLOAT64, const float f_epsilon = 0);
};

#endif // _DATADEFINE_H_
