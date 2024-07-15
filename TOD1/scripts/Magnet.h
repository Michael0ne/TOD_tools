class Magnet : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Magnet", "Node", false, false);
		
		//	TODO: add members and methods to Magnet.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Magnet::Create();