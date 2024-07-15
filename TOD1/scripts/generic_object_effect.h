class generic_object_effect : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("generic_object_effect", "Node", false, false);
		
		//	TODO: add members and methods to generic_object_effect.
		
		inst->CalculateSize();
		
		return inst;
	};
};

generic_object_effect::Create();