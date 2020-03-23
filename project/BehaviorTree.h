#pragma once

class Blackboard;

// States
enum BehaviorState
{
	Fail,
	Running,
	Succeed
};

// BASE
class Behavior
{
public:
	Behavior() = default;
	virtual ~Behavior() = default;
	virtual BehaviorState Execute(std::shared_ptr<Blackboard> pBlackBoard) = 0;

protected:
	BehaviorState m_CurrentState = BehaviorState::Fail;
};



class BehaviorComposite : public Behavior
{
public:
	explicit BehaviorComposite(std::vector<std::shared_ptr<Behavior>> childrenBehaviors)
	{
		m_ChildrenBehaviors = childrenBehaviors;
	}


	virtual BehaviorState Execute(std::shared_ptr<Blackboard> pBlackBoard) override = 0;

protected:
	std::vector<std::shared_ptr<Behavior>> m_ChildrenBehaviors = {};
};


// SELECTOR
class BehaviorSelector : public BehaviorComposite
{
public:
	explicit BehaviorSelector(std::vector<std::shared_ptr<Behavior>> childrenBehaviors) :
		BehaviorComposite(childrenBehaviors) {}
	virtual ~BehaviorSelector() = default;

	virtual BehaviorState Execute(std::shared_ptr<Blackboard> pBlackBoard) override;
};

// SEQUENCE
class BehaviorSequence : public BehaviorComposite
{
public:
	explicit BehaviorSequence(std::vector<std::shared_ptr<Behavior>> childrenBehaviors) 
		: BehaviorComposite(childrenBehaviors) 
	{}

	virtual ~BehaviorSequence() = default;

	virtual BehaviorState Execute(std::shared_ptr<Blackboard> pBlackBoard) override;
};

//--- PARTIAL SEQUENCE ---
class BehaviorPartialSequence : public BehaviorSequence
{
public:
	explicit BehaviorPartialSequence(std::vector<std::shared_ptr<Behavior>> childrenBehaviors)
		: BehaviorSequence(childrenBehaviors) 
	{}

	virtual ~BehaviorPartialSequence() = default;

	virtual BehaviorState Execute(std::shared_ptr<Blackboard> pBlackBoard) override;

private:
	unsigned int m_CurrentBehaviorIndex = 0;
};
#pragma endregion

class BehaviorConditional : public Behavior
{
public:
	explicit BehaviorConditional(std::function<bool(std::shared_ptr<Blackboard>)> fp) 
		: m_fpConditional(fp) 
	{}

	virtual BehaviorState Execute(std::shared_ptr<Blackboard> pBlackBoard) override;

private:
	std::function<bool(std::shared_ptr<Blackboard>)> m_fpConditional = nullptr;
};

class BehaviorAction : public Behavior
{
public:
	explicit BehaviorAction(std::function<BehaviorState(std::shared_ptr<Blackboard>)> fp) 
		: m_fpAction(fp) 
	{}

	virtual BehaviorState Execute(std::shared_ptr<Blackboard> pBlackBoard) override;

private:
	std::function<BehaviorState(std::shared_ptr<Blackboard>)> m_fpAction = nullptr;
};


class BehaviorTree final
{
public:
	explicit BehaviorTree(std::shared_ptr<Blackboard> pBlackBoard, std::shared_ptr<Behavior> pRootComposite)
		: m_pBlackBoard(pBlackBoard)
		, m_pRootComposite(pRootComposite) 
	{}

	BehaviorState Update()
	{
		if (m_pRootComposite == nullptr)
			return m_CurrentState = BehaviorState::Fail;

		return m_CurrentState = m_pRootComposite->Execute(m_pBlackBoard);
	}
	std::shared_ptr<Blackboard> GetBlackboard() const
	{
		return m_pBlackBoard;
	}

private:
	BehaviorState m_CurrentState = BehaviorState::Fail;
	std::shared_ptr<Blackboard> m_pBlackBoard = nullptr;
	std::shared_ptr<Behavior> m_pRootComposite = nullptr;
};
