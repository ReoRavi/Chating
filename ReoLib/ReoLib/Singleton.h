#pragma once

//===============================================
// ISingleton
// Info : 디자인 패턴중 하나, 전 영역의 걸쳐 하나의 클래스의 단 하나의 인스턴스만을 생성한다.
template <class T>
class ISingleton
{
private:
	static T* m_Instance;
public:
	ISingleton(void) { /* empty */ };
	~ISingleton(void) { delete m_Instance; };

	static T* GetInstance(void)
	{
		if (m_Instance == NULL)
		{
			m_Instance = new T();
		}
		return m_Instance;
	}
};

template <class T> T* ISingleton<T>::m_Instance = 0;