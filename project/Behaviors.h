#pragma once
#include "Blackboard.h"
#include "IExamInterface.h"
#include "BehaviorTree.h"
#include "Exam_HelperStructs.h"

// conditionals
inline BehaviorState HasMedKid(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountMedKit;
	bool dataAvailable = pBlackboard->GetData("AmountMedKit", amountMedKit);
	if (!dataAvailable) BehaviorState::Fail;

	if (amountMedKit > 0) return BehaviorState::Succeed;

	else return BehaviorState::Fail;
}

inline BehaviorState ShouldHeal(std::shared_ptr<Blackboard> pBlackboard)
{
	AgentInfo agent;
	bool dataAvailable = pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) BehaviorState::Fail;

	if(agent.Health < 5)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState IsHungry(std::shared_ptr<Blackboard> pBlackboard)
{
	AgentInfo agent;

	bool dataAvailable = pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if (agent.Energy < 3)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState HasFood(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountFood = false;

	bool dataAvailable = pBlackboard->GetData("AmountFood", amountFood);
	if (!dataAvailable) return BehaviorState::Fail;

	if (amountFood > 0)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState HasPistol(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountPistol = false;

	bool dataAvailable = pBlackboard->GetData("AmountGuns", amountPistol);
	if (!dataAvailable) return BehaviorState::Fail;

	if (amountPistol > 0)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState HasNoPistol(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountPistol = false;

	bool dataAvailable = pBlackboard->GetData("AmountGuns", amountPistol);
	if (!dataAvailable) return BehaviorState::Fail;

	if (amountPistol <= 0)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState WantToAttack(std::shared_ptr<Blackboard> pBlackboard)
{
	int nrEnemies{};
	AgentInfo agent;

	bool dataAvailable = pBlackboard->GetData("AmountEnemiesFOV", nrEnemies) && pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if ((nrEnemies <= 2 && nrEnemies > 0) || (nrEnemies > 0 && agent.IsInHouse))
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState SeesItem(std::shared_ptr<Blackboard> pBlackboard)
{
	int nrItemsFOV{};

	bool dataAvailable = pBlackboard->GetData("AmountItemsFOV", nrItemsFOV);
	if (!dataAvailable) return BehaviorState::Fail;

	if (nrItemsFOV > 0)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState ShouldFleeWithGun(std::shared_ptr<Blackboard> pBlackboard)
{
	float fleeTime{};
	bool fleeing = false;
	AgentInfo agent;

	bool dataAvailable = pBlackboard->GetData("Agent", agent) && pBlackboard->GetData("FleeTime", fleeTime) && pBlackboard->GetData("FleeingWithGun", fleeing);
	if (!dataAvailable) return BehaviorState::Fail;

	if ((fleeTime > 0.0f || fleeing) && !agent.IsInHouse && agent.Energy > 3.0f)
	{
		pBlackboard->ChangeData("FleeingWithGun", true);
		return BehaviorState::Succeed;
	}
	else
	{
		pBlackboard->ChangeData("FleeingWithGun", false);
		return  BehaviorState::Fail;
	}
}

inline BehaviorState ShouldFleeWithoutGun(std::shared_ptr<Blackboard> pBlackboard)
{
	float fleeTime{};
	bool fleeing = false;
	AgentInfo agent;

	bool dataAvailable = pBlackboard->GetData("FleeTime", fleeTime) && pBlackboard->GetData("FleeingWithoutGun", fleeing) 
		&& pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if ((fleeTime > 0.0f || fleeing) && !agent.IsInHouse && agent.Energy > 3.0f)
	{
		pBlackboard->ChangeData("FleeingWithoutGun", true);
		return BehaviorState::Succeed;
	}
	else
	{
		pBlackboard->ChangeData("FleeingWithoutGun", false);
		return  BehaviorState::Fail;
	}
}

inline BehaviorState NeedsItem(std::shared_ptr<Blackboard> pBlackboard)
{
	int amountFood{};
	int amountPistols{};
	int amountMedKits{};


	bool dataAvailable = pBlackboard->GetData("AmountFood", amountFood) && pBlackboard->GetData("AmountGuns", amountPistols) && pBlackboard->GetData("AmountMedKit", amountMedKits);
	if (!dataAvailable) return BehaviorState::Fail;

	if (amountFood <= 1 || amountPistols <=0 || amountMedKits <=0)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState ReachedCheckpoint(std::shared_ptr<Blackboard> pBlackboard)
{
	AgentInfo agent;

	bool dataAvailable = pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if (Distance(pBlackboard->GetInterface()->World_GetCheckpointLocation(), agent.Position) < 2.0f)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState NotExploredHouse(std::shared_ptr<Blackboard> pBlackboard)
{
	bool searched;

	bool dataAvailable = pBlackboard->GetData("SearchedHouse", searched);
	if (!dataAvailable) return BehaviorState::Fail;

	if (!searched)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState HouseNotSearched(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<Elite::Vector2> houses;

	bool dataAvailable = pBlackboard->GetData("HousesNotSearchedPos", houses);
	if (!dataAvailable) return BehaviorState::Fail;

	if (!houses.empty())
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState InsideHouse(std::shared_ptr<Blackboard> pBlackboard)
{
	AgentInfo agent;

	bool dataAvailable = pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if (agent.IsInHouse)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

// action
inline BehaviorState Eat(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> items;
	AgentInfo pAgent;
	int amountFood;
	std::vector<bool> usedSlots;
	bool dataAvailable = pBlackboard->GetData("Agent", pAgent) && pBlackboard->GetData("Items", items) 
		&& pBlackboard->GetData("AmountFood", amountFood) && pBlackboard->GetData("UsedInventorySlots", usedSlots);
	if (!dataAvailable) return BehaviorState::Fail;

	for(size_t i{}; i < items.size(); ++i)
	{
		if (items[i].Type == eItemType::FOOD && usedSlots[i])
		{
			if(pBlackboard->GetInterface()->Inventory_UseItem(i))
			{
				if (pBlackboard->GetInterface()->Inventory_RemoveItem(i))
				{
					amountFood--;
				}
				items[i].Type = eItemType::GARBAGE;
				usedSlots[i] = false;
				break;
			}
		}
	}

	pBlackboard->ChangeData("Items", items);
	pBlackboard->ChangeData("AmountFood", amountFood);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);

	return BehaviorState::Succeed;
}

inline BehaviorState Attack(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> items;
	AgentInfo agent;
	int amoundPistols;
	std::vector<bool> usedSlots;
	EnemyInfo nearestEnemy{};
	SteeringPlugin_Output steering;
	bool dataAvailable = pBlackboard->GetData("Agent", agent) && pBlackboard->GetData("Items", items) && pBlackboard->GetData("NearestEnemy", nearestEnemy)
		&& pBlackboard->GetData("AmountGuns", amoundPistols) && pBlackboard->GetData("UsedInventorySlots", usedSlots) && pBlackboard->GetData("Steering", steering);;
	if (!dataAvailable) return BehaviorState::Fail;

	float angle = 0.0f;
	Elite::Vector2 agentDir{ cosf(agent.Orientation - (float)M_PI * 0.5f), sinf(agent.Orientation - (float)M_PI * 0.5f) };
	Elite::Vector2 otherDir = nearestEnemy.Location - agent.Position;
	float dot = Elite::Dot(agentDir, otherDir);
	float magA = agentDir.Magnitude();
	float magB = otherDir.Magnitude();
	angle = (float)acos(dot / (magA * magB));

	if (Elite::Distance(agent.Position, nearestEnemy.Location) * sinf(angle) < nearestEnemy.Size * 0.75f)
	{
		for (size_t i{}; i < items.size(); ++i)
		{
			if (items[i].Type == eItemType::PISTOL && usedSlots[i])
			{
				if (!pBlackboard->GetInterface()->Inventory_UseItem(i))
				{
					if (pBlackboard->GetInterface()->Inventory_RemoveItem(i))
					{
						amoundPistols--;
					}
					items[i].Type = eItemType::GARBAGE;
					usedSlots[i] = false;
					break;
				}
			}
		}
	}
	else
	{
		auto nextTargetPos = nearestEnemy.Location;
		steering.LinearVelocity = nextTargetPos - agent.Position;
		steering.LinearVelocity.Normalize();
		steering.LinearVelocity *= agent.MaxLinearSpeed;
	}

	pBlackboard->ChangeData("Items", items);
	pBlackboard->ChangeData("AmountGuns", amoundPistols);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);
	pBlackboard->ChangeData("Agent", agent);
	pBlackboard->ChangeData("Steering", steering);

	return BehaviorState::Succeed;
}

inline BehaviorState Heal(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<ItemInfo> items;
	AgentInfo agent;
	int amoundMedKit;
	std::vector<bool> usedSlots;
	bool dataAvailable = pBlackboard->GetData("Agent", agent) && pBlackboard->GetData("Items", items)
		&& pBlackboard->GetData("AmountMedKit", amoundMedKit) && pBlackboard->GetData("UsedInventorySlots", usedSlots);
	if (!dataAvailable) return BehaviorState::Fail;

	for (size_t i{}; i < items.size(); ++i)
	{
		if (items[i].Type == eItemType::MEDKIT && usedSlots[i])
		{
			if (!pBlackboard->GetInterface()->Inventory_UseItem(i))
			{
				if (pBlackboard->GetInterface()->Inventory_RemoveItem(i))
				{
					amoundMedKit--;
					//items[i].Type = eItemType::GARBAGE;
					//usedSlots[i] = false;
				}
				items[i].Type = eItemType::GARBAGE;
				usedSlots[i] = false;
				break;
			}
		}
	}

	pBlackboard->ChangeData("Items", items);
	pBlackboard->ChangeData("AmountMedKit", amoundMedKit);
	pBlackboard->ChangeData("UsedInventorySlots", usedSlots);
	pBlackboard->ChangeData("Agent", agent);

	return BehaviorState::Succeed;
}

inline BehaviorState Flee(std::shared_ptr<Blackboard> pBlackboard)
{
	AgentInfo agent;
	EnemyInfo nearestEnemy{};
	SteeringPlugin_Output steering;
	int nrEnemies;
	std::vector<HouseInfo> houses;

	bool dataAvailable = pBlackboard->GetData("Agent", agent) && pBlackboard->GetData("NearestEnemy", nearestEnemy)
		&& pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("AmountEnemiesFOV", nrEnemies) && pBlackboard->GetData("HousesFOV", houses);
	if (!dataAvailable) return BehaviorState::Fail;

	Elite::Vector2  target{};
	if (houses.empty() || agent.IsInHouse)
	{
		target = nearestEnemy.Location - agent.Position;//nearestEnemy.Location - agent.Position;
		target = agent.Position - target;
		target = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(target);
	}
	else
	{
		target = houses.front().Center;
		target = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(target);
	}

	steering.LinearVelocity = target; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed
	pBlackboard->GetInterface()->Draw_Circle(target, 2, { 1,1,1 }, 0);
	if (Elite::Distance(target, agent.Position) < 2.f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
	}
	steering.RunMode = true;
	pBlackboard->ChangeData("Steering", steering);
	return BehaviorState::Succeed;
}

inline BehaviorState pBehaviorSeekCheckpoint(std::shared_ptr<Blackboard> pBlackboard)
{
	AgentInfo agent;
	SteeringPlugin_Output steering;

	bool dataAvailable = pBlackboard->GetData("Agent", agent) && pBlackboard->GetData("Steering", steering);
	if (!dataAvailable) return BehaviorState::Fail;
	auto t = pBlackboard->GetInterface()->World_GetCheckpointLocation();
	Elite::Vector2 nextTargetPos = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(pBlackboard->GetInterface()->World_GetCheckpointLocation());
	steering.LinearVelocity = nextTargetPos - agent.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agent.Position) < 2.f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
	}

	steering.RunMode = false;
	pBlackboard->ChangeData("Steering", steering);

	return BehaviorState::Succeed;
}

inline BehaviorState SeekItem(std::shared_ptr<Blackboard> pBlackboard)
{
	AgentInfo agent;
	SteeringPlugin_Output steering;
	Elite::Vector2 itemPos;
	std::vector<Elite::Vector2> ignoreItems;

	bool dataAvailable = pBlackboard->GetData("Agent", agent) && pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("NearestItemPosition", itemPos)
		&& pBlackboard->GetData("IgnoreItemsPos", ignoreItems);
	if (!dataAvailable) return BehaviorState::Fail;

	for(auto item : ignoreItems)
	{
		if (itemPos == item)
			return BehaviorState::Fail;
	}


	Elite::Vector2 nextTargetPos = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(itemPos);
	steering.LinearVelocity = nextTargetPos - agent.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agent.Position) < 2.f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
	}
	steering.RunMode = false;
	pBlackboard->ChangeData("Steering", steering);

	return BehaviorState::Succeed;
}

inline BehaviorState SeekHouse(std::shared_ptr<Blackboard> pBlackboard)
{
	HouseInfo house;
	SteeringPlugin_Output steering;
	AgentInfo agent;
	std::vector<Elite::Vector2> searchedHouses;

	bool dataAvailable = pBlackboard->GetData("InsideHouse", house) && pBlackboard->GetData("HousesSearchedPos", searchedHouses)
		&& pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if(house.Size.x == 0) return BehaviorState::Fail;

	for (auto houseSearched : searchedHouses)
	{
		if (house.Center == houseSearched)
		{
			return BehaviorState::Fail;
		}
	}

	Elite::Vector2 target = house.Center;

	Elite::Vector2 nextTargetPos = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(target);
	steering.LinearVelocity = nextTargetPos - agent.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agent.Position) < 2.f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
	}
	steering.RunMode = false;
	pBlackboard->ChangeData("Steering", steering);

	return  BehaviorState::Succeed;
}

inline BehaviorState ExploreHouse(std::shared_ptr<Blackboard> pBlackboard)
{
	HouseInfo house;
	SteeringPlugin_Output steering;
	AgentInfo agent;
	std::vector<std::vector<Elite::Vector2>> grid;
	std::vector<Elite::Vector2> searchedHouses;

	bool dataAvailable = pBlackboard->GetData("InsideHouse", house) && pBlackboard->GetData("Grid", grid)
		&& pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("Agent", agent) && pBlackboard->GetData("HousesSearchedPos", searchedHouses);
	if (!dataAvailable) return BehaviorState::Fail;

	if (grid.empty())
	{
		pBlackboard->ChangeData("SearchedHouse", true);
		return BehaviorState::Fail;
	}

	Elite::Vector2 target = grid.back().back();

	Elite::Vector2 nextTargetPos = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(target);
	steering.LinearVelocity = nextTargetPos - agent.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agent.Position) < 2.0f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
		if(grid.back().empty())
		{
			grid.pop_back();
			if (grid.empty())
			{
				pBlackboard->ChangeData("SearchedHouse", true);
			}
		}
		else
		{
			grid.back().pop_back();
			if (grid.back().empty())
				grid.pop_back();
			if (grid.empty())
			{
				pBlackboard->ChangeData("SearchedHouse", true);
			}
		}
		pBlackboard->ChangeData("Grid", grid);

	}
	steering.RunMode = false;
	pBlackboard->ChangeData("Steering", steering);
	return  BehaviorState::Succeed;
}

inline BehaviorState ResetHouses(std::shared_ptr<Blackboard> pBlackboard)
{
	std::vector<Elite::Vector2> searchedHouses;
	std::vector<Elite::Vector2> notSearchedHouses;

	bool dataAvailable = pBlackboard->GetData("HousesNotSearchedPos", notSearchedHouses) && pBlackboard->GetData("HousesNotSearchedPos", searchedHouses);
	if (!dataAvailable) return BehaviorState::Fail;

	searchedHouses.clear();
	notSearchedHouses.clear();

	pBlackboard->ChangeData("HousesNotSearchedPos", notSearchedHouses);
	pBlackboard->ChangeData("HousesSearchedPos", searchedHouses);
	return  BehaviorState::Succeed;
}