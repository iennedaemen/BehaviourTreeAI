#pragma once
#include <unordered_map>
#include <memory>
#include <iostream>

class BlackBoardField
{
public:
	BlackBoardField() = default;
	virtual ~BlackBoardField() = default;
};

template<typename T>
class BlackboardField : public BlackBoardField
{
public:
	explicit BlackboardField(T data) : m_Data(data)
	{}
	T GetData() { return m_Data; };
	void SetData(T data) { m_Data = data; }

private:
	T m_Data;
};


class Blackboard final
{
public:
	IExamInterface* GetInterface()
	{
		return m_pInterface;
	}

	void SetInterface(IExamInterface* interface)
	{
		m_pInterface = interface;
	}

	template<typename T> 
	bool AddData(const std::string& name, T data)
	{
		auto it = m_BlackboardData.find(name);
		if (it == m_BlackboardData.end())
		{
			m_BlackboardData[name] = std::make_shared<BlackboardField<T>>(data);
			return true;
		}
		std::cout << name.c_str() << " with data " << typeid(T).name() << " already exists in blackboard -> bool Blackboard::AddData()\n";
		return false;
	}

	template<typename T> 
	bool ChangeData(const std::string& name, T data)
	{
		auto it = m_BlackboardData.find(name);
		if (it != m_BlackboardData.end())
		{
			std::shared_ptr <BlackboardField<T>> pVar = dynamic_pointer_cast<BlackboardField<T>>(m_BlackboardData[name]);
			if (pVar)
			{
				pVar->SetData(data);
				return true;
			}
		}
		std::cout << name << " with data " << typeid(T).name() << " not found in blackboard -> bool Blackboard::ChangeData()\n";
		return false;
	}

	template<typename T> 
	bool GetData(const std::string& name, T& data)
	{
		auto temp = m_BlackboardData[name];
		std::shared_ptr <BlackboardField<T>> pVar = dynamic_pointer_cast<BlackboardField<T>>(m_BlackboardData[name]);
		if (pVar != nullptr)
		{
			data = pVar->GetData();
			return true;
		}
		std::cout << name << " with data " << typeid(T).name() << " not found in blackboard -> bool Blackboard::GetData()\n";
		return false;
	}

private:
	std::unordered_map<std::string, std::shared_ptr<BlackBoardField>> m_BlackboardData;
	IExamInterface* m_pInterface;
};

