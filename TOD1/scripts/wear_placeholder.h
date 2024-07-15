class wear_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("wear_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to wear_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

wear_placeholder::Create();