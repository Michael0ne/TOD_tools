class animal_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("animal_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to animal_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

animal_placeholder::Create();