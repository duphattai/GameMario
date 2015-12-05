#pragma once
#include "ItemInBox.h"


class LuckyBox : public GameObject
{
	StateMachine<LuckyBox>*		m_stateMachine;

	ItemInBox*					m_item;
	int							m_countItem;

	ItemTypes					m_itemType;

	bool						m_finishAnimation;

	int							m_currentFrame;
	vector<Frame>				m_frameList;

	bool						m_makeEffect;
public:
	LuckyBox(LuckyBoxsType, ItemTypes, IDMap, int countCoin = 1);
	~LuckyBox();

	void					setCurrentFrame(int frame);
	int						getCurrentFrame(){ return m_currentFrame; }

	bool					isCollision(GameObject*);

	void					update();
	void					updateVelocity();
	void					draw(LPD3DXSPRITE SpriteHandler);

	int						getCountItem(){ return m_countItem; }
	void					setCountItem(int x){ m_countItem = x; }

	StateMachine<LuckyBox>*	getStateMachine(){ return m_stateMachine; }

	int						getSizeFrameList(){ return m_frameList.size(); }
	void					setFrameList(vector<Frame> list){ m_frameList = list; }

	ItemInBox*				getItem(){ return m_item; }

	void					changeItemsType(LuckyBoxsType type);
	LuckyBoxsType			getTypeItem(){ return m_item->getItemsType(); }

	void					setMakeEffect(bool x){ m_makeEffect = x; }
	bool					getMakeEffect(){ return m_makeEffect; }

	virtual Box				getBouding();
};
