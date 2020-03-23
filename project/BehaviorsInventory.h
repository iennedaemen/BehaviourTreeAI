#pragma once
#include "Blackboard.h"
#include "IExamInterface.h"
#include "BehaviorTree.h"
#include "Exam_HelperStructs.h"

// conditionals
inline BehaviorState RoomInInventory(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<bool> usedSlots;
	bool dataAvailable = pBlackboard->GetData("UsedInventorySlots", usedSlots);
	if (!dataAvailable) BehaviorState::Fail;

	for(bool used : usedSlots)
	{
		if(used == false)
			return BehaviorState::Succeed;
	}
	return  BehaviorState::Fail;
}

inline BehaviorState HasItem(std::shared_ptr<Blackboard> pBlackboard)
{
	ItemInfo item;
	bool dataAvailable = pBlackboard->GetData("PickupItem", item);
	if (!dataAvailable) BehaviorState::Fail;

	if (item.Type != eItemType::GARBAGE)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState IsFood(std::shared_ptr<Blackboard> pBlackboard)
{
	ItemInfo item;
	bool dataAvailable = pBlackboard->GetData("PickupItem", item);
	if (!dataAvailable) BehaviorState::Fail;

	if (item.Type == eItemType::FOOD)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}
inline BehaviorState IsMedkit(std::shared_ptr<Blackboard> pBlackboard)
{
	ItemInfo item;
	bool dataAvailable = pBlackboard->GetData("PickupItem", item);
	if (!dataAvailable) BehaviorState::Fail;

	if (item.Type == eItemType::MEDKIT)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}
inline BehaviorState IsPistol(std::shared_ptr<Blackboard> pBlackboard)
{
	ItemInfo item;
	bool dataAvailable = pBlackboard->GetData("PickupItem", item);
	if (!dataAvailable) BehaviorState::Fail;

	if (item.Type == eItemType::PISTOL)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState NeedsFood(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountFood;
	bool dataAvailable = pBlackboard->GetData("AmountFood", amountFood);
	if (!dataAvailable) BehaviorState::Fail;

	if (amountFood <= 2)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}
inline BehaviorState NeedsMedkit(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountMedkit;
	bool dataAvailable = pBlackboard->GetData("AmountMedKit", amountMedkit);
	if (!dataAvailable) BehaviorState::Fail;

	if (amountMedkit < 2)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}
inline BehaviorState NeedsPistol(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountGun;
	bool dataAvailable = pBlackboard->GetData("AmountGuns", amountGun);
	if (!dataAvailable) BehaviorState::Fail;

	if (amountGun < 2)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState canLoseFood(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountFood;
	bool dataAvailable = pBlackboard->GetData("AmountFood", amountFood);
	if (!dataAvailable) BehaviorState::Fail;

	if (amountFood > 3)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}
inline BehaviorState CanLoseMedkit(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountMedkit;
	bool dataAvailable = pBlackboard->GetData("AmountMedKit", amountMedkit);
	if (!dataAvailable) BehaviorState::Fail;

	if (amountMedkit > 2)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}
inline BehaviorState CanLosePistol(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountGun;
	bool dataAvailable = pBlackboard->GetData("AmountGuns", amountGun);
	if (!dataAvailable) BehaviorState::Fail;

	if (amountGun > 2)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

// behaviors
inline BehaviorState RemoveFood(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> inventory;
	std::vector<bool> usedSlots;
	int amountFood;

	bool dataAvailable = pBlackboard->GetData("UsedInventorySlots", usedSlots) && pBlackboard->GetData("Items", inventory)
		&& pBlackboard->GetData("AmountFood", amountFood);
	if (!dataAvailable) return BehaviorState::Fail;

	for (size_t i{}; i < 5; ++i)
	{
		if (inventory[i].Type == eItemType::FOOD && usedSlots[i])
		{
			if(pBlackboard->GetInterface()->Inventory_UseItem(i))
			{
				amountFood--;
				inventory[i].Type = eItemType::GARBAGE;
			}
			if (pBlackboard->GetInterface()->Inventory_RemoveItem(i))
			{
				usedSlots[i] = false;
				break;
			}
		}
	}

	pBlackboard->ChangeData("Items", inventory);
	pBlackboard->ChangeData("AmountFood", amountFood);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);

	return BehaviorState::Succeed;
}
inline BehaviorState RemoveMedkit(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> inventory;
	std::vector<bool> usedSlots;
	int amountMedkit;

	bool dataAvailable = pBlackboard->GetData("UsedInventorySlots", usedSlots) && pBlackboard->GetData("Items", inventory)
		&& pBlackboard->GetData("AmountMedKit", amountMedkit);
	if (!dataAvailable) return BehaviorState::Fail;

	for (size_t i{}; i < 5; ++i)
	{
		if (inventory[i].Type == eItemType::MEDKIT && usedSlots[i])
		{
			if (pBlackboard->GetInterface()->Inventory_UseItem(i))
			{
				amountMedkit--;
				inventory[i].Type = eItemType::GARBAGE;
			}
			if (pBlackboard->GetInterface()->Inventory_RemoveItem(i))
			{
				usedSlots[i] = false;
				break;
			}
		}
	}

	pBlackboard->ChangeData("Items", inventory);
	pBlackboard->ChangeData("AmountMedKit", amountMedkit);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);

	return BehaviorState::Succeed;
}
inline BehaviorState RemovePistol(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> inventory;
	std::vector<bool> usedSlots;
	int amountGuns;

	bool dataAvailable = pBlackboard->GetData("UsedInventorySlots", usedSlots) && pBlackboard->GetData("Items", inventory)
		&& pBlackboard->GetData("AmountGuns", amountGuns);
	if (!dataAvailable) return BehaviorState::Fail;

	for (size_t i{}; i < 5; ++i)
	{
		if (inventory[i].Type == eItemType::PISTOL && usedSlots[i])
		{
			if (pBlackboard->GetInterface()->Inventory_RemoveItem(i))
			{
				amountGuns--;
				usedSlots[i] = false;
				inventory[i].Type = eItemType::GARBAGE;
				break;
			}
		}
	}

	pBlackboard->ChangeData("Items", inventory);
	pBlackboard->ChangeData("AmountGuns", amountGuns);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);

	return BehaviorState::Succeed;
}

inline BehaviorState StoreFood(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> inventory;
	std::vector<bool> usedSlots;
	ItemInfo pickupItem;
	int amountFood;

	bool dataAvailable = pBlackboard->GetData("UsedInventorySlots", usedSlots) && pBlackboard->GetData("Items", inventory)
		&& pBlackboard->GetData("PickupItem", pickupItem) && pBlackboard->GetData("AmountFood", amountFood);
	if (!dataAvailable)
		return BehaviorState::Fail;

	for (size_t i{}; i < 5; ++i)
	{
		if (!usedSlots[i])
		{
			inventory[i] = pickupItem;
			pBlackboard->GetInterface()->Inventory_AddItem(i, pickupItem);
			usedSlots[i] = true;
			amountFood++;
			pickupItem.Type = eItemType::GARBAGE;
			break;
		}
		if( i == 4) return BehaviorState::Fail;
	}

	pBlackboard->ChangeData("Items", inventory);
	pBlackboard->ChangeData("AmountFood", amountFood);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);
	pBlackboard->ChangeData("PickupItem", pickupItem);

	return BehaviorState::Succeed;
}
inline BehaviorState StoreMedkit(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> inventory;
	std::vector<bool> usedSlots;
	ItemInfo pickupItem;
	int amountMedkit;

	bool dataAvailable = pBlackboard->GetData("UsedInventorySlots", usedSlots) && pBlackboard->GetData("Items", inventory)
		&& pBlackboard->GetData("PickupItem", pickupItem) && pBlackboard->GetData("AmountMedKit", amountMedkit);
	if (!dataAvailable) return BehaviorState::Fail;

	for (size_t i{}; i < 5; ++i)
	{
		if (!usedSlots[i])
		{
			pBlackboard->GetInterface()->Inventory_AddItem(i, pickupItem);
			inventory[i] = pickupItem;
			usedSlots[i] = true;
			amountMedkit++;
			pickupItem.Type = eItemType::GARBAGE;
			break;
		}

		if (i == 4)
		{
			return BehaviorState::Fail;
		}
	}

	pBlackboard->ChangeData("Items", inventory);
	pBlackboard->ChangeData("AmountMedKit", amountMedkit);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);
	pBlackboard->ChangeData("PickupItem", pickupItem);

	return BehaviorState::Succeed;
}
inline BehaviorState StorePistol(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> inventory;
	std::vector<bool> usedSlots;
	ItemInfo pickupItem;
	int amountPistol;

	bool dataAvailable = pBlackboard->GetData("UsedInventorySlots", usedSlots) && pBlackboard->GetData("Items", inventory)
		&& pBlackboard->GetData("PickupItem", pickupItem) && pBlackboard->GetData("AmountGuns", amountPistol);
	if (!dataAvailable) return BehaviorState::Fail;

	for (size_t i{}; i < 5; ++i)
	{
		if (!usedSlots[i])
		{
			pBlackboard->GetInterface()->Inventory_AddItem(i, pickupItem);
			inventory[i] = pickupItem;
			usedSlots[i] = true;
			amountPistol++;
			pickupItem.Type = eItemType::GARBAGE;
			break;
		}

		if (i == 4)
		{
			return BehaviorState::Fail;
		}
	}

	pBlackboard->ChangeData("Items", inventory);
	pBlackboard->ChangeData("AmountGuns", amountPistol);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);
	pBlackboard->ChangeData("PickupItem", pickupItem);

	return BehaviorState::Succeed;
}

inline BehaviorState IgnoreItem(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<Elite::Vector2> ignoreItems;
	ItemInfo item;

	bool dataAvailable = pBlackboard->GetData("IgnoreItemsPos", ignoreItems) && pBlackboard->GetData("PickupItem", item);
	if (!dataAvailable) return BehaviorState::Fail;

	ignoreItems.push_back(item.Location);

	pBlackboard->ChangeData("IgnoreItemsPos", ignoreItems);
	return BehaviorState::Succeed;
}
inline BehaviorState ResetIgnoreItems(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<Elite::Vector2> ignoreItems;

	bool dataAvailable = pBlackboard->GetData("IgnoreItemsPos", ignoreItems);
	if (!dataAvailable) return BehaviorState::Fail;

	ignoreItems.clear();

	pBlackboard->ChangeData("IgnoreItemsPos", ignoreItems);
	return BehaviorState::Succeed;
}