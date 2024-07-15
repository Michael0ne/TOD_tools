class goodie_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("goodie_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to goodie_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

goodie_placeholder::Create();