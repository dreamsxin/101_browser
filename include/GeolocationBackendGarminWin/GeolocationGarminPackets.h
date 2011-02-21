/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MTAx_GeolocationGarminWin_GeolocationGarminPackets_h
#define _MTAx_GeolocationGarminWin_GeolocationGarminPackets_h

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)
typedef struct
{
	uint8_t  mPacketType;
	uint8_t  mReserved1;
	uint16_t mReserved2;
	uint16_t mPacketId;
	uint16_t mReserved3;
	uint32_t mDataSize;
	uint8_t  mData[1];
} Packet_t;

typedef uint16_t Command_Id_Type;

typedef struct
{
	uint8_t  mPacketType;
	uint8_t  mReserved1;
	uint16_t mReserved2;
	uint16_t mPacketId;
	uint16_t mReserved3;
	uint32_t mDataSize;
	Command_Id_Type mCommand;
} Device_Command_Packet_t;

#pragma pack(pop)

enum
{
	PacketType_USB_Protocol_Layer = 0,
	PacketType_Application_Layer = 20
};

enum
{
	Pid_Data_Available = 2,
	Pid_Start_Session = 5,
	Pid_Session_Started = 6 
};

enum
{
	Pid_Protocol_Array = 253, /* may not be implemented in all devices */
	Pid_Product_Rqst = 254,
	Pid_Product_Data = 255,
	Pid_Ext_Product_Data = 248 /* may not be implemented in all devices */
};

// 4.2 L001 – Link Protocol 1
enum
{
	Pid_Command_Data = 10,
	Pid_Xfer_Cmplt = 12,
	Pid_Date_Time_Data = 14,
	Pid_Position_Data = 17,
	Pid_Prx_Wpt_Data = 19,
	Pid_Records = 27,
	Pid_Rte_Hdr = 29,
	Pid_Rte_Wpt_Data = 30,
	Pid_Almanac_Data = 31,
	Pid_Trk_Data = 34,
	Pid_Wpt_Data = 35,
	Pid_Pvt_Data = 51,
	Pid_Rte_Link_Data = 98,
	Pid_Trk_Hdr = 99,
	Pid_FlightBook_Record = 134,
	Pid_Lap = 149,
	Pid_Wpt_Cat = 152,
	Pid_Run = 990,
	Pid_Workout = 991,
	Pid_Workout_Occurrence = 992,
	Pid_Fitness_User_Profile = 993,
	Pid_Workout_Limits = 994,
	Pid_Course = 1061,
	Pid_Course_Lap = 1062,
	Pid_Course_Point = 1063,
	Pid_Course_Trk_Hdr = 1064,
	Pid_Course_Trk_Data = 1065,
	Pid_Course_Limits = 1066
};

// 6.3.1 A010 – Device Command Protocol 1
enum
{
	Cmnd_Abort_Transfer = 0, /* abort current transfer */
	Cmnd_Transfer_Alm = 1, /* transfer almanac */
	Cmnd_Transfer_Posn = 2, /* transfer position */
	Cmnd_Transfer_Prx = 3, /* transfer proximity waypoints */
	Cmnd_Transfer_Rte = 4, /* transfer routes */
	Cmnd_Transfer_Time = 5, /* transfer time */
	Cmnd_Transfer_Trk = 6, /* transfer track log */
	Cmnd_Transfer_Wpt = 7, /* transfer waypoints */
	Cmnd_Turn_Off_Pwr = 8, /* turn off power */
	Cmnd_Start_Pvt_Data = 49, /* start transmitting PVT data */
	Cmnd_Stop_Pvt_Data = 50, /* stop transmitting PVT data */
	Cmnd_FlightBook_Transfer = 92, /* transfer flight records */
	Cmnd_Transfer_Laps = 117, /* transfer fitness laps */
	Cmnd_Transfer_Wpt_Cats = 121, /* transfer waypoint categories */
	Cmnd_Transfer_Runs = 450, /* transfer fitness runs */
	Cmnd_Transfer_Workouts = 451, /* transfer workouts */
	Cmnd_Transfer_Workout_Occurrences = 452, /* transfer workout occurrences */
	Cmnd_Transfer_Fitness_User_Profile = 453, /* transfer fitness user profile */
	Cmnd_Transfer_Workout_Limits = 454, /* transfer workout limits */
	Cmnd_Transfer_Courses = 561, /* transfer fitness courses */
	Cmnd_Transfer_Course_Laps = 562, /* transfer fitness course laps */
	Cmnd_Transfer_Course_Points = 563, /* transfer fitness course points */
	Cmnd_Transfer_Course_Tracks = 564, /* transfer fitness course tracks */
	Cmnd_Transfer_Course_Limits = 565 /* transfer fitness course limits */
};

#pragma pack(push, 1)
typedef struct
{
	uint8_t  tag;
	uint16_t data;
} Protocol_Data_Type;
#pragma pack(pop)

typedef Protocol_Data_Type Protocol_Array_Type[];

// 7.3.13 radian_position_type
#pragma pack(push, 1)
typedef struct
{
	double lat; /* latitude in radians */
	double lon; /* longitude in radians */
} radian_position_type;
#pragma pack(pop)

// 7.4.40 D800_Pvt_Data_Type
#pragma pack(push, 1)
typedef struct
{
	float alt; /* altitude above WGS 84 ellipsoid (meters) */
	float epe; /* estimated position error, 2 sigma (meters) */
	float eph; /* epe, but horizontal only (meters) */
	float epv; /* epe, but vertical only (meters) */
	uint16_t fix; /* type of position fix */
	double tow; /* time of week (seconds) */
	radian_position_type posn; /* latitude and longitude (radians) */
	float east; /* velocity east (meters/second) */
	float north; /* velocity north (meters/second) */
	float up; /* velocity up (meters/second) */
	float msl_hght; /* height of WGS84 ellipsoid above MSL(meters)*/
	int16_t leap_scnds; /* difference between GPS and UTC (seconds) */
	uint32_t wn_days; /* week number days */
} D800_Pvt_Data_Type;
#pragma pack(pop)

enum
{
	fix_unusable = 0, /* failed integrity check */
	fix_invalid = 1, /* invalid or unavailable */
	fix_2D = 2, /* two dimensional */
	fix_3D = 3, /* three dimensional */
	fix_2D_diff = 4, /* two dimensional differential */
	fix_3D_diff = 5 /* three dimensional differential */
};

#ifdef __cplusplus
}
#endif

#endif
