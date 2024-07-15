class AA_chain_gun_AICtrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("AA_chain_gun_AICtrl", "Node", false, false);
		
		//	TODO: add members and methods to AA_chain_gun_AICtrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

AA_chain_gun_AICtrl::Create();