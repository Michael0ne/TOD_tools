class cutscene_default_weapon : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("cutscene_default_weapon", "Node", false, false);
		
		//	TODO: add members and methods to cutscene_default_weapon.
		
		inst->CalculateSize();
		
		return inst;
	};
};

cutscene_default_weapon::Create();