#include "Gameplay/Ability.h"

namespace Ability
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combo, "Ability.Combo", "连击能力标签 / 连击按键事件")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Charged, "Ability.Charged", "重击能力标签 / 蓄力按键事件")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Healing, "Ability.Healing", "治疗术能力标签")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dash, "Ability.Dash", "冲刺能力标签 / 冲刺按键事件")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AICombo, "Ability.AICombo", "AI 连击攻击能力激活事件")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AICharged, "Ability.AICharged", "AI 蓄力攻击能力激活事件")

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ComboCheck, "Ability.ComboCheck", "连击段落推进检查事件")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ChargedCheck, "Ability.ChargedCheck", "蓄力循环检查事件")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(ChargedRelease, "Ability.ChargedRelease", "蓄力释放事件")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(DashEnd, "Ability.DashEnd", "冲刺结束事件")

	namespace Cooldown
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combo, "Ability.Cooldown.Combo", "连击冷却标签")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Charged, "Ability.Cooldown.Charged", "重击冷却标签")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Healing, "Ability.Cooldown.Healing", "治疗术冷却标签")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dash, "Ability.Cooldown.Dash", "冲刺冷却标签")
	}

	namespace Data
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Magnitude, "Ability.Data.Magnitude", "能力通过 SetByCaller 传入 GE 的数值倍率")
	}

	namespace State
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dashing, "Ability.State.Dashing", "冲刺中状态标签")
	}
}
