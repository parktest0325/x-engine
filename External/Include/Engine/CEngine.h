#pragma once

class CEngine
{
private:
	static CEngine* g_This;

public:
	static CEngine* GetInst()
	{
		if (g_This == nullptr) {
			g_This = new CEngine;
		}
		return g_This;
	}

	static void Destroy()
	{
		if (g_This != nullptr)
		{
			delete g_This;
			g_This = nullptr;
		}
	}

private:
	CEngine();
};
