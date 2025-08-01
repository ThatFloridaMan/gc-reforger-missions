class GC_CommandosPlayerControllerClass : SCR_PlayerControllerClass
{
}

class GC_CommandosPlayerController : SCR_PlayerController
{
	static GC_CommandosPlayerController GetInstance(int playerId)
	{
		return GC_CommandosPlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
	}
	
	void ActivateObjective(string name)
	{
		Rpc(RPC_ActivateObjective, name);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RPC_ActivateObjective(string name)
	{
		Print("GRAY.RPC_ActivateObjective = " + name);
		GC_CommandosManager cm = GC_CommandosManager.GetInstance();
		GC_CommandosDefector obj = GC_CommandosDefector.Cast(cm.GetObjective(name));
		if(!obj)
			return;
		
		obj.ActivateLocal();
	}
}