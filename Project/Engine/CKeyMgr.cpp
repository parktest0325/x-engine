#include "pch.h"
#include "CKeyMgr.h"

UINT g_KeyValue[(UINT)KEY::KEY_END]
=
{
	'W',
	'S',
	'A',
	'D',

	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_RETURN,
	VK_ESCAPE,
	VK_SPACE,
	VK_LSHIFT,
	VK_MENU,
	VK_CONTROL,
};

CKeyMgr::CKeyMgr()
{
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::init()
{
	for (UINT i = 0; i < (UINT)KEY::KEY_END; ++i)
	{
		tKeyInfo info = {};
		info.State = KEY_STATE::NONE;
		info.PrevPressed = false;
		m_vecKey.push_back(info);
	}
}

void CKeyMgr::tick()
{
	for (size_t i = 0; i < m_vecKey.size(); ++i)
	{
		// 현재 프레임에서 눌렸는지 체크
		if (GetAsyncKeyState(g_KeyValue[i]) & 0x8001)
		{
			if (!m_vecKey[i].PrevPressed)
			{
				m_vecKey[i].State = KEY_STATE::TAP;
			}
			else
			{
				m_vecKey[i].State = KEY_STATE::PRESSED;
			}
			m_vecKey[i].PrevPressed = true;
		}
		else
		{
			if (m_vecKey[i].PrevPressed)
			{
				m_vecKey[i].State = KEY_STATE::RELEASED;
			}
			else
			{
				m_vecKey[i].State = KEY_STATE::NONE;
			}
			m_vecKey[i].PrevPressed = false;
		}
	}
}
