#pragma once
#include "Blackboard.h"
#include "BehaviorTree.h"
#include "Exam_HelperStructs.h"

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

inline BehaviorState ExploredRightUp(std::shared_ptr<Blackboard> pBlackboard)
{
	bool searched;

	bool dataAvailable = pBlackboard->GetData("SearchedRightUp", searched);
	if (!dataAvailable) return BehaviorState::Fail;

	if (!searched)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState ExploredRightDown(std::shared_ptr<Blackboard> pBlackboard)
{
	bool searched;

	bool dataAvailable = pBlackboard->GetData("SearchedRightDown", searched);
	if (!dataAvailable) return BehaviorState::Fail;

	if (!searched)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState ExploredLeftDown(std::shared_ptr<Blackboard> pBlackboard)
{
	bool searched;

	bool dataAvailable = pBlackboard->GetData("SearchedLeftDown", searched);
	if (!dataAvailable) return BehaviorState::Fail;

	if (!searched)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}

inline BehaviorState ExploredLeftUp(std::shared_ptr<Blackboard> pBlackboard)
{
	bool searched;

	bool dataAvailable = pBlackboard->GetData("SearchedLeftUp", searched);
	if (!dataAvailable) return BehaviorState::Fail;

	if (!searched)
		return BehaviorState::Succeed;
	else
		return  BehaviorState::Fail;
}



/////////////
// ACTIONS
/////////////

inline BehaviorState SeekHouse(std::shared_ptr<Blackboard> pBlackboard)
{
	HouseInfo house;
	SteeringPlugin_Output steering;
	AgentInfo agent;

	bool dataAvailable = pBlackboard->GetData("InsideHouse", house)
		&& pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	float dist = 100000.0f;
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

inline BehaviorState ExploreHouseRightUp(std::shared_ptr<Blackboard> pBlackboard)
{
	HouseInfo house;
	SteeringPlugin_Output steering;
	AgentInfo agent;
	bool searched;

	bool dataAvailable = pBlackboard->GetData("InsideHouse", house) && pBlackboard->GetData("SearchedRightUp", searched)
		&& pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if (searched) return BehaviorState::Fail;

	float dist = 100000.0f;
	Elite::Vector2 target = { house.Center.x + house.Size.x / 2, house.Center.y + house.Size.y / 2 };

	Elite::Vector2 nextTargetPos = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(target);
	steering.LinearVelocity = nextTargetPos - agent.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agent.Position) < 2.f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
		pBlackboard->ChangeData("SearchedRightUp", true);
	}
	steering.RunMode = false;
	pBlackboard->ChangeData("Steering", steering);
	return  BehaviorState::Succeed;
}

inline BehaviorState ExploreHouseRightDown(std::shared_ptr<Blackboard> pBlackboard)
{
	HouseInfo house;
	SteeringPlugin_Output steering;
	AgentInfo agent;
	bool searched;

	bool dataAvailable = pBlackboard->GetData("InsideHouse", house) && pBlackboard->GetData("SearchedRightDown", searched)
		&& pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if (searched) return BehaviorState::Fail;

	Elite::Vector2 target = { house.Center.x + house.Size.x / 2 , house.Center.y - house.Size.y / 2 };

	Elite::Vector2 nextTargetPos = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(target);
	steering.LinearVelocity = nextTargetPos - agent.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agent.Position) < 0.1f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
		pBlackboard->ChangeData("SearchedRightDown", true);
	}
	steering.RunMode = false;
	pBlackboard->ChangeData("Steering", steering);

	return  BehaviorState::Succeed;
}

inline BehaviorState ExploreHouseLeftDown(std::shared_ptr<Blackboard> pBlackboard)
{
	HouseInfo house;
	SteeringPlugin_Output steering;
	AgentInfo agent;
	bool searched;

	bool dataAvailable = pBlackboard->GetData("InsideHouse", house) && pBlackboard->GetData("SearchedLeftDown", searched)
		&& pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if (searched) return BehaviorState::Fail;

	float dist = 100000.0f;
	Elite::Vector2 target = { house.Center.x - house.Size.x / 2 , house.Center.y - house.Size.y / 2 };

	Elite::Vector2 nextTargetPos = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(target);
	steering.LinearVelocity = nextTargetPos - agent.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agent.Position) < 2.f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
		pBlackboard->ChangeData("SearchedLeftDown", true);
	}
	steering.RunMode = false;
	pBlackboard->ChangeData("Steering", steering);

	return  BehaviorState::Succeed;
}

inline BehaviorState ExploreHouseLeftUp(std::shared_ptr<Blackboard> pBlackboard)
{
	HouseInfo house;
	SteeringPlugin_Output steering;
	AgentInfo agent;
	bool searched;

	bool dataAvailable = pBlackboard->GetData("InsideHouse", house) && pBlackboard->GetData("SearchedLeftUp", searched)
		&& pBlackboard->GetData("Steering", steering) && pBlackboard->GetData("Agent", agent);
	if (!dataAvailable) return BehaviorState::Fail;

	if (searched) return BehaviorState::Fail;

	float dist = 100000.0f;
	Elite::Vector2 target = { house.Center.x - house.Size.x / 2 + 3 , house.Center.y + house.Size.y / 2 - 3 };

	Elite::Vector2 nextTargetPos = pBlackboard->GetInterface()->NavMesh_GetClosestPathPoint(target);
	steering.LinearVelocity = nextTargetPos - agent.Position; //Desired Velocity
	steering.LinearVelocity.Normalize(); //Normalize Desired Velocity
	steering.LinearVelocity *= agent.MaxLinearSpeed; //Rescale to Max Speed

	if (Distance(nextTargetPos, agent.Position) < 2.f)
	{
		steering.LinearVelocity = Elite::ZeroVector2;
		pBlackboard->ChangeData("SearchedLeftUp", true);
	}
	steering.RunMode = false;
	pBlackboard->ChangeData("Steering", steering);

	return  BehaviorState::Succeed;
}