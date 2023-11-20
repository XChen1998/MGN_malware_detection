void __fastcall WaveOfEmperor(IChar IPlayer, int pPacket, int pPos)
{
	int pSkill = IPlayer.GetSkillPointer(39);

	if (IPlayer.IsValid() && pSkill)
	{
		ISkill xSkill((void*)pSkill);
		int nSkillGrade = xSkill.GetGrade();
		int nTargetID = 0; char bType = 0; void *pTarget = 0;
		CPacket::Read((char*)pPacket, (char*)pPos, "bd", &bType, &nTargetID);
		int nMana = (77 + (nSkillGrade * 2));

		if (bType == 0 && nTargetID)
			pTarget = CPlayer::FindPlayer(nTargetID);

		if (bType == 1 && nTargetID)
			pTarget = CMonster::FindMonster(nTargetID);

		if (bType >= 2)
			return;

		if (pTarget && nSkillGrade && IPlayer.IsValid())
		{
			IChar Target(pTarget);

			if (IPlayer.GetCurMp() < nMana)
				return;

			if (pTarget == IPlayer.GetOffset())
				return;

			if(IPlayer.IsValid() && Target.IsValid())
			{
				if (!IPlayer.IsInRange(Target,300))
					return;

				int Around = Target.GetObjectListAround(3);
				int nDmg = (IPlayer.GetMaxMagAttack() + (((IPlayer.GetMaxMagAttack() / 1) + (CChar::GetInt((int)IPlayer.GetOffset()) / 1)) + ((CChar::GetWis((int)IPlayer.GetOffset()) / 1) + (nSkillGrade * 180)))) + (IPlayer.GetLevel() * 10);
				if(Target.GetType() == 0) nDmg = (nDmg * SHMWOE) / 100;
				IPlayer._ShowBattleAnimation(Target, 39);
				IPlayer.SetDirection(Target);
				IPlayer.DecreaseMana(nMana);

				while(Around)
				{
					IChar Object((void*)*(DWORD*)Around);

					if (Object.IsValid() && IPlayer.IsValid() && (*(int (__thiscall **)(int, int, DWORD))(*(DWORD *)IPlayer.GetOffset() + 176))((int)IPlayer.GetOffset(), (int)Object.GetOffset(), 0))
						IPlayer.OktayDamageArea(Object,nDmg,39);
			
					Around = CBaseList::Pop((void*)Around);
				}
			}
		} else {
			IPlayer.CouldntExecuteSkill();
		}
	}
}