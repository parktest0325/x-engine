#pragma once
class CTimeMgr
	: public CSingleton<CTimeMgr>
{
	SINGLE(CTimeMgr)

private:
	LARGE_INTEGER	m_llFrequency;	// 초당 카운트 수
	LARGE_INTEGER	m_llCurCount;	// 현재 카운트 수
	LARGE_INTEGER	m_llPrevCount;	// 이전 프레임 카운트 수

	float			m_DT;			// 프레임당 걸린시간
	float			m_Time;			// 프로그램 실행 이후 누적시간
	float			m_AccTime;		// 1초를 체크하기위한 시간누적변수
	int				m_FrmCount;

public:
	void init();
	void tick();
};

