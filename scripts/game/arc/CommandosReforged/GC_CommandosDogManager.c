[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "")]
class GC_CommandosDogManagerClass : ScriptComponentClass
{
}
//to.ClearFlags(EntityFlags.VISIBLE);
class GC_CommandosDogManager : ScriptComponent
{
	[Attribute(defvalue: "{CDB8EF4A6115A227}worlds/arc/CommandosReforged/Prefabs/OP/Character_GC_Cdos_DOG.et", uiwidget: UIWidgets.ResourceNamePicker, desc: "Dog prefab (Sets player as a dog)")]
	protected ResourceName m_dogPrefab;	
	
	protected ref array<GC_CommandosFootPrints> m_footPrints = {};
	protected bool m_isDog = false;
	
	static GC_CommandosDogManager Instance()
	{
	 	PlayerController pc = GetGame().GetPlayerController();
		if(!pc)
			return null;
		
		return GC_CommandosDogManager.Cast(pc.FindComponent(GC_CommandosDogManager));
	}
	
	void Register(GC_CommandosFootPrints component)
	{
		m_footPrints.Insert(component);
		
		if(m_isDog)
			component.SetActive();
	}
	
	void SetActive(bool state)
	{
		if(m_isDog == state)
			return;
		
		if(state)
		{
			foreach(GC_CommandosFootPrints component : m_footPrints)
			{
				component.SetActive();
			}
			SetEventMask(GetOwner(),EntityEvent.FIXEDFRAME);
			GetGame().GetInputManager().AddActionListener("VONDirect", EActionTrigger.DOWN, ActionBark);
		}
		else
		{
			foreach(GC_CommandosFootPrints component : m_footPrints)
			{
				component.SetDeactive();
			}
			ClearEventMask(GetOwner(), EntityEvent.FIXEDFRAME);
			GetGame().GetInputManager().RemoveActionListener("VONDirect", EActionTrigger.DOWN, ActionBark);
		}
		
		m_isDog = state;
	}
	
	override protected void EOnFixedFrame(IEntity owner, float timeSlice)
	{
		if(!m_isDog)
			return;
		
		BaseWorld world = GetGame().GetWorld();
		world.SetCameraHDRBrightness(world.GetCurrentCameraId(), 10.0);
	}

	override protected void OnPostInit(IEntity owner)
	{
		if(RplSession.Mode() == RplMode.Dedicated)
			return;
		
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetOwner());
		playerController.m_OnControlledEntityChanged.Insert(OnControlEntityChange);
	}
	
	void OnControlEntityChange(IEntity from, IEntity to)
	{
		EntityPrefabData epd = to.GetPrefabData();
		if (!epd)
			return;

		BaseContainer bc = epd.GetPrefab();
		if (!bc)
			return;

		if (!SCR_BaseContainerTools.IsKindOf(bc, m_dogPrefab))
		{
			if(m_isDog)
				SetActive(false);
		
			return;
		}
		SetActive(true);
	}
	
	void ActionBark()
	{
		GC_CommandosPlayerController pc = GC_CommandosPlayerController.Cast(GetOwner());
		if(!pc)
			return;
		
		IEntity player = pc.GetControlledEntity();
		if(!player)
			return;
		
		RplComponent rpl = SCR_EntityHelper.GetEntityRplComponent(player);
		if(!rpl)
			return;
		
		Rpc(RPC_AskBark, Replication.FindItemId(player));
	}
	
	[RplRpc(RplChannel.Unreliable, RplRcver.Server)]
	protected void RPC_AskBark(RplId rplId)
	{
		GC_CommandosManager dm = GC_CommandosManager.GetInstance();
		dm.DoBark(rplId);
	}
}