#ifndef OBDCOMMS_H
#define OBDCOMMS_H
#include "obdconvs.h"
// flagrantly stolen from obdgpslogger (I added some more data and removed a column)

struct obdservicecmd {
	unsigned int cmdid; ///< Command ID [eg 0C == engine rpm]
	int bytes_returned; ///< Number of bytes we expect back for this item. Elm327 optimisation [0 == no optimisation]
	const char *db_column; ///< Database column name.
	const char *human_name; ///< Human friendly name. http://www.kbmsystems.net/obd_tech.htm
	float min_value; ///< Minimum allowable value [after conversion]
	float max_value; ///< Maximum allowable value [after conversion]
	const char *units; ///< Human friendly representation of units
	OBDConvFunc conv; ///< Function to convert OBD values for this command to a useful number
};
/// List of all OBD Service commands
/// Borrowed from various sources, mainly http://en.wikipedia.org/wiki/Table_of_OBD-II_Codes
const static struct obdservicecmd obdcmds_mode1[] = {
	{ 0x00, 4, NULL,            "PIDs supported 00-20" , 0, 0, "Bit Encoded", NULL  },
	{ 0x01, 4, "dtc_cnt",       "Monitor status since DTCs cleared" , 0, 0, "Bit Encoded", NULL },
	{ 0x02, 4, "dtcfrzf",       "DTC that caused required freeze frame data storage" , 0, 0, "Bit Encoded", NULL },
	{ 0x03, 8, "fuelsys",       "Fuel system 1 and 2 status" , 0, 0, "Bit Encoded", NULL },
	{ 0x04, 1, "load_pct",      "Calculated LOAD" , 0, 100, "%", obdConvert_04 },
	{ 0x05, 1, "temp",          "Engine Coolant Temp" , -40, 215, "*C", obdConvert_05 }, // J1979 calls this "ect"
	{ 0x06, 1, "shrtft13",      "Short Trim, B1&3", -100, 99.22, "%", obdConvert_06_09 },
	{ 0x07, 1, "longft13",      "Long Trim, B1&3", -100, 99.22, "%", obdConvert_06_09 },
	{ 0x08, 1, "shrtft24",      "Short Trim, B2&4", -100, 99.22, "%", obdConvert_06_09 },
	{ 0x09, 1, "longft24",      "Long Trim, B2&4", -100, 99.22, "%", obdConvert_06_09 },
	{ 0x0A, 1, "frp",           "Fuel Rail Pressure", -100, 99.22, "%", obdConvert_0A },
	{ 0x0B, 1, "map",           "Intake Pressure", 0, 765, "kPa", obdConvert_0B },
	{ 0x0C, 2, "rpm",           "Engine rev/min", 0, 16383.75, "rpm", obdConvert_0C },
	{ 0x0D, 1, "vss",           "Vehicle Speed", 0, 255, "kmh", obdConvert_0D },
	{ 0x0E, 1, "sparkadv",      "Cyl. 1 Timing Advance", -64, 63.5, "deg", obdConvert_0E },
	{ 0x0F, 1, "iat",           "Intake Air Temp", -40, 215, "*C", obdConvert_0F },
	{ 0x10, 2, "maf",           "Mass Air Flow", 0, 655.35, "g/s", obdConvert_10 },
	{ 0x11, 1, "throttlepos",   "Absolute Throttle Position", 1, 100, "%", obdConvert_11 },
	{ 0x12, 1, "air_stat",      "Commanded Secondary Air Status" , 0, 0, "Bit Encoded", NULL },
	{ 0x13, 1, "o2sloc",        "O2 Sensor spots" , 0, 0, "Bit Encoded", NULL },
	{ 0x14, 2, "o2s11",         "1B1S1-1 O2 trim", 0, 1.275, "V", obdConvert_14_1B },
	{ 0x15, 2, "o2s12",         "1B1S2-2 O2 trim", 0, 1.275, "V", obdConvert_14_1B },
	{ 0x16, 2, "o2s13",         "1B2S3-1 O2 trim", 0, 1.275, "V", obdConvert_14_1B },
	{ 0x17, 2, "o2s14",         "1B2S4-2 O2 trim", 0, 1.275, "V", obdConvert_14_1B },
	{ 0x18, 2, "o2s21",         "2B3S1-1 O2 trim", 0, 1.275, "V", obdConvert_14_1B },
	{ 0x19, 2, "o2s22",         "2B3S2-2 O2 trim", 0, 1.275, "V", obdConvert_14_1B },
	{ 0x1A, 2, "o2s23",         "2B4S3-1 O2 trim", 0, 1.275, "V", obdConvert_14_1B },
	{ 0x1B, 2, "o2s24",         "2B4S4-2 O2 trim", 0, 1.275, "V", obdConvert_14_1B },
	{ 0x1C, 1, "obdsup",        "OBD requirements to which vehicle is designed" , 0, 0, "Bit Encoded", NULL },
	{ 0x1D, 1, "o2sloc2",       "O2 Sensor spots" , 0, 0, "Bit Encoded", NULL },
	{ 0x1E, 1, "pto_stat",      "Auxiliary Input Status" , 0, 0, "Bit Encoded", NULL },
	{ 0x1F, 2, "runtm",         "Time Since Start", 0, 65535, "seconds", obdConvert_1F },
	{ 0x20, 4, NULL,            "PIDs supported 21-40" , 0, 0, "Bit Encoded", NULL },
	{ 0x21, 4, "mil_dist",      "Distance travelled w/ MIL on", 0, 65535, "km", obdConvert_21 },
	{ 0x22, 2, "frpm",          "Rail Pres: Mvac", 0, 5177.265, "kPa", obdConvert_22 },
	{ 0x23, 2, "frpd",          "Fuel Rail Pressure", 0, 655350, "kPa", obdConvert_23 },
	{ 0x24, 4, "lambda11",      "1-B1S1 O2 ratio", 0, 2, "l/V", obdConvert_24_2B },
	{ 0x25, 4, "lambda12",      "1-B1S2 O2 ratio", 0, 2, "l/V", obdConvert_24_2B },
	{ 0x26, 4, "lambda13",      "1-B2S3 O2 ratio", 0, 2, "l/V", obdConvert_24_2B },
	{ 0x27, 4, "lambda14",      "1-B2S4 O2 ratio", 0, 2, "l/V", obdConvert_24_2B },
	{ 0x28, 4, "lambda21",      "2-B3S1 O2 ratio", 0, 2, "l/V", obdConvert_24_2B },
	{ 0x29, 4, "lambda22",      "2-B3S2 O2 ratio", 0, 2, "l/V", obdConvert_24_2B },
	{ 0x2A, 4, "lambda23",      "2-B4S3 O2 ratio", 0, 2, "l/V", obdConvert_24_2B },
	{ 0x2B, 4, "lambda24",      "2-B4S4 O2 ratio", 0, 2, "l/V", obdConvert_24_2B },
	{ 0x2C, 1, "egr_pct",       "Commanded EGR", 0, 100, "%", obdConvert_2C },
	{ 0x2D, 1, "egr_err",       "EGR Error", -100, 99.22, "%", obdConvert_2D },
	{ 0x2E, 1, "evap_pct",      "Commanded Evap Purge", 0, 100, "%", obdConvert_2E },
	{ 0x2F, 1, "fli",           "Fuel Level Input", 0, 100, "%", obdConvert_2F },
	{ 0x30, 1, "warm_ups",      "# of Warmups w/o DTCs", 0, 255, "", obdConvert_30 },
	{ 0x31, 2, "clr_dist",      "Distance w/o DTCs", 0, 65535, "km", obdConvert_31 },
	{ 0x32, 2, "evap_vp",       "Evap Sys Vapor Pressure", -8192, 8192, "Pa", obdConvert_32 },
	{ 0x33, 1, "baro",          "Barometric Pressure", 0, 255, "kPa", obdConvert_33 },
	{ 0x34, 4, "lambdac11",     "1-B1S1 O2 ratio", 0, 2, "l/mA", obdConvert_34_3B },
	{ 0x35, 4, "lambdac12",     "1-B1S2 O2 ratio", 0, 2, "l/mA", obdConvert_34_3B },
	{ 0x36, 4, "lambdac13",     "1-B2S3 O2 ratio", 0, 2, "l/mA", obdConvert_34_3B },
	{ 0x37, 4, "lambdac14",     "1-B2S4 O2 ratio", 0, 2, "l/mA", obdConvert_34_3B },
	{ 0x38, 4, "lambdac21",     "2-B3S1 O2 ratio", 0, 2, "l/mA", obdConvert_34_3B },
	{ 0x39, 4, "lambdac22",     "2-B3S2 O2 ratio", 0, 2, "l/mA", obdConvert_34_3B },
	{ 0x3A, 4, "lambdac23",     "2-B4S3 O2 ratio", 0, 2, "l/mA", obdConvert_34_3B },
	{ 0x3B, 4, "lambdac24",     "2-B4S4 O2 ratio", 0, 2, "l/mA", obdConvert_34_3B },
	{ 0x3C, 2, "catemp11",      "Cat Temp B1S1", -40, 6513.5, "*C", obdConvert_3C_3F },
	{ 0x3D, 2, "catemp21",      "Cat Temp B2S1", -40, 6513.5, "*C", obdConvert_3C_3F },
	{ 0x3E, 2, "catemp12",      "Cat Temp B1S2", -40, 6513.5, "*C", obdConvert_3C_3F },
	{ 0x3F, 2, "catemp22",      "Cat Temp B2S2", -40, 6513.5, "*C", obdConvert_3C_3F },
	{ 0x40, 4, NULL,            "PIDs supported 41-60" , 0, 0, "Bit Encoded", NULL },
	{ 0x41, 4, NULL,            "Monitor status this driving cycle" , 0, 0, "Bit Encoded", NULL },
	{ 0x42, 2, "vpwr",          "Control mod volts", 0, 65535, "V", obdConvert_42 },
	{ 0x43, 2, "load_abs",      "Absolute Load Value", 0, 25700, "%", obdConvert_43 },
	{ 0x44, 2, "lambda",        "Fuel/air Ratio", 0, 2, "f/a", obdConvert_44 },
	{ 0x45, 1, "tp_r",          "Relative Throttle", 0, 100, "%", obdConvert_45 },
	{ 0x46, 1, "aat",           "Ambient Air Temperature", -40, 215, "*C", obdConvert_46 },
	{ 0x47, 1, "tp_b",          "Absolute Throttle Position B", 0, 100, "%", obdConvert_47_4B },
	{ 0x48, 1, "tp_c",          "Absolute Throttle Position C", 0, 100, "%", obdConvert_47_4B },
	{ 0x49, 1, "app_d",         "Accelerator Pedal D", 0, 100, "%", obdConvert_47_4B },
	{ 0x4A, 1, "app_e",         "Accelerator Pedal E", 0, 100, "%", obdConvert_47_4B },
	{ 0x4B, 1, "app_f",         "Accelerator Pedal F", 0, 100, "%", obdConvert_47_4B },
	{ 0x4C, 1, "tac_pct",       "Commanded Throttle Act.", 0, 100, "%", obdConvert_4C },
	{ 0x4D, 2, "mil_time",      "Time w/ MIL on", 0, 65525, "min", obdConvert_4D },
	{ 0x4E, 2, "clr_time",      "Time w/o DTCs", 0, 65535, "min", obdConvert_4E },
	{ 0x4F, 4, NULL,            "External Test Equipment Configuration #1" , 0, 0, "Bit Encoded", NULL }, /// wrong
	{ 0x50, 4, NULL,            "External Test Equipment Configuration #2" , 0, 0, "Bit Encoded", NULL }, /// also wrong
	{ 0x51, 2, "fuel_type",     "Fuel Type", 0, 0, "Bit Encoded", NULL },
	{ 0x52, 2, "alch_pct",      "Ethanol fuel %", 0, 100, "%", obdConvert_52 },
	{ 0x53, 2, "evap_vpa",      "Absolute Evap System Vapor Pressure", 0, 327.675, "kPa", obdConvert_53 },
	{ 0x54, 2, "evap_vp",       "Evap System Vapor Pressure", -32768, 32767, "Pa", obdConvert_54 },

	// these four have strange conversion formulas - two data points from one PID
	{ 0x55, 2, "stso2ft1",      "Short Term Secondary O2 Sensor Fuel Trim – Bank 1/3", -100, 99.22, "%", NULL },
	{ 0x56, 2, "lgso2ft1",      "Long Term Secondary O2 Sensor Fuel Trim – Bank 1/3", -100, 99.22, "%", NULL },
	{ 0x57, 2, "stso2ft2",      "Short Term Secondary O2 Sensor Fuel Trim – Bank 2/4", -100, 99.22, "%", NULL },
	{ 0x58, 2, "lgso2ft2",      "Long Term Secondary O2 Sensor Fuel Trim – Bank 2/4", -100, 99.22, "%", NULL },

	{ 0x59, 2, "frp",           "Fuel Rail Pressure (absolute)", 0, 655350, "kPa", obdConvert_59 },
	{ 0x5A, 1, "app_r",         "Relative Accellerator Pedal Position", 0, 100, "%", obdConvert_47_4B },
	{ 0x5B, 1, "bat_pwr",       "Hybrid Battery Pack Remaining Life", 0, 100, "%", obdConvert_47_4B },
	{ 0x5C, 1, "eot",           "Oil Temp", -40, 215, "*C", obdConvert_46 },
	{ 0x5D, 2, "fuel_timing",   "Injection Timing", -310, 301.99, "deg", obdConvert_5D },
	{ 0x5E, 2, "fuel_rate",     "Engine Fuel Rate", 0, 3276.75, "L/h", obdConvert_5E },
	{ 0x5F, 1, "emis_sup",      "Emission requirements to which vehicle is designed", 0, 0, "Bit Encoded", NULL },
	{ 0x60, 4, NULL,            "PIDs supported 61-80" , 0, 0, "Bit Encoded", NULL },
	{ 0x61, 1, "tq_dd",         "Driver's torque demand" , -125, 130, "%", obdConvert_61 },
	{ 0x62, 1, "tq_act",        "Actual Engine - Percent Torque" , -125, 130, "%", obdConvert_61 },
	{ 0x63, 2, "tq_ref",        "Engine Reference Torque" , 0, 65535, "Nm", obdConvert_4D },
};
#endif
