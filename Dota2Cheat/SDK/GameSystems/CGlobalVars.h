#pragma once
#include "../Base/VClass.h"
struct CGlobalVars : public VClass {
public:
	float			    m_realtime;
	int				    m_frame;
	float			    m_abs_frametime;
	float			    m_abs_frame_start_time;
	float			    m_curtime;
	float		        m_frametime;
	int				    m_max_clients;
	int				    m_tick_count;
	float		        m_interval;
	float		        m_interp_amt;
	int	                m_frame_simulation_ticks;
	int				    m_network_protocol;
private:
	bool			    m_client;
	bool		        m_remote_client;
	int			        m_timestamp_networking_base;
	int			        m_timestamp_randomize_window;
};