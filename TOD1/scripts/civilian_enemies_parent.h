class civilian_enemies_parent : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("civilian_enemies_parent", "Node", false, false);
		
		//	TODO: add members and methods to civilian_enemies_parent.
		
		inst->CalculateSize();
		
		return inst;
	};
};

civilian_enemies_parent::Create();