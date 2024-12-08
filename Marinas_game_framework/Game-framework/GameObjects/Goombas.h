#pragma once

#include "GameObjects/GameObject.h"
#include "GameObjects/Player.h"
#include "Utilities/Animator.h"


class CGoombas final : public CGameObject
{
public:

	typedef std::function<void(const uint32_t index)> Callback;

};

