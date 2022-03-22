#include "DataDefine.h"
DataDefine::DataDefine(const char* datum_name, const char* units_name, const SIMCONNECT_DATATYPE datum_type,
	const float f_epsilon) :
	datum_name(datum_name),
	units_name(units_name),
	datum_type(datum_type),
	f_epsilon(f_epsilon) {}

