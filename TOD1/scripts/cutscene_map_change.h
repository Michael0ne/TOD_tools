class cutscene_map_change : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("cutscene_map_change", "Node", false, false);
		
		//	TODO: add members and methods to cutscene_map_change.
		
		inst->CalculateSize();
		
		return inst;
	};
};

cutscene_map_change::Create();