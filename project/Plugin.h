#pragma once
#include "IExamPlugin.h"
#include "Exam_HelperStructs.h"
#include "BehaviorTree.h"

class IBaseInterface;
class IExamInterface;

class Plugin :public IExamPlugin
{
public:
	Plugin() {};
	virtual ~Plugin() {};

	void Initialize(IBaseInterface* pInterface, PluginInfo& info) override;
	void DllInit() override;
	void DllShutdown() override;

	void InitGameDebugParams(GameDebugParams& params) override;
	void Update(float dt) override;

	SteeringPlugin_Output UpdateSteering(float dt) override;
	void Render(float dt) const override;

private:
	//Interface, used to request data from/perform actions with the AI Framework
	IExamInterface* m_pInterface = nullptr;
	vector<HouseInfo> GetHousesInFOV() const;
	vector<EntityInfo> GetEntitiesInFOV() const;

	Elite::Vector2 m_Target = {};
	bool m_CanRun = false; //Demo purpose
	bool m_GrabItem = false; //Demo purpose
	bool m_UseItem = false; //Demo purpose
	bool m_RemoveItem = false; //Demo purpose
	float m_AngSpeed = 0.f; //Demo purpose

	std::shared_ptr<Blackboard> m_pBlackboard = nullptr;
	std::shared_ptr<BehaviorTree> m_pBehaviorTree = nullptr;

	vector<Elite::Vector2> m_HousesNotSearched;
	vector<Elite::Vector2> m_HousesSearched;
	vector<HouseInfo> m_HousesSeen;


	// inventory
	static int inventorySize;
	ItemInfo m_ItemInfo;
	std::vector<bool> m_UsedItemSlots { false, false, false, false, false };
	std::vector<ItemInfo> m_Inventory { ItemInfo(), ItemInfo(), ItemInfo(), ItemInfo(), ItemInfo() };
	unsigned int m_PistolSlot;
	unsigned int m_MedkitSlot;
	unsigned int m_FoodSlot;
	unsigned int m_Ammo;
	int m_PistolCount;
	int m_MedkitCount;
	int m_FoodCount;

	bool m_Search = false;

	Elite::Vector2 m_TempTarget = {0,0};
};

//ENTRY
//This is the first function that is called by the host program
//The plugin returned by this function is also the plugin used by the host program
extern "C"
{
	__declspec (dllexport) IPluginBase* Register()
	{
		return new Plugin();
	}
}