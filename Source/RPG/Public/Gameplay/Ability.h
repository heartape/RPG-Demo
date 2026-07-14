#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace Ability
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Charged);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Healing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dash);
	// AI 敌人专用攻击事件标签：由 StateTree 任务经 ASC->HandleGameplayEvent 激活对应 AI 攻击能力
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AICombo);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AICharged);

	namespace Cooldown
	{
		// 各能力的冷却标签：Cooldown GE 施加后由 ASC 持有，CheckCooldown 据此判断是否在冷却中
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Charged);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Healing);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dash);
	}

	namespace Data
	{
		// 通用倍率数据标签：能力填充 SetByCaller，GE 修改器读取该标签得到数值
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Magnitude);
	}

	// GameplayEvent 事件标签：由 AnimNotify 或控制器发送，经 ASC->HandleGameplayEvent 派发到能力内的 WaitGameplayEvent 任务
	// Combo：连击按键（激活 + 连击中的再次按键缓存）
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combo);
	// ComboCheck：连击段落末尾由 AnimNotify_CheckCombo 发送，触发段落推进检查
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ComboCheck);
	// Charged：蓄力攻击按键（激活）
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Charged);
	// ChargedCheck：蓄力循环中由 AnimNotify_CheckChargedAttack 发送，决定继续循环或释放
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ChargedCheck);
	// ChargedRelease：松开蓄力键时由控制器发送
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ChargedRelease);
	// Dash：冲刺按键（激活）
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dash);
	// DashEnd：冲刺蒙太奇由 AnimNotify_EndDash 发送，提前恢复重力
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DashEnd);
	// AIComboCheck：AI 连击段落末尾由 AnimNotify_CheckCombo 发送，触发 AI 连击能力自动推进
	// 复用 ComboCheck 即可，AI 能力也监听 ComboCheck 事件
	// AIChargedCheck：AI 蓄力循环中由 AnimNotify_CheckChargedAttack 发送，决定继续循环或释放
	// 复用 ChargedCheck 即可，AI 能力也监听 ChargedCheck 事件

	namespace State
	{
		// 冲刺中状态标签：Dash 能力激活时施加，MultiJump 据此在冲刺期间忽略跳跃
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dashing);
	}
}
