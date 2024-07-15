class pallet : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("pallet", "Node", false, false);
		
		//	TODO: add members and methods to pallet.
		
		inst->CalculateSize();
		
		return inst;
	};
};

pallet::Create();