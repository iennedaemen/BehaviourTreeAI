#include "stdafx.h"
#include "BehaviorTree.h"

BehaviorState BehaviorSelector::Execute(std::shared_ptr<Blackboard> pBlackBoard)
{
	for (auto child : m_ChildrenBehaviors)
	{
		m_CurrentState = child->Execute(pBlackBoard);
		switch (m_CurrentState)
		{
		case BehaviorState::Fail:
			continue; 
			break;
		case BehaviorState::Succeed:
			return m_CurrentState; 
			break;
		case BehaviorState::Running:
			return m_CurrentState; 
			break;
		default:
			continue; 
			break;
		}
	}
	m_CurrentState = BehaviorState::Fail;
	return m_CurrentState;
}
//SEQUENCE
BehaviorState BehaviorSequence::Execute(std::shared_ptr<Blackboard> pBlackBoard)
{
	for (auto child : m_ChildrenBehaviors)
	{
		m_CurrentState = child->Execute(pBlackBoard);
		switch (m_CurrentState)
		{
		case BehaviorState::Fail:
			return m_CurrentState; 
			break;
		case BehaviorState::Succeed:
			continue; 
			break;
		case BehaviorState::Running:
			return m_CurrentState; 
			break;
		default:
			m_CurrentState = BehaviorState::Succeed;
			return m_CurrentState; 
			break;
		}
	}
	m_CurrentState = BehaviorState::Succeed;
	return m_CurrentState;
}
//PARTIAL SEQUENCE
BehaviorState BehaviorPartialSequence::Execute(std::shared_ptr<Blackboard> pBlackBoard)
{
	while (m_CurrentBehaviorIndex < m_ChildrenBehaviors.size())
	{
		m_CurrentState = m_ChildrenBehaviors[m_CurrentBehaviorIndex]->Execute(pBlackBoard);
		switch (m_CurrentState)
		{
		case BehaviorState::Fail:
			m_CurrentBehaviorIndex = 0;
			return m_CurrentState; break;
		case BehaviorState::Succeed:
			++m_CurrentBehaviorIndex;
			return m_CurrentState = Running; break;
		case BehaviorState::Running:
			return m_CurrentState; break;
		}
	}

	m_CurrentBehaviorIndex = 0;
	m_CurrentState = BehaviorState::Succeed;
	return m_CurrentState;
}
#pragma endregion

BehaviorState BehaviorConditional::Execute(std::shared_ptr<Blackboard> pBlackBoard)
{
	if (m_fpConditional == nullptr)
		return BehaviorState::Fail;

	switch (m_fpConditional(pBlackBoard))
	{
	case true:
		return m_CurrentState = BehaviorState::Succeed;
	case false:
		return m_CurrentState = BehaviorState::Fail;
	}
	return m_CurrentState = BehaviorState::Fail;
}

BehaviorState BehaviorAction::Execute(std::shared_ptr<Blackboard> pBlackBoard)
{
	if (m_fpAction == nullptr)
		return BehaviorState::Fail;

	return m_CurrentState = m_fpAction(pBlackBoard);
}