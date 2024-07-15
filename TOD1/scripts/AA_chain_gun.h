class AA_chain_gun : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("AA_chain_gun", "Node", false, false);
		
		//	TODO: add members and methods to AA_chain_gun.
		
		inst->CalculateSize();
		
		return inst;
	};
};

AA_chain_gun::Create();