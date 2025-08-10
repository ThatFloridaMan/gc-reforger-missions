//------------------------------------------------------------------------------------------------
//! Extend ACE E-tool for placing trenches outside of building editor mode
//! First use of GadgetActivate keybind will start the preview, a second will place it
modded class SCR_CampaignBuildingGadgetToolComponent : SCR_GadgetComponent
{
	[Attribute(defvalue: "-1", desc: "Maximum number of placement for this tool")]
	protected int m_iACE_Trenches_MaxPrefabCount;
	
	protected int m_iACE_Trenches_CurrentPrefabCount = 0;
	
	override void ACE_Trenches_RequestPlace() 
	{
		
		if ((m_iACE_Trenches_MaxPrefabCount != -1) && (m_iACE_Trenches_CurrentPrefabCount >= m_iACE_Trenches_MaxPrefabCount))
			return;
		
		super.ACE_Trenches_RequestPlace();
		m_iACE_Trenches_CurrentPrefabCount++;
	}
}